#include "CCollisionMgr.h"

#include <windows.h>

#include "CUnit.h"
#include "CCollider.h"


SINGLETON_DEFINITION(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
	OutputDebugString(L"CCollisionMgr::CCollisionMgr()\n");

	mCollisionUnits.clear();
}

CCollisionMgr::~CCollisionMgr()
{
	OutputDebugString(L"~CCollisionMgr::CCollisionMgr()\n");

	mCollisionUnits.clear();
}

void CCollisionMgr::AddUnit(CUnit* t)
{
	mCollisionUnits.push_back(t);
}
void CCollisionMgr::Update(float tDeltaTime)
{
	//��ϵ� ������ 2�� �̸��̸� �浹�˻縦 �������� �ʴ´�.
	if (mCollisionUnits.size() < 2)
	{
		return;
	}


	//������ ����( *tItor )�� ������ ����( *tItor_0 )�� �浹�˻��Ѵ�.
	list<CUnit*>::iterator tItor;
	for (tItor = mCollisionUnits.begin(); tItor != mCollisionUnits.end(); ++tItor)
	{
		list<CUnit*>::iterator tItor_0 = tItor;
		++tItor_0;

		for (;tItor_0 != mCollisionUnits.end();++tItor_0)
		{
			if ((*tItor)->GetIsActive() && (*tItor_0)->GetIsActive())
			{
				//������ �� ��ü�� �浹 ó��
				DoCollision( (*tItor), (*tItor_0) );
			}
		}
	}
}



bool CCollisionMgr::DoCollision(CUnit* tThis, CUnit* tOther)
{
	CCollider* tpThis = tThis->mpCollider;
	CCollider* tpOther = tOther->mpCollider;

	bool tIsCollision = false;

	tIsCollision = tpThis->DoCollision(tpOther);
	//���� �浹 �����̴�.
	if (tIsCollision)
	{
		//�浹ü ��Ͽ� ��� ������ �浹ü�� ��ϵǾ� �ִ��� �˻�
		//	( ����, �浹ü ��Ͽ� �浹ü�� ��ϵǾ� �ִٸ� ������ ��� ������ �浹�� �Ͼ�� ���̴� )
		bool tIsBe = tpThis->DoCheckBeInCollisions(tpOther);
		//�浹 �����̶��
		if (!tIsBe)
		{
			//�浹�� �����̶�� ���� ����� �浹ü�� ����Ѵ�.
			tpThis->AddToCollisions(tpOther);
			tpOther->AddToCollisions(tpThis);

			OutputDebugString(L"Enter Collision\n");

			tpThis->GetOwnerObject()->OnEnterCollision(tpOther);
			tpOther->GetOwnerObject()->OnEnterCollision(tpThis);
		}
		else //�浹 ���̶�� 
		{
			//OutputDebugString(L"Stay Collision\n");

			tpThis->GetOwnerObject()->OnStayCollision(tpOther);
			tpOther->GetOwnerObject()->OnStayCollision(tpThis);
		}		
	}
	//�浹 ������
	else if (tpThis->DoCheckBeInCollisions(tpOther) )
	{
		//�浹�� ������ ���� ����� �浹ü�� ��������Ѵ�.
		tpThis->EraseFromCollisions(tpOther);
		tpOther->EraseFromCollisions(tpThis);

		OutputDebugString(L"Exit Collision\n");

		tpThis->GetOwnerObject()->OnExitCollision(tpOther);
		tpOther->GetOwnerObject()->OnExitCollision(tpThis);
	}


	return false;
}