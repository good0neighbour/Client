#include "CD3D.h"

HRESULT CD3D::Create(HWND thWnd)
{
    mhWnd - thWnd;

    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(mhWnd, &rc);
    UINT width = rc.right - rc.left;            //스크린의 너비
    UINT height = rc.bottom - rc.top;           //스크린의 높이

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,       //그래픽 카드 장치에서 랜더링 처리
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,      //CPU에서 랜더링 처리
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes); //해당 배열의 크기를 안다

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
        //피처레벨을 결정한 것이다. 피처레벨은 우리가 어느 버전의 DirectX를 사용할 것인지 알려준다 
        //이를테면 D3D_FEATURE_LEVEL_11_0 이라면 다이렉트엑스11.0버전을 사용하는 것이다
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
    // swap chain을 만들 것이다
    // 스왑체인 이란 실제로 랜더링할 데이터가 담길 백버퍼와 프론트버퍼를 말한다
    // 화면에 장면을 그린뒤 이 두 버퍼가 서로 바꿔치기 되는 형태가 마치 체인과 닮아 스왑체인 이라는 명칭이 붙었다
    //
    //팩토리 패턴이 적용된 것으로 추정한다
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
        //swap chain desc 설정
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
        sd.BufferCount = 1;                     //하나의 백버퍼만을 사용하도록 한다
        sd.BufferDesc.Width = width;            //백버퍼의 너비와 높이를 설정한다
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  //백버퍼로 일반적인 32bit의 서피스를 지정한다
        sd.BufferDesc.RefreshRate.Numerator = 60;       //주사율: 1초당 몇 장의 백버퍼를 프론트 버퍼와 바꿔치기 하는지를 나타내는 숫자 
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   //백퍼버의 용도를 설정한다. 백버퍼는 랜더링 타겟이다
        sd.OutputWindow = mhWnd;
        //멀티 샘플링을 끈다
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;         //윈도우 모드로 설정한다( 또는 풀스크린 모드도 가능하다 )

        hr = dxgiFactory->CreateSwapChain(mpd3dDevice, &sd, &mpSwapChain);
    }

    // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
    dxgiFactory->MakeWindowAssociation(mhWnd, DXGI_MWA_NO_ALT_ENTER);

    dxgiFactory->Release();

    if (FAILED(hr))
        return hr;

    // Create a render target view
    //swap chain으로부터 백버퍼를 얻는다 
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr))
        return hr;
    //그렇게 얻은 백버퍼를 가지고 랜더타겟뷰를 만든다. 여기까지 하고 나면 백버퍼는 해제한다
    hr = mpd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mpRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
        return hr;

    //OMSetRenderTargets를 사용하여 랜더타겟뷰를 랜더링 파이프라인에 바인딩시킨다
    //(깊이버퍼 스텐실버퍼는 연결하지 않았다)
    //이렇게 하면, 랜더링 파이프라인을 통해 만들어진 랜더링 프레임의 이미지가 이제 랜더타겟뷰 즉 백버퍼에 그려지게 된다 
    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, nullptr);




    //depth
            //깊이버퍼를 위한 description을 만든다
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

    //depth stencil view 를 만든다
    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = mpd3dDevice->CreateDepthStencilView(mpDepthStencil, &descDSV, &mpDepthStencilView);
    if (FAILED(hr))
        return hr;

    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);









    //뷰포트를 설정한다. 뷰포트는 화면에서 랜더링 프레임을 보일 영역이다 
    // 지금은 윈도우 전체 크기와 동일하게 설정하였다
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