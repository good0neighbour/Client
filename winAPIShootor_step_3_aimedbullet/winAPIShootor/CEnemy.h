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
	//CEnemyŸ���� ��ü�� �����ϴ� �Լ�
	//		���� �����ڸ� �̿��Ͽ� ��ü�� ����!!!!!!
	virtual CObjectRyu* clone() override
	{
		return new CEnemy(*this);
	}

public:

	virtual void Update(float tDeltaTime) override;


	//���ΰ� ��ü�� �Ϲ�źȯ�� �߻��ϴ� ����� ������
	void DoFire(vector<CBullet*>& tBullets);

private:
	//������ źȯ �̶� ����
	int mCurIndexBullet = 0;

	//Ÿ�̸Ӹ� ���� ����
public:
	float mTimeTick = 0.0f;	//deltatime������ ����


	virtual void OnTimer() override;
};

