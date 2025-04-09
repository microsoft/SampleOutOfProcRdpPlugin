#include "Compositor.h"

// ----------------------------------------------------------------------------
// Constructor / Destructor
// ----------------------------------------------------------------------------
Compositor::Compositor()
    : m_containerHwnd(nullptr)
    , m_compositorHwnd(nullptr)
{
    // Any initialization code you need
}

Compositor::~Compositor()
{
    // Any cleanup code you need
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

// ----------------------------------------------------------------------------
// Static window procedures
// ----------------------------------------------------------------------------
LRESULT CALLBACK Compositor::ContainerWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }

    return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Compositor::CompositorWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;
    case WM_ERASEBKGND:
    {
        // Paint the background yourself if you like, or just return 1
        // after using the brush to fill the client area.
        HDC hdc = reinterpret_cast<HDC>(wParam);
        RECT rc;
        GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));
        return 1;  // tells Windows we handled it
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);

        SetBkMode(hdc, TRANSPARENT);

        DrawText(hdc, L"Compositor Window", -1, &rect,
            DT_SINGLELINE | DT_CENTER | DT_VCENTER);

        EndPaint(hwnd, &ps);
    }
    default:
        return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// ----------------------------------------------------------------------------
// Creates the container window (top-level).
// ----------------------------------------------------------------------------
HRESULT Compositor::CreateContainerWindow()
{
    // Register the window class
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = ContainerWndProc;
    wc.hInstance = m_instance;
    wc.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"TestOPContainerClass";

    if (!::RegisterClassEx(&wc))
    {
        return NULL;
    }

    // Create the window
    m_containerHwnd = ::CreateWindowEx(
        0,                       // Optional window style extensions
        L"TestOPContainerClass",  // Window class
        L"Test Container Window",            // Window title
        WS_OVERLAPPEDWINDOW,       // Window style
        CW_USEDEFAULT,             // x-position
        CW_USEDEFAULT,             // y-position
        800,                       // Width
        600,                       // Height
        NULL,                      // Parent window
        NULL,                      // Menu
        m_instance,  // Instance
        NULL                       // Additional app data
    );

    return m_containerHwnd ? S_OK : ::GetLastError();
}

// ----------------------------------------------------------------------------
// Creates the compositor child window within the container.
// ----------------------------------------------------------------------------
HRESULT Compositor::CreateCompositorWindow()
{
    // Register a window class for the compositor window.
    WNDCLASSA wndclass{};
    wndclass.lpfnWndProc = CompositorWndProc;
    wndclass.hInstance = m_instance;
    wndclass.lpszClassName = "CompositorVdiWindow";
    ::RegisterClassA(&wndclass);

    // Create the compositor window as a child of the container.
    m_compositorHwnd = ::CreateWindowExA(
        0,
        "CompositorVdiWindow",
        "Child Window",
        WS_CHILD | WS_VISIBLE,
        50, 50, 300, 200,
        m_containerHwnd,
        NULL,
        wndclass.hInstance,
        NULL
    );

    return m_compositorHwnd ? S_OK : ::GetLastError();
}

HRESULT Compositor::Run(ComPtr<IWTSWindowParentService> pWindowParentService)
{
    IStream* pStream;
    Log(L"CoMarshalInterThreadInterfaceInStream", ::CoMarshalInterThreadInterfaceInStream(
        __uuidof(IWTSWindowParentService),
        pWindowParentService.Get(),
        &pStream
    ));

    m_thread = std::thread(&Compositor::WorkerThreadMain, this, pStream);

    return S_OK;
}

// ----------------------------------------------------------------------------
// The main STA thread logic. This is your original RunSTAThread code.
// ----------------------------------------------------------------------------
int Compositor::WorkerThreadMain(IStream* pStream)
{
    m_instance = ::GetModuleHandle(nullptr);

    // Initialize COM for this thread
    Log(L"CoInitializeStaThread", ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED));

    // Retrieve IWTSWindowParentService from the stream
    ComPtr<IWTSWindowParentService> pParentService;
    Log(L"CoGetInterfaceAndReleaseStream",
        ::CoGetInterfaceAndReleaseStream(
            pStream,
            __uuidof(IWTSWindowParentService),
            reinterpret_cast<void**>(pParentService.GetAddressOf())));

    // Create the container window
    Log(L"CreateContainerWindow", CreateContainerWindow());
    Log(L"CreateCompositorWindow", CreateCompositorWindow());
    Log(L"SetParent", pParentService->SetParent(NULL, RdpContainerWindow::OPContainer, m_compositorHwnd));

    // Standard message loop
    MSG msg{};
    while (::GetMessageW(&msg, nullptr, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessageW(&msg);
    }

    ::CoUninitialize();
    return static_cast<int>(msg.wParam);
}
