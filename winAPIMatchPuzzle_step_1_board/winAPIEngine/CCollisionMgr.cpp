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
	//등록된 유닛이 2개 미만이면 충돌검사를 수행하지 않는다.
	if (mCollisionUnits.size() < 2)
	{
		return;
	}


	//임의의 유닛( *tItor )과 임의의 유닛( *tItor_0 )을 충돌검사한다.
	list<CUnit*>::iterator tItor;
	for (tItor = mCollisionUnits.begin(); tItor != mCollisionUnits.end(); ++tItor)
	{
		list<CUnit*>::iterator tItor_0 = tItor;
		++tItor_0;

		for (;tItor_0 != mCollisionUnits.end();++tItor_0)
		{
			if ((*tItor)->GetIsActive() && (*tItor_0)->GetIsActive())
			{
				//임의의 두 객체의 충돌 처리
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
	//현재 충돌 상태이다.
	if (tIsCollision)
	{
		//충돌체 목록에 상대 유닛의 충돌체가 등록되어 있는지 검사
		//	( 만약, 충돌체 목록에 충돌체가 등록되어 있다면 이전에 어느 순간에 충돌이 일어났던 것이다 )
		bool tIsBe = tpThis->DoCheckBeInCollisions(tpOther);
		//충돌 시작이라면
		if (!tIsBe)
		{
			//충돌이 시작이라면 서로 상대의 충돌체를 등록한다.
			tpThis->AddToCollisions(tpOther);
			tpOther->AddToCollisions(tpThis);

			OutputDebugString(L"Enter Collision\n");

			tpThis->GetOwnerObject()->OnEnterCollision(tpOther);
			tpOther->GetOwnerObject()->OnEnterCollision(tpThis);
		}
		else //충돌 중이라면 
		{
			//OutputDebugString(L"Stay Collision\n");

			tpThis->GetOwnerObject()->OnStayCollision(tpOther);
			tpOther->GetOwnerObject()->OnStayCollision(tpThis);
		}		
	}
	//충돌 종료라면
	else if (tpThis->DoCheckBeInCollisions(tpOther) )
	{
		//충돌이 종료라면 서로 상대의 충돌체를 등록해제한다.
		tpThis->EraseFromCollisions(tpOther);
		tpOther->EraseFromCollisions(tpThis);

		OutputDebugString(L"Exit Collision\n");

		tpThis->GetOwnerObject()->OnExitCollision(tpOther);
		tpOther->GetOwnerObject()->OnExitCollision(tpThis);
	}


	return false;
}