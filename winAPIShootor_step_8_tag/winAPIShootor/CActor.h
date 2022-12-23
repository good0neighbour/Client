#pragma once

#include "CUnit.h"
#include <vector>
using namespace std;

class CBullet;

//���ΰ� ��ü ��� ������ Ŭ�����̴�.
class CActor: public CUnit
{
public:
	CActor();
	virtual ~CActor();

	CActor(const CActor& t);
	void operator=(const CActor& t);

public:
	//CActorŸ���� ��ü�� �����ϴ� �Լ�
	//		���� �����ڸ� �̿��Ͽ� ��ü�� ����!!!!!!
	virtual CObjectRyu* clone() override
	{
		return new CActor(*this);
	}
	//<-- ��ӱ����� Ŭ������ ��ü�� �θ�Ŭ���� ������Ÿ������ �ٷ�� �ִ�

public:
	//���ΰ� ��ü�� �Ϲ�źȯ�� �߻��ϴ� ����� ������
	void DoFire(vector<CBullet*>& tBullets);

	virtual void Update(float tDeltaTime) override;


private:
	//������ źȯ �̶� ����
	int mCurIndexBullet = 0;
};

