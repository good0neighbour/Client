#pragma once

#include "CUnit.h"

class CExplosion : public CUnit
{
public:
	CExplosion();
	virtual ~CExplosion();

	CExplosion(const CExplosion& t);
	void operator=(const CExplosion& t);

	virtual void LateUpdate();

public:
	//CExplosionŸ���� ��ü�� �����ϴ� �Լ�
	//		���� �����ڸ� �̿��Ͽ� ��ü�� ����!!!!!!
	virtual CObjectRyu* clone() override
	{
		return new CExplosion(*this);
	}
};

