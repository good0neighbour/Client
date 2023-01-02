

#include<stdio.h>


/*
	포인터 타입의 원소의 배열



*/

int main()
{
	//선언과 초기화
	//정수타입의 원소들, 원소들의 갯수는 5개, 배열이름은 tArrayA, 원소들의 초기값은 0
	int tArrayA[5] = {0,0,0,0,0};


	//포인터 타입의 (간접참조하는 곳에 변수는 정수타입)의 원소들, 원소들의 갯수는 5개, 배열이름은 tArrayB, 원소들의 초기값은 NULL
	int* tArrayB[5] = {NULL,NULL,NULL,NULL,NULL};

	//int(*tArray)[5] 이것과 헷갈리지 말자. 한글자씩 꼼꼼히 해석하면 된다.


	for (int ti = 0;ti<5;++ti)
	{
		printf("%p\n", tArrayB[ti]);
	}


	int tA = 1;
	int tB = 2;
	int tC = 3;
	int tD = 4;
	int tE = 5;

	tArrayB[0] = &tA;
	tArrayB[1] = &tB;
	tArrayB[2] = &tC;
	tArrayB[3] = &tD;
	tArrayB[4] = &tE;

	printf("--pointer element value----\n");
	for (int ti = 0;ti<5;++ti)
	{
		printf("%p\n", tArrayB[ti]);
	}
	printf("--variables address----\n");
	printf("%p\n", &tA);
	printf("%p\n", &tB);
	printf("%p\n", &tC);
	printf("%p\n", &tD);
	printf("%p\n", &tE);

	printf("--간접참조 value----\n");
	for (int ti = 0;ti < 5;++ti)
	{
		printf("%d\n", *tArrayB[ti]);
	}


	//포인터 타입의 (간접참조하는 곳에 변수는 포인터타입(간접참조하는 곳에 변수는 정수타입))의 원소들, 원소들의 갯수는 5개, 배열이름은 tArrayC, 원소들의 초기값은 NULL
	int** tArrayC[5] = {NULL};

	return 0;
}