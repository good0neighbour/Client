#include <iostream>
using namespace std;

void InsertionSort(int tArray[], int tLength)
{
	int ti = 0;
	int tEdge = 0;
	int tTemp = 0;

	for (ti = 1; ti < tLength; ++ti)
	{
		//'unsorted부분집합'의 '가장 처음에 있는 원소'를 기억해둠
		tTemp = tArray[ti];		//<--unsorted부분집합에 가장 첫번째 원소
		tEdge = ti;				//<-- '경계'는 insorted부분집합에 가장 첫번째 원소로 한다

		//처음 진입 시 tArray[tEdeg - 1]은 sorted의 맨 마지막 원소를 나타내는 것이다
		while (tEdge > 0 && tArray[tEdge - 1] > tTemp)
		{
			//unsorted에 원소를 뒤에서부터 하나씩 순회
			tArray[tEdge] = tArray[tEdge - 1];
			tEdge = tEdge - 1;	//soted 왼 원소들을 하나씩 뒤에서부터
		}

		//여기까지 오면, 삽입할 위치를 결정했다
		//insertion 삽입
		tArray[tEdge] = tTemp;
	}
}

int main()
{
	int tArray[] = { 69, 10, 30, 2, 16 };
	int tLength = 5;

	//before
	for (auto t : tArray)
	{
		cout << t << "\t";
	}
	cout << endl;

	InsertionSort(tArray, tLength);

	//after
	for (auto t : tArray)
	{
		cout << t << "\t";
	}
	cout << endl;

    return 0;
}