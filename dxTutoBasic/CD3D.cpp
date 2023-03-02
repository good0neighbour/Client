#include "CD3D.h"

HRESULT CD3D::Create(HWND thWnd)
{
    mhWnd - thWnd;

    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(mhWnd, &rc);
    UINT width = rc.right - rc.left;            //��ũ���� �ʺ�
    UINT height = rc.bottom - rc.top;           //��ũ���� ����

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,       //�׷��� ī�� ��ġ���� ������ ó��
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,      //CPU���� ������ ó��
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes); //�ش� �迭�� ũ�⸦ �ȴ�

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        mDriverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, &mpd3dDevice, &mFeatureLevel, &mpImmediateContext);

        if (hr == E_INVALIDARG)
        {
            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                D3D11_SDK_VERSION, &mpd3dDevice, &mFeatureLevel, &mpImmediateContext);
        }
        //��ó������ ������ ���̴�. ��ó������ �츮�� ��� ������ DirectX�� ����� ������ �˷��ش� 
        //�̸��׸� D3D_FEATURE_LEVEL_11_0 �̶�� ���̷�Ʈ����11.0������ ����ϴ� ���̴�
        if (SUCCEEDED(hr))
            break;
    }
    if (FAILED(hr))
        return hr;


    //DXGI DirectX Graphics Infrastructure
    // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
    IDXGIFactory1* dxgiFactory = nullptr;
    {
        IDXGIDevice* dxgiDevice = nullptr;
        hr = mpd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
        if (SUCCEEDED(hr))
        {
            IDXGIAdapter* adapter = nullptr;
            hr = dxgiDevice->GetAdapter(&adapter);
            if (SUCCEEDED(hr))
            {
                hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory)); //
                adapter->Release();
            }
            dxgiDevice->Release();
        }
    }
    if (FAILED(hr))
        return hr;

    // Create swap chain
    //
    // swap chain�� ���� ���̴�
    // ����ü�� �̶� ������ �������� �����Ͱ� ��� ����ۿ� ����Ʈ���۸� ���Ѵ�
    // ȭ�鿡 ����� �׸��� �� �� ���۰� ���� �ٲ�ġ�� �Ǵ� ���°� ��ġ ü�ΰ� ��� ����ü�� �̶�� ��Ī�� �پ���
    //
    //���丮 ������ ����� ������ �����Ѵ�
    IDXGIFactory2* dxgiFactory2 = nullptr;
    hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
    if (dxgiFactory2)
    {
        // DirectX 11.1 or later
        hr = mpd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&mpd3dDevice1));
        if (SUCCEEDED(hr))
        {
            (void)mpImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&mpImmediateContext1));
        }
        //swap chain desc ����
        DXGI_SWAP_CHAIN_DESC1 sd = {};
        sd.Width = width;
        sd.Height = height;
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;

        hr = dxgiFactory2->CreateSwapChainForHwnd(mpd3dDevice, mhWnd, &sd, nullptr, nullptr, &mpSwapChain1);
        if (SUCCEEDED(hr))
        {
            hr = mpSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&mpSwapChain));
        }

        dxgiFactory2->Release();
    }
    else
    {
        // DirectX 11.0 systems
        //
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 1;                     //�ϳ��� ����۸��� ����ϵ��� �Ѵ�
        sd.BufferDesc.Width = width;            //������� �ʺ�� ���̸� �����Ѵ�
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  //����۷� �Ϲ����� 32bit�� ���ǽ��� �����Ѵ�
        sd.BufferDesc.RefreshRate.Numerator = 60;       //�ֻ���: 1�ʴ� �� ���� ����۸� ����Ʈ ���ۿ� �ٲ�ġ�� �ϴ����� ��Ÿ���� ���� 
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   //���۹��� �뵵�� �����Ѵ�. ����۴� ������ Ÿ���̴�
        sd.OutputWindow = mhWnd;
        //��Ƽ ���ø��� ����
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;         //������ ���� �����Ѵ�( �Ǵ� Ǯ��ũ�� ��嵵 �����ϴ� )

        hr = dxgiFactory->CreateSwapChain(mpd3dDevice, &sd, &mpSwapChain);
    }

    // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
    dxgiFactory->MakeWindowAssociation(mhWnd, DXGI_MWA_NO_ALT_ENTER);

    dxgiFactory->Release();

    if (FAILED(hr))
        return hr;

    // Create a render target view
    //swap chain���κ��� ����۸� ��´� 
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr))
        return hr;
    //�׷��� ���� ����۸� ������ ����Ÿ�ٺ並 �����. ������� �ϰ� ���� ����۴� �����Ѵ�
    hr = mpd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mpRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
        return hr;

    //OMSetRenderTargets�� ����Ͽ� ����Ÿ�ٺ並 ������ ���������ο� ���ε���Ų��
    //(���̹��� ���ٽǹ��۴� �������� �ʾҴ�)
    //�̷��� �ϸ�, ������ ������������ ���� ������� ������ �������� �̹����� ���� ����Ÿ�ٺ� �� ����ۿ� �׷����� �ȴ� 
    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, nullptr);




    //depth
            //���̹��۸� ���� description�� �����
            // 
            // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = mpd3dDevice->CreateTexture2D(&descDepth, nullptr, &mpDepthStencil);
    if (FAILED(hr))
        return hr;

    //depth stencil view �� �����
    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = mpd3dDevice->CreateDepthStencilView(mpDepthStencil, &descDSV, &mpDepthStencilView);
    if (FAILED(hr))
        return hr;

    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);









    //����Ʈ�� �����Ѵ�. ����Ʈ�� ȭ�鿡�� ������ �������� ���� �����̴� 
    // ������ ������ ��ü ũ��� �����ϰ� �����Ͽ���
    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    mpImmediateContext->RSSetViewports(1, &vp);

    return S_OK;
}
void CD3D::Destroy()
{
    if (mpImmediateContext) mpImmediateContext->ClearState();


    //depth
    if (mpDepthStencil) mpDepthStencil->Release();
    if (mpDepthStencilView) mpDepthStencilView->Release();


    if (mpRenderTargetView) mpRenderTargetView->Release();
    if (mpSwapChain1) mpSwapChain1->Release();
    if (mpSwapChain) mpSwapChain->Release();
    if (mpImmediateContext1) mpImmediateContext1->Release();
    if (mpImmediateContext) mpImmediateContext->Release();
    if (mpd3dDevice1) mpd3dDevice1->Release();
    if (mpd3dDevice) mpd3dDevice->Release();
}

void CD3D::Clear(float tR, float tG, float tB)
{
    XMVECTORF32 tColor;
    tColor.f[0] = tR;
    tColor.f[1] = tG;
    tColor.f[2] = tB;

    mpImmediateContext->ClearRenderTargetView(mpRenderTargetView, tColor);// Colors::DarkGray);

    //depth
    mpImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void CD3D::Clear(XMVECTORF32 tColor)
{
    //ryu_report
    mpImmediateContext->ClearRenderTargetView(mpRenderTargetView, tColor);// Colors::DarkGray);

    //depth
    mpImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void CD3D::Present()
{
    mpSwapChain->Present(0, 0);
}