#include <iostream>
#include <stack>
using namespace std;

#define MAX_ROW 6
#define MAX_COL 6

//0: 빈곳, 1: 칠한 곳, 2: 벽(경계)
//6 by 6 짜리 격자보드
int mGrid[MAX_ROW][MAX_COL] =
{
	2, 2, 2, 2, 2, 2,
	2, 0, 0, 0, 0, 2,
	2, 0, 0, 0, 0, 2,
	2, 2, 2, 2, 2, 2,
	2, 0, 0, 0, 0, 2,
	2, 2, 2, 2, 2, 2,
};

typedef stack<int> CIntStack;

CIntStack mIntStack;	//행, 열 위치 정보를 기억시켜둘 자료구조

void DisplayGrid();
//재귀버전
void DoFloodFill(int tCol, int tRow);
//비재귀버전
void DoFloodFillNoRecur(int tCol, int tRow);

int main()
{
	//before
	DisplayGrid();

	//flood fill 적용
	DoFloodFill(1, 1);

	cout << endl;
	//after
	DisplayGrid();

	return 0;
}

void DisplayGrid()
{
	string tString = "";
	char tszTemp[256] = { 0 };

	for (int tRow = 0; tRow < MAX_ROW; ++tRow)
	{
		for (int tCol = 0; tCol < MAX_COL; ++tCol)
		{
			sprintf_s(tszTemp, "%d", mGrid[tRow][tCol]);

			tString = tString + tszTemp;	//c++스타일 문자열 + c스타일 문자열
		}

		tString = tString + "\n";
	}

	cout << tString;
}

void DoFloodFill(int tCol, int tRow)
{
	//이미 칠했거나 벽(경계)라면 칠하지 않는다
	if (1 == mGrid[tRow][tCol] || 2 == mGrid[tRow][tCol])
	{
		//base case
		return;
	}
	else
	{
		//빈 곳(cell)이라면
		//recursive case

		//칠한다
		mGrid[tRow][tCol] = 1;

		DoFloodFill(tCol - 1, tRow);	//좌측 이웃 셀
		DoFloodFill(tCol + 1, tRow);	//우측 이웃 셀
		DoFloodFill(tCol, tRow - 1);	//상측 이웃 셀
		DoFloodFill(tCol, tRow + 1);	//하측 이웃 셀
	}
}

void DoFloodFillNoRecur(int tCol, int tRow)
{

}

//#include <iostream>
//
////플러드 필 알고리즘 flood fill
////  원래는 색칠하기 알고리즘 이지만, 너무 느려서 그렇게 사용되지는 않고
////      대신 응용되어 임의의 게임 알고리즘의 기반이 된다.
//
////복습
////팩토리얼 함수를 재귀와 비재귀로 만들어보자.
//
////재귀버전의 팩토리얼 함수를 작성해보세요. 테스트 값은 4
//
//int DoFactorialRecursion(int tN);
//
//int DoFactorial(int tN);
//
//int main()
//{
//	int tResult;
//
//	tResult = DoFactorialRecursion(4);
//	std::cout << tResult <<std::endl;
//
//	tResult = DoFactorial(4);
//	std::cout << tResult <<std::endl;
//
//    return 0;
//}
//
//int DoFactorialRecursion(int tN)
//{
//	if (tN <= 1)
//	{
//		return 1;
//	}
//	else
//	{
//		return tN * DoFactorialRecursion(tN - 1);
//	}
//}
//
//int DoFactorial(int tN)
//{
//	if (tN <= 1)
//	{
//		return 1;
//	}
//	else
//	{
//		int tTemp = 1;
//		//누적곱
//		for (int ti = tN; ti > 0; --ti)
//		{
//			tTemp = tTemp * ti;
//		}
//
//		return tTemp;
//	}
//}