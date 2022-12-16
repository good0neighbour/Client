#include "CAPIEngine.h"

#include "Resource.h"
#include "CTexture.h"

#include "CInputMgr.h"

HINSTANCE CAPIEngine::hInst = nullptr;


CAPIEngine::CAPIEngine()
{
    //������� �ʱ�ȭ
    memset(szTitle, 0, sizeof(WCHAR) * MAX_LOADSTRING);
    memset(szWindowClass, 0, sizeof(WCHAR) * MAX_LOADSTRING);
}
CAPIEngine::~CAPIEngine()
{

}

BOOL CAPIEngine::Create(HINSTANCE hInstance, int nCmdShow)
{
    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadStringW(hInstance, IDC_WINAPIENGINE, szWindowClass, MAX_LOADSTRING);

    lstrcpyW(szTitle, L"winAPIShootor Title");
    lstrcpyW(szWindowClass, L"winAPIShootor");

    MyRegisterClass(hInstance);

    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    return TRUE;
}
//MSG CAPIEngine::Run()
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

MSG CAPIEngine::Run()
{
    mhDC = GetDC(mhWnd);            //���� ȭ�� DC�� ��´�
                                                    //GetDC�� ��𼭵� ���� ȭ��  DC�� ���  window api�Լ��̴�.

    mpBackBuffer = new CTexture();
    mpBackBuffer->CreateBackBuffer(hInst, mhDC);

    CInputMgr::GetInstance();



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


    CInputMgr::GetInstance()->ReleaseInstance();


    if (nullptr != mpBackBuffer)
    {
        delete mpBackBuffer;
        mpBackBuffer = nullptr;
    }



    ReleaseDC(mhWnd, mhDC);
    

    return msg;
}


void CAPIEngine::OnCreate()
{
    OutputDebugString(L"CAPIEngine::OnCreate\n");
}
void CAPIEngine::OnDestroy()
{
    OutputDebugString(L"CAPIEngine::OnDestroy\n");
}
void CAPIEngine::OnUpdate(float tDeltaTime)
{
    //�Է�ó�� ���� ����
    CInputMgr::GetInstance()->Update();

    //OutputDebugString(L"CAPIEngine::OnUpdate\n");



    //���ڿ�
    //TextOut(mhDC, 0, 0, TEXT("Good Boy."), 9);
    //TextOut(mhDC, 0, 25, TEXT("0102345%$#ufoGHJ"), 16);
    //TextOut(mhDC, 0, 50, TEXT("�ȳ��ϼ���.��ǳ���Դϴ�."), 13);

    //LPCWSTR tszwStr = L"�ѱ��� visual studio community";
    //int tCount = lstrlenW(tszwStr);     //wide string�� ���� ���� ���� �Լ�
    //TextOut(mhDC, 0, 75, tszwStr, tCount);


    ////�簢��
    //Rectangle(mhDC, 200, 200, 200 + 100, 200 + 50);

    ////���� �׸���
    //MoveToEx(mhDC, 550, 200, nullptr);
    //LineTo(mhDC, 600, 300);

    ////�ﰢ�� �׸���
    //MoveToEx(mhDC, 350, 200, nullptr);
    //LineTo(mhDC, 450, 300);
    //LineTo(mhDC, 500, 200);
    //LineTo(mhDC, 350, 200);


    ////Ÿ��
    //Ellipse(mhDC, 400, 100, 400 + 100, 100 + 50);

    ////��
    //Ellipse(mhDC, 0, 400, 0 + 100, 400 + 100);
}


void CAPIEngine::DrawCircle(float tX, float tY, float tRadius)
{
    //Ellipse(mhDC, tX - tRadius, tY - tRadius, tX + tRadius, tY + tRadius);

    //���� �ĸ���ۿ� �׸���
    Ellipse(mpBackBuffer->mhDCMem, tX - tRadius, tY - tRadius, tX + tRadius, tY + tRadius);
}

void CAPIEngine::DrawTexture(float tX, float tY, CTexture* tpTexture)
{
    //���� �ĸ���ۿ� �׸���
    BitBlt(this->mpBackBuffer->mhDCMem,
        tX, tY,
        tpTexture->mBitmapInfo.bmWidth, tpTexture->mBitmapInfo.bmHeight,
        tpTexture->mhDCMem,
        0, 0,
        SRCCOPY);
}

/*
    ((COLORREF)(((BYTE)(r) | ((WORD)((BYTE)(g)) << 8)) | (((DWORD)(BYTE)(b)) << 16)))

    �����: 2����Ʈ �̻� �����͸� ����Ʈ�� � ������ �޸𸮻� ��ġ�ϴ��� ?
    ��Ʋ ����� : x86�迭�� CPU������ ��Ʋ��������� ó���Ѵ�
                    2����Ʈ �̻��� �����͸� ��������Ʈ���� ��ġ�Ѵ�.

    �� �����
                    2����Ʈ �̻��� �����͸� ��������Ʈ���� ��ġ�Ѵ�.
*/
void CAPIEngine::Clear(float tR, float tG, float tB)
{    
    RECT tRect = { 0 };
    //Ŭ���̾�Ʈ ������ ũ�⸦ ������ window api
    GetClientRect(mhWnd, &tRect);

    //step_0
    //Rectangle(mhDC, 0, 0, tRect.right, tRect.bottom);
    //���� �ĸ���ۿ� �׸���.
    //Rectangle(mpBackBuffer->mhDCMem, 0, 0, tRect.right, tRect.bottom);




    //step_1
    // GDI( Graphic Device Interace ) Object
    //Pen �ܰ����� �׸��� �����̴�.
    HPEN hPen;                      //�츮�� ���� ����� ������ ���̴�.
    HPEN hPenOld;                   //�̹� �⺻�����Ǿ� �ִ� ���̴�.

    //Pen ���� ���� ĥ�ϴ� �����̴�.
    HBRUSH hBrush;                  //�츮�� ���� ����� ������ �귯���̴�.
    HBRUSH hBrushOld;               //�̹� �⺻�����Ǿ� �ִ� �귯���̴�.

    HDC thDC = mpBackBuffer->mhDCMem;       //�ĸ������ �޸�DC

    //�Ű������� ������ ���� [0,1]����ȭ�� ��ġ�� �����µ�
    //�����쿡�� �ٷ�� �����Ұ��� [0,255]�� �ٷ�Ƿ�
    //��ġ�� ��ȯ���־���.
    hPen = CreatePen(PS_SOLID, 2, RGB(tR*255, tG*255, tB*255)); //PS_SOLID <--PS PenStyle �Ǽ�
    hBrush = CreateSolidBrush(RGB(tR * 255, tG * 255, tB * 255));

    hPenOld = (HPEN)SelectObject(thDC, hPen);
    hBrushOld = (HBRUSH)SelectObject(thDC, hBrush);

            Rectangle(thDC, 0, 0, tRect.right, tRect.bottom);

    hPen = (HPEN)SelectObject(thDC, hPenOld);
    hBrush = (HBRUSH)SelectObject(thDC, hBrushOld);

    DeleteObject(hBrush);
    DeleteObject(hPen);

}
//double buffer pattern�� ����
void CAPIEngine::Present()
{
    BitBlt(mhDC,                                                   //�������: ����ȭ��DC
        0, 0,
        800, 600,
        this->mpBackBuffer->mhDCMem,            //�ĸ����: ������ �غ��� memoryDC
        0, 0,
        SRCCOPY);
}






ATOM CAPIEngine::MyRegisterClass(HINSTANCE hInstance)
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

BOOL CAPIEngine::InitInstance(HINSTANCE hInstance, int nCmdShow)
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
LRESULT CALLBACK CAPIEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
    {
        //������ Ÿ�̸Ӱ� �۵��ϴ����� �׽�Ʈ����
        OutputDebugString(L"------------------WM_TIMER Enemy DoFire!!----\n");
    }
    break;


    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // �޴� ������ ���� �м��մϴ�:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        //BeginPaint~ EndPaint�� WM_PAINT�޽����ο����� ��ȿ�ϴ�.

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);        //DC Device Context(WindowsOS���� �����ϴ� �׸�����ġ�� �߻�ȭ�� ����) �� handle�� ��´�
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...

        ////���ڿ�
        //TextOut(hdc, 0, 0, TEXT("Good Boy."), 9);
        //TextOut(hdc, 0, 25, TEXT("0102345%$#ufoGHJ"), 16);
        //TextOut(hdc, 0, 50, TEXT("�ȳ��ϼ���.��ǳ���Դϴ�."), 13);

        //LPCWSTR tszwStr = L"�ѱ��� visual studio community";
        //int tCount = lstrlenW(tszwStr);     //wide string�� ���� ���� ���� �Լ�
        //TextOut(hdc, 0, 75, tszwStr, tCount);


        ////�簢��
        //Rectangle(hdc, 200, 200, 200 + 100, 200 + 50);

        ////���� �׸���
        //MoveToEx(hdc, 550, 200, nullptr);
        //LineTo(hdc, 600, 300);

        ////�ﰢ�� �׸���
        //MoveToEx(hdc, 350, 200, nullptr);
        //LineTo(hdc, 450, 300);
        //LineTo(hdc, 500, 200);
        //LineTo(hdc, 350, 200);


        ////Ÿ��
        //Ellipse(hdc, 400, 100, 400 + 100, 100 + 50);

        ////��
        //Ellipse(hdc, 0, 400, 0 + 100, 400 + 100);


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
INT_PTR CALLBACK CAPIEngine::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
