#pragma once


#include "ryumacro.h"

//�߰�, ������ ������ �ڷᱸ���� ����
#include <list>
using namespace std;

class CUnit;

class CCollisionMgr
{
private:
	//�浹 ó�� ���� ����� �� ���ֵ��� ������ �ڷᱸ��
	//�� �ڷᱸ���� ��� ���ֵ��� �浹�˻�� �浹ó���� ����� �ȴ�.
	list<CUnit*> mCollisionUnits;

public:
	void AddUnit(CUnit* t);	//���� �߰�
	void Update(float tDeltaTime);	//�浹�� ���±��а� �浹 ó���� ���� �Լ�


private:
	//���� �� ���� �浹 �˻�
	bool DoCollision(CUnit* tThis, CUnit* tOther);


	SINGLETON_DECLARE(CCollisionMgr)
};

