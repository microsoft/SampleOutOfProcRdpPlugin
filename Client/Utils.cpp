#include <unknwn.h>      // Ensures you get the classic IUnknown, this should come before any other wrl headers, since, this makes the app to default to classic COM and not wrl com
#include <wrl/client.h>
#include <windows.h>
#include <roapi.h>

#include "pch.h"
#include "Utils.h"
#include "../MidlGeneratedFiles/IWTSPlugin_h.h"

#include <winrt/Windows.ApplicationModel.AppExtensions.h>
#include <winrt/Windows.Foundation.Collections.h>

using namespace winrt::Windows::ApplicationModel::AppExtensions;

std::vector<CLSID> Utils::GetRdpPluginClassIds()
{
    {
        std::vector<CLSID> classIds;

        // Initialize the Windows Runtime. Apartment type is STA by default.
        auto hr = RoInitialize(RO_INIT_MULTITHREADED);
        try
        {
            // AppExtensionCatalog for the RDP plugins. All RDP plugins are registered under this catalog i.e. com.microsoft.rdp.plugin.wtsplugin
            auto catalog = AppExtensionCatalog::Open(L"com.microsoft.rdp.plugin.wtsplugin");

            // Asynchronously find all extensions in the catalog
            auto findAllOp = catalog.FindAllAsync();
            auto extensions = findAllOp.get(); // Wait for completion

            // Iterate through each AppExtension in the catalog
            for (const AppExtension& extension : extensions)
            {
                try {
                    // Asynchronously get the extension's properties
                    auto getPropsOp = extension.GetExtensionPropertiesAsync();
                    auto properties = getPropsOp.get(); // Wait for completion

                    // Attempt to retrieve the "ComServer" property
                    if (properties.HasKey(L"ComServer"))
                    {
                        auto propertiesClsid = properties.Lookup(L"ComServer").as<winrt::Windows::Foundation::Collections::IPropertySet>();
                        if (!propertiesClsid.HasKey(L"#text"))
                        {
                            continue;
                        }
                        auto comServerGUID = propertiesClsid.Lookup(L"#text").as<winrt::Windows::Foundation::IPropertyValue>();
                        auto guidString = L"{" + comServerGUID.GetString() + L"}";

                        // Validate the GUID format
                        CLSID clsid;
                        HRESULT hr = CLSIDFromString(guidString.c_str(), &clsid);
                        if (SUCCEEDED(hr))
                        {
                            classIds.emplace_back(clsid);
                        }
                        else
                        {
                            std::cerr << "Invalid GUID format:" << winrt::to_string(guidString) << std::endl;
                        }
                    }
                }
                catch (const winrt::hresult_error& ex) {
                    // Log WinRT initialization or catalog opening errors
                    std::cerr << "WinRT error during GetRdpPluginClassIds: HRESULT:" << std::hex << ex.code().value << " " << winrt::to_string(ex.message()) << std::endl;
                }
            }
        }
        catch (const winrt::hresult_error& ex)
        {
            // Log WinRT initialization or catalog opening errors
            std::cerr << "WinRT error during GetRdpPluginClassIds: HRESULT:" << std::hex << ex.code().value << " " << winrt::to_string(ex.message()) << std::endl;
        }
        catch (const std::exception& ex)
        {
            // Handle any standard C++ exceptions
            std::cerr << "Standard exception in GetRdpPluginClassIds:" << ex.what();
        }

        if (SUCCEEDED(hr))
        {
            RoUninitialize();
        }

        return classIds;
    }
}

void Utils::LoadAllSamplePlugins(std::vector<CLSID>& classIds) {
    for (const CLSID classId : classIds) {
        HRESULT hr = LoadOneSamplePlugin(classId);

        // Check the HRESULT for errors if necessary
        if (FAILED(hr)) {
            LPWSTR pluginId;
            if (FAILED(StringFromCLSID(classId, &pluginId))) {
                std::cerr << "LoadOneSamplePlugin for PluginFactory (Unknown) failed with HRESULT 0x%08X" << hr << std::endl;
            }
            else
            {
                // Handle error (e.g., log, break the loop, or continue depending on the needs)
                std::wcout << "LoadOneSamplePlugin for PluginFactory " << pluginId << "failed with HRESULT 0x " << hr << std::endl;
            }
        }
    }
}

HRESULT Utils::LoadOneSamplePlugin(__in CLSID pluginClassId)
{
    HRESULT hr;
    LONG count;
    Microsoft::WRL::ComPtr<IWTSPluginFactory> pluginFactory;
    LPWSTR pluginId;

    hr = StringFromCLSID(pluginClassId, &pluginId);
    if (FAILED(hr)) {
        std::cerr << "StringFromCLSID for PluginFactory (Unknown) failed with HRESULT 0x%08X" << hr << std::endl;
        return hr;
    }
    else {
        std::wcout << "StringFromCLSID for PluginFactory succeeded for pluginId:" << pluginId << std::endl;
    }

    hr = CoCreateInstance(pluginClassId,
        nullptr,
        CLSCTX_LOCAL_SERVER | CLSCTX_DISABLE_AAA,
        __uuidof(IWTSPluginFactory),
        (PVOID*)&pluginFactory);

    if (FAILED(hr)) {
        std::wcout << "CoCreateInstance for PluginFactory " << pluginId << " failed with HRESULT 0x " << hr << std::endl;
        return hr;
    } 
    else {
        std::wcout << "CoCreateInstance for PluginFactory " << pluginId << " succeeded." << std::endl;
    }

    hr = pluginFactory->GetPluginCount(&count);

    if (FAILED(hr)) {
        std::wcout << "GetPluginCount for PluginFactory " << pluginId << " failed with HRESULT 0x " << hr << std::endl;
        return hr;
    }
    else {
        std::wcout << "GetPluginCount for PluginFactory " << pluginId << " succeeded." << std::endl;
    }

    for (ULONG counter = 0; counter < count; counter++)
    {
        // The plugin is not initialized here. We just declare it. 
        // The CreatePlugin does the actual MakeAndInitialize call with a concrete class (rather than interface IWTSPlugin)
        Microsoft::WRL::ComPtr<IWTSPlugin> plugin;

        hr = pluginFactory->CreatePlugin(counter, &plugin);

        if (FAILED(hr)) {
            std::wcout << "CreatePlugin for index " << counter << " in PluginFactory" << pluginId << " failed with HRESULT 0x % " << hr << std::endl;
            continue; // Continue to the next plugin if there’s an error
        }
        else {
          std::wcout << "CreatePlugin for index " << counter << " in PluginFactory" << pluginId << " succeeded." << std::endl;
        }

        hr = AddPluginWithFactory(plugin.Detach(), pluginFactory.Detach());
        if (FAILED(hr)) {
            std::wcout << "AddPlugin to DVC Manager for index " << counter << " in PluginFactory " << pluginId << " failed with HRESULT 0x % " << hr << std::endl;
            continue;
        }
        else {
            std::wcout << "AddPlugin to DVC Manager for index " << counter << " in PluginFactory " << pluginId << " succeeded." << std::endl;
        }
    }
    return hr;
}

HRESULT Utils::AddPluginWithFactory(__in IWTSPlugin* pPlugin, __in IWTSPluginFactory* pPluginFactory) {

    ptrUserPlugins.push_back(pPlugin);
    ptrUserPluginsFactories.push_back(pPluginFactory);

    pPlugin->AddRef();
    pPluginFactory->AddRef();

    return S_OK;
}

HRESULT Utils::Terminate() {
    for (IWTSPlugin* pPlugin : ptrUserPlugins) {
        pPlugin->Terminated();
        pPlugin->Release();
    }

    for (IWTSPluginFactory* pPluginFactory : ptrUserPluginsFactories) {
        pPluginFactory->Release();
    }
    ptrUserPluginsFactories.clear();
    return S_OK;
}

void Utils::PrintPluginComClsIds(const std::vector<CLSID> &classIds)
{
    for (CLSID classId : classIds) {
        LPWSTR pluginId;
        HRESULT hr = StringFromCLSID(classId, &pluginId);
        if (FAILED(hr)) {
            std::cerr << "GetRdpPluginClassIds for PluginFactory (Unknown) failed with HRESULT 0x%08X" << hr << std::endl;
        }
        else
        {
            std::wcout << "GetRdpPluginClassIds for PluginFactory " << pluginId << " succeeded." << std::endl;
        }
    }
}
