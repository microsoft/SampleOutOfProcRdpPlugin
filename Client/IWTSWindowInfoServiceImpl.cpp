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

IFACEMETHODIMP CToyPluginWindowInfoServiceImpl::SubscribeWindowChanged(HWND remoteHwnd, IWTSWindowChangedCallback* windowChanged)
{

    std::cout << "CToyPluginWindowInfoServiceImpl::SubscribeWindowChanged called" << std::endl;
    return S_OK;
}

IFACEMETHODIMP CToyPluginWindowInfoServiceImpl::UnsubscribeWindowChanged(HWND remoteHwnd, IWTSWindowChangedCallback* windowChanged)
{
    return E_NOTIMPL;
}
