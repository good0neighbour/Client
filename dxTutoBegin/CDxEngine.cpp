#include "CDxEngine.h"


//HINSTANCE CDxEngine::hInst = nullptr;


CDxEngine::CDxEngine()
{
    //멤버변수 초기화
    memset(szTitle, 0, sizeof(WCHAR) * MAX_LOADSTRING);
    memset(szWindowClass, 0, sizeof(WCHAR) * MAX_LOADSTRING);
}
CDxEngine::~CDxEngine()
{

}

BOOL CDxEngine::Create(HINSTANCE hInstance, int nCmdShow)
{
    // 전역 문자열을 초기화합니다.
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadStringW(hInstance, IDC_WINAPIENGINE, szWindowClass, MAX_LOADSTRING);

    lstrcpyW(szTitle, L"winMatchPuzzle Title");
    lstrcpyW(szWindowClass, L"winMatchPuzzle");

    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    return TRUE;
}
//MSG CDxEngine::Run()
//{
//    OnCreate();     //<--상속구조에 다형성을 이용한다
//
//
//    HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_WINAPIENGINE));
//
//    MSG msg = { 0 };    //MSG구조체 타입의 지역변수 초기화
//
//    // 기본 메시지 루프입니다:
//    while (GetMessage(&msg, nullptr, 0, 0))
//    {
//        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//    }
//
//
//    OnDestroy();   //<--상속구조에 다형성을 이용한다
//
//    return msg;
//}

MSG CDxEngine::Run()
{
    mhDC = GetDC(mhWnd);            //현재 화면 DC를 얻는다
                                                    //GetDC는 어디서든 현재 화면  DC를 얻는  window api함수이다.

    //DirectX 초기화
    InitDevice();

    OnCreate();     //<--상속구조에 다형성을 이용한다

    MSG msg = { 0 };    //MSG구조체 타입의 지역변수 초기화


        
    QueryPerformanceFrequency(&mTickPerSecond);         //초당 틱수 얻기 
    //현재 틱수를 얻는다
    QueryPerformanceCounter(&mTime);
    //'게임 루프game loop'를 고려한 구조로 개조
    //              <--- 게임루프 패턴을 적용했다.
    while (WM_QUIT != msg.message)      //<--종료메시지가 아니라면 처리, 종료메시지이면 종료 
    {
        if (PeekMessage(&msg, nullptr, 0,0, PM_REMOVE)) 
        {
            //참
            //윈도우 데스크탑 응용프로그램 처리
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {            
            //game loop
            LARGE_INTEGER tTime;
            //현재 틱수를 얻는다
            QueryPerformanceCounter(&tTime);
            //한프레임에 걸린 시간을 측정한다.
            mDeltaTime = (float)(tTime.QuadPart - mTime.QuadPart) / (float)mTickPerSecond.QuadPart;
            /*
               예를 들면

               초당 틱이 100 이 나온다고 가정하고

               tTime이 300
               mTime이 290 이라고 가정하면
               mDeltaTime = (300 - 290)/100 = 10/100 = 1/10 = 0.1초
               이것이 한 프레임에 걸리는 실제시간이다
           */

            //이제 현재 틱이 이전 틱이 된다.
            mTime = tTime;

            //mDeltaTime을 게임플레이 로직 수준의 게임코드에서는 조작하지 못하도록 매개변수로 넘겼다. 
            OnUpdate(mDeltaTime);     //<--상속구조에 다형성을 이용한다
        }
    }


    OnDestroy();   //<--상속구조에 다형성을 이용한다

    //DirectX해제
    CleanupDevice();

    ReleaseDC(mhWnd, mhDC);
    

    return msg;
}


void CDxEngine::OnCreate()
{
    OutputDebugString(L"CDxEngine::OnCreate\n");
}
void CDxEngine::OnDestroy()
{
    OutputDebugString(L"CDxEngine::OnDestroy\n");
}
void CDxEngine::OnUpdate(float tDeltaTime)
{
    
}

void CDxEngine::Clear(float tR, float tG, float tB)
{    
    mpImmediateContext->ClearRenderTargetView(mpRenderTargetView, Colors::MidnightBlue);
}
//double buffer pattern을 적용
void CDxEngine::Present()
{
    mpSwapChain->Present(0, 0);
}

ATOM CDxEngine::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;     
    //<--- WndProc에 static을 적용하여 전역함수화하였다. 왜냐하면, lpfnWndProc가 전역함수 형식을 원하기 때문이다.
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, nullptr);//MAKEINTRESOURCE(IDI_WINAPIENGINE));     //<--
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_WINAPIENGINE);       //<--
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, nullptr);//MAKEINTRESOURCE(IDI_SMALL));     //<--

    return RegisterClassExW(&wcex);
}

BOOL CDxEngine::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    //멤버변수에 기억
    mhWnd = hWnd;

    //의도하는 클라이언트 영역client rect의 크기를 지정하자
    RECT tRect = {0,0, 800, 600};
    //다음 함수를 호출하면 타이틀바, 메뉴 등까지 고려하여 다시 window rect를 계산해준다
    AdjustWindowRect(&tRect, WS_OVERLAPPEDWINDOW, FALSE);

    //새롭게 갱신된 window rect 데이터를 가지고 윈도우의 위치를 지정하자
    SetWindowPos(hWnd, HWND_TOPMOST, 0,0, tRect.right - tRect.left, tRect.bottom - tRect.top, 
        SWP_NOMOVE | SWP_NOZORDER) ;



    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK CDxEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
    {
        //윈도우 타이머가 작동하는지만 테스트하자
        OutputDebugString(L"------------------WM_TIMER Enemy DoFire!!----\n");
    }
    break;


    
    case WM_PAINT:
    {
        //BeginPaint~ EndPaint는 WM_PAINT메시지부에서만 유효하다.

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);        //DC Device Context(WindowsOS에서 제공하는 그리기장치의 추상화된 도구) 의 handle을 얻는다
        
        EndPaint(hWnd, &ps);            //DC를 해제
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK CDxEngine::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

HRESULT CDxEngine::InitDevice()
{
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
        D3D_DRIVER_TYPE_HARDWARE,   //그래픽 카드 장치(GPU + VRAM)에서 렌더링 처리
        D3D_DRIVER_TYPE_WARP,       //고속 성능의 소프트웨어 렌더링 처리, HARDWARE에서 온전히 렌더링 처리가 되지 않는 경우 최선의 방법으로 소프트웨어적으로 처리 <-- dx 9.1~10.1까지만 지원
        D3D_DRIVER_TYPE_REFERENCE,  //CPU에서 완전히 소프트웨어적으로 렌더링 처리, 매우 느림, 개발용
        //( 앞에 단계의 드라이버들의 동작이 모두 실패해도 이것으로 가능하다. 어떤 경우라도 작동 )
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    //피쳐레벨: '그래픽 처리 장치가 어떤 다이렉트 엑스 버전을 지원하는지에 대한 정의다.
    //GPU가 지원하는 기능의 정의
    // 기본적으로 상위의 피쳐레벨은 하위의 피쳐레벨을 포함한다.
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
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 1;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
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

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr))
        return hr;

    hr = mpd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mpRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
        return hr;

    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, nullptr);

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

void CDxEngine::CleanupDevice()
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