#include "CAniSeq.h"

#include "ryumacro.h"

#include "CTexture.h"

CAniSeq::CAniSeq()
{
    mId = "";

    mTimeInterval = 0.0f;

    mTotalFrameCount = 0;

    mCurFrameIndex = 0;
    mAniTime = 0.0f;
}

CAniSeq::~CAniSeq()
{
    vector<CTexture*>::iterator tItor;
    for (tItor = mTexs.begin(); tItor < mTexs.end(); ++tItor)
    {
        SAFE_RELEASE((*tItor));
    }
    mTexs.clear();
}

CAniSeq::CAniSeq(const CAniSeq& t)
{
    mId = t.mId;

    //stl의 vector의 =연산자는 기본 동작이 복사 copy다.
    mTexs = t.mTexs;

    mTimeInterval = t.mTimeInterval;

    mTotalFrameCount = t.mTotalFrameCount;

    mCurFrameIndex = t.mCurFrameIndex;
    mAniTime = t.mAniTime;
}

void CAniSeq::operator=(const CAniSeq& t)
{
    mId = t.mId;

    //stl의 vector의 =연산자는 기본 동작이 복사 copy다.
    mTexs = t.mTexs;

    mTimeInterval = t.mTimeInterval;

    mTotalFrameCount = t.mTotalFrameCount;

    mCurFrameIndex = t.mCurFrameIndex;
    mAniTime = t.mAniTime;
}

void CAniSeq::Update(float tDeltaTime)
{
    //애니메이션 프레임 진행 루틴
    if (mAniTime >= mTimeInterval)
    {
        //todo
        //스프라이트 프레임의 번호를 하나씩 증가시킨다.
        if (mCurFrameIndex < mTotalFrameCount - 1)
        {
            mCurFrameIndex++;
        }
        else
        {
            mCurFrameIndex = 0;
        }

        mAniTime = 0.0f;
    }
    else
    {
        mAniTime = mAniTime + tDeltaTime;
    }
}