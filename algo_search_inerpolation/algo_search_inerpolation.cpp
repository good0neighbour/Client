/*
    이분검색(이진검색) interpolation search( binary search )
*/
#include <iostream>
using namespace std;

void DoBinarySearch(int tArray[], int tBegin, int tEnd, int tData)
{
    int tIndexMiddle = 0;

    //가운데 위치 선정
    tIndexMiddle = (tBegin + tEnd) / 2;

    if (tData == tArray[tIndexMiddle])
    {
        //base case
        cout << "검색 성공" << endl;
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
    return 0;
}