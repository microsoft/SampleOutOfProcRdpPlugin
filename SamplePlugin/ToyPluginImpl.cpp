#include "pch.h"
#include "ToyPluginImpl.h"
#include "IWTSVirtualChannelCallbackImpl.h"
#include "../MidlGeneratedFiles/IWTSPlugin_h.h"
#include "DirectX3DRendering.hpp"
#include "TermSrvNotification.hpp"

// Inherited via IWTSPlugin
// Called by the Remote Desktop Services subsystem (specifically, the Remote Desktop Session Host process) 
// when the plugin is loaded during a Remote Desktop session initialization.
// IWTSVirtualChannelManager is thus, passed to the plugin by the Remote Desktop Services subsystem.
IFACEMETHODIMP CToyPluginImpl::Initialize(IWTSVirtualChannelManager* pChannelMgr)
{
    std::wcout << "CToyPluginImpl::Initialize started" << std::endl;
    // pChannelMgr->QueryInterface(IID_PPV_ARGS(&_pPluginServiceProvider));
    // _pPluginServiceProvider->GetService(RDCLIENT_WINDOW_INFO_SERVICE, &_pWindowInfoService);

    unsigned char* pChannelName = (unsigned char*)"SamplePluginChannel";
    std::wcout << "Creating listener for channel SamplePluginChannel." << std::endl;
    auto hr =  pChannelMgr->CreateListener(pChannelName, 0, this, _pListener.ReleaseAndGetAddressOf());
    std::wcout << "CToyPluginImpl::Initialize ended\n" << std::endl;
    return hr;
}

IFACEMETHODIMP CToyPluginImpl::Connected(void)
{
    std::wcout << "CToyPluginImpl::Connected started" << std::endl;
    std::wcout << "CToyPluginImpl::Connected ended\n" << std::endl;

    std::cout << "Registering for session notifications." << std::endl;
    GetSessionNotifications();
    std::cout << "Registered for session notifications." << std::endl;

    std::cout << "Rendering using DirectX" << std::endl;
    DirectX3DRender(GetModuleHandle(NULL), GetCommandLineW(), SW_SHOW);
    std::cout << "Rendered using DirectX" << std::endl;

    return S_OK;
}

IFACEMETHODIMP CToyPluginImpl::Disconnected(DWORD dwDisconnectCode)
{
    std::wcout << "CToyPluginImpl::Disconnected started" << std::endl;
    std::wcout << "CToyPluginImpl::Disconnected ended\n" << std::endl;
    return S_OK;
}

IFACEMETHODIMP CToyPluginImpl::Terminated(void)
{
    std::wcout << "CToyPluginImpl::Terminated started" << std::endl;
    _pListener->Release();
    _pWindowInfoService->Release();
    _pPluginServiceProvider->Release();
    std::wcout << "CToyPluginImpl::Terminated ended\n" << std::endl;
    return S_OK;
}

// Inherited via IWTSListenerCallback
IFACEMETHODIMP CToyPluginImpl::OnNewChannelConnection(
    IWTSVirtualChannel* pChannel,
    BSTR data,
    BOOL* pbAccept,
    IWTSVirtualChannelCallback** ppCallback)
{
    std::wcout << "CToyPluginImpl::OnNewChannelConnection started" << std::endl;
    // Assuming the connection is accepted trivialy, since, this is a toy plugin.
    *pbAccept = true;

    // Create a callback instance which is called when channel gets some data.
    ComPtr<IWTSVirtualChannelCallback> pVirtualChannelCallback = 
        Make<CToyPluginVirtualChannelCallbackImpl>(pChannel, _pWindowInfoService);
    pVirtualChannelCallback.CopyTo(ppCallback);

    std::wcout << "CToyPluginImpl::OnNewChannelConnection ended\n" << std::endl;

    return S_OK;
}
