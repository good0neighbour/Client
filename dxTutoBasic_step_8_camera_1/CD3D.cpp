#include "CD3D.h"

//
// i) device, context 생성:           그래픽처리장치가 다이렉트엑스의 어느 기능까지 지원하는지 검토
// ii) SwapChain 생성: dxgi~
// iii) RenderTargetView 설정 ( viewport 설정도 포함 )
// 
//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT CD3D::Create(HWND thWnd)
{

    //윈도우 핸들 설정
    mhWnd = thWnd;

    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(this->mhWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,           //그래픽 카드 장치에서 랜더링 처리
        D3D_DRIVER_TYPE_WARP,                   //고속 성능의 소프트웨어 랜더링 처리. HARDWARE에서 온전히 랜더링 처리가 이루어지지 못하는 경우 최선의 방법으로 소프트웨어적으로 랜더링 처리 <-- 9.1~10.1까지만 지원
        D3D_DRIVER_TYPE_REFERENCE,          //CPU에서 완전히 소프트웨어적으로 랜더링 처리, 매우 느림, 개발용( 앞에 단계의 드라이버들이 모두 실패해도 이것으로 가능. 어떤 경우에도 작동 )
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    //피처레벨: '그래픽 처리 장치가 어떤 다이렉트엑스 를 지원하는지'에 대한 정의이다
    // 'GPU가 지원하는 기능세트' 의 정의
    // 기본적으로 상위의 피처레벨은 하위피처레벨의 기능을 포함한다
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

        if (SUCCEEDED(hr))
            break;
    }
    if (FAILED(hr))
        return hr;

    //여기까지 진행하면, 해당 그래픽 처리장치가 DirectX의 어는 수준까지 지원하는지 알아낸 것이다.


    //DXGI DirectX Graphics Infrastructure

    //DXGI는 그래픽 관련 처리를 위해 준비된 (Direct3D와는) 별도의 구조물이다
    // 더블버퍼구조와 스왑체인등의 범용적?인 그래픽 관련 처리 구조를 만드는 역할을 담당하고 있다
    //예전 이전 버전에서는 Direct3D에 통합되어 있던 기능이지만
    //      Direct3D에서 분리되어 나와 별도의 기능집합으로 만들어졌다

    //COM: 바이너리 단위의 호환가능한 부품
    //DirectX도 COM기술을 기반으로 만들어져 있다.
    //
    //__uuidof 임의의 COM객체의 식별자 얻는 연산자이다.

    // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
    IDXGIFactory1* dxgiFactory = nullptr;
    {
        //C++에서 제공하는 형변환 연산자
        // static_cast<T>: 정적 형변환
        // dynamic_cast<T>: 동적 형변환
        // const_cast<T>: 상수속성을 제거하는 형변환
        // reinterpret_cast<T>: C++의 형변환 연산자 중 하나. 어느 타입이라도 형변환 가능하다
        IDXGIDevice* dxgiDevice = nullptr;
        hr = mpd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));//dxgiDevice DXGI계층에서 다루는 device
        if (SUCCEEDED(hr))
        {
            IDXGIAdapter* adapter = nullptr;
            hr = dxgiDevice->GetAdapter(&adapter);  //adapter DXGI계층에서 다루는 그래픽 장치의 추상화된 객체( 모니터까지 포함 )
            if (SUCCEEDED(hr))
            {
                hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
                adapter->Release();
            }
            dxgiDevice->Release();
        }
    }
    if (FAILED(hr))
        return hr;

    // Create swap chain
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
        //해당 메모리의 용도와 특성을 설명
        DXGI_SWAP_CHAIN_DESC1 sd = {};
        sd.Width = width;
        sd.Height = height;
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //32비트 색상 정보를 사용
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   //랜더타겟용으로 만든다
        sd.BufferCount = 1; //back buffer의 갯수

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
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 1; //back buffer의 갯수
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  //32비트 색상 정보를 사용
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//랜더타겟용으로 만든다
        sd.OutputWindow = mhWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        hr = dxgiFactory->CreateSwapChain(mpd3dDevice, &sd, &mpSwapChain);
    }

    // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
    //풀스크린 전환 단축키 Alt+Enter는 사용하지 않겠다.
    dxgiFactory->MakeWindowAssociation(mhWnd, DXGI_MWA_NO_ALT_ENTER);

    dxgiFactory->Release();

    if (FAILED(hr))
        return hr;


    //SwapChain을 만든다.



    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr))
        return hr;

    hr = mpd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mpRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
        return hr;

    //mpImmediateContext 실제 랜더링을 담당하는 객체
    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, nullptr);

    //랜더타겟뷰 설정

    //텍스처 설명
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;            //여러 해상도의 텍스쳐 이미지를 만드는 것을 mipmap이라고 한다
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//깊이버퍼24, 스텐실8 --> 32
    descDepth.SampleDesc.Count = 1;     //샘플링은 텍스처를 다룰때 보자. 샘플링설명. 샘플링함수?는 1개
    descDepth.SampleDesc.Quality = 0;   //샘플링설명.퀄리티
    descDepth.Usage = D3D11_USAGE_DEFAULT;  //기본 용도의 버퍼이다
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL; //깊이/스텐실버퍼용 이다.
    descDepth.CPUAccessFlags = 0;   //cpu접근은 불허한다
    descDepth.MiscFlags = 0;    //기타 등등
    //텍스처생성
    //DirectX11에서 깊이/스텐실버퍼는 텍스처 리소스의 한 종류로 취급한다
    mpd3dDevice->CreateTexture2D(&descDepth, nullptr, &mpDepthStencil);

    //depth/stencil view 설명
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;  //데이터 포맷은 descDepth를 따른다
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;  //2차원 형태의 데이터로 본다
    descDSV.Texture2D.MipSlice = 0;         //밉맵 수준은 기본

    //Depth/StencilView 생성
    mpd3dDevice->CreateDepthStencilView(mpDepthStencil, &descDSV, &mpDepthStencilView);
    //Depth/StencilView를 OM에 설정
    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);

    //설명
    D3D11_DEPTH_STENCIL_DESC depthEnableDesc;
    D3D11_DEPTH_STENCIL_DESC depthDisableDesc;

    //memset 같은거
    //depth buffer 활성 상태 객체 생성
    ZeroMemory(&depthEnableDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthEnableDesc.DepthEnable = true;

    mpd3dDevice->CreateDepthStencilState(&depthEnableDesc, &mpDepthEnableState);

    //depth buffer 비활성 상태 객체 생성
    ZeroMemory(&depthDisableDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthDisableDesc.DepthEnable = false;

    mpd3dDevice->CreateDepthStencilState(&depthDisableDesc, &mpDepthDisableState);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;         //정규화되어있음 [0,1]
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    mpImmediateContext->RSSetViewports(1, &vp);
    //RS Rasterize State

    //뷰포트 설정

    return S_OK;
}


void CD3D::Destroy()
{
    //랜더링 상태들을 클리어
    if (mpImmediateContext) mpImmediateContext->ClearState();

    //객체들을 릴리즈(해제)

    if (mpDepthStencilView) mpDepthStencilView->Release();
    if (mpDepthStencil) mpDepthStencil->Release();
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

    //랜더타겟뷰(여기서는 백버퍼)를 임의의 색상값으로 채운다
    mpImmediateContext->ClearRenderTargetView(mpRenderTargetView, tColor);

    //깊이버퍼의 값들은 모두 일단 1로 설정했다
    mpImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CD3D::Clear(XMVECTORF32 tColor)
{
    //랜더타겟뷰(여기서는 백버퍼)를 임의의 색상값으로 채운다
    mpImmediateContext->ClearRenderTargetView(mpRenderTargetView, tColor);

    //깊이버퍼의 값들은 모두 일단 1로 설정했다
    mpImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CD3D::Present()
{
    //최종적으로 랜더링 결과를 내어서 출력해라
    mpSwapChain->Present(0, 0);
}



void CD3D::DoTurnOnDepthBuffer()
{
    mpImmediateContext->OMSetDepthStencilState(mpDepthEnableState, 1);
}

void CD3D::DoTurnOffDepthBuffer()
{
    mpImmediateContext->OMSetDepthStencilState(mpDepthDisableState, 1);
}