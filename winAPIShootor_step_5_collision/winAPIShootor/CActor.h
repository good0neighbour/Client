#pragma once

#include "CUnit.h"
#include <vector>
using namespace std;

class CBullet;

//주인공 기체 라는 개념의 클래스이다.
class CActor: public CUnit
{
public:
	CActor();
	virtual ~CActor();

	CActor(const CActor& t);
	void operator=(const CActor& t);

public:
	//CActor타입의 객체를 복제하는 함수
	//		복사 생성자를 이용하여 객체를 생성!!!!!!
	virtual CObjectRyu* clone() override
	{
		return new CActor(*this);
	}
	//<-- 상속구조의 클래스의 객체는 부모클래스 포인터타입으로 다룰수 있다

public:
	//주인공 기체는 일반탄환을 발사하는 기능을 가진다
	void DoFire(vector<CBullet*>& tBullets);

	virtual void Update(float tDeltaTime) override;


private:
	//임의의 탄환 이란 개념
	int mCurIndexBullet = 0;
};

