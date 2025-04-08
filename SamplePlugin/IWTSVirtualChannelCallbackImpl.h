#pragma once
#include "pch.h"
#include <wrl.h>
#include "../MidlGeneratedFiles/IWTSPlugin_h.h"

using namespace Microsoft::WRL;

class CToyPluginVirtualChannelCallbackImpl
    : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IWTSVirtualChannelCallback>
{
private:
    ComPtr<IWTSVirtualChannel> _pChannel;
    ComPtr<IWTSWindowInfoService> _pWindowInfoService;
    ComPtr<IWTSWindowChangedCallback> _pWindowChangedCallback;

public:
    CToyPluginVirtualChannelCallbackImpl(
        ComPtr<IWTSVirtualChannel> channel, 
        ComPtr<IWTSWindowInfoService> pWindowInfoService);

    // Inherited via RuntimeClass
    IFACEMETHODIMP OnDataReceived(ULONG cbSize, BYTE* pBuffer) override;
    IFACEMETHODIMP OnClose(void) override;
};