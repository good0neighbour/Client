#pragma once


#include "ryumacro.h"

//추가, 삭제에 유리한 자료구조를 선택
#include <list>
using namespace std;

class CUnit;

class CCollisionMgr
{
private:
	//충돌 처리 검토 대상이 될 유닛들을 관리할 자료구조
	//이 자료구조에 담긴 유닛들은 충돌검사와 충돌처리의 대상이 된다.
	list<CUnit*> mCollisionUnits;

public:
	void AddUnit(CUnit* t);	//유닛 추가
	void Update(float tDeltaTime);	//충돌의 상태구분과 충돌 처리용 갱신 함수


private:
	//유닛 대 유닛 충돌 검사
	bool DoCollision(CUnit* tThis, CUnit* tOther);


	SINGLETON_DECLARE(CCollisionMgr)
};

