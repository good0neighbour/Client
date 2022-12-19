#pragma once

#include "CObjectRyu.h"

//클래스 전방선언
class CAPIEngine;
class CTexture;

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

	virtual void OnTimer() {};


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


public:
	//순수 가상 함수
	//clone이라는 형태로 형태를 강제하여 구현하는 것이다
	virtual CObjectRyu* clone() = 0;
};

