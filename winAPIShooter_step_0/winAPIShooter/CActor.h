#pragma once

#include "CUnit.h"

//주인공 기체라는 개념의 클래스다.
class CActor: public CUnit
{
public:
	CActor();
	~CActor();

	CActor(const CActor& t);
	void operator=(const CActor& t);

public:
	//CActor타입의 객체를 복제하는 함수
	//	복사 생성자를 이용하여 객체를 생성
	virtual CObjectRyu* clone() override
	{
		return new CActor(*this);
	}
	//<-- 상속구조의 클래스의 객체는 부모클래스 포인터타입으로 다룰 수 있다
};

