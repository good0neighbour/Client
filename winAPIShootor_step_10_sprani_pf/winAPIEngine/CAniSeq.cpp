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

    //stl�� vector�� =�����ڴ� �⺻ ������ ���� copy��.
    mTexs = t.mTexs;

    mTimeInterval = t.mTimeInterval;

    mTotalFrameCount = t.mTotalFrameCount;

    mCurFrameIndex = t.mCurFrameIndex;
    mAniTime = t.mAniTime;
}

void CAniSeq::operator=(const CAniSeq& t)
{
    mId = t.mId;

    //stl�� vector�� =�����ڴ� �⺻ ������ ���� copy��.
    mTexs = t.mTexs;

    mTimeInterval = t.mTimeInterval;

    mTotalFrameCount = t.mTotalFrameCount;

    mCurFrameIndex = t.mCurFrameIndex;
    mAniTime = t.mAniTime;
}

void CAniSeq::Update(float tDeltaTime)
{
    //�ִϸ��̼� ������ ���� ��ƾ
    if (mAniTime >= mTimeInterval)
    {
        //todo
        //��������Ʈ �������� ��ȣ�� �ϳ��� ������Ų��.
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