#pragma once

#include <windows.h>
#include <thread>
#include <wrl.h>
#include "IWTSPlugin_h.h"
#include "Log.h"

using Microsoft::WRL::ComPtr;

class Compositor
{
public:
    Compositor();
    ~Compositor();

    HRESULT Run(ComPtr<IWTSWindowParentService> pWindowParentService);
    HRESULT Shutdown();

private:

    int WorkerThreadMain(IStream* pStream);

    static LRESULT CALLBACK ContainerWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK CompositorWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    HRESULT CreateContainerWindow();
    HRESULT CreateCompositorWindow();

private:
    HWND m_containerHwnd;
    HWND m_compositorHwnd;
    HINSTANCE m_instance;
    std::thread m_thread;
};