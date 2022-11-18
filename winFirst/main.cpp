#include <windows.h>

//step_0
//������ ����ũž �������α׷��� �������� WinMain�Լ��̴�.

//�Ű�������
//  HINSTANCE hInstance     <--- ������ �������α׷� �ĺ��� �ڵ�. ���� ������ ������� ��� ����. //�� �������α׷��� ����
//  HINSTANCE hPrevInstance <-- �� �����찡 ������� ���� �������� �ڵ� //���� �������α׷� �ν��Ͻ�
//  LPWSTR    lpCmdLine     <-- �������α׷��� ���۵� �� �־����� �ɼ�   LPWSTR long pointer wide string
//  int       nCmdShow      <-- �ش� ������ �������α׷��� � ���·� ������ �������� ���� �ɼ�
//
//�Ű������� �־����� ���ڵ��� '�����츦 �����ϴµ� �ʿ��� �غ�� �ڿ���'��
//�ü���� WinMain�� ȣ���ϸ鼭 �˾Ƽ� �Ѱ��ش�

//���λ� h�� �ٴ� ���� '�ڵ�'handle �̶�� ������ �����̴�
//<-- ������ �������α׷��� ���������� �ڿ��� ���� �ּҸ� �����Ͽ� ����� �� ����.
//      �׷��Ƿ� ������ ��� '�ڵ�'�̶�� ������ ����Ͽ� �ڿ�resource�� ���������� �����Ѵ�.
//int APIENTRY wWinMain(HINSTANCE hInstance,
//    HINSTANCE hPrevInstance,
//    LPWSTR    lpCmdLine,
//    int       nCmdShow)
//{
//    return 0;
//}

//�̷��� �������α׷� ������
//�ý��� Ŀ�� ������ �и��� ������
//������ �ü���� '��Ƽ�׽�ŷ' ���α׷��� ������ �� �� �Ѵ�
//��, ������ �� �������α׷��� �߸��� ������ ���ߴٰ� �ؼ�
//�ü���� �߸��� �ൿ�� ���̸� �� �ǹǷ�
//���� ���α׷��� �ڿ��� ������� �����ϰ� �ִ� �����̴�
//
//�ܼ� �������α׷��� �������̴�.
//-------�ü���� �����ϴ� �ڿ���. '�ڵ�handle'�̶�� �������� �ٷ��-------

//      ����, �����쿡�� �����ϴ� �ܼ��� ������ �������� �ܼ��� �ƴ϶� ������ ���α׷����μ��� �ܼ� ���ķ����ʹ�



//step_1
// <---------- ������ Ŭ����
//  <--���������Ÿ�� Ŭ������ �ƴϴ�. '�з�'�� �� �׸��̶�� �ǹ̴�

//��, ������ Ŭ������ ������ ���������α׷��� ���� �� '�Ϻ�'�� ��Ÿ���� ����
//
//'������ Ŭ����'��� ������ �ִ� ������
//������鿡�� �����ϰ� �ٷ������ ��, '�ߺ��� �����͵�'�� �����Ͽ� �����ϱ� ���� �����̴�.
//�׷���, �ü�� ���忡�� ���� ȿ�������� �ڿ��� �����Ǳ� �����̴�. ( ��)�޸𸮰� ���� )
//
//�̸��׸� ���´� ���� �����͸��Ӥ�

//int APIENTRY wWinMain(HINSTANCE hInstance,
//    HINSTANCE hPrevInstance,
//    LPWSTR    lpCmdLine,
//    int       nCmdShow)
//{
//    //�ü���� �ý��� Ŀ�ο� ���� �����Ǵ� ������ ���� '�ڿ�'�̹Ƿ� �����ͷ� �����ϴ� ���� �ƴ�
//    //�̷��� ������ ������ ��Ģ�� �����͸� ���������ν� �ü������ �����ϴ� '������'��� �ڿ���
//    //����� �����ϰ� �ȴ�
//    //<-- window api�� ������� ���´�� ���̵��ϴ´�� �Ѵ�.
//
//    //������ Ŭ���� ����
//    WNDCLASSEXW wcex;   //������ Ŭ���� ����ü Ÿ���� ����
//
//    wcex.cbSize = sizeof(WNDCLASSEX);       //cb~ class buffer
//
//    wcex.style = CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc = nullptr;//WndProc;   //�Լ� ������ lpfnWndProc�� �Լ��� ��� lpfn~ long pointer function�� �ǹ�
//    wcex.cbClsExtra = 0;                    //������ Ŭ������ ���� ������ �޸�
//    wcex.cbWndExtra = 0;                    //�����츦 ���� ������ �޸�
//    wcex.hInstance = hInstance;             //�ش� �������� �ĺ���
//    //wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
//    wcex.hIcon = LoadIcon(hInstance, nullptr);
//    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
//    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//    //wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);    //lpsz~ long pointer string zero
//    wcex.lpszMenuName = nullptr;
//    wcex.lpszClassName = L"winFirst";//szWindowClass;               //������ Ŭ���� �̵�,   L�� �ٿ� �����ڵ�(2byte���� ǥ�� ���)�� ������Ų��.
//    wcex.hIconSm = LoadIcon(wcex.hInstance, nullptr);//MAKEINTRESOURCE(IDI_SMALL));
//
//    RegisterClassExW(&wcex);        //������ Ŭ���� ������ �����Ͽ� �ü���� ���
//
//    return 0;
//}



//step_2
//<----------������ ����

//int APIENTRY wWinMain(HINSTANCE hInstance,
//    HINSTANCE hPrevInstance,
//    LPWSTR    lpCmdLine,
//    int       nCmdShow)
//{
//    HINSTANCE hInst;
//    LPCWSTR szWindowClass = L"winFirst";
//    LPCWSTR szTitle = L"title";
//
//    //�ü���� �ý��� Ŀ�ο� ���� �����Ǵ� ������ ���� '�ڿ�'�̹Ƿ� �����ͷ� �����ϴ� ���� �ƴ�
//    //�̷��� ������ ������ ��Ģ�� �����͸� ���������ν� �ü������ �����ϴ� '������'��� �ڿ���
//    //����� �����ϰ� �ȴ�
//    //<-- window api�� ������� ���´�� ���̵��ϴ´�� �Ѵ�.
//
//    //������ Ŭ���� ����
//    WNDCLASSEXW wcex;   //������ Ŭ���� ����ü Ÿ���� ����
//
//    wcex.cbSize = sizeof(WNDCLASSEX);       //cb~ class buffer
//
//    wcex.style = CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc = nullptr;//WndProc;   //�Լ� ������ lpfnWndProc�� �Լ��� ��� lpfn~ long pointer function�� �ǹ�
//    wcex.cbClsExtra = 0;                    //������ Ŭ������ ���� ������ �޸�
//    wcex.cbWndExtra = 0;                    //�����츦 ���� ������ �޸�
//    wcex.hInstance = hInstance;             //�ش� �������� �ĺ���
//    //wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
//    wcex.hIcon = LoadIcon(hInstance, nullptr);
//    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
//    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//    //wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);    //lpsz~ long pointer string zero
//    wcex.lpszMenuName = nullptr;
//    wcex.lpszClassName = szWindowClass;//szWindowClass;               //������ Ŭ���� �̵�,   L�� �ٿ� �����ڵ�(2byte���� ǥ�� ���)�� ������Ų��.
//    wcex.hIconSm = LoadIcon(wcex.hInstance, nullptr);//MAKEINTRESOURCE(IDI_SMALL));
//
//    RegisterClassExW(&wcex);        //������ Ŭ���� ������ �����Ͽ� �ü���� ���
//
//
//
//    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.
//
//    //������ �����츦 �����Ѵ�.
//    //������ �ڵ�hWnd(������ �ĺ���)�� ����
//    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);//<--CW~ CreateWindow
//
//    //hInstance�� â�� �޶� ������ â �����̸� �ϳ��̴�
//    //hWnd�� ������ ������ �ϴ��� â���� ��������� ���̴�
//
//    //CreateWindowW�� �����츦 �����. �� �Լ��� ȣ���ϸ� WM_CREATE�޼����� ��������
//    //���� '�޼�������' �ڵ忡 �������� ���ߴ�.
//    //�� WM_CREATE�� �޼������� �ڵ尡 �۵��ϱ� ���� ��������.
//    //'�޽���ť'�� ��� ���̴�
//    //
//    //�� �ڵ�� ����� ������ ����. <-- CreateWindow�� ȣ���ϸ� WM_CREATE�� �߻�, �׷��� ó���� �� ��.
//
//    if (!hWnd)
//    {
//        return FALSE;
//    }
//
//    ShowWindow(hWnd, nCmdShow);     //<--������ ���̱�( ���̳�/�Ⱥ��̳� )
//    UpdateWindow(hWnd);             //<--������ ȭ���� �����Ѵ�
//
//    return 0;
//}



//step_3

// <----------�̺�Ʈ �帮��event driven,
//              �޽���message, �޽������� Message loop, ������ ���ν��� window procedure, �޽���ťMessage Queue

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
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

int APIENTRY wWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR    lpCmdLine,
    int       nCmdShow)
{
    HINSTANCE hInst;
    LPCWSTR szWindowClass = L"winFirst";
    LPCWSTR szTitle = L"title";

    //�ü���� �ý��� Ŀ�ο� ���� �����Ǵ� ������ ���� '�ڿ�'�̹Ƿ� �����ͷ� �����ϴ� ���� �ƴ�
    //�̷��� ������ ������ ��Ģ�� �����͸� ���������ν� �ü������ �����ϴ� '������'��� �ڿ���
    //����� �����ϰ� �ȴ�
    //<-- window api�� ������� ���´�� ���̵��ϴ´�� �Ѵ�.

    //������ Ŭ���� ����
    WNDCLASSEXW wcex;   //������ Ŭ���� ����ü Ÿ���� ����

    wcex.cbSize = sizeof(WNDCLASSEX);       //cb~ class buffer

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;   //�Լ� ������ lpfnWndProc�� �Լ��� ��� lpfn~ long pointer function�� �ǹ�
    wcex.cbClsExtra = 0;                    //������ Ŭ������ ���� ������ �޸�
    wcex.cbWndExtra = 0;                    //�����츦 ���� ������ �޸�
    wcex.hInstance = hInstance;             //�ش� �������� �ĺ���
    //wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hIcon = LoadIcon(hInstance, nullptr);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    //wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);    //lpsz~ long pointer string zero
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;//szWindowClass;               //������ Ŭ���� �̵�,   L�� �ٿ� �����ڵ�(2byte���� ǥ�� ���)�� ������Ų��.
    wcex.hIconSm = LoadIcon(wcex.hInstance, nullptr);//MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);        //������ Ŭ���� ������ �����Ͽ� �ü���� ���

    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

    //������ �����츦 �����Ѵ�.
    //������ �ڵ�hWnd(������ �ĺ���)�� ����
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);//<--CW~ CreateWindow

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);     //<--������ ���̱�( ���̳�/�Ⱥ��̳� )
    UpdateWindow(hWnd);             //<--������ ȭ���� �����Ѵ�



    HACCEL hAccelTable = LoadAccelerators(hInstance, nullptr);// MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;    //<--������ �޽��� ����ü: �̺�Ʈ�� �����쿡�� �޼������ Ī�ϰ� �ִ�.
                //�̺�Ʈ: ��ɾ�� �����͸� ���� �߻�ȭ�س��� ����

    // �⺻ �޽��� �����Դϴ�:
    //      Get~: ��block ����Լ�
    //      Post~ : �ͺ�nonblock ���� �Լ�
    while (GetMessage(&msg, nullptr, 0, 0))//GetMessage ������ �޽��� ť�κ��� �޽����� ���´�
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) //���������� ó���� �켱���� �Ѵ�.
        {
            TranslateMessage(&msg);     //�޽����� �ؼ�(����)�Ѵ�.
            DispatchMessage(&msg);      //������ ���ν����� �Ѱ� ó���� �����Ѵ�.
        }
    }

    return (int)msg.wParam;
}