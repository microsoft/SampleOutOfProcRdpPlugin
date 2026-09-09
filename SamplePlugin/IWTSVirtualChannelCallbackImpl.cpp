#include "pch.h"
#include <sstream>
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

    // The buffer is NOT null-terminated, so build a std::string from the exact byte count.
    std::string data(reinterpret_cast<char*>(pBuffer), cbSize);
    Log("Plugin received: " + data);

    std::istringstream iss(data);
    std::string textMessage;
    int firstNumber = 0;
    int secondNumber = 0;
    iss >> textMessage >> firstNumber >> secondNumber;
    int sum = firstNumber + secondNumber;
    Log("Plugin computed sum: " + std::to_string(firstNumber) + " + " + std::to_string(secondNumber) + " = " + std::to_string(sum));


    const char* message = "Goku left the power pole in between Kami's lookout and Coren Tower.";
    std::string response = std::string(message) + " | Sum = " + std::to_string(sum);
    _pChannel->Write(static_cast<ULONG>(response.size()), (BYTE*)response.c_str(), nullptr);
    
    // Note: We are passing a nullptr here since, this is just a sample. In real world scenario, you should pass a valid HWND of the window which we want to track.
    _pWindowInfoService->SubscribeWindowChanged(nullptr, _pWindowChangedCallback.Get());
    return S_OK;
}

IFACEMETHODIMP CToyPluginVirtualChannelCallbackImpl::OnClose(void)
{
    return S_OK;
}
