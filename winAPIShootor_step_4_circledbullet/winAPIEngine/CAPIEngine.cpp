#include "CAPIEngine.h"

#include "Resource.h"
#include "CTexture.h"

#include "CInputMgr.h"

HINSTANCE CAPIEngine::hInst = nullptr;


CAPIEngine::CAPIEngine()
{
    //멤버변수 초기화
    memset(szTitle, 0, sizeof(WCHAR) * MAX_LOADSTRING);
    memset(szWindowClass, 0, sizeof(WCHAR) * MAX_LOADSTRING);
}
CAPIEngine::~CAPIEngine()
{

}

BOOL CAPIEngine::Create(HINSTANCE hInstance, int nCmdShow)
{
    // 전역 문자열을 초기화합니다.
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadStringW(hInstance, IDC_WINAPIENGINE, szWindowClass, MAX_LOADSTRING);

    lstrcpyW(szTitle, L"winAPIShootor Title");
    lstrcpyW(szWindowClass, L"winAPIShootor");

    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    return TRUE;
}
//MSG CAPIEngine::Run()
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

MSG CAPIEngine::Run()
{
    mhDC = GetDC(mhWnd);            //현재 화면 DC를 얻는다
                                                    //GetDC는 어디서든 현재 화면  DC를 얻는  window api함수이다.

    mpBackBuffer = new CTexture();
    mpBackBuffer->CreateBackBuffer(hInst, mhDC);

    CInputMgr::GetInstance();



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
    //입력처리 상태 갱신
    CInputMgr::GetInstance()->Update();

    //OutputDebugString(L"CAPIEngine::OnUpdate\n");



    //문자열
    //TextOut(mhDC, 0, 0, TEXT("Good Boy."), 9);
    //TextOut(mhDC, 0, 25, TEXT("0102345%$#ufoGHJ"), 16);
    //TextOut(mhDC, 0, 50, TEXT("안녕하세요.폭풍류입니다."), 13);

    //LPCWSTR tszwStr = L"한글판 visual studio community";
    //int tCount = lstrlenW(tszwStr);     //wide string에 글자 갯수 세기 함수
    //TextOut(mhDC, 0, 75, tszwStr, tCount);


    ////사각형
    //Rectangle(mhDC, 200, 200, 200 + 100, 200 + 50);

    ////선분 그리기
    //MoveToEx(mhDC, 550, 200, nullptr);
    //LineTo(mhDC, 600, 300);

    ////삼각형 그리기
    //MoveToEx(mhDC, 350, 200, nullptr);
    //LineTo(mhDC, 450, 300);
    //LineTo(mhDC, 500, 200);
    //LineTo(mhDC, 350, 200);


    ////타원
    //Ellipse(mhDC, 400, 100, 400 + 100, 100 + 50);

    ////원
    //Ellipse(mhDC, 0, 400, 0 + 100, 400 + 100);
}


void CAPIEngine::DrawCircle(float tX, float tY, float tRadius)
{
    //Ellipse(mhDC, tX - tRadius, tY - tRadius, tX + tRadius, tY + tRadius);

    //이제 후면버퍼에 그린다
    Ellipse(mpBackBuffer->mhDCMem, tX - tRadius, tY - tRadius, tX + tRadius, tY + tRadius);
}

void CAPIEngine::DrawTexture(float tX, float tY, CTexture* tpTexture)
{
    //이제 후면버퍼에 그린다
    BitBlt(this->mpBackBuffer->mhDCMem,
        tX, tY,
        tpTexture->mBitmapInfo.bmWidth, tpTexture->mBitmapInfo.bmHeight,
        tpTexture->mhDCMem,
        0, 0,
        SRCCOPY);
}

/*
    ((COLORREF)(((BYTE)(r) | ((WORD)((BYTE)(g)) << 8)) | (((DWORD)(BYTE)(b)) << 16)))

    엔디언: 2바이트 이상에 데이터를 바이트를 어떤 순서로 메모리상에 배치하느냐 ?
    리틀 엔디언 : x86계열의 CPU에서는 리틀엔디언으로 처리한다
                    2바이트 이상의 데이터를 하위바이트부터 배치한다.

    빅 엔디언
                    2바이트 이상의 데이터를 상위바이트부터 배치한다.
*/
void CAPIEngine::Clear(float tR, float tG, float tB)
{    
    RECT tRect = { 0 };
    //클라이언트 영역에 크기를 얻어오는 window api
    GetClientRect(mhWnd, &tRect);

    //step_0
    //Rectangle(mhDC, 0, 0, tRect.right, tRect.bottom);
    //이제 후면버퍼에 그린다.
    //Rectangle(mpBackBuffer->mhDCMem, 0, 0, tRect.right, tRect.bottom);




    //step_1
    // GDI( Graphic Device Interace ) Object
    //Pen 외곽선을 그리는 도구이다.
    HPEN hPen;                      //우리가 새로 만들어 설정할 펜이다.
    HPEN hPenOld;                   //이미 기본설정되어 있던 펜이다.

    //Pen 바탕 색을 칠하는 도구이다.
    HBRUSH hBrush;                  //우리가 새로 만들어 설정할 브러쉬이다.
    HBRUSH hBrushOld;               //이미 기본설정되어 있던 브러쉬이다.

    HDC thDC = mpBackBuffer->mhDCMem;       //후면버퍼의 메모리DC

    //매개변수로 들어오는 값은 [0,1]정규화한 수치가 들어오는데
    //윈도우에서 다루는 색상요소값은 [0,255]로 다루므로
    //수치를 변환해주었다.
    hPen = CreatePen(PS_SOLID, 2, RGB(tR*255, tG*255, tB*255)); //PS_SOLID <--PS PenStyle 실선
    hBrush = CreateSolidBrush(RGB(tR * 255, tG * 255, tB * 255));

    hPenOld = (HPEN)SelectObject(thDC, hPen);
    hBrushOld = (HBRUSH)SelectObject(thDC, hBrush);

            Rectangle(thDC, 0, 0, tRect.right, tRect.bottom);

    hPen = (HPEN)SelectObject(thDC, hPenOld);
    hBrush = (HBRUSH)SelectObject(thDC, hBrushOld);

    DeleteObject(hBrush);
    DeleteObject(hPen);

}
//double buffer pattern을 적용
void CAPIEngine::Present()
{
    BitBlt(mhDC,                                                   //전면버퍼: 현재화면DC
        0, 0,
        800, 600,
        this->mpBackBuffer->mhDCMem,            //후면버퍼: 별도로 준비한 memoryDC
        0, 0,
        SRCCOPY);
}






ATOM CAPIEngine::MyRegisterClass(HINSTANCE hInstance)
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

BOOL CAPIEngine::InitInstance(HINSTANCE hInstance, int nCmdShow)
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
LRESULT CALLBACK CAPIEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
    {
        //윈도우 타이머가 작동하는지만 테스트하자
        OutputDebugString(L"------------------WM_TIMER Enemy DoFire!!----\n");
    }
    break;


    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
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
        //BeginPaint~ EndPaint는 WM_PAINT메시지부에서만 유효하다.

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);        //DC Device Context(WindowsOS에서 제공하는 그리기장치의 추상화된 도구) 의 handle을 얻는다
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        ////문자열
        //TextOut(hdc, 0, 0, TEXT("Good Boy."), 9);
        //TextOut(hdc, 0, 25, TEXT("0102345%$#ufoGHJ"), 16);
        //TextOut(hdc, 0, 50, TEXT("안녕하세요.폭풍류입니다."), 13);

        //LPCWSTR tszwStr = L"한글판 visual studio community";
        //int tCount = lstrlenW(tszwStr);     //wide string에 글자 갯수 세기 함수
        //TextOut(hdc, 0, 75, tszwStr, tCount);


        ////사각형
        //Rectangle(hdc, 200, 200, 200 + 100, 200 + 50);

        ////선분 그리기
        //MoveToEx(hdc, 550, 200, nullptr);
        //LineTo(hdc, 600, 300);

        ////삼각형 그리기
        //MoveToEx(hdc, 350, 200, nullptr);
        //LineTo(hdc, 450, 300);
        //LineTo(hdc, 500, 200);
        //LineTo(hdc, 350, 200);


        ////타원
        //Ellipse(hdc, 400, 100, 400 + 100, 100 + 50);

        ////원
        //Ellipse(hdc, 0, 400, 0 + 100, 400 + 100);


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
