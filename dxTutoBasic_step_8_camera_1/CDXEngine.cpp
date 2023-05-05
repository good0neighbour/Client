#include "CDXEngine.h"



//static ���� �ʱ�ȭ
HINSTANCE CDXEngine::hInst = 0;

ATOM CDXEngine::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, nullptr);//MAKEINTRESOURCE(IDI_WINAPITRANING00));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_WINAPIENGINE);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, nullptr);//MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL CDXEngine::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);


    mhWnd = hWnd;



    if (!hWnd)
    {
        return FALSE;
    }

    //Ŭ���̾�Ʈ����client rect�� 800by600���� ������� �Ѵ�. 
    RECT tRect = {0,0, 800, 600};

    // �־��� rect�� Ŭ���̾�Ʈ �������� ��Ƽ� 
    //Ÿ��Ʋ��, �޴� ����� ����Ͽ� ������ ũ�⸦ �ٽ� ������ش�( tRect ���� )
    AdjustWindowRect(&tRect, WS_OVERLAPPEDWINDOW, FALSE);

    int tWidth = tRect.right - tRect.left;
    int tHeight = tRect.bottom - tRect.top;
    //������ ũ�⸦ �ٽ� �����ߴ�.
    SetWindowPos(hWnd, HWND_TOPMOST, 100, 100, tWidth, tHeight, SWP_NOMOVE | SWP_NOZORDER);
    




    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


void CDXEngine::Create(HINSTANCE hInstance, int nCmdShow)
{
    /*LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIENGINE, szWindowClass, MAX_LOADSTRING);*/

    wsprintf(szTitle, L"winAPI");
    wsprintf(szWindowClass, L"winAPI");

    MyRegisterClass(hInstance);

    InitInstance(hInstance, nCmdShow);
}

MSG CDXEngine::Run()
{
    //�Է°����� ����
    CInputMgr::GetInstance()->Create();


    //�������� ����ȭ��DC�� ��´�.
    //mhDC = GetDC(mhWnd);
    
    //DirectX �ʱ�ȭ �κ�
    //InitDevice();
    mpD3D = new CD3D();
    mpD3D->Create(mhWnd);



    OnCreate();

    
    //����Ÿ�̸� �Լ����� ����ߴ�. 
    QueryPerformanceFrequency(&mSecond);    //1�ʿ� �� tick�̳�?
    QueryPerformanceCounter(&mTime);//�ð��� ����
    //���⼭ �ð� �̶�� ������ '�ý���'�� ������ �� ������ ī��Ʈ, ���� tick�̶�� �Ѵ�


    //HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_WINAPIENGINE));
    MSG msg = { 0 };
    
    while ( WM_QUIT != msg.message)     //���⼭ �ٷ� ���� '������ ���� ����'�� ó��
    {        
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            //DeltaTime( �� �����ӿ� �ɸ��� �ð� ) �� ���Ѵ�
            //DeltaTime(�ð�����) = (���� ������ �ð�ƽ - ������ ������ �ð�ƽ)/1�ʴ� ƽ
            LARGE_INTEGER tTime;
            QueryPerformanceCounter(&tTime);//�ð��� ����

            mDeltaTime = (float)(tTime.QuadPart - mTime.QuadPart) / (float)mSecond.QuadPart;

            mTime = tTime;

            //Ű�Է°����� ����
            CInputMgr::GetInstance()->Update();

            OnUpdate(mDeltaTime);
        }
    }


    OnDestroy();



    //ReleaseDC(mhWnd, mhDC);
    //DirectX ����
    //CleanupDevice();
    if (mpD3D)
    {
        mpD3D->Destroy();

        delete mpD3D;
        mpD3D = nullptr;
    }

    CInputMgr::GetInstance()->ReleaseInstance();


    return msg;
    //return (int)msg.wParam;
}



void CDXEngine::Clear(float tR,float tG, float tB)
{
    mpD3D->Clear(tR, tG, tB);
}

void CDXEngine::Clear(XMVECTORF32 tColor)
{
    mpD3D->Clear(tColor);
}

void CDXEngine::Present()
{
    mpD3D->Present();
}



LRESULT CALLBACK CDXEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_TIMER:
    {
        //OutputDebugString(L">>>>>>WM_TIMER Enemy DoFire~!!!!!\n");
    }
    break;
    
    //�����ΰ� �׸��⸦ �����ϱ� ���� �̺�Ʈ ó���̴�.
    case WM_PAINT:
    {
        PAINTSTRUCT ps; //�������� Ŭ���̾�Ʈ�� �κ������� �׷��� �� �����Ƿ� �� ������ ���� �����͵��� ������ ���̴�.       
        HDC hdc = BeginPaint(hWnd, &ps);    //hDC�� ��´�
        
      


        EndPaint(hWnd, &ps);        //hDC�� ��ȯ�Ѵ�
    }
    break;
    /*case WM_ACTIVATE:
    {

    }
    break;
    case WM_ACT*/
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



void CDXEngine::OnUpdate(float tDeltaTime)
{
}

// i) device, context ����:           �׷���ó����ġ�� ���̷�Ʈ������ ��� ��ɱ��� �����ϴ��� ����
// ii) SwapChain ����: dxgi~
// iii) RenderTargetView ���� ( viewport ������ ���� )
// 
//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
//HRESULT CDXEngine::InitDevice()
//{
//    HRESULT hr = S_OK;
//
//    RECT rc;
//    GetClientRect(this->mhWnd, &rc);
//    UINT width = rc.right - rc.left;
//    UINT height = rc.bottom - rc.top;
//
//    UINT createDeviceFlags = 0;
//#ifdef _DEBUG
//    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
//
//    D3D_DRIVER_TYPE driverTypes[] =
//    {
//        D3D_DRIVER_TYPE_HARDWARE,           //�׷��� ī�� ��ġ���� ������ ó��
//        D3D_DRIVER_TYPE_WARP,                   //��� ������ ����Ʈ���� ������ ó��. HARDWARE���� ������ ������ ó���� �̷������ ���ϴ� ��� �ּ��� ������� ����Ʈ���������� ������ ó�� <-- 9.1~10.1������ ����
//        D3D_DRIVER_TYPE_REFERENCE,          //CPU���� ������ ����Ʈ���������� ������ ó��, �ſ� ����, ���߿�( �տ� �ܰ��� ����̹����� ��� �����ص� �̰����� ����. � ��쿡�� �۵� )
//    };
//    UINT numDriverTypes = ARRAYSIZE(driverTypes);
//
//    //��ó����: '�׷��� ó�� ��ġ�� � ���̷�Ʈ���� �� �����ϴ���'�� ���� �����̴�
//    // 'GPU�� �����ϴ� ��ɼ�Ʈ' �� ����
//    // �⺻������ ������ ��ó������ ������ó������ ����� �����Ѵ�
//    D3D_FEATURE_LEVEL featureLevels[] =
//    {
//        D3D_FEATURE_LEVEL_11_1,
//        D3D_FEATURE_LEVEL_11_0,
//        D3D_FEATURE_LEVEL_10_1,
//        D3D_FEATURE_LEVEL_10_0,
//    };
//    UINT numFeatureLevels = ARRAYSIZE(featureLevels);
//
//    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
//    {
//        mDriverType = driverTypes[driverTypeIndex];
//        hr = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
//            D3D11_SDK_VERSION, &mpd3dDevice, &mFeatureLevel, &mpImmediateContext);
//
//        if (hr == E_INVALIDARG)
//        {
//            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
//            hr = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
//                D3D11_SDK_VERSION, &mpd3dDevice, &mFeatureLevel, &mpImmediateContext);
//        }
//
//        if (SUCCEEDED(hr))
//            break;
//    }
//    if (FAILED(hr))
//        return hr;
//
//    //������� �����ϸ�, �ش� �׷��� ó����ġ�� DirectX�� ��� ���ر��� �����ϴ��� �˾Ƴ� ���̴�.
//
//
//    //DXGI DirectX Graphics Infrastructure
//
//    //DXGI�� �׷��� ���� ó���� ���� �غ�� (Direct3D�ʹ�) ������ �������̴�
//    // ������۱����� ����ü�ε��� ������?�� �׷��� ���� ó�� ������ ����� ������ ����ϰ� �ִ�
//    //���� ���� ���������� Direct3D�� ���յǾ� �ִ� ���������
//    //      Direct3D���� �и��Ǿ� ���� ������ ����������� ���������
//
//    //COM: ���̳ʸ� ������ ȣȯ������ ��ǰ
//    //DirectX�� COM����� ������� ������� �ִ�.
//    //
//    //__uuidof ������ COM��ü�� �ĺ��� ��� �������̴�.
//
//    // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
//    IDXGIFactory1* dxgiFactory = nullptr;
//    {
//        //C++���� �����ϴ� ����ȯ ������
//        // static_cast<T>: ���� ����ȯ
//        // dynamic_cast<T>: ���� ����ȯ
//        // const_cast<T>: ����Ӽ��� �����ϴ� ����ȯ
//        // reinterpret_cast<T>: C++�� ����ȯ ������ �� �ϳ�. ��� Ÿ���̶� ����ȯ �����ϴ�
//        IDXGIDevice* dxgiDevice = nullptr;
//        hr = mpd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));//dxgiDevice DXGI�������� �ٷ�� device
//        if (SUCCEEDED(hr))
//        {
//            IDXGIAdapter* adapter = nullptr;
//            hr = dxgiDevice->GetAdapter(&adapter);  //adapter DXGI�������� �ٷ�� �׷��� ��ġ�� �߻�ȭ�� ��ü( ����ͱ��� ���� )
//            if (SUCCEEDED(hr))
//            {
//                hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
//                adapter->Release();
//            }
//            dxgiDevice->Release();
//        }
//    }
//    if (FAILED(hr))
//        return hr;
//
//    // Create swap chain
//    IDXGIFactory2* dxgiFactory2 = nullptr;
//    hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
//    if (dxgiFactory2)
//    {
//        // DirectX 11.1 or later
//        hr = mpd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&mpd3dDevice1));
//        if (SUCCEEDED(hr))
//        {
//            (void)mpImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&mpImmediateContext1));
//        }
//        //�ش� �޸��� �뵵�� Ư���� ����
//        DXGI_SWAP_CHAIN_DESC1 sd = {};
//        sd.Width = width;
//        sd.Height = height;
//        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //32��Ʈ ���� ������ ���
//        sd.SampleDesc.Count = 1;
//        sd.SampleDesc.Quality = 0;
//        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   //����Ÿ�ٿ����� �����
//        sd.BufferCount = 1; //back buffer�� ����
//
//        hr = dxgiFactory2->CreateSwapChainForHwnd(mpd3dDevice, mhWnd, &sd, nullptr, nullptr, &mpSwapChain1);
//        if (SUCCEEDED(hr))
//        {
//            hr = mpSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&mpSwapChain));
//        }
//
//        dxgiFactory2->Release();
//    }
//    else
//    {
//        // DirectX 11.0 systems
//        DXGI_SWAP_CHAIN_DESC sd = {};
//        sd.BufferCount = 1; //back buffer�� ����
//        sd.BufferDesc.Width = width;
//        sd.BufferDesc.Height = height;
//        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  //32��Ʈ ���� ������ ���
//        sd.BufferDesc.RefreshRate.Numerator = 60;
//        sd.BufferDesc.RefreshRate.Denominator = 1;
//        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//����Ÿ�ٿ����� �����
//        sd.OutputWindow = mhWnd;
//        sd.SampleDesc.Count = 1;
//        sd.SampleDesc.Quality = 0;
//        sd.Windowed = TRUE;
//
//        hr = dxgiFactory->CreateSwapChain(mpd3dDevice, &sd, &mpSwapChain);
//    }
//
//    // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
//    //Ǯ��ũ�� ��ȯ ����Ű Alt+Enter�� ������� �ʰڴ�.
//    dxgiFactory->MakeWindowAssociation(mhWnd, DXGI_MWA_NO_ALT_ENTER);
//
//    dxgiFactory->Release();
//
//    if (FAILED(hr))
//        return hr;
//
//
//    //SwapChain�� �����.
//
//
//
//    // Create a render target view
//    ID3D11Texture2D* pBackBuffer = nullptr;
//    hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
//    if (FAILED(hr))
//        return hr;
//
//    hr = mpd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mpRenderTargetView);
//    pBackBuffer->Release();
//    if (FAILED(hr))
//        return hr;
//
//    //mpImmediateContext ���� �������� ����ϴ� ��ü
//    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, nullptr);
//
//    //����Ÿ�ٺ� ����
//
//    //�ؽ�ó ����
//    D3D11_TEXTURE2D_DESC descDepth = {};
//    descDepth.Width = width;
//    descDepth.Height = height;
//    descDepth.MipLevels = 1;            //���� �ػ��� �ؽ��� �̹����� ����� ���� mipmap�̶�� �Ѵ�
//    descDepth.ArraySize = 1;
//    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//���̹���24, ���ٽ�8 --> 32
//    descDepth.SampleDesc.Count = 1;     //���ø��� �ؽ�ó�� �ٷ궧 ����. ���ø�����. ���ø��Լ�?�� 1��
//    descDepth.SampleDesc.Quality = 0;   //���ø�����.����Ƽ
//    descDepth.Usage = D3D11_USAGE_DEFAULT;  //�⺻ �뵵�� �����̴�
//    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL; //����/���ٽǹ��ۿ� �̴�.
//    descDepth.CPUAccessFlags = 0;   //cpu������ �����Ѵ�
//    descDepth.MiscFlags = 0;    //��Ÿ ���
//    //�ؽ�ó����
//    //DirectX11���� ����/���ٽǹ��۴� �ؽ�ó ���ҽ��� �� ������ ����Ѵ�
//    mpd3dDevice->CreateTexture2D(&descDepth, nullptr, &mpDepthStencil);
//
//    //depth/stencil view ����
//    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
//    descDSV.Format = descDepth.Format;  //������ ������ descDepth�� ������
//    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;  //2���� ������ �����ͷ� ����
//    descDSV.Texture2D.MipSlice = 0;         //�Ӹ� ������ �⺻
//    
//    //Depth/StencilView ����
//    mpd3dDevice->CreateDepthStencilView(mpDepthStencil, &descDSV, &mpDepthStencilView);
//    //Depth/StencilView�� OM�� ����
//    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);
//
//
//
//
//    // Setup the viewport
//    D3D11_VIEWPORT vp;
//    vp.Width = (FLOAT)width;
//    vp.Height = (FLOAT)height;
//    vp.MinDepth = 0.0f;         //����ȭ�Ǿ����� [0,1]
//    vp.MaxDepth = 1.0f;
//    vp.TopLeftX = 0;
//    vp.TopLeftY = 0;
//    mpImmediateContext->RSSetViewports(1, &vp);
//    //RS Rasterize State
//
//    //����Ʈ ����
//
//    return S_OK;
//}
//
//
//void CDXEngine::CleanupDevice()
//{
//    //������ ���µ��� Ŭ����
//    if (mpImmediateContext) mpImmediateContext->ClearState();
//
//    //��ü���� ������(����)
//
//    if (mpDepthStencilView) mpDepthStencilView->Release();
//    if (mpDepthStencil) mpDepthStencil->Release();
//    if (mpRenderTargetView) mpRenderTargetView->Release();
//    if (mpSwapChain1) mpSwapChain1->Release();
//    if (mpSwapChain) mpSwapChain->Release();
//    if (mpImmediateContext1) mpImmediateContext1->Release();
//    if (mpImmediateContext) mpImmediateContext->Release();
//    if (mpd3dDevice1) mpd3dDevice1->Release();
//    if (mpd3dDevice) mpd3dDevice->Release();
//}