/*
    순차 검색 sequential search
*/

#include <iostream>
using namespace std;

//순차 검색 알고리즘
void SequentialSearch(int tArray[], int tLength, int tData)
{
    int ti = 0;

    while (ti < tLength && tData != tArray[ti]) //자료구조의 마지막 원소가 아님 && 찾으려는 값이 아님
    {
        ti++;
    }

    //찾으려는 값을 찾았음
    //OR
    //자료구조의 마지막 원소까지 모두 비교해봄
    if (ti < tLength)
    {
        cout << ti + 1 << "번째 만에 " << "검색 성공" << endl;
        cout << tArray[ti] << endl;
    }
    else
    {
        cout << ti + 1 << "번째 만에 " << "검색 실패" << endl;
    }
}

//이 함수는 원소들의 값이 모두 '오름차순정렬'되어 있다고 가정하고 작성한 경우이다
void SequentialSearch_1(int tArray[], int tLength, int tData)
{
    int ti = 0;

    while (ti < tData) //찾으려는 값이 원소의 값보다 크거나 같을 때까지 반복하여 값 비교
    {
        ti++;
    }

    //원소의 값이
    //찾으려는 값보다 크거나 같은 것이다
    if (tArray[ti] == tData)
    {
        cout << ti + 1 << "번째 만에 " << "검색 성공" << endl;
        cout << tArray[ti] << endl;
    }
    else
    {
        cout << ti + 1 << "번째 만에 " << "검색 실패" << endl;
    }
}


int main()
{
    //step_0
    int tArray[] = { 8, 30, 1, 9, 11, 19, 2 };
    int tLength = 7;

    SequentialSearch(tArray, tLength, 9);
    SequentialSearch(tArray, tLength, 6);

    cout << endl;
    //정렬되어 있다면, 검색에 대해서 성질이 좀 달라진다( <-- 좀더 효율적인 가능성이 있다)
    //step_1
    int tArray_1[] = { 1, 2, 8, 9, 11, 19, 30 };    //오름차순 정렬되어 있는 데이터
    int tLength_1 = 7;

    SequentialSearch_1(tArray_1, tLength_1, 9);
    SequentialSearch_1(tArray_1, tLength_1, 6);

    return 0;
}