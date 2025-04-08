#include "wrl.h"
#include "Windows.h"
#include <appmodel.h>
#include <string>
#include <iostream>

#include "Log.h"
#include "ToyPlugin.h"

#pragma comment(lib, "runtimeobject.lib")
#pragma comment(lib, "Shlwapi.lib")

using namespace Microsoft::WRL;

static HANDLE g_hStopEvent = NULL;

// 1. CoCreatableClass: Helps set up the necessary infrastructure so that the COM class CToyPlugin can be created using CoCreateInstance.
// 2. CoCreatableClass(CoCreatableClass): registers a factory for CToyPlugin, so that when someone calls CoCreateInstance(CLSID_CToyPlugin), the object can actually be created and returned.
// 3. Ensures that CToyPlugin supports COM's rules (e.g., reference counting, interface querying).
// 4. In classic COM, IClassFactory would have to be implemented manually for CToyPlugin. In WRL, CoCreatableClass does this for us.
// 5. Now CreatePlugin and GetPluginCount can be used by the RDP clients, since, CToyPlugin is not exposed.
// See https://learn.microsoft.com/en-us/cpp/cppcx/wrl/how-to-create-a-classic-com-component-using-wrl?view=msvc-170
CoCreatableClass(CToyPlugin);

inline void AbortOnFailed(HRESULT hr, const wchar_t* functionName)
{
    if (FAILED(hr))
    {
        auto errorMessage = Log(functionName, hr);
        MessageBoxW(nullptr, errorMessage.c_str(), L"Error", MB_OK | MB_ICONERROR);

        ::ExitProcess(static_cast<UINT>(hr));
    }
}

void InitializeEvent()
{
    g_hStopEvent = CreateEvent(
        NULL,    // default security attributes
        TRUE,    // manual-reset event
        FALSE,   // initial state is nonsignaled
        NULL     // object name (unnamed)
    );

    AbortOnFailed(g_hStopEvent == NULL ? E_HANDLE : S_OK, L"InitializeEvent");
}

void WaitForShutdown()
{
    while (true)
    {
        // Wait indefinitely for the event to be signaled
        DWORD dwWaitResult = WaitForSingleObject(g_hStopEvent, 1000);

        switch (dwWaitResult)
        {
            // Event object was signaled
        case WAIT_OBJECT_0:
            // Handle the shutdown here
            Log(L"Wait for shutdown Signaled");
            return;

        default:
            // Spin here
            break;
        }
    }
}

static void Shutdown()
{
    Log(L"Signaling Shutdown");
    if (g_hStopEvent)
    {
        SetEvent(g_hStopEvent);
    }
}

bool HasPackageIdentity()
{
    UINT32 length = 0;
    LONG result = ::GetCurrentPackageFullName(&length, nullptr);
    if (result == ERROR_INSUFFICIENT_BUFFER)
    {
        // Use a smart pointer to avoid manual new/delete.
        std::unique_ptr<wchar_t[]> packageFullName(new wchar_t[length]);

        result = ::GetCurrentPackageFullName(&length, packageFullName.get());
        if (result == ERROR_SUCCESS)
        {
            // Build a log message with the retrieved package name
            std::wstring msg = L"Package Full Name: ";
            msg += packageFullName.get();

            // Log the package full name
            Log(msg.c_str());
            return true;
        }
        else
        {
            // Optionally log the failure reason
            Log(L"Failed to retrieve the package full name even though a buffer size was available.");
        }
    }
    else
    {
        // Optionally log if the result was something other than ERROR_INSUFFICIENT_BUFFER
        Log(L"No package identity (not running in a packaged context) or another error occurred.");
    }

    // If we reach here, the app does not have a package identity
    return false;
}

void LogTokenInformation()
{
    DWORD isAppContainer = 0;
    DWORD tokenInformationLength = sizeof(DWORD);

    if (!::GetTokenInformation(GetCurrentProcessToken(),
        TOKEN_INFORMATION_CLASS::TokenIsAppContainer,
        &isAppContainer,
        tokenInformationLength,
        &tokenInformationLength))
    {
        Log(L"Failed to retrieve AppContainer information from the token.");
        return;
    }

    auto boolToWString = [](bool b) -> std::wstring { return b ? L"true" : L"false"; };

    auto message = L"Process Information:\r\n-----Is AppContainer: "
        + boolToWString(isAppContainer == TRUE)
        + L"\r\n-----Has Package Identity: "
        + boolToWString(HasPackageIdentity());

    Log(message.c_str());
}

int main()
{
    // IGlobalOptions is a COM interface that allows applications to configure various global settings for the COM runtime. 
    // It provides a way to tweak COM behaviors such as: garbage collection, fast rundown, marshalling settings, etc
    // ComPtr is a Windows Runtime Library (WRL) Template construct.
    ComPtr<IGlobalOptions> globalOptions;

    // This initializes the COM library for use in a multi-threaded apartment (MTA). 
    // COINIT_MULTITHREADED allows COM objects to be accessed from multiple threads concurrently.
    AbortOnFailed(CoInitializeEx(NULL, COINIT_MULTITHREADED), L"CoInitializeEx");

    // CoCreateInstance is used to create an instance of uninitialized GlobalOptions COM object.
    // CLSCTX_INPROC_SERVER indicates that the object is being loaded from an in-process DLL.
    // CLSID_GlobalOptions is the class id of the IGlobalOptions implementation.
    AbortOnFailed(CoCreateInstance(CLSID_GlobalOptions, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&globalOptions)), L"CoCreateInstance-GlobalOptions");

    AbortOnFailed(globalOptions->Set(COMGLB_RO_SETTINGS, COMGLB_FAST_RUNDOWN), L"SetGlobalOptions");
    AbortOnFailed(CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0), L"CoInitializeSecurity");

    // Creates a singleton instance of the Module<OutOfProc> object.
    // Registers a shutdown callback when the last instance object of the module is released.
    auto& module = Module<OutOfProc>::Create(Shutdown);

    // registers COM objects, making them available to clients.
    // Registers the COM objects so that clients can create them via CoCreateInstance. 
    // Calls CoRegisterClassObject internally to expose the class factory.
    AbortOnFailed(module.RegisterObjects(), L"RegisterObjects");
    Log(L"Registered WRL Classes");

    LogTokenInformation();

    WaitForShutdown();

    CoUninitialize();

    if (g_hStopEvent)
    {
        CloseHandle(g_hStopEvent);
        g_hStopEvent = NULL;
    }
}