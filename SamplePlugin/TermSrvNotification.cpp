
// listen_lsm_notifs_async.cpp (excerpt)
// Changes from the original:
//  * Moves the blocking wait loop onto a worker thread.
//  * Uses Ctrl+C to trigger shutdown: Unregister → join worker.
//  * Keeps main thread free (you could run a message loop or service loop here).

#define NOMINMAX
#include <windows.h>
#include <rpc.h>
#include "TermSrvNotification.hpp"
#include "../MidlGeneratedFiles/tspubrpc_min.h"    // MIDL-generated from the minimal IDL

#pragma comment(lib, "Rpcrt4.lib")


// Provide the rundown expected by the MIDL-generated client stub.
// Rundown functions are server-side cleanup callbacks. But whenever we have 
// typedef [context_handle] void* NOTIFY_HANDLE;
// the MIDL compiler assumes a function named:NOTIFY_HANDLE_rundown. 
// This is used by server side to cleanup. But since this is client side, we provide a stub.
// Keep the exact name and calling convention (__RPC_USER).
extern "C" void __RPC_USER NOTIFY_HANDLE_rundown(void* h)
{
    // No-op on the client. The server actually owns the context cleanup.
    (void)h;
}


void* __RPC_USER MIDL_user_allocate(size_t len) { return std::malloc(len); }
void  __RPC_USER MIDL_user_free(void* p) { std::free(p); }

static std::atomic<bool> g_stop{ false };
static NOTIFY_HANDLE     g_hNotify = nullptr;
static RPC_BINDING_HANDLE g_hBinding = nullptr;

BOOL WINAPI CtrlHandler(DWORD ctrlType)
{
    std::wcout << L"CtrlHandler invoked: " << ctrlType << L"\n";
    if (ctrlType == CTRL_C_EVENT || ctrlType == CTRL_BREAK_EVENT ||
        ctrlType == CTRL_CLOSE_EVENT || ctrlType == CTRL_SHUTDOWN_EVENT)
    {
        g_stop = true;

        // Trigger RpcWaitAsyncNotification to unwind by unregistering.
        if (g_hNotify)
        {
            // Ignore error; this is best-effort during shutdown.
            RpcUnRegisterAsyncNotification(&g_hNotify);
        }
        std::wcout << L"CtrlHandler invoked and exit criteria fulfilled: " << ctrlType << L"\n";
        return TRUE;
    }
    std::wcout << L"CtrlHandler invoked and exit criteria NOT fulfilled: " << ctrlType << L"\n";
    return FALSE;
}

/// <summary>
/// Composes and creates an ncacn_np (named pipe) RPC binding to \\PIPE\\LSM_API_service.
/// </summary>
/// <returns>Returns a handle to the binding.</returns>
RPC_BINDING_HANDLE BindLSM()
{
    std::wcout << L"Entered BindLSM." << std::endl;
    RPC_STATUS status = RPC_S_OK;
    RPC_WSTR strBinding = nullptr;
    RPC_BINDING_HANDLE h = nullptr;

    status = RpcStringBindingComposeW(
        /*ObjUuid*/  nullptr,
        (RPC_WSTR)L"ncacn_np",
        (RPC_WSTR)L".",
        (RPC_WSTR)L"\\PIPE\\LSM_API_service",
        /*Options*/  nullptr,
        &strBinding
    );
    if (status)
    {
        throw std::runtime_error("RpcStringBindingComposeW failed: " + std::to_string(status));
    }

    status = RpcBindingFromStringBindingW(strBinding, &h);
    RpcStringFreeW(&strBinding);
    if (status)
    {
        throw std::runtime_error("RpcBindingFromStringBindingW failed: " + std::to_string(status));
    }
    std::wcout << L"Exiting BindLSM." << std::endl;
    return h;
}

static std::wstring MaskToString(unsigned long m)
{
    std::wstring s;
    auto add = [&](const wchar_t* t) { if (!s.empty()) s += L" | "; s += t; };

    if (m & WTS_NOTIFY_CONNECT)      add(L"CONNECT");
    if (m & WTS_NOTIFY_DISCONNECT)   add(L"DISCONNECT");
    if (m & WTS_NOTIFY_LOCK)         add(L"LOCK");
    if (m & WTS_NOTIFY_UNLOCK)       add(L"UNLOCK");
    if (s.empty()) s = L"NONE";
    return s;
}

int GetSessionNotifications()
{
    SetConsoleCtrlHandler(CtrlHandler, TRUE);

    try
    {
        // Determine current process session (set to -1 to listen for all).
        /*
        DWORD sessionId = 0;
        if (!ProcessIdToSessionId(GetCurrentProcessId(), &sessionId))
            throw std::runtime_error("ProcessIdToSessionId failed: " + std::to_string(GetLastError()));

        LONG targetSession = static_cast<LONG>(sessionId); // or -1 for all
        */

        std::wcout << L"Entered GetSessionNotifications." << std::endl;

        LONG targetSession = static_cast<LONG>(-1);

        g_hBinding = BindLSM();

        // Register interest (lock/unlock + rdp connect/disconnect)
        TNotificationId mask =
            WTS_NOTIFY_LOCK | WTS_NOTIFY_UNLOCK |
            WTS_NOTIFY_CONNECT | WTS_NOTIFY_DISCONNECT;

        std::wcout << L"Starting RpcRegisterAsyncNotification." << std::endl;
        HRESULT hr = RpcRegisterAsyncNotification(g_hBinding, targetSession, mask, &g_hNotify);
        std::wcout << L"Ended RpcRegisterAsyncNotification. HRESULT: " << std::hex << hr << std::endl;
        if (FAILED(hr))
        {
            throw std::runtime_error("RpcRegisterAsyncNotification failed: 0x" + std::to_string(hr));
        }
            
        std::wcout << L"Listening on Session " << targetSession << L" (Ctrl+C to stop)…\n";

        // ---- Worker thread does the blocking waits ----
        std::thread worker([&] {
            for (;;)
            {
                if (g_stop.load())
                {
                    break;
                }

                PSESSION_CHANGE entries = nullptr;
                ULONG count = 0;

                std::wcout << L"Starting RpcWaitAsyncNotification." << std::endl;
                HRESULT hrWait = RpcWaitAsyncNotification(g_hNotify, &entries, &count);
                std::wcout << L"Ended RpcWaitAsyncNotification. HRESULT: " << std::hex << hrWait << std::endl;

                if (FAILED(hrWait))
                {
                    // If we’re stopping and unregistered, expect failure; just exit.
                    if (g_stop.load()) 
                    {
                        break;
                    }

                    std::wcerr << L"RpcWaitAsyncNotification failed: 0x" << std::hex << hrWait << L"\n";
                    // Consider retry/backoff or break depending on your resiliency policy.
                    break;
                }

                for (ULONG i = 0; i < count; ++i)
                {
                    const auto& e = entries[i];
                    std::wcout << L"[Session " << e.SessionId << L"] " << MaskToString(e.NotificationId) << L"\n";
                }
                if (entries)
                {
                    MIDL_user_free(entries);
                }
            }
        });

        // ---- Main thread remains responsive here ----
        // For demo, just idle until Ctrl+C; real app can run UI/Service loop.
        while (!g_stop.load())
        {
            Sleep(100);
        }

        // Start shutdown: Unregister then join worker.
        if (g_hNotify)
        {
            RpcUnRegisterAsyncNotification(&g_hNotify); // wakes RpcWaitAsyncNotification
            g_hNotify = nullptr;
        }
        if (worker.joinable())
        {
            worker.join();
        }

        if (g_hBinding)
        {
            RpcBindingFree(&g_hBinding);
            g_hBinding = nullptr;
        }

        std::wcout << L"Stopped.\n";
    }
    catch (const std::exception& ex)
    {
        std::wcerr << L"Error: " << ex.what() << L"\n";
        if (g_hNotify) 
        { 
            RpcUnRegisterAsyncNotification(&g_hNotify); g_hNotify = nullptr; 
        }
        if (g_hBinding) {
            RpcBindingFree(&g_hBinding); g_hBinding = nullptr;
        }
        return 1;
    }
    return 0;
}
