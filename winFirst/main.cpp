#include <windows.h>

//step_0
//윈도우 데스크탑 응용프로그램의 시작점은 WinMain함수이다.

//매개변수들
//  HINSTANCE hInstance     <--- 이 응용프로그램의 원본
//  HINSTANCE hPrevInstance <-- 이전 응용프로그램 인스턴스
//  LPWSTR    lpCmdLine     <-- 응용프로그램이 시작될 때 주어지는 옵션
//  int       nCmdShow      <-- 해당 윈도우 응용프로그램이 어떤 상태로 보여질 것인지에 대한 옵션
//
//매개변수로 주어지는 인자들은 '윈도우를 생성하는데 필요한 준비된 자원들'로
//운영체제가 WinMain을 호출하면서 알아서 넘겨준다

//접두사 h가 붙는 것은 '핸들'handle 이라는 개념의 변수이다
//<-- 윈도우 응용프로그램은 직접적으로 자원에 대한 주소를 접근하여 사용할 수 없다.
//      그러므로 포인터 대신 '핸들'이라는 개념을 사용하여 자원resource에 간접적으로 접근한다.
int APIENTRY wWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR    lpCmdLine,
    int       nCmdShow)
{
    return 0;
}