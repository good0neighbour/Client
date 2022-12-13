#pragma once

#include "CUnit.h"

class CEnemy: public CUnit
{
public:
	CEnemy();
	virtual ~CEnemy();

	CEnemy(const CEnemy& t);
	void operator=(const CEnemy& t);

public:
	//CEnemy타입의 객체를 복제하는 함수
	//	복사 생성자를 이용하여 객체를 생성
	virtual CObjectRyu* clone() override
	{
		return new CEnemy(*this);
	}
};

