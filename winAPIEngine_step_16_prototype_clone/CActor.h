#pragma once

#include "CUnit.h"

//���ΰ� ��ü��� ������ Ŭ������.
class CActor: public CUnit
{
public:
	CActor();
	~CActor();

	CActor(const CActor& t);
	void operator=(const CActor& t);

public:
	//CActorŸ���� ��ü�� �����ϴ� �Լ�
	//	���� �����ڸ� �̿��Ͽ� ��ü�� ����
	virtual CObjectRyu* clone() override
	{
		return new CActor(*this);
	}
	//<-- ��ӱ����� Ŭ������ ��ü�� �θ�Ŭ���� ������Ÿ������ �ٷ� �� �ִ�
};

