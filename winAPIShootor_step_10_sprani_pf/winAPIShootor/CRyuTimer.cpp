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
        //todo 일정시간 간격으로 실행할 코드
        //mpEnemy->DoFire(mBulletsEnemy);
        mFunction(*mpUnit);

        //time tick을 초기 상태로 되돌려줌
        mTimeTick = 0.0f;
    }
    else
    {
        //delta time 을 누적
        mTimeTick = mTimeTick + tDeltaTime;
    }
}