#pragma once

#include "CUnit.h"

//주인공 기체의 탄환 개념
class CBullet: public CUnit
{
public:
	CBullet();
	virtual ~CBullet();

	CBullet(const CBullet& t);
	void operator=(const CBullet& t);

public:
	//CBullet타입의 객체를 복제하는 함수
	//		복사 생성자를 이용하여 객체를 생성!!!!!!
	virtual CObjectRyu* clone() override
	{
		return new CBullet(*this);
	}
};

