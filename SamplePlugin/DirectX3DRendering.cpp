// SpinningSphere.cpp
// Build: cl /EHsc SpinningSphere.cpp /DUNICODE /DWIN32 /DWIN32_LEAN_AND_MEAN d3d11.lib d3dcompiler.lib dxgi.lib user32.lib gdi32.lib

#include "DirectX3DRendering.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>


using namespace DirectX;
using namespace::std;

// === Global D3D handles ===
HWND                    g_hWnd{};
IDXGISwapChain*         g_swap{};
ID3D11Device*           g_dev{};
ID3D11DeviceContext*    g_ctx{};
ID3D11RenderTargetView* g_rtv{};
ID3D11DepthStencilView* g_dsv{};
ID3D11Texture2D*        g_ds{};
ID3D11Buffer*           g_vb{};
ID3D11Buffer*           g_ib{};
UINT                    g_indexCount{};
ID3D11VertexShader*     g_vs{};
ID3D11PixelShader*      g_ps{};
ID3D11InputLayout*      g_il{};
ID3D11Buffer*           g_cb{};
UINT                    g_width{};
UINT                    g_height{};
std::atomic_bool        g_running{ false };
std::thread             g_renderThread;

struct Vertex { XMFLOAT3 pos; XMFLOAT3 nrm; };
struct CB
{
    XMMATRIX mvp;
    XMMATRIX model;
    XMFLOAT3 lightDir; float pad0;
    XMFLOAT3 eyePos;   float pad1;
};

void SafeRelease(IUnknown* p) { if (p) p->Release(); }

// === Shader compilation ===
HRESULT CompileShader(const char* src, const char* entry, const char* profile, ID3DBlob** blobOut)
{
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
    flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3DCompile(src, strlen(src), nullptr, nullptr, nullptr, entry, profile, flags, 0, blobOut, &errorBlob);
    if (FAILED(hr) && errorBlob)
        OutputDebugStringA((const char*)errorBlob->GetBufferPointer());
    SafeRelease(errorBlob);
    return hr;
}

// === Generate sphere vertices ===
void CreateSphere(unsigned stacks, unsigned slices, std::vector<Vertex>& outV, std::vector<uint32_t>& outI)
{
    outV.clear(); outI.clear();
    for (unsigned i = 0; i <= stacks; ++i)
    {
        float v = (float)i / (float)stacks;
        float phi = v * XM_PI;
        for (unsigned j = 0; j <= slices; ++j)
        {
            float u = (float)j / (float)slices;
            float theta = u * XM_2PI;
            float x = sinf(phi) * cosf(theta);
            float y = cosf(phi);
            float z = sinf(phi) * sinf(theta);
            XMFLOAT3 n(x, y, z);
            outV.push_back({ XMFLOAT3(x, y, z), n });
        }
    }
    auto idx = [slices](unsigned i, unsigned j) { return i * (slices + 1) + j; };
    for (unsigned i = 0; i < stacks; ++i)
    {
        for (unsigned j = 0; j < slices; ++j)
        {
            uint32_t i0 = idx(i, j);
            uint32_t i1 = idx(i + 1, j);
            uint32_t i2 = idx(i + 1, j + 1);
            uint32_t i3 = idx(i, j + 1);
            outI.push_back(i0); outI.push_back(i1); outI.push_back(i2);
            outI.push_back(i0); outI.push_back(i2); outI.push_back(i3);
        }
    }
}

HRESULT CreateDeviceAndSwapChain(UINT w, UINT h)
{
    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferCount = 2;
    sd.BufferDesc.Width = w;
    sd.BufferDesc.Height = h;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    D3D_FEATURE_LEVEL fls[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
    D3D_FEATURE_LEVEL fl{};
    UINT flags = 0;
#if defined(_DEBUG)
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    return D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        flags, fls, _countof(fls), D3D11_SDK_VERSION, &sd, &g_swap, &g_dev, &fl, &g_ctx);
}

HRESULT CreateRTVAndDSV(UINT w, UINT h)
{
    g_width = w;
    g_height = h;

    ID3D11Texture2D* backBuffer{};
    HRESULT hr = g_swap->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    if (FAILED(hr)) return hr;
    hr = g_dev->CreateRenderTargetView(backBuffer, nullptr, &g_rtv);
    SafeRelease(backBuffer);
    if (FAILED(hr)) return hr;

    D3D11_TEXTURE2D_DESC dsd{};
    dsd.Width = w; dsd.Height = h;
    dsd.MipLevels = 1; dsd.ArraySize = 1;
    dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsd.SampleDesc.Count = 1;
    dsd.Usage = D3D11_USAGE_DEFAULT;
    dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    hr = g_dev->CreateTexture2D(&dsd, nullptr, &g_ds);
    if (FAILED(hr)) return hr;
    hr = g_dev->CreateDepthStencilView(g_ds, nullptr, &g_dsv);
    if (FAILED(hr)) return hr;

    // Set viewport whenever render targets are (re)created
    D3D11_VIEWPORT vp{};
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    vp.Width = static_cast<float>(w);
    vp.Height = static_cast<float>(h);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    g_ctx->RSSetViewports(1, &vp);

    return S_OK;
}

HRESULT CreatePipelineAndGeometry()
{
    const char* vsSrc = R"(
cbuffer CB : register(b0)
{
    float4x4 mvp;
    float4x4 model;
    float3 lightDir; float _pad0;
    float3 eyePos;   float _pad1;
};
struct VSIN { float3 pos:POSITION; float3 nrm:NORMAL; };
struct VSOUT { float4 pos:SV_Position; float3 nrm:NORMAL; float3 worldPos:TEXCOORD0; };
VSOUT main(VSIN i)
{
    VSOUT o;
    float4 wp = mul(float4(i.pos,1), model);
    o.worldPos = wp.xyz;
    o.nrm = normalize(mul(float4(i.nrm,0), model).xyz);
    o.pos = mul(wp, mvp);
    return o;
})";

    const char* psSrc = R"(
cbuffer CB : register(b0)
{
    float4x4 mvp;
    float4x4 model;
    float3 lightDir; float _pad0;
    float3 eyePos;   float _pad1;
};
struct PSIN { float4 pos:SV_Position; float3 nrm:NORMAL; float3 worldPos:TEXCOORD0; };
float4 main(PSIN i) : SV_Target
{
    float3 N = normalize(i.nrm);
    float3 L = normalize(-lightDir);
    float3 V = normalize(eyePos - i.worldPos);
    float3 H = normalize(L + V);
    float diff = saturate(dot(N,L));
    float spec = pow(saturate(dot(N,H)), 32.0);
    float3 color = float3(0.1,0.3,0.8) * (0.2 + 0.8*diff) + spec.xxx;
    return float4(color, 1.0);
})";

    ID3DBlob* vsb{}, * psb{};
    if (FAILED(CompileShader(vsSrc, "main", "vs_5_0", &vsb))) return E_FAIL;
    if (FAILED(CompileShader(psSrc, "main", "ps_5_0", &psb))) { SafeRelease(vsb); return E_FAIL; }

    g_dev->CreateVertexShader(vsb->GetBufferPointer(), vsb->GetBufferSize(), nullptr, &g_vs);
    g_dev->CreatePixelShader(psb->GetBufferPointer(), psb->GetBufferSize(), nullptr, &g_ps);

    D3D11_INPUT_ELEMENT_DESC il[] = {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,  D3D11_INPUT_PER_VERTEX_DATA,0},
        {"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,12, D3D11_INPUT_PER_VERTEX_DATA,0}
    };
    g_dev->CreateInputLayout(il, 2, vsb->GetBufferPointer(), vsb->GetBufferSize(), &g_il);
    SafeRelease(vsb); SafeRelease(psb);

    std::vector<Vertex> verts;
    std::vector<uint32_t> inds;
    CreateSphere(32, 64, verts, inds);
    g_indexCount = (UINT)inds.size();

    D3D11_BUFFER_DESC bd{};
    D3D11_SUBRESOURCE_DATA srd{};

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.ByteWidth = (UINT)(verts.size() * sizeof(Vertex));
    srd.pSysMem = verts.data();
    g_dev->CreateBuffer(&bd, &srd, &g_vb);

    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.ByteWidth = (UINT)(inds.size() * sizeof(uint32_t));
    srd.pSysMem = inds.data();
    g_dev->CreateBuffer(&bd, &srd, &g_ib);

    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.ByteWidth = sizeof(CB);
    g_dev->CreateBuffer(&bd, nullptr, &g_cb);

    return S_OK;
}

void Resize(UINT w, UINT h)
{
    if (!g_ctx || !g_swap) return;
    g_ctx->OMSetRenderTargets(0, nullptr, nullptr);
    SafeRelease(g_rtv);
    SafeRelease(g_dsv);
    SafeRelease(g_ds);
    g_swap->ResizeBuffers(0, w, h, DXGI_FORMAT_UNKNOWN, 0);
    CreateRTVAndDSV(w, h);
}

static void RenderLoop()
{
    auto start = std::chrono::steady_clock::now();
    MSG msg{};
    while (g_running.load())
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) { g_running.store(false); break; }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        float t = std::chrono::duration<float>(std::chrono::steady_clock::now() - start).count();

        XMMATRIX model = XMMatrixRotationY(t);
        XMVECTOR eye = XMVectorSet(0, 0, -3, 1);
        XMVECTOR at = XMVectorZero();
        XMVECTOR up = XMVectorSet(0, 1, 0, 0);
        XMMATRIX view = XMMatrixLookAtLH(eye, at, up);
        XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, g_width / (float)g_height, 0.1f, 100.0f);
        XMMATRIX mvp = model * view * proj;

        CB cb{};
        cb.mvp = XMMatrixTranspose(mvp);
        cb.model = XMMatrixTranspose(model);
        XMStoreFloat3(&cb.lightDir, XMVector3Normalize(XMVectorSet(0.5f, -1.0f, -0.5f, 0)));
        XMStoreFloat3(&cb.eyePos, eye);
        g_ctx->UpdateSubresource(g_cb, 0, nullptr, &cb, 0, 0);

        FLOAT clearColor[4] = { 0.05f, 0.05f, 0.1f, 1.0f };
        g_ctx->ClearRenderTargetView(g_rtv, clearColor);
        g_ctx->ClearDepthStencilView(g_dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
        g_ctx->OMSetRenderTargets(1, &g_rtv, g_dsv);

        UINT stride = sizeof(Vertex), offset = 0;
        g_ctx->IASetInputLayout(g_il);
        g_ctx->IASetVertexBuffers(0, 1, &g_vb, &stride, &offset);
        g_ctx->IASetIndexBuffer(g_ib, DXGI_FORMAT_R32_UINT, 0);
        g_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        g_ctx->VSSetShader(g_vs, nullptr, 0);
        g_ctx->PSSetShader(g_ps, nullptr, 0);
        g_ctx->VSSetConstantBuffers(0, 1, &g_cb);
        g_ctx->PSSetConstantBuffers(0, 1, &g_cb);

        g_ctx->DrawIndexed(g_indexCount, 0, 0);
        g_swap->Present(1, 0);
    }
}

// === Window proc ===
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_DESTROY) { PostQuitMessage(0); return 0; }
    else if (msg == WM_SIZE)
        Resize(LOWORD(lParam), HIWORD(lParam));
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int DirectX3DRender(HINSTANCE hInst, LPWSTR cmdLine, int nCmdShow)
{
    if (g_running.load()) return 0; // Already running

    WNDCLASS wc{ CS_OWNDC, WndProc, 0,0, GetModuleHandle(NULL), nullptr, LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_WINDOW + 1), nullptr, L"SphereWnd" };
    RegisterClass(&wc);
    g_hWnd = CreateWindow(L"SphereWnd", L"Direct3D11 Spinning Sphere", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        100, 100, 800, 600, nullptr, nullptr, wc.hInstance, nullptr);

    RECT rc; GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left, height = rc.bottom - rc.top;

    if (FAILED(CreateDeviceAndSwapChain(width, height))) return -1;
    if (FAILED(CreateRTVAndDSV(width, height))) return -2;
    if (FAILED(CreatePipelineAndGeometry()))   return -3;

    g_running.store(true);
    g_renderThread = std::thread(RenderLoop);
    return 0;
}

void ShutdownDirectX()
{
    if (g_running.exchange(false))
    {
        PostMessage(g_hWnd, WM_QUIT, 0, 0);
        if (g_renderThread.joinable()) g_renderThread.join();
    }
    SafeRelease(g_cb);
    SafeRelease(g_ib);
    SafeRelease(g_vb);
    SafeRelease(g_il);
    SafeRelease(g_vs);
    SafeRelease(g_ps);
    SafeRelease(g_rtv);
    SafeRelease(g_dsv);
    SafeRelease(g_ds);
    SafeRelease(g_swap);
    SafeRelease(g_ctx);
    SafeRelease(g_dev);
}
