#include "CAPIEngine.h"

#include "Resource.h"

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
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIENGINE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
}

MSG CAPIEngine::Run()
{
    OnCreate();     //<--상속구조에 다형성을 이용한다

    //HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_WINAPIENGINE));

    MSG msg = { 0 };    //MSG구조체 타입의 지역변수 초기화

    // 기본 메시지 루프입니다:
    /*while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }*/

    //'게임 루프game loop'를 고려한 구조로 개조
    while (WM_QUIT != msg.message)  //<--종료메시지가 아니라면 처리, 종료메시지면 종료
    {
        //메시지 큐에서 메시지를 가져와 처리

        //즉, 윈도우 응용프로그램에 대한 처리는 di절에서
        //그 외에 idle time은 '게임세계의 갱신 처리'에 쓰겠다 라는 것이다 <-- idle time 이라는 것이 중요하다

        //PeekMessage는 메시지큐에 메시지가 있으면 바로 '참'을 리턴한다.
        //메시지큐에 메시지가 없다면 대기하지 않고 바로 '거짓'을 리턴한다.
        //      두번째매개변수는 윈도우 핸들인데 nullptr을 넣어주면 된다
        //      세번째, 네번째는 키보드, 마우스 의 입력을 받을 때만 범위지정으로 쓰는 것인데 여기서는 기본값을 넣는다
        //      <--PM_REMOVE옵션은 메세지를 읽어들이고 나서 메시지큐에서 해당 메시지를 제거하겠다는 옵션이다.
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            //참
            //윈도우 데스크탑 응용프로그램 처리
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            //거짓
            //IDLE_TIME
            //게임 프로그램의 처리
            OnUpdate();
        }
    }

    OnDestroy();    //<--상속구조에 다형성을 이용한다

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

void CAPIEngine::OnUpdate()
{
    //OutputDebugString(L"CAPIEngine::OnUpdate\n");
}

ATOM CAPIEngine::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    //<--- WndProc에 Static을 적용하여 전역함수화하였다. 왜냐하면, IpfnWndProc가 전역함수 형식을 원하기 때문이다.
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIENGINE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPIENGINE);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

    //클라이언트 영역의 크기를 지정하자
    RECT tRect = {0, 0, 800, 600};
    //다음 함수를 호출하면 타이틀바, 메뉴 등까지 고려하여 다시 window rect를 계산해준다
    AdjustWindowRect(&tRect, WS_EX_OVERLAPPEDWINDOW, FALSE);

    //새롭게 갱신된 window rect 데이터를 가지고 윈도우의 위치를 지정하자
    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, tRect.right - tRect.left, tRect.bottom - tRect.top,
        SWP_NOMOVE | SWP_NOZORDER);

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
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
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