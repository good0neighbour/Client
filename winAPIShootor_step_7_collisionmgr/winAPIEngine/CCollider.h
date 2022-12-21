#pragma once

#include "SVector2D.h"

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

	inline const CUnit* GetOwnerObject() const
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
};

