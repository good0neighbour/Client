#include "CRyuTimer.h"


void CRyuTimer::SetTimer(float tTimeInterval, std::function<void(CUnit&)> tFunction, CUnit* tpUnit)
{
	mTimeInterval = tTimeInterval;

	mFunction = tFunction;

	mpUnit = tpUnit;
}

void CRyuTimer::Update(float tDeltaTime)
{
    if (mTimeTick >= mTimeInterval)
    {
        //todo �����ð� �������� ������ �ڵ�
        //mpEnemy->DoFire(mBulletsEnemy);
        mFunction(*mpUnit);

        //time tick�� �ʱ� ���·� �ǵ�����
        mTimeTick = 0.0f;
    }
    else
    {
        //delta time �� ����
        mTimeTick = mTimeTick + tDeltaTime;
    }
}