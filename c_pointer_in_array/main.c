

#include<stdio.h>


/*
	������ Ÿ���� ������ �迭



*/

int main()
{
	//����� �ʱ�ȭ
	//����Ÿ���� ���ҵ�, ���ҵ��� ������ 5��, �迭�̸��� tArrayA, ���ҵ��� �ʱⰪ�� 0
	int tArrayA[5] = {0,0,0,0,0};


	//������ Ÿ���� (���������ϴ� ���� ������ ����Ÿ��)�� ���ҵ�, ���ҵ��� ������ 5��, �迭�̸��� tArrayB, ���ҵ��� �ʱⰪ�� NULL
	int* tArrayB[5] = {NULL,NULL,NULL,NULL,NULL};

	//int(*tArray)[5] �̰Ͱ� �򰥸��� ����. �ѱ��ھ� �Ĳ��� �ؼ��ϸ� �ȴ�.


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

	printf("--�������� value----\n");
	for (int ti = 0;ti < 5;++ti)
	{
		printf("%d\n", *tArrayB[ti]);
	}


	//������ Ÿ���� (���������ϴ� ���� ������ ������Ÿ��(���������ϴ� ���� ������ ����Ÿ��))�� ���ҵ�, ���ҵ��� ������ 5��, �迭�̸��� tArrayC, ���ҵ��� �ʱⰪ�� NULL
	int** tArrayC[5] = {NULL};

	return 0;
}