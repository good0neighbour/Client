#pragma once

#include "SVector2D.h"

//추가, 삭제에 유리한 자료구조를 선택
#include <list>
using namespace std;

//'충돌체' 클래스
// 충돌 검사를 수행하는 클래스
//	DoCollision함수

class CUnit;

class CCollider
{
public:
	CCollider();
	~CCollider();

	CCollider(const CCollider& t);

	void operator=(const CCollider& t);

public:
	bool DoCollision(CCollider* tOther);

	void Create(SVector2D tPosition, float tWidth = 64.0f, float tHeight = 64.0f, float tAnchorX = 0.5f, float tAnchorY = 0.5f);

	void Update();

	//충돌체 목록에 충돌체 등록
	void AddToCollisions(CCollider* tpCollider);
	//충돌체 목록에서 충돌체 등록해제
	void EraseFromCollisions(CCollider* tpCollider);
	//임의의 충돌체가 충돌체 목록에 들어있는지 검토
	bool DoCheckBeInCollisions(CCollider* tpCollider);

	inline SVector2D GetPosition()
	{
		return mPosition;
	}

	inline float GetAnchorX()
	{
		return mAnchorX;
	}
	inline float GetAnchorY()
	{
		return mAnchorY;
	}
	inline float GetWidth()
	{
		return mWidth;
	}
	inline float GetHeight()
	{
		return mHeight;
	}

	inline void SetOwnerObject(CUnit* tpObject)
	{
		mpObject = tpObject;
	}

	inline CUnit* GetOwnerObject()
	{
		return mpObject;
	}

private:
	bool DoCollisionAABB(CCollider* tOther);

private:
	SVector2D mPosition;

	//[0,1]로 정규화된 기준에서의 위치
	float mAnchorX = 0.0f;
	float mAnchorY = 0.0f;

	float mWidth = 0.0f;
	float mHeight = 0.0f;

	//이 충돌체의 소유자 객체 참조
	CUnit* mpObject = nullptr;

	//충돌체 목록 정보: 이 충돌체와 충돌한 충돌체들을 기록해두기 위한 자료구조
	//	<-- 충돌상태를 구분하기 위한 용도로 만들었다.
	//	---> 충돌상태: 충돌시작, 충돌 중, 충돌 종료
	list<CCollider*> mCollisions;
};

