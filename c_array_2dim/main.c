

#include<stdio.h>


//�迭: ������ Ÿ���� ���ҵ鿡 �������� �޸� ��

//������ �迭���� �Ű������� �ѱ�� ǥ�����
//���������� �ؼ��Ǵ� �κ��� ���ڸ� ������� �ʾƵ� ������
//�������� �� ���ڸ� ����ؾ߸� �Ѵ�.
//<--�׷��� �����Ϸ��� � ��Ģ���� �޸𸮸� �ؼ����� �� �� �ִ�.
void DoChangeElement(int tArray[][3], int tRow, int tCol, int tValue);

//tArray�� �ּҰ��� �����Ƿ� *���� ������ �������� �����Ͽ���, ���������Ǵ� ���� ��Ģ�� 'int¥�� 3������'�̴�.
void DoChangeElementPtr(int(*tArray)[3], int tRow, int tCol, int tValue);

void DoChangeElementPtr2(int(*tArray)[3], int tRow, int tCol, int tValue);


int main()
{
	//���� Ÿ���� ���ҵ�, ���� ������ 5��, �迭�� �̸��� tArrayA, �ʱⰪ�� ��� 0
	int tArrayA[5] = { 0,0,0,0,0 };	//1���� �迭

	//3���� �迭
	int tArrayC[2][5][3];

	//2���� �迭
	//5��3��¥�� 2���� �迭
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


	//��±���
	//��
	for (int tRow = 0;tRow < 5;++tRow)
	{
		//��
		for (int tCol = 0;tCol < 3;++tCol)
		{
			printf("%d\t", tArrayB[tRow][tCol]);
		}

		printf("\n");
	}

	//�ش� ������ �Լ��� ����� ���α׷� ������ ��������.
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
	//DoChangeElementPtr�� ���� ����� �ϵ���
	//������ ���� ����, �������� ����
	//int* tpPtr= *(tArray + tRow);//2���� ���ؿ��� tArray�� �����ּҰ����� �ؼ��Ͽ� ��������
	//*(tpPtr + tCol) = tValue;//1���� ���ؿ��� tpPtr�� �����ּҰ����� �ؼ��Ͽ� ��������

	*(*tArray + (3 * tRow + tCol)) = tValue;

	//���⼭�� �ؼ� ���
	//tArray 2���� �迭�� �����ּҰ�
	//*tArray 1���� �迭�� �����ּҰ� <--- 2���� �迭�� ��ü�� 1�����̴�.
}