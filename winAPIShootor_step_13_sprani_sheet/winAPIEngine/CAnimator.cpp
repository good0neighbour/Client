#include "CAnimator.h"

#include "ryumacro.h"

//#include "CAniSeq.h"
#include "CAPIEngine.h"

#include "CTexture.h"

CAnimator::CAnimator()
{
	mId = "";

	mpCurAniSeq = nullptr;
	mStrKeyCurAniSeq = "";

	mAniSeqs.clear();

	mpEngine = nullptr;
	mpObject = nullptr;


	mStrKeyPrevAniSeq = "";
}
CAnimator::~CAnimator()
{
	unordered_map<string, CAniSeq*>::iterator tItor;
	for (tItor = mAniSeqs.begin(); tItor != mAniSeqs.end(); ++tItor)
	{
		SAFE_DELETE(tItor->second);
	}

	mAniSeqs.clear();
}

CAnimator::CAnimator(const CAnimator& t)
{
	mId = t.mId;

	mpCurAniSeq = t.mpCurAniSeq;		//??
	mStrKeyCurAniSeq = t.mStrKeyCurAniSeq;

	//���� ����
	//�ִϸ����ʹ� �� ���� ��ü���� ���� ������ �ϹǷ� ���� ���縦 �����Ͽ���.
	mAniSeqs.clear();
	unordered_map<string, CAniSeq*>::const_iterator tItor;
	for (tItor = t.mAniSeqs.begin(); tItor != t.mAniSeqs.end(); ++tItor)
	{
		//������ ������ü��ŭ �ִϸ��̼ǽ����� ������ �����
		CAniSeq* tpAniSeq = new CAniSeq();

		*tpAniSeq = *tItor->second;

		mAniSeqs.insert(make_pair(tpAniSeq->mId, tpAniSeq));
	}

	//shallow copy
	//������ �������� ���Ǵ� �ڿ��̴�.
	mpEngine = t.mpEngine;
	//�� �κ��� SetOwnObject���� �缳���� ���̴�.
	mpObject = t.mpObject;

	mStrKeyPrevAniSeq = t.mStrKeyPrevAniSeq;
}

void CAnimator::operator=(const CAnimator& t)
{
	mId = t.mId;

	mpCurAniSeq = t.mpCurAniSeq;		//??
	mStrKeyCurAniSeq = t.mStrKeyCurAniSeq;

	//���� ����
	//�ִϸ����ʹ� �� ���� ��ü���� ���� ������ �ϹǷ� ���� ���縦 �����Ͽ���.
	mAniSeqs.clear();
	unordered_map<string, CAniSeq*>::const_iterator tItor;
	for (tItor = t.mAniSeqs.begin(); tItor != t.mAniSeqs.end(); ++tItor)
	{
		//������ ������ü��ŭ �ִϸ��̼ǽ����� ������ �����
		CAniSeq* tpAniSeq = new CAniSeq();

		*tpAniSeq = *tItor->second;

		mAniSeqs.insert(make_pair(tpAniSeq->mId, tpAniSeq));
	}


	//shallow copy
	//������ �������� ���Ǵ� �ڿ��̴�.
	mpEngine = t.mpEngine;
	//�� �κ��� SetOwnObject���� �缳���� ���̴�.
	mpObject = t.mpObject;

	mStrKeyPrevAniSeq = t.mStrKeyPrevAniSeq;
}


bool CAnimator::AddAniSeq(const string& tId, float tTimeInterval, int tTotalFrameCount, LPCWSTR tpFileName, ANI_PO tPlayOption)
{
	//CAniSeq* Ÿ���� ��ü�� ����� ���� �����Ѵ�
	CAniSeq* tpAniSeq = new CAniSeq();

	tpAniSeq->mId = tId;
	tpAniSeq->mTimeInterval = tTimeInterval;
	tpAniSeq->mTotalFrameCount = tTotalFrameCount;


	tpAniSeq->mPlayOption = tPlayOption;

	//�� ������ ������ŭ ������ ������ �����Ͽ� �߰��Ѵ�
	for (int ti = 0; ti < tTotalFrameCount; ++ti)
	{
		CTexture* tpTex = nullptr;
		tpTex = new CTexture();		
		tpTex->AddRef();

		//bongbong_0.bmp, bongbong_1.bmp, 
		//�̸� ���ص� �����̸� ��Ģ�� �ٰ��Ͽ� �ۼ��ߴ�
		WCHAR szTemp[256] = { 0 };
		wsprintf(szTemp, L"%s_%d.bmp", tpFileName, ti);

		tpTex->LoadTexture(mpEngine->GetHInstance(), mpEngine->GetHDC(), szTemp);
		tpAniSeq->mTexs.push_back(tpTex);
	}

	mAniSeqs.insert(make_pair(tId, tpAniSeq));

	return true;
}

void CAnimator::UpdateAnimation(float tDeltaTime)
{
	//test data
//mStrKeyCurAniSeq = "ani_idle_actor";
	//mStrKeyCurAniSeq = "ani_super_actor";


	unordered_map<string, CAniSeq*>::iterator tItor = mAniSeqs.find(mStrKeyCurAniSeq);

	if (tItor != mAniSeqs.end())
	{
		mpCurAniSeq = tItor->second;

		mpCurAniSeq->Update(tDeltaTime);
	}
}
void CAnimator::Render(float tX, float tY)
{
	if (!mpCurAniSeq)
	{
		return;
	}


	//���� �ִϸ��̼� ��������
	int tIndex = mpCurAniSeq->mCurFrameIndex;
	//������ ���� ������  �� �����Ѵ�
	CTexture* tpTex = nullptr;
	tpTex = mpCurAniSeq->mTexs[tIndex];

	if (tpTex)
	{
		mpEngine->DrawTexture(tX, tY, tpTex);
	}

	LateUpdate();

}

void CAnimator::SetDefaultAniSeq(const string& tStrDefaultAniSeq)
{
	//���� �÷��� ���� �ִϸ��̼� �������� ������ �д�
	mStrKeyPrevAniSeq = tStrDefaultAniSeq;
	//���� �÷����� �ִϸ��̼� �������� �����Ѵ�
	mStrKeyCurAniSeq = tStrDefaultAniSeq;

	if (mpCurAniSeq)
	{
		//�ִϸ��̼� ������ �÷��� ���� ���� �ʱ�ȭ
		mpCurAniSeq->mCurFrameIndex = 0;
		mpCurAniSeq->mAniTime = 0.0f;
	}
}
void CAnimator::PlayAni(const string& tStrAniSeq)
{
	//���� �÷��� ���� �ִϸ��̼� �������� ������ �д�
	mStrKeyPrevAniSeq = mStrKeyCurAniSeq;     

	//���� �÷����� �ִϸ��̼� �������� �����Ѵ�
	mStrKeyCurAniSeq = tStrAniSeq;

	if (mpCurAniSeq)
	{
		//�ִϸ��̼� ������ �÷��� ���� ���� �ʱ�ȭ
		mpCurAniSeq->mCurFrameIndex = 0;
		mpCurAniSeq->mAniTime = 0.0f;
	}
}
void CAnimator::LateUpdate()
{
	//�ִϸ��̼� ������ �÷��� �ɼǿ� ���� �б�
	switch (mpCurAniSeq->mPlayOption)
	{
	case ANI_PO::LOOP:
	{
	}
	break;
	case ANI_PO::ONCE:
	{
		//���� �÷��̵ǰ� �ִ� �ִϸ��̼� �������� ONCE �ɼ��̸�
		//������ �����ӿ� �������� ��
		//���� �ִϸ��̼� �������� �÷��� �����Ѵ�

		if (mpCurAniSeq->mCurFrameIndex == mpCurAniSeq->mTotalFrameCount - 1)
		{
			//���⿡ ������ ��������Ʈ ������ ���� �ð��� �־��� ������,
			//�̷��� ���� ������ ������ ��������Ʈ ������ �� �� �� ���ӷ��� �����Ӹ� �����ǹǷ� 
			//�츮�� �ǵ��� ��� ����� �������� �ʱ� �����̴�.
			if (mpCurAniSeq->mAniTime >= mpCurAniSeq->mTimeInterval)
			{
				mStrKeyCurAniSeq = mStrKeyPrevAniSeq;

				//�ִϸ��̼� ������ �÷��� ���� ���� �ʱ�ȭ
				mpCurAniSeq->mCurFrameIndex = 0;
				mpCurAniSeq->mAniTime = 0.0f;
			}
		}
	}
	break;
	}
}


void CAnimator::Create(CAPIEngine* tpEngine)
{
	mpEngine = tpEngine;
}
void CAnimator::Destroy()
{

}