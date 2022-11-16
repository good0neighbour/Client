// cpp_stl_priorityQ.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

/*
* priority_queue
* 우선순위 큐
*/
#include <iostream>
#include <queue>
using namespace std;

int main()
{
    //priority_queue 기본 설정은 최대힙
    priority_queue<int> tPriorityQ;

    tPriorityQ.push(8);
    tPriorityQ.push(1);
    tPriorityQ.push(6);
    tPriorityQ.push(2);

    while (!tPriorityQ.empty())
    {
        cout << tPriorityQ.top() << endl;

        tPriorityQ.pop();   //8, 6, 2, 1
    }

    return 0;
}