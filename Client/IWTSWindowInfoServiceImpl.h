#pragma once
#include "pch.h"
#include <wrl.h>
#include "../MidlGeneratedFiles/IWTSPlugin_h.h"

using namespace Microsoft::WRL;

class CToyPluginWindowInfoServiceImpl 
    : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IWTSWindowInfoService>
{
public:
    IFACEMETHODIMP GetWindowInfo(HWND remoteHwnd);
    IFACEMETHODIMP GetRdpClientProcessId();
    IFACEMETHODIMP GetRdpSessionType();
    IFACEMETHODIMP SubscribeWindowChanged(HWND remoteHwnd, IWTSWindowChangedCallback* windowChanged);
    IFACEMETHODIMP UnsubscribeWindowChanged(HWND remoteHwnd, IWTSWindowChangedCallback* windowChanged);
};

