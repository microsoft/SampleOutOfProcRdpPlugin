#include "pch.h"

#include "ToyPlugin.h"
#include "ToyPluginImpl.h"
#include "Log.h"

#ifndef VCAPITYPE
#define VCAPITYPE __stdcall
#endif

typedef HRESULT(VCAPITYPE* PFNVCAPIGETINSTANCE)(
    REFIID refiid,
    PULONG pNumObjs,
    PVOID* ppObjArray
    );

IFACEMETHODIMP CToyPlugin::CreatePlugin(long index, IWTSPlugin** plugin)
{
    if (!plugin)
    {
        Log(L"plugin is null", E_POINTER);
        return E_POINTER;
    }

    if (index != 1)
    {
        Log(L"index not 1", E_NOTIMPL);
        return E_NOTIMPL;
    }

    ComPtr<CToyPluginImpl> pToyPlugin;
    HRESULT hr = MakeAndInitialize<CToyPluginImpl>(&pToyPlugin);

    if (FAILED(hr))
    {
        return hr;
    }

    // Return the plugin pointer to the caller and detach from the smart pointer. 
    *plugin = pToyPlugin.Detach();

    return S_OK;
}

IFACEMETHODIMP CToyPlugin::GetPluginCount(long* count)
{
    // Assuming there is only 1 plugin per process, since, this is a toy example.
    *count = 1;
    return S_OK;
}