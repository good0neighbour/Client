#include "CD3D.h"



HRESULT CD3D::Create(HWND thWnd)
{
    //윈도우 핸들 설정
    mhWnd = thWnd;


    //directx초기화 

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
        D3D_DRIVER_TYPE_HARDWARE,                           //그래픽 카드 장치(GPU+VRAM)에서 랜더링 처리
        D3D_DRIVER_TYPE_WARP,                                   //고속 성능의 소프트웨어 랜더링 처리, HARDWARE 에서 온전히 랜더링 처리가 되지 않는 경우 최선의 방법으로 소프트웨어적으로 처리 <-- dx 9.1~ 10.1까지만 지원
        D3D_DRIVER_TYPE_REFERENCE,                          //CPU에서 완전히 소프트웨어적으로 랜더링 처리, 매우 느림, 개발용
        //( 앞에 단계의 드라이버들의 동작이 모두 실패해도 이것으로 가능하다. 어떤 경우라도 작동 )
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    //피처레벨: '그래픽 처리 장치가 어떤 다이렉트 엑스 버전을 지원하는지에 대한 정의이다. 
    //GPU가 지원하는 기능세트 의 정의
    // 기본적으로 상위의 피처레벨은 하위의 피처레벨을 포함한다.
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


    //여기까지 진행하면, 해당 그래픽 처리 장치가 DirectX의 어느 수준까지 지원하는지 알아낸 것이다. 
    //드라이버 타입 결정, 피처레벨 결정


    //DirectX의 Direct3D(d3d)에 device, device context 가 '렌더링'을 담당한다.


    //DXGI DirectX Graphics Infrastructure 다이렉트엑스 그래픽 시스템의 기반 구조 

    /*
        DXGI는 그래픽 관련 처리를위해 준비된  (Direct3D와는 )별도의 구조물이다.

        // 더블버퍼구조와 스왑체인등의 범용적?인 그래픽 관련 처리 구조를 만드는 역할을 담당하고 있다
        //예전 이전 버전에서는 Direct3D에 통합되어 있던 기능이지만
        //      Direct3D에서 분리되어 나와 별도의 기능집합으로 만들어졌다
        //      ( <-- 왜냐하면, 거의 해당 구조로 정형화되어 있는 일반적인 형태이기 때문이다 )

    */


    /*
        COM Compoent Object Model
            바이너리 단위의 호환 가능한 부품


        // __uuidof 임의의 COM객체의 식별자를 얻는 연산자이다.
        //  ( 즉, 바이너리 단위이므로 클래스의 이름 등은 모른다. 그러한 것들 대신 식별자 로서 별도의 수를 아이디(식별자)로 부여해두는 것이다.
        //      그 아이디를 얻는다 라는 것이다 )

        DirectX도 COM기술을 기반으로 만들어진 것이다.
    */

    /*

        mpd3dDevice ==>

        dxgiDevice ==> adapter ==> dxgiFactory             -->     mpSwapChain
        ==>dxgiFactory2 ==>                                          -->      mpSwapChain1


        만약 dxgiFactory2가 얻어졌다면              dx11.1버전으로 동작
        그렇지 않다면                                         dx11버전으로 동작
    */


    // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
    IDXGIFactory1* dxgiFactory = nullptr;
    {
        IDXGIDevice* dxgiDevice = nullptr;
        hr = mpd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
        if (SUCCEEDED(hr))
        {
            IDXGIAdapter* adapter = nullptr;
            hr = dxgiDevice->GetAdapter(&adapter);          //adater DXGI관점에서 본, 그래픽 처리 장치를 추상화해놓은 객체(  모니터까지 포함 )
            if (SUCCEEDED(hr))
            {
                hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
                adapter->Release();         //COM객체를 해제할 때 사용하는 함수 
            }
            dxgiDevice->Release();
        }
    }
    if (FAILED(hr))
        return hr;

    //dxgiFactory 를 얻었다. 


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
        //스왑체인 데이터의 설명문
        DXGI_SWAP_CHAIN_DESC1 sd = {};
        sd.Width = width;
        sd.Height = height;
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;

        hr = dxgiFactory2->CreateSwapChainForHwnd(mpd3dDevice, this->mhWnd, &sd, nullptr, nullptr, &mpSwapChain1);
        if (SUCCEEDED(hr))
        {
            hr = mpSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&mpSwapChain));
        }

        dxgiFactory2->Release();
    }
    else
    {
        // DirectX 11.0 systems
        //스왑체인 데이터의 설명문
        DXGI_SWAP_CHAIN_DESC sd = {};           //description
        sd.BufferCount = 1;                     //back buffer갯수는 1개
        sd.BufferDesc.Width = width;                //버퍼의 너비
        sd.BufferDesc.Height = height;              //버퍼의 높이 
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;       //렌더타겟 출력용 버퍼
        sd.OutputWindow = this->mhWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        hr = dxgiFactory->CreateSwapChain(mpd3dDevice, &sd, &mpSwapChain);
    }

    // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
    dxgiFactory->MakeWindowAssociation(this->mhWnd, DXGI_MWA_NO_ALT_ENTER);

    dxgiFactory->Release();

    if (FAILED(hr))
        return hr;


    //여기까지 오면, SwapChain 생성 완료



    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr))
        return hr;

    hr = mpd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mpRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
        return hr;

    //여기까지 오면, 렌더 타겟 뷰 생성 완료

    //OM Output Merger 
    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, nullptr);


    //텍스처 설명
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;        //여러 해상도의 텍스처 이미지를 만드는 것을 mipmap이라고 한다
    descDepth.ArraySize = 1;        //
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//깊이버퍼24, 스텐실8 -->32, unsigned normalize [0,1]
    descDepth.SampleDesc.Count = 1;    //샘플링은 텍스처를 다룰때 보자. 샘플링설명.샘플링함수?는 1개
    descDepth.SampleDesc.Quality = 0;    //샘플링설명.퀄리티
    descDepth.Usage = D3D11_USAGE_DEFAULT;  //기본 용도의 버퍼이다 
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;     //깊이/스텐실버퍼용이다.
    descDepth.CPUAccessFlags = 0;    //cpu접근은 불허한다
    descDepth.MiscFlags = 0;    //기타 등등
    //텍스처 생성
    //DirectX11에서 깊이/스텐실버퍼는 텍스처 리소스의 한 종류로 취급한다
    mpd3dDevice->CreateTexture2D(&descDepth, nullptr, &mpDepthStencil);

    //depth/stencil view 설명
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;     //데이터 포맷은 descDepth를 따른다
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;    //2차원 형태의 데이터로 본다
    descDSV.Texture2D.MipSlice = 0;            //밉맵 수준은 기본

    //Depth/StencilView 생성
    mpd3dDevice->CreateDepthStencilView(mpDepthStencil, &descDSV, &mpDepthStencilView);


    //Depth/StencilView를 OM에 설정
    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);




    //활성, 비활성 상태에 대한 설명
    D3D11_DEPTH_STENCIL_DESC depthEnableDesc;
    D3D11_DEPTH_STENCIL_DESC depthDisableDesc;

    ZeroMemory(&depthEnableDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));//메모리에 0으로 세팅
    depthEnableDesc.DepthEnable = true;
    mpd3dDevice->CreateDepthStencilState(&depthEnableDesc, &mpDepthEnableState);

    ZeroMemory(&depthDisableDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));//메모리에 0으로 세팅
    depthDisableDesc.DepthEnable = false;
    mpd3dDevice->CreateDepthStencilState(&depthDisableDesc, &mpDepthDisableState);









    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    mpImmediateContext->RSSetViewports(1, &vp);
    //RS~ RenderState

    return S_OK;
}
void CD3D::Destroy()
{
    if (mpImmediateContext) mpImmediateContext->ClearState();

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
    //XM~ DirectX Math
    XMVECTORF32 tColor;
    tColor.f[0] = tR;
    tColor.f[1] = tG;
    tColor.f[2] = tB;


    mpImmediateContext->ClearRenderTargetView(mpRenderTargetView, tColor);

    //깊이버퍼는 처음에 1로 세팅하겠다
    mpImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void CD3D::Clear(XMVECTORF32 tColor)
{
    mpImmediateContext->ClearRenderTargetView(mpRenderTargetView, tColor);

    //깊이버퍼는 처음에 1로 세팅하겠다
    mpImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void CD3D::Present()
{
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