#include <iostream>
#include <stdarg.h> //<--가변인자 문법을 사용하기 위해 포함. c문법

using namespace std;

//가변인자
//매개변수의 개수가 고정되지 않고 N개인 것에 대한 처리

//첫번째 매개변수: 인자의 갯수, 그 이후는 가변갯수의 인자. 이것은 ...으로 표기한다
int DoAdds(int tCountArg, ...);

int main()
{
    int tResult = 0;

    tResult = DoAdds(2, 3, 2);
    cout << tResult << endl;

    tResult = DoAdds(5, 3, 2, 2, 1, 4);
    cout << tResult << endl;

    tResult = DoAdds(7, 3, 2, 2, 1, 4, 1, 1);
    cout << tResult << endl;

    return 0;
}

int DoAdds(int tCountArg, ...)
{
    int tResult = 0;

    va_list tpArgs; //<--- 포인터 변수다. 실체는 배열(?)

    //가변인자 리스트 사용 시작
    va_start(tpArgs, tCountArg);//<-- 포인터 변수가 ...으로 표기된 것 중 첫번째 매개변수를 가리키게 함

    int tArg = 0;
    for (int ti = 0; ti < tCountArg; ++ti)
    {
        tArg = va_arg(tpArgs, int);//<---매개변수를 하나씩 추출, 타입은 int로 형변환

        tResult = tResult + tArg;
    }

    //가변인자 리스트 사용 끝
    va_end(tpArgs);

    return tResult;
}