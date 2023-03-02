#pragma once

#include "CObjectRyu.h"

#include <string>
using namespace std;

//클래스 전방선언
class CAPIEngine;
class CTexture;

class CCollider;

class CAnimator;

class CUnit: public CObjectRyu
{
public:
	CUnit();
	virtual ~CUnit();
	//복사생성자
	CUnit(const CUnit& t);
	//복사대입연산자
	void operator=(const CUnit& t);		//연쇄 대입은 고려하지 않겠다.

public:
	virtual void Create(CAPIEngine* tpEngine);
	virtual void Destroy();
	virtual void Update(float tDeltaTime);
	virtual void Render();

	virtual void LateUpdate() {};



	virtual void OnTimer() {};

	//충돌 처리 프로그램 구조를 다형성을 이용하여 작성함
	virtual void OnEnterCollision(CCollider* tOther) {};
	virtual void OnStayCollision(CCollider* tOther) {};
	virtual void OnExitCollision(CCollider* tOther) {};


	


	inline void SetPosition(SVector2D tPosition)
	{
		mPosition = tPosition;
	}
	inline SVector2D GetPosition()
	{
		return mPosition;
	}

	inline void SetPosition(float tX, float tY)
	{
		//mX = tX;
		//mY = tY;
		SVector2D tVector(tX, tY);
		mPosition = tVector;
	}

	inline void SetRadius(float tRadius)
	{
		mRadius = tRadius;
	}

	inline const float GetRadius() const
	{
		return mRadius;
	}

	inline void SetAnchorPoint(float tAnchorX, float tAnchorY)
	{
		mAnchorX = tAnchorX;
		mAnchorY = tAnchorY;
	}
	inline void SetTexture(CTexture* tpTexture)
	{
		mpTexture = tpTexture;
	}


	inline void SetVelocity(SVector2D tVelocity)
	{
		mVelocity = tVelocity;
	}


	inline void SetIsActive(bool tIsActive)
	{
		mIsActive = tIsActive;
	}

	inline bool GetIsActive()
	{
		return mIsActive;
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

	inline void SetTag(string tTag)
	{
		mTag = tTag;
	}
	inline string GetTag() const
	{
		return mTag;
	}

	/*
	bool Intersects(CUnit& t) 
	{
		//A 사각형
		float tLeftX = 0.0f;
		float tRightX = 0.0f;
		float tTopY = 0.0f;
		float tBottomY = 0.0f;


		tLeftX = mPosition.mX - GetWidth() * GetAnchorX();
		tRightX = mPosition.mX - GetWidth() * GetAnchorX() + GetWidth();
		tTopY = mPosition.mY - GetHeight() * GetAnchorY();
		tBottomY = mPosition.mY - GetHeight() * GetAnchorY() + GetHeight();

		//B 사각형
		float tLeftX_Other = 0.0f;
		float tRightX_Other = 0.0f;
		float tTopY_Other = 0.0f;
		float tBottomY_Other = 0.0f;

		tLeftX_Other = t.GetPosition().mX - t.GetWidth() * t.GetAnchorX();
		tRightX_Other = t.GetPosition().mX - t.GetWidth() * t.GetAnchorX() + t.GetWidth();
		tTopY_Other = t.GetPosition().mY - t.GetHeight() * t.GetAnchorY();
		tBottomY_Other = t.GetPosition().mY - t.GetHeight() * t.GetAnchorY() + t.GetHeight();

		if (tRightX < tLeftX_Other)
		{
			return false;
		}

		if (tLeftX > tRightX_Other)
		{
			return false;
		}

		if (tBottomY < tTopY_Other)
		{
			return false;
		}

		if (tTopY > tBottomY_Other)
		{
			return false;
		}

		//위의 경우들을 모두 거치고 여기까지 왔다면, 충돌인 경우이다.
		return true;
	}
	*/




//public:
private:
	//2차원, 연속적인 공간을 가정하여 데이터와 타입을 디자인했다.
	SVector2D mVelocity;

	float mRadius = 0.0f;


	//pivot 
	// 피벗 개념을 달성하기 위해 필요한 데이터들을 멤버변수로 만들었다.
	//실제 출력 위치 
	float mDisplayX = 0.0f;
	float mDisplayY = 0.0f;
	//[0,1]로 정규화된 기준에서의 위치
	float mAnchorX = 0.0f;
	float mAnchorY = 0.0f;
	//비트맵의 너비, 높이
	float mWidth = 0.0f;
	float mHeight = 0.0f;
	

	//외관을 나타낼 비트맵 데이터
	CTexture* mpTexture = nullptr;

	//활성, 비활성 여부 변수
	bool mIsActive = false;

	//엔진 참조 변수
	CAPIEngine* mpEngine = nullptr;



public:	//일단 쉬운 작성을 위해 은닉화 생략
	//쉬운 예시를 위해 충돌체는 하나로 가정하자.
	//vector<CCollider*> mColliders;	//물론 여러개로 할수도 있다 
	CCollider* mpCollider = nullptr;


	//물체를 구분할 수 있게 태그 개념을 만들겠다. 
	//태그는 문자열 타입으로 하겠다. 
	string mTag = "";

	CAnimator* mpAnimator = nullptr;

public:
	//순수 가상 함수
	//clone이라는 형태로 형태를 강제하여 구현하는 것이다
	virtual CObjectRyu* clone() = 0;

	CAnimator* CreateAnimation(const string& tId, CAPIEngine* tpEngine);
	void DestroyAnimation();

	void SetDefaultAniSeq(const string& tStrDefaultAniSeq);
	void PlayAni(const string& tStrAniSeq);

};

