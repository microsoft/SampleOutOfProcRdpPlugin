#define UNICODE
#define _UNICODE
#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wrl/client.h>
#include <string>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "ole32")

using Microsoft::WRL::ComPtr;

// ------------------------------------------------------------
// Globals
// ------------------------------------------------------------
HINSTANCE               g_hInstance = nullptr;
HWND                    g_hWnd = nullptr;

ComPtr<ID2D1Factory>            g_d2dFactory;            // D2D factory
ComPtr<IDWriteFactory>          g_dwriteFactory;         // DWrite factory
ComPtr<ID2D1HwndRenderTarget>   g_renderTarget;          // Render target bound to HWND
ComPtr<ID2D1SolidColorBrush>    g_brush;                 // A simple brush
ComPtr<IDWriteTextFormat>       g_textFormat;            // Text format (font, size, etc.)

// Text to render
const wchar_t* g_text = L"Hello, DirectX (Direct2D + DirectWrite)!";

// Forward declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT CreateDeviceIndependentResources();
HRESULT CreateDeviceResources();
void    DiscardDeviceResources();
void    OnPaint();
void    OnResize(UINT width, UINT height);

// ------------------------------------------------------------
// Entry point
// ------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    g_hInstance = hInstance;
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    // (Optional) Basic DPI awareness for sharper text on high-DPI screens.
    SetProcessDPIAware();

    // Register window class
    const wchar_t CLASS_NAME[] = L"D2DTextSampleWindowClass";
    WNDCLASS wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = g_hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (!RegisterClass(&wc))
        return 0;

    // Create window
    g_hWnd = CreateWindowEx(
        0, CLASS_NAME, L"DirectX Text (Direct2D + DirectWrite)",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 450,
        nullptr, nullptr, g_hInstance, nullptr);

    if (!g_hWnd)
        return 0;

    ShowWindow(g_hWnd, nCmdShow);

    // Initialize factories (device-independent resources)
    if (FAILED(CreateDeviceIndependentResources()))
        return 0;

    // Main message loop
    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DiscardDeviceResources();
    g_textFormat.Reset();
    g_dwriteFactory.Reset();
    g_d2dFactory.Reset();

    CoUninitialize();
    return static_cast<int>(msg.wParam);
}

// ------------------------------------------------------------
// Direct2D/DirectWrite creation
// ------------------------------------------------------------
HRESULT CreateDeviceIndependentResources()
{
    // D2D factory
    D2D1_FACTORY_OPTIONS options{};
#if defined(_DEBUG)
    options.debugLevel = D2D1_DEBUG_LEVEL_ERROR;
#endif

    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, g_d2dFactory.GetAddressOf());
    if (FAILED(hr)) return hr;

    // DWrite factory
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(g_dwriteFactory.GetAddressOf()));
    if (FAILED(hr)) return hr;

    // Text format
    hr = g_dwriteFactory->CreateTextFormat(
        L"Segoe UI",           // Font family
        nullptr,               // Font collection (system)
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        32.0f,                 // Font size (DIPs)
        L"en-us",              // Locale
        g_textFormat.GetAddressOf());
    if (FAILED(hr)) return hr;

    // Center the text
    g_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    g_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    return S_OK;
}

HRESULT CreateDeviceResources()
{
    if (g_renderTarget) return S_OK; // already created

    // Query current client size
    RECT rc{};
    GetClientRect(g_hWnd, &rc);
    const D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    // Create an HWND render target
    D2D1_RENDER_TARGET_PROPERTIES rtProps =
        D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE),
            96.0f, 96.0f);

    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps =
        D2D1::HwndRenderTargetProperties(g_hWnd, size, D2D1_PRESENT_OPTIONS_NONE);

    HRESULT hr = g_d2dFactory->CreateHwndRenderTarget(rtProps, hwndProps, g_renderTarget.GetAddressOf());
    if (FAILED(hr)) return hr;

    // Create a solid color brush
    hr = g_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
        g_brush.GetAddressOf());
    return hr;
}

void DiscardDeviceResources()
{
    g_brush.Reset();
    g_renderTarget.Reset();
}

// ------------------------------------------------------------
// Paint & Resize
// ------------------------------------------------------------
void OnPaint()
{
    if (FAILED(CreateDeviceResources()))
        return;

    g_renderTarget->BeginDraw();

    // Clear background (dark gray)
    g_renderTarget->Clear(D2D1::ColorF(0.10f, 0.10f, 0.10f, 1.0f));

    // Layout rectangle spanning the client area
    D2D1_SIZE_F rtSize = g_renderTarget->GetSize();
    D2D1_RECT_F layoutRect = D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height);

    // Draw the text
    g_renderTarget->DrawTextW(
        g_text,
        static_cast<UINT32>(wcslen(g_text)),
        g_textFormat.Get(),
        layoutRect,
        g_brush.Get());

    HRESULT hr = g_renderTarget->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
    {
        // The render target became invalid (e.g., device lost).
        DiscardDeviceResources();
        InvalidateRect(g_hWnd, nullptr, FALSE);
    }
}

void OnResize(UINT width, UINT height)
{
    if (g_renderTarget)
    {
        // Resize the HWND render target
        g_renderTarget->Resize(D2D1::SizeU(width, height));
    }
}

// ------------------------------------------------------------
// Window Procedure
// ------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps{};
        BeginPaint(hWnd, &ps);
        OnPaint();
        EndPaint(hWnd, &ps);
    }
    return 0;

    case WM_SIZE:
        OnResize(LOWORD(lParam), HIWORD(lParam));
        return 0;

    case WM_DISPLAYCHANGE:
        InvalidateRect(hWnd, nullptr, FALSE);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}