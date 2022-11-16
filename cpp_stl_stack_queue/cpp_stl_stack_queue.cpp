// cpp_stl_stack_queue.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <iostream>
#include <stack>
#include <queue>
using namespace std;

/*
* stl의 stack, queue
* 
*   동작명세가 정해져있는 자료구조
* 
*   stack:LIFO Last Input First Output
*   queue:FIFO First Input First Output
*/

int main()
{
    //stack
    stack<int> tStack;

    tStack.push(3);
    tStack.push(2);
    tStack.push(1);
    tStack.push(777);

    while (!tStack.empty())
    {
        cout << tStack.top() << endl;

        tStack.pop();
    }

    cout << endl;
    //queue
    queue<int> tQueue;

    tQueue.push(3);
    tQueue.push(2);
    tQueue.push(1);
    tQueue.push(777);

    while (!tQueue.empty())
    {
        cout << tQueue.front() << endl;

        tQueue.pop();
    }

    //---------------------------------
    // 
    // //stack, queue는 동작명세가 정해져있다. 즉, 순회 동작은 정해져 있지 않으므로 불가능하다.
    //stack<int>::iterator tItor;
    //queue<int>::iterator tItor;

    return 0;
}