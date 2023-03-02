#include "CDxEngine.h"




//HINSTANCE CDxEngine::hInst = nullptr;


CDxEngine::CDxEngine()
{
    //������� �ʱ�ȭ
    memset(szTitle, 0, sizeof(WCHAR) * MAX_LOADSTRING);
    memset(szWindowClass, 0, sizeof(WCHAR) * MAX_LOADSTRING);
}
CDxEngine::~CDxEngine()
{

}

BOOL CDxEngine::Create(HINSTANCE hInstance, int nCmdShow)
{
    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadStringW(hInstance, IDC_WINAPIENGINE, szWindowClass, MAX_LOADSTRING);

    lstrcpyW(szTitle, L"winMatchPuzzle Title");
    lstrcpyW(szWindowClass, L"winMatchPuzzle");

    MyRegisterClass(hInstance);

    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    return TRUE;
}
//MSG CDxEngine::Run()
//{
//    OnCreate();     //<--��ӱ����� �������� �̿��Ѵ�
//
//
//    HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_WINAPIENGINE));
//
//    MSG msg = { 0 };    //MSG����ü Ÿ���� �������� �ʱ�ȭ
//
//    // �⺻ �޽��� �����Դϴ�:
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
//    OnDestroy();   //<--��ӱ����� �������� �̿��Ѵ�
//
//    return msg;
//}

MSG CDxEngine::Run()
{
    mhDC = GetDC(mhWnd);            //���� ȭ�� DC�� ��´�
                                                    //GetDC�� ��𼭵� ���� ȭ��  DC�� ���  window api�Լ��̴�.

    //DirectX�ʱ�ȭ
    InitDevice();




    OnCreate();     //<--��ӱ����� �������� �̿��Ѵ�

    MSG msg = { 0 };    //MSG����ü Ÿ���� �������� �ʱ�ȭ


        
    QueryPerformanceFrequency(&mTickPerSecond);         //�ʴ� ƽ�� ��� 
    //���� ƽ���� ��´�
    QueryPerformanceCounter(&mTime);
    //'���� ����game loop'�� ����� ������ ����
    //              <--- ���ӷ��� ������ �����ߴ�.
    while (WM_QUIT != msg.message)      //<--����޽����� �ƴ϶�� ó��, ����޽����̸� ���� 
    {
        if (PeekMessage(&msg, nullptr, 0,0, PM_REMOVE)) 
        {
            //��
            //������ ����ũž �������α׷� ó��
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {            
            //game loop
            LARGE_INTEGER tTime;
            //���� ƽ���� ��´�
            QueryPerformanceCounter(&tTime);
            //�������ӿ� �ɸ� �ð��� �����Ѵ�.
            mDeltaTime = (float)(tTime.QuadPart - mTime.QuadPart) / (float)mTickPerSecond.QuadPart;
            /*
               ���� ���

               �ʴ� ƽ�� 100 �� ���´ٰ� �����ϰ�

               tTime�� 300
               mTime�� 290 �̶�� �����ϸ�
               mDeltaTime = (300 - 290)/100 = 10/100 = 1/10 = 0.1��
               �̰��� �� �����ӿ� �ɸ��� �����ð��̴�
           */

            //���� ���� ƽ�� ���� ƽ�� �ȴ�.
            mTime = tTime;

            //mDeltaTime�� �����÷��� ���� ������ �����ڵ忡���� �������� ���ϵ��� �Ű������� �Ѱ��. 
            OnUpdate(mDeltaTime);     //<--��ӱ����� �������� �̿��Ѵ�
        }
    }


    OnDestroy();   //<--��ӱ����� �������� �̿��Ѵ�

    //DirectX����
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
    //XM~ DirectX Math
    XMVECTORF32 tColor;
    tColor.f[0] = tR;
    tColor.f[1] = tG;
    tColor.f[2] = tB;


    mpImmediateContext->ClearRenderTargetView(mpRenderTargetView, tColor);

    //���̹��۴� ó���� 1�� �����ϰڴ�
    mpImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CDxEngine::Clear(XMVECTORF32 tColor)
{
    mpImmediateContext->ClearRenderTargetView(mpRenderTargetView, tColor);

    //���̹��۴� ó���� 1�� �����ϰڴ�
    mpImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
//double buffer pattern�� ����
void CDxEngine::Present()
{
    mpSwapChain->Present(0, 0);


    /*
        SyncInterval

        bitblt
        0 - The presentation occurs immediately, there is no synchronization.
        ���������̼��� ��� �߻��ϸ� ����ȭ���� �ʴ´�

        flip
        0 - Cancel the remaining time on the previously presented frame and discard this frame if a newer frame is queued.
        ������ ǥ�õ� �������� ���� ���� �ð��� ����ϰ� 
        ���ο� �������� ��� ���Ѵ�

        Flags
        Present a frame from each buffer (starting with the current buffer) to the output.
        ����ü�ο��� ������ ������ ������ �����͸� �����Ѵ�.
    
    */
}






ATOM CDxEngine::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;     
    //<--- WndProc�� static�� �����Ͽ� �����Լ�ȭ�Ͽ���. �ֳ��ϸ�, lpfnWndProc�� �����Լ� ������ ���ϱ� �����̴�.
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
    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    //��������� ���
    mhWnd = hWnd;

    //�ǵ��ϴ� Ŭ���̾�Ʈ ����client rect�� ũ�⸦ ��������
    RECT tRect = {0,0, 800, 600};
    //���� �Լ��� ȣ���ϸ� Ÿ��Ʋ��, �޴� ����� ����Ͽ� �ٽ� window rect�� ������ش�
    AdjustWindowRect(&tRect, WS_OVERLAPPEDWINDOW, FALSE);

    //���Ӱ� ���ŵ� window rect �����͸� ������ �������� ��ġ�� ��������
    SetWindowPos(hWnd, HWND_TOPMOST, 0,0, tRect.right - tRect.left, tRect.bottom - tRect.top, 
        SWP_NOMOVE | SWP_NOZORDER) ;



    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK CDxEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
    {
        //������ Ÿ�̸Ӱ� �۵��ϴ����� �׽�Ʈ����
        OutputDebugString(L"------------------WM_TIMER Enemy DoFire!!----\n");
    }
    break;


   
    case WM_PAINT:
    {
        //BeginPaint~ EndPaint�� WM_PAINT�޽����ο����� ��ȿ�ϴ�.

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);        //DC Device Context(WindowsOS���� �����ϴ� �׸�����ġ�� �߻�ȭ�� ����) �� handle�� ��´�
        


        EndPaint(hWnd, &ps);            //DC�� ����
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

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
        D3D_DRIVER_TYPE_HARDWARE,                           //�׷��� ī�� ��ġ(GPU+VRAM)���� ������ ó��
        D3D_DRIVER_TYPE_WARP,                                   //��� ������ ����Ʈ���� ������ ó��, HARDWARE ���� ������ ������ ó���� ���� �ʴ� ��� �ּ��� ������� ����Ʈ���������� ó�� <-- dx 9.1~ 10.1������ ����
        D3D_DRIVER_TYPE_REFERENCE,                          //CPU���� ������ ����Ʈ���������� ������ ó��, �ſ� ����, ���߿�
        //( �տ� �ܰ��� ����̹����� ������ ��� �����ص� �̰����� �����ϴ�. � ���� �۵� )
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    //��ó����: '�׷��� ó�� ��ġ�� � ���̷�Ʈ ���� ������ �����ϴ����� ���� �����̴�. 
    //GPU�� �����ϴ� ��ɼ�Ʈ �� ����
    // �⺻������ ������ ��ó������ ������ ��ó������ �����Ѵ�.
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


    //������� �����ϸ�, �ش� �׷��� ó�� ��ġ�� DirectX�� ��� ���ر��� �����ϴ��� �˾Ƴ� ���̴�. 
    //����̹� Ÿ�� ����, ��ó���� ����


    //DirectX�� Direct3D(d3d)�� device, device context �� '������'�� ����Ѵ�.


    //DXGI DirectX Graphics Infrastructure ���̷�Ʈ���� �׷��� �ý����� ��� ���� 

    /*
        DXGI�� �׷��� ���� ó�������� �غ��  (Direct3D�ʹ� )������ �������̴�.

        // ������۱����� ����ü�ε��� ������?�� �׷��� ���� ó�� ������ ����� ������ ����ϰ� �ִ�
        //���� ���� ���������� Direct3D�� ���յǾ� �ִ� ���������
        //      Direct3D���� �и��Ǿ� ���� ������ ����������� ���������
        //      ( <-- �ֳ��ϸ�, ���� �ش� ������ ����ȭ�Ǿ� �ִ� �Ϲ����� �����̱� �����̴� )

    */


    /*
        COM Compoent Object Model 
            ���̳ʸ� ������ ȣȯ ������ ��ǰ 


        // __uuidof ������ COM��ü�� �ĺ��ڸ� ��� �������̴�. 
        //  ( ��, ���̳ʸ� �����̹Ƿ� Ŭ������ �̸� ���� �𸥴�. �׷��� �͵� ��� �ĺ��� �μ� ������ ���� ���̵�(�ĺ���)�� �ο��صδ� ���̴�.
        //      �� ���̵� ��´� ��� ���̴� )
    
        DirectX�� COM����� ������� ������� ���̴�. 
    */

    /*
    
        mpd3dDevice ==>

        dxgiDevice ==> adapter ==> dxgiFactory             -->     mpSwapChain
        ==>dxgiFactory2 ==>                                          -->      mpSwapChain1


        ���� dxgiFactory2�� ������ٸ�              dx11.1�������� ����
        �׷��� �ʴٸ�                                         dx11�������� ����
    */


    // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
    IDXGIFactory1* dxgiFactory = nullptr;
    {
        IDXGIDevice* dxgiDevice = nullptr;
        hr = mpd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
        if (SUCCEEDED(hr))
        {
            IDXGIAdapter* adapter = nullptr;
            hr = dxgiDevice->GetAdapter(&adapter);          //adater DXGI�������� ��, �׷��� ó�� ��ġ�� �߻�ȭ�س��� ��ü(  ����ͱ��� ���� )
            if (SUCCEEDED(hr))
            {
                hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
                adapter->Release();         //COM��ü�� ������ �� ����ϴ� �Լ� 
            }
            dxgiDevice->Release();
        }
    }
    if (FAILED(hr))
        return hr;

    //dxgiFactory �� �����. 


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
        //����ü�� �������� ����
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
        //����ü�� �������� ����
        DXGI_SWAP_CHAIN_DESC sd = {};           //description
        sd.BufferCount = 1;                     //back buffer������ 1��
        sd.BufferDesc.Width = width;                //������ �ʺ�
        sd.BufferDesc.Height = height;              //������ ���� 
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;       //����Ÿ�� ��¿� ����
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


    //������� ����, SwapChain ���� �Ϸ�



    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr))
        return hr;

    hr = mpd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mpRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
        return hr;

    //������� ����, ���� Ÿ�� �� ���� �Ϸ�

    //OM Output Merger 
    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, nullptr);

    //�ؽ��� ����
    D3D11_TEXTURE2D_DESC descDepth = { };
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;    //���� �ػ��� �ؽ��� �̹����� ����� ���� mipmap�̶�� �Ѵ�
    descDepth.ArraySize = 1;    //
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//���̹���24, ���ٽ�8 -->32, unsigned normalize [0, 1]
    descDepth.SampleDesc.Count = 1;         //���ø��� �ؽ��ĸ� �ٷ� �� ����. ���ø�����.���ø��Լ�?�� 1��
    descDepth.SampleDesc.Quality = 0;       //���ø�����.����Ƽ
    descDepth.Usage = D3D11_USAGE_DEFAULT;  //�⺻ �뵵�� ���۴�
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL; //����/���ٽǹ��ۿ��̴�
    descDepth.CPUAccessFlags = 0;   //cpu������ �����Ѵ�
    descDepth.MiscFlags = 0;        //��Ÿ ���
    //�ؽ��� ����
    //DirectX11���� ����/���ٽǹ��۴� �ؽ��� ���ҽ��� �� ������ ����Ѵ�
    mpd3dDevice->CreateTexture2D(&descDepth, nullptr, &mpDepthStencil);

    //depth/stencil view ����
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;  //������ ������ descDepth�� ������
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;  //2���� ������ �����ͷ� ����
    descDSV.Texture2D.MipSlice = 0;     //�Ӹ� ������ �⺻

    //Depth/StencilView ����
    mpd3dDevice->CreateDepthStencilView(mpDepthStencil, &descDSV, &mpDepthStencilView);
    //Depth/StencilView�� OM�� ����
    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);

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
