#include "pch.h"
#include "IWTSWindowInfoServiceImpl.h"

IFACEMETHODIMP CToyPluginWindowInfoServiceImpl::GetWindowInfo(HWND remoteHwnd)
{
    return E_NOTIMPL;
}

IFACEMETHODIMP CToyPluginWindowInfoServiceImpl::GetRdpClientProcessId()
{
    return E_NOTIMPL;
}

IFACEMETHODIMP CToyPluginWindowInfoServiceImpl::GetRdpSessionType()
{
    return E_NOTIMPL;
}

IFACEMETHODIMP CToyPluginWindowInfoServiceImpl::SubscribeWindowChanged(HWND remoteHwnd, IWTSWindowChangedCallback* windowChangedCallback)
{

    std::cout << "CToyPluginWindowInfoServiceImpl::SubscribeWindowChanged called" << std::endl;
    windowChangedCallback->WindowChanged(nullptr);
    return S_OK;
}

IFACEMETHODIMP CToyPluginWindowInfoServiceImpl::UnsubscribeWindowChanged(HWND remoteHwnd, IWTSWindowChangedCallback* windowChangedCallback)
{
    return E_NOTIMPL;
}
