#include "CAPIEngine.h"

#include "Resource.h"

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
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIENGINE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
}

MSG CAPIEngine::Run()
{
    mhDC = GetDC(mhWnd);    //���� ȭ�� DC�� ��´�
                            //GetDC�� ��𼭵� ���� ȭ�� DC�� ��� window api�Լ���.

    OnCreate();     //<--��ӱ����� �������� �̿��Ѵ�

    //HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_WINAPIENGINE));

    MSG msg = { 0 };    //MSG����ü Ÿ���� �������� �ʱ�ȭ

    // �⺻ �޽��� �����Դϴ�:
    /*while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }*/

    //'���� ����game loop'�� ������ ������ ����
    //      <--- ���ӷ��� ������ �����ߴ�.
    while (WM_QUIT != msg.message)  //<--����޽����� �ƴ϶�� ó��, ����޽����� ����
    {
        //�޽��� ť���� �޽����� ������ ó��

        //��, ������ �������α׷��� ���� ó���� di������
        //�� �ܿ� idle time�� '���Ӽ����� ���� ó��'�� ���ڴ� ��� ���̴� <-- idle time �̶�� ���� �߿��ϴ�

        //PeekMessage�� �޽���ť�� �޽����� ������ �ٷ� '��'�� �����Ѵ�.
        //�޽���ť�� �޽����� ���ٸ� ������� �ʰ� �ٷ� '����'�� �����Ѵ�.
        //      �ι�°�Ű������� ������ �ڵ��ε� nullptr�� �־��ָ� �ȴ�
        //      ����°, �׹�°�� Ű����, ���콺 �� �Է��� ���� ���� ������������ ���� ���ε� ���⼭�� �⺻���� �ִ´�
        //      <--PM_REMOVE�ɼ��� �޼����� �о���̰� ���� �޽���ť���� �ش� �޽����� �����ϰڴٴ� �ɼ��̴�.
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            //��
            //������ ����ũž �������α׷� ó��
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            //����
            //IDLE_TIME
            //���� ���α׷��� ó��
            OnUpdate();
        }
    }

    OnDestroy();    //<--��ӱ����� �������� �̿��Ѵ�

    ReleaseDC(mhWnd, mhDC);
    //<--���� ȭ�� DC�� �ڵ��� ������� �����Ѵ�

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

    ////���ڿ�
    //TextOut(mhDC, 0, 0, TEXT("Nationalism"), 11);
    //TextOut(mhDC, 0, 25, TEXT("012345!@#$%^&*()"), 16);
    //TextOut(mhDC, 0, 50, TEXT("�ȳ��ϼ���."), 6);
    //
    //LPCWSTR tszwStr = L"�ѱ� visual studio community";
    //int tCount = lstrlenW(tszwStr); //wide string�� ���� ���� ���� �Լ�
    //TextOut(mhDC, 0, 75, tszwStr, tCount);
    //
    ////�簢��
    //Rectangle(mhDC, 200, 200, 200 + 100, 200 + 50);
    //
    ////���� �׸���
    //MoveToEx(mhDC, 550, 200, nullptr);
    //LineTo(mhDC, 600, 300);
    //
    ////�ﰢ�� �׸���
    //MoveToEx(mhDC, 350, 200, nullptr);
    //LineTo(mhDC, 450, 300);
    //LineTo(mhDC, 500, 200);
    //LineTo(mhDC, 350, 200);
    //
    ////Ÿ��
    //Ellipse(mhDC, 400, 100, 400 + 100, 100 + 50);
    //
    ////��
    //Ellipse(mhDC, 0, 400, 0 + 100, 400 + 100);
}

void CAPIEngine::DrawCircle(float tX, float tY, float tRadius)
{
    Ellipse(mhDC, tX - tRadius, tY - tRadius, tX + tRadius, tY + tRadius);
}

void CAPIEngine::Clear()
{
    RECT tRect = { 0 };
    //Ŭ���̾�Ʈ ������ ũ�⸦ ������ window api
    GetClientRect(mhWnd, &tRect);

    Rectangle(mhDC, 0, 0, tRect.right, tRect.bottom);
}

ATOM CAPIEngine::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    //<--- WndProc�� Static�� �����Ͽ� �����Լ�ȭ�Ͽ���. �ֳ��ϸ�, IpfnWndProc�� �����Լ� ������ ���ϱ� �����̴�.
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
    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    mhWnd = hWnd;

    //Ŭ���̾�Ʈ ������ ũ�⸦ ��������
    RECT tRect = {0, 0, 800, 600};
    //���� �Լ��� ȣ���ϸ� Ÿ��Ʋ��, �޴� ����� �����Ͽ� �ٽ� window rect�� ������ش�
    AdjustWindowRect(&tRect, WS_EX_OVERLAPPEDWINDOW, FALSE);

    //���Ӱ� ���ŵ� window rect �����͸� ������ �������� ��ġ�� ��������
    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, tRect.right - tRect.left, tRect.bottom - tRect.top,
        SWP_NOMOVE | SWP_NOZORDER);

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
        //BeginPaint~ EndPaint�� WM_PAINT�޽����ο����� ��ȿ�ϴ�

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);    //DC Device Context(WindowsOS���� �����ϴ� �׸�����ġ�� �߻�ȭ�� ����)�� handle�� ��´�
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...

        ////���ڿ�
        //TextOut(hdc, 0, 0, TEXT("Nationalism"), 11);
        //TextOut(hdc, 0, 25, TEXT("012345!@#$%^&*()"), 16);
        //TextOut(hdc, 0, 50, TEXT("�ȳ��ϼ���."), 6);

        //LPCWSTR tszwStr = L"�ѱ� visual studio community";
        //int tCount = lstrlenW(tszwStr); //wide string�� ���� ���� ���� �Լ�
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

        EndPaint(hWnd, &ps);    //DC�� ����
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