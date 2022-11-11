/*
    이분검색(이진검색) interpolation search( binary search )
*/
#include <iostream>
using namespace std;

int gCount = 0;

void DoBinarySearch(int tArray[], int tBegin, int tEnd, int tData)
{
    int tIndexMiddle = 0;

    gCount++;

    //base case
    //원소가 1개인 경우(부분집합이 1개가 된 경우 <-- 더 이상 이분할 수 없는 상태가 된 것이다)
    if (tBegin == tEnd)
    {
        if (tData == tArray[tBegin])
        {
            cout << "검색 성공" << endl;
            cout << "--gCount: " << gCount << endl;
        }
        else
        {
            cout << "검색 실패" << endl;
        }

        return;    //리턴하는 값은 없다.
    }

    //가운데 위치 선정
    tIndexMiddle = (tBegin + tEnd) / 2;

    if (tData == tArray[tIndexMiddle])
    {
        //base case
        cout << "검색 성공" << endl;
        cout << "--gCount: " << gCount << endl;
    }
    else if (tData < tArray[tIndexMiddle])
    {
        //recursive case
        DoBinarySearch(tArray, tBegin, tIndexMiddle - 1, tData);
    }
    else if (tData > tArray[tIndexMiddle])
    {
        //recursive case
        DoBinarySearch(tArray, tIndexMiddle + 1, tEnd, tData);
    }
    else
    {
        //base case
        cout << "검색 실패" << endl;
    }
}

int main()
{
    int tArray[] = { 1, 2, 8, 9, 11, 19, 29 };
    int tLength = 7;

    int tData = 29;
    DoBinarySearch(tArray, 0, tLength - 1, tData);

    return 0;
}