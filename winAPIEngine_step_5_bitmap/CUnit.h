#pragma once

//클래스 전방선언
class CAPIEngine;

class CUnit
{
public:
	CUnit();
	~CUnit();
	//복사생성자
	CUnit(const CUnit& t);
	//복사대입연산자
	void operator=(const CUnit& t);	//연쇄 대입은 고려하지 않겠다.

public:
	void Render(CAPIEngine* tpEngine);

	inline void SetPosition(float tX, float tY)
	{
		mX = tX;
		mY = tY;
	}

	inline void SetRaidus(float tRadius)
	{
		mRadius = tRadius;
	}

public:
	//2차원, 연속적인 공간을 가정하여 데이터와 타입을 디자인했다.
	float mY = 0.0f;
	float mX = 0.0f;

	float mRadius = 0.0f;
};

