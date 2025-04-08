#pragma once

#include "pch.h"
#include "../MidlGeneratedFiles/IWTSPlugin_h.h"
#include <wrl.h>
#include <wrl/implements.h>
#include <wrl/module.h>

using namespace Microsoft::WRL;

class CToyPluginImpl
    : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IWTSPlugin, IWTSListenerCallback>
{
public:
    CToyPluginImpl() = default;

    // Inherited via IWTSPlugin
    IFACEMETHODIMP Initialize(IWTSVirtualChannelManager* pChannelMgr) override;

    IFACEMETHODIMP Connected(void) override;

    IFACEMETHODIMP Disconnected(DWORD dwDisconnectCode) override;

    IFACEMETHODIMP Terminated(void) override;

    // Inherited via IWTSListenerCallback
    HRESULT __stdcall OnNewChannelConnection(
        IWTSVirtualChannel* pChannel,
        BSTR data, BOOL* pbAccept,
        IWTSVirtualChannelCallback** ppCallback) override;

private:
    ComPtr<IWTSListener> _pListener;
    ComPtr<IWTSWindowInfoService> _pWindowInfoService;
    ComPtr<IWTSPluginServiceProvider> _pPluginServiceProvider;
};