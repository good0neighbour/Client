#pragma once

#include "CUnit.h"
#include <vector>
using namespace std;

class CBullet;

class CEnemy: public CUnit
{
public:
	CEnemy();
	virtual ~CEnemy();

	CEnemy(const CEnemy& t);
	void operator=(const CEnemy& t);

public:
	//CEnemy타입의 객체를 복제하는 함수
	//		복사 생성자를 이용하여 객체를 생성!!!!!!
	virtual CObjectRyu* clone() override
	{
		return new CEnemy(*this);
	}

public:

	virtual void Update(float tDeltaTime) override;


	//주인공 기체는 일반탄환을 발사하는 기능을 가진다
	void DoFire(vector<CBullet*>& tBullets);

private:
	//임의의 탄환 이란 개념
	int mCurIndexBullet = 0;

	//타이머를 위한 변수
public:
	float mTimeTick = 0.0f;	//deltatime누적용 변수


	virtual void OnTimer() override;
};

