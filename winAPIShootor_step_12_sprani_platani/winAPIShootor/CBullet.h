#pragma once

#include "CUnit.h"

//���ΰ� ��ü�� źȯ ����
class CBullet: public CUnit
{
public:
	CBullet();
	virtual ~CBullet();

	CBullet(const CBullet& t);
	void operator=(const CBullet& t);

public:
	//CBulletŸ���� ��ü�� �����ϴ� �Լ�
	//		���� �����ڸ� �̿��Ͽ� ��ü�� ����!!!!!!
	virtual CObjectRyu* clone() override
	{
		return new CBullet(*this);
	}
};

