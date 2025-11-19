#include "pch.h"
#include "IWTSVirtualChannelCallbackImpl.h"
#include "IWTSWindowChangedCallbackImpl.h"

CToyPluginVirtualChannelCallbackImpl::CToyPluginVirtualChannelCallbackImpl(
    ComPtr<IWTSVirtualChannel> channel, ComPtr<IWTSWindowInfoService> pWindowInfoService) 
    : _pChannel(channel), _pWindowInfoService(pWindowInfoService)
{
    _pWindowChangedCallback = Make<CToyPluginWindowChangedCallbackImpl>();
}

IFACEMETHODIMP CToyPluginVirtualChannelCallbackImpl::OnDataReceived(ULONG cbSize, BYTE* pBuffer)
{

    std::string data(reinterpret_cast<char*>(pBuffer), cbSize);
    std::cout << "Plugin received: " << data << std::endl;

    const char* response = "Goku left the power pole in between Kami's lookout and Coren Tower.";
    _pChannel->Write(strlen(response), (BYTE*)response, nullptr);
    
    // Note: We are passing a nullptr here since, this is just a sample. In real world scenario, you should pass a valid HWND of the window which we want to track.
    _pWindowInfoService->SubscribeWindowChanged(nullptr, _pWindowChangedCallback.Get());
    return S_OK;
}

IFACEMETHODIMP CToyPluginVirtualChannelCallbackImpl::OnClose(void)
{
    return S_OK;
}
