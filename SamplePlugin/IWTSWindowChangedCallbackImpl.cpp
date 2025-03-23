#include "pch.h"
#include "IWTSWindowChangedCallbackImpl.h"

IFACEMETHODIMP CToyPluginWindowChangedCallbackImpl::WindowChanged(const WTSWindowInfo* windowInfo)
{
    Log(L"CToyPluginWindowChangedCallbackImpl::WindowChanged called.");
    return S_OK;
}
