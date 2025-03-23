#include "pch.h"
#include "ToyPluginImpl.h"
#include "IWTSVirtualChannelCallbackImpl.h"
#include "../MidlGeneratedFiles/IWTSPlugin_h.h"

// Inherited via IWTSPlugin
// Called by the Remote Desktop Services subsystem (specifically, the Remote Desktop Session Host process) 
// when the plugin is loaded during a Remote Desktop session initialization.
// IWTSVirtualChannelManager is thus, passed to the plugin by the Remote Desktop Services subsystem.
IFACEMETHODIMP CToyPluginImpl::Initialize(IWTSVirtualChannelManager* pChannelMgr)
{
    pChannelMgr->QueryInterface(IID_PPV_ARGS(&_pPluginServiceProvider));
    _pPluginServiceProvider->GetService(RDCLIENT_WINDOW_INFO_SERVICE, &_pWindowInfoService);

    unsigned char* pChannelName = (unsigned char*)"SamplePluginChannel";
    return pChannelMgr->CreateListener(pChannelName, 0, this, _pListener.ReleaseAndGetAddressOf());
}

IFACEMETHODIMP CToyPluginImpl::Connected(void)
{
    return S_OK;
}

IFACEMETHODIMP CToyPluginImpl::Disconnected(DWORD dwDisconnectCode)
{
    return S_OK;
}

IFACEMETHODIMP CToyPluginImpl::Terminated(void)
{
    _pListener->Release();
    _pWindowInfoService->Release();
    _pPluginServiceProvider->Release();
    return S_OK;
}

// Inherited via IWTSListenerCallback
IFACEMETHODIMP CToyPluginImpl::OnNewChannelConnection(
    IWTSVirtualChannel* pChannel,
    BSTR data,
    BOOL* pbAccept,
    IWTSVirtualChannelCallback** ppCallback)
{
    // Assuming the connection is accepted trivialy, since, this is a toy plugin.
    *pbAccept = true;

    // Create a callback instance which is called when channel gets some data.
    ComPtr<IWTSVirtualChannelCallback> pVirtualChannelCallback = 
        Make<CToyPluginVirtualChannelCallbackImpl>(pChannel, _pWindowInfoService);
    pVirtualChannelCallback.CopyTo(ppCallback);

    return S_OK;
}
