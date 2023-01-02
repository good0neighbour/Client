

#include<stdio.h>


//배열: 동일한 타입의 원소들에 연속적인 메모리 블럭

//다차원 배열에서 매개변수로 넘길시 표기법은
//최종적으로 해석되는 부분은 숫자를 명시하지 않아도 되지만
//나머지는 다 숫자를 명시해야만 한다.
//<--그래야 컴파일러가 어떤 규칙으로 메모리를 해석할지 알 수 있다.
void DoChangeElement(int tArray[][3], int tRow, int tCol, int tValue);

//tArray에 주소값을 담으므로 *으로 포인터 변수임을 선언하였고, 간접참조되는 곳에 규칙은 'int짜리 3개묶음'이다.
void DoChangeElementPtr(int(*tArray)[3], int tRow, int tCol, int tValue);

void DoChangeElementPtr2(int(*tArray)[3], int tRow, int tCol, int tValue);


int main()
{
	//정수 타입의 원소들, 원소 갯수는 5개, 배열의 이름은 tArrayA, 초기값은 모두 0
	int tArrayA[5] = { 0,0,0,0,0 };	//1차원 배열

	//3차원 배열
	int tArrayC[2][5][3];

	//2차원 배열
	//5행3열짜리 2차원 배열
	int tArrayB[5][3] =
	{
		1,0,0,
		0,2,0,
		0,2,0,
		0,2,0,
		0,0,3

	};
	/*{
		{1,0,0},
		{0,2,0},
		{0,2,0},
		{0,2,0},
		{0,0,3}
	};*/


	//출력구문
	//행
	for (int tRow = 0;tRow < 5;++tRow)
	{
		//열
		for (int tCol = 0;tCol < 3;++tCol)
		{
			printf("%d\t", tArrayB[tRow][tCol]);
		}

		printf("\n");
	}

	//해당 한줄을 함수로 만들어 프로그램 구조를 변경하자.
	//void DoChangeElement(,int tRow,int tCol,int tValue)
	//tArrayB[0][0] = 777;
	//DoChangeElement(tArrayB, 0, 0, 777);
	//DoChangeElement(tArrayB, 4, 2, 777);
	DoChangeElementPtr2(tArrayB, 2, 1, 333);

	printf("\n");

	for (int tRow = 0;tRow < 5;++tRow)
	{
		
		for (int tCol = 0;tCol < 3;++tCol)
		{
			printf("%d\t", tArrayB[tRow][tCol]);
		}

		printf("\n");
	}

	return 0;
}
void DoChangeElement(int tArray[][3], int tRow, int tCol, int tValue)
{
	tArray[tRow][tCol] = tValue;
}
void DoChangeElementPtr(int(*tArray)[3], int tRow, int tCol, int tValue)
{
	tArray[tRow][tCol] = tValue;
}
void DoChangeElementPtr2(int(*tArray)[3], int tRow, int tCol, int tValue)
{
	//DoChangeElementPtr과 같은 기능을 하도록
	//포인터 증감 연산, 간접참조 연산
	//int* tpPtr= *(tArray + tRow);//2차원 수준에서 tArray를 시작주소값으로 해석하여 간접참조
	//*(tpPtr + tCol) = tValue;//1차원 수준에서 tpPtr을 시작주소값으로 해석하여 간접참조

	*(*tArray + (3 * tRow + tCol)) = tValue;

	//여기서의 해석 방법
	//tArray 2차원 배열의 시작주소값
	//*tArray 1차원 배열의 시작주소값 <--- 2차원 배열도 실체는 1차원이다.
}