#include "CAnimator.h"

#include "ryumacro.h"

//#include "CAniSeq.h"
#include "CAPIEngine.h"

#include "CTexture.h"

#include "ryumacro.h"

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


bool CAnimator::AddAniSeq(const string& tId, float tTimeInterval, int tTotalFrameCount, LPCWSTR tpFileName, ANI_PO tPlayOption, ANI_SO tSpriteOption, int tRow, int tCol)
{
	//CAniSeq* Ÿ���� ��ü�� ����� ���� �����Ѵ�
	CAniSeq* tpAniSeq = new CAniSeq();

	tpAniSeq->mId = tId;
	tpAniSeq->mTimeInterval = tTimeInterval;
	tpAniSeq->mTotalFrameCount = tTotalFrameCount;


	tpAniSeq->mPlayOption = tPlayOption;

	tpAniSeq->mSpriteOption = tSpriteOption;
	tpAniSeq->mRow = tRow;
	tpAniSeq->mCol = tCol;

	//������ �ϳ��� ���� �ϳ� �� ������ ���� �̷��� ó��
	if (ANI_SO::FRAME_FILE == tSpriteOption)
	{
		//�� ������ ������ŭ ������ ������ �����Ͽ� �߰��Ѵ�
		for (int ti = 0; ti < tTotalFrameCount; ++ti)
		{
			/*CTexture* tpTex = nullptr;
			tpTex = new CTexture();
			tpTex->AddRef();
			
			WCHAR szTemp[256] = { 0 };
			wsprintf(szTemp, L"%s_%d.bmp", tpFileName, ti);

			tpTex->LoadTexture(mpEngine->GetHInstance(), mpEngine->GetHDC(), szTemp);

			tpAniSeq->mTexs.push_back(tpTex);*/

			//���� �ڵ忡�� �ڿ� ���Ͽ��� �ε��ϰ� �� �����Ϳ� �����ϴ� �ڵ尡 ȥ��Ǿ� �־����� 
			// ������ �����ϴ� �ڵ常 ���⿡ �����Ѵ�.
			// �ڿ��� ���Ͽ��� �ε��ϴ� �κ��� ���α׷� �ʱ⿡ ó���ϰ� 
			// ���⼭�� ���� �ڿ������ڿ� ���Ǹ� �Ͽ� ���ϴ� �ڿ��� ��´�
			//tId.c_str(), ti
			char szTemp[256] = { 0 };
			sprintf_s(szTemp, "%s_%d", tId.c_str(), ti);
			string tString = szTemp;
			tpAniSeq->mTexs.push_back(FIND_TEX(tString));

			//������ �������� �ʺ�, ���̸� ����
			tpAniSeq->mSpriteWidth = tpAniSeq->mTexs[0]->mBitmapInfo.bmWidth / 1;
			tpAniSeq->mSpriteHeight = tpAniSeq->mTexs[0]->mBitmapInfo.bmHeight / 1;
		}		
	}
	//������ N���� �ϳ��� ���Ͽ� ��� ���
	else if (ANI_SO::SHEET_FILE == tSpriteOption)
	{
		/*CTexture* tpTex = nullptr;
		tpTex = new CTexture();
		tpTex->AddRef();
		
		WCHAR szTemp[256] = { 0 };
		wsprintf(szTemp, L"%s.bmp", tpFileName);

		tpTex->LoadTexture(mpEngine->GetHInstance(), mpEngine->GetHDC(), szTemp);
		tpAniSeq->mTexs.push_back(tpTex);*/
		//���� �ڵ忡�� �ڿ� ���Ͽ��� �ε��ϰ� �� �����Ϳ� �����ϴ� �ڵ尡 ȥ��Ǿ� �־����� 
			// ������ �����ϴ� �ڵ常 ���⿡ �����Ѵ�.
			// �ڿ��� ���Ͽ��� �ε��ϴ� �κ��� ���α׷� �ʱ⿡ ó���ϰ� 
			// ���⼭�� ���� �ڿ������ڿ� ���Ǹ� �Ͽ� ���ϴ� �ڿ��� ��´�
		tpAniSeq->mTexs.push_back(FIND_TEX(tId));


		//������ �������� �ʺ�, ���̸� ����
		tpAniSeq->mSpriteWidth = tpAniSeq->mTexs[0]->mBitmapInfo.bmWidth/tCol;
		tpAniSeq->mSpriteHeight = tpAniSeq->mTexs[0]->mBitmapInfo.bmHeight / tRow;
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

	//������ �ϳ��� ���� �ϳ� �� ������ ���� �̷��� ó��
	if (ANI_SO::FRAME_FILE == mpCurAniSeq->mSpriteOption)
	{
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
	//������ N���� �ϳ��� ���Ͽ� ��� ���
	else if (ANI_SO::SHEET_FILE == mpCurAniSeq->mSpriteOption)
	{
		//���� �ִϸ��̼� ��������
		int tIndex = mpCurAniSeq->mCurFrameIndex;
		//������ ���� ������  �� �����Ѵ�
		CTexture* tpTex = nullptr;
		tpTex = mpCurAniSeq->mTexs[0];		//<-- �̹����� �� ���� ����

		if (tpTex)
		{
			//mpEngine->DrawTexture(tX, tY, tpTex);
			mpEngine->DrawTexturePartial(tX, tY, tpTex, mpCurAniSeq->mRow, mpCurAniSeq->mCol, tIndex);
		}

		LateUpdate();
	}

}

void CAnimator::SetDefaultAniSeq(const string& tStrDefaultAniSeq)
{
	//���� �÷��� ���� �ִϸ��̼� �������� ������ �д�
	mStrKeyPrevAniSeq = tStrDefaultAniSeq;
	//���� �÷����� �ִϸ��̼� �������� �����Ѵ�
	mStrKeyCurAniSeq = tStrDefaultAniSeq;


	unordered_map<string, CAniSeq*>::iterator tItor = mAniSeqs.find(mStrKeyCurAniSeq);

	if (tItor != mAniSeqs.end())
	{
		mpCurAniSeq = tItor->second;
	}


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