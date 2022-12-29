#pragma once

#include "CUnit.h"

class CExplosion : public CUnit
{
public:
	CExplosion();
	virtual ~CExplosion();

	CExplosion(const CExplosion& t);
	void operator=(const CExplosion& t);

public:
	//CExplosion타입의 객체를 복제하는 함수
	//		복사 생성자를 이용하여 객체를 생성!!!!!!
	virtual CObjectRyu* clone() override
	{
		return new CExplosion(*this);
	}
};

