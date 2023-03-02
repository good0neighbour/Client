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

    //stl�� vector�� =�����ڴ� �⺻������ ����copy�̴�.
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

    //stl�� vector�� =�����ڴ� �⺻������ ����copy�̴�.
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
    //�ִϸ��̼� ������ ���� ��ƾ
    if (mAniTime >= mTimeInterval)
    {        
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