#include "CAniSeq.h"

#include "ryumacro.h"

#include "CTexture.h"

CAniSeq::CAniSeq()	
{
    mId = "";
        
    mTexs.clear();

    mTimeInterval = 0.0f;

    mTotalFrameCount = 0;

    mCurFrameIndex = 0;
    mAniTime = 0.0f;

    mPlayOption = ANI_PO::LOOP;
    mSpriteOption = ANI_SO::FRAME_FILE;          


    mSpriteWidth = 0;       
    mSpriteHeight = 0;      

    mRow = 0;           
    mCol = 0;               
    mFrameCOL = 0;         
}
CAniSeq::~CAniSeq()
{
    vector<CTexture*>::iterator tItor;
    for (tItor = mTexs.begin(); tItor != mTexs.end(); ++tItor)
    {
        SAFE_RELEASE((*tItor));
        //SAFE_DELETE((*tItor));
        //if (*tItor)
        //{
        //   // delete (*tItor);
        //    (*tItor)->Release();
        //    *tItor = nullptr;
        //}
    }
    mTexs.clear();
}

CAniSeq::CAniSeq(const CAniSeq& t)
{
    mId = t.mId;

    //stl의 vector의 =연산자는 기본동작이 복사copy이다.
    mTexs = t.mTexs;

    mTimeInterval = t.mTimeInterval;

    mTotalFrameCount = t.mTotalFrameCount;

    mCurFrameIndex = t.mCurFrameIndex;
    mAniTime = t.mAniTime;

    mPlayOption = t.mPlayOption;
    mSpriteOption = t.mSpriteOption;


    mSpriteWidth = t.mSpriteWidth;
    mSpriteHeight = t.mSpriteHeight;

    mRow = t.mRow;
    mCol = t.mCol;
    mFrameCOL = t.mFrameCOL;
}

void CAniSeq::operator=(const CAniSeq& t)
{
    mId = t.mId;

    //stl의 vector의 =연산자는 기본동작이 복사copy이다.
    mTexs = t.mTexs;

    mTimeInterval = t.mTimeInterval;

    mTotalFrameCount = t.mTotalFrameCount;

    mCurFrameIndex = t.mCurFrameIndex;
    mAniTime = t.mAniTime;

    mPlayOption = t.mPlayOption;
    mSpriteOption = t.mSpriteOption;


    mSpriteWidth = t.mSpriteWidth;
    mSpriteHeight = t.mSpriteHeight;

    mRow = t.mRow;
    mCol = t.mCol;
    mFrameCOL = t.mFrameCOL;
}

void CAniSeq::Update(float tDeltaTime)
{
    //애니메이션 프레임 진행 루틴
    if (mAniTime >= mTimeInterval)
    {        
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