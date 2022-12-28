#include "CAnimator.h"

#include "ryumacro.h"

#include "CAniSeq.h"
#include "CAPIEngine.h"

#include "CTexture.h"

CAnimator::CAnimator()
{
	mId = "";

	mpCurAniSeq = nullptr;
	mStrKeyCurAniSeq = "";

	mAniSeq.clear();

	mpEngine = nullptr;
	mpObject = nullptr;

	mStrKeyPrevAniSeq = "";
}
CAnimator::~CAnimator()
{
	unordered_map<string, CAniSeq*>::iterator tItor;
	for (tItor = mAniSeq.begin(); tItor != mAniSeq.end(); ++tItor)
	{
		SAFE_DELETE(tItor->second);
	}

	mAniSeq.clear();
}

CAnimator::CAnimator(const CAnimator& t)
{
	mId = t.mId;

	mpCurAniSeq = t.mpCurAniSeq;	//??
	mStrKeyCurAniSeq = t.mStrKeyCurAniSeq;

	//깊은 복사
	//애니메이터는 각 유닛 객체마다 따로 가져야 하므로 깊은 복사를 수행하였다.
	mAniSeq.clear();
	unordered_map<string, CAniSeq*>::const_iterator tItor;
	for (tItor = mAniSeq.begin(); tItor != mAniSeq.end(); ++tItor)
	{
		//복사의 원본 객체만큼 애니메이션 시퀀스 갯수를 만든다
		CAniSeq* tpAniSeq = new CAniSeq();

		*tpAniSeq = *tItor->second;

		mAniSeq.insert(make_pair(tpAniSeq->mId, tpAniSeq));
	}

	//shallow copy
	//엔진은 공용으로 사용되는 자원이다.
	mpEngine = t.mpEngine;
	//이 부분은 SetOwnObject에서 재설정될 것이다.
	mpObject = t.mpObject;

	mStrKeyPrevAniSeq = t.mStrKeyPrevAniSeq;
}

void CAnimator::operator =(const CAnimator& t)
{
	mId = t.mId;

	mpCurAniSeq = t.mpCurAniSeq;	//??
	mStrKeyCurAniSeq = t.mStrKeyCurAniSeq;

	//깊은 복사
	//애니메이터는 각 유닛 객체마다 따로 가져야 하므로 깊은 복사를 수행하였다.
	mAniSeq.clear();
	unordered_map<string, CAniSeq*>::const_iterator tItor;
	for (tItor = t.mAniSeq.begin(); tItor != t.mAniSeq.end(); ++tItor)
	{
		//복사의 원본 객체만큼 애니메이션 시퀀스 갯수를 만든다
		CAniSeq* tpAniSeq = new CAniSeq();

		*tpAniSeq = *tItor->second;

		mAniSeq.insert(make_pair(tpAniSeq->mId, tpAniSeq));
	}

	//shallow copy
	//엔진은 공용으로 사용되는 자원이다.
	mpEngine = t.mpEngine;
	//이 부분은 SetOwnObject에서 재설정될 것이다.
	mpObject = t.mpObject;

	mStrKeyPrevAniSeq = t.mStrKeyPrevAniSeq;
}

bool CAnimator::AddAniSeq(const string& tId, float tTimeInterval, int tTotalFrameCount, LPCWSTR tpFileName, ANI_PO tPlayOption)
{
	//CAniSeq* 타입의 객체를 만들고 값을 결정한다
	CAniSeq* tpAniSeq = new CAniSeq();

	tpAniSeq->mId = tId;
	tpAniSeq->mTimeInterval = tTimeInterval;
	tpAniSeq->mTotalFrameCount = tTotalFrameCount;

	tpAniSeq->mPlayOption = tPlayOption;

	//총 프레임 개수만큼 프레임 정보를 생성하여 추가한다
	for (int ti = 0; ti < tTotalFrameCount; ti++)
	{
		CTexture* tpTex = nullptr;
		tpTex = new CTexture();

		tpTex->AddRef();

		//bongbong_0.bmp, bongbong_1.bmp
		//미리 정해둔 파일이름 규칙에 근거하여 저장했다.
		WCHAR szTemp[256] = { 0 };
		wsprintf(szTemp, L"%s_%d.bmp", tpFileName, ti);

		tpTex->LoadTexture(mpEngine->GetHInstance(), mpEngine->GetHDC(), szTemp);
		tpAniSeq->mTexs.push_back(tpTex);
	}

	mAniSeq.insert(make_pair(tId, tpAniSeq));

	return true;
}

void CAnimator::UpdateAnimation(float tDeltaTime)
{
	//test data
	//mStrKeyCurAniSeq = "ani_idle_actor";
	//mStrKeyCurAniSeq = "ani_super_actor";

	unordered_map<string, CAniSeq*>::const_iterator tItor = mAniSeq.find(mStrKeyCurAniSeq);

	if (tItor != mAniSeq.end())
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

	//현재 애니메이션 시퀀스에
	int tIndex = mpCurAniSeq->mCurFrameIndex;
	//임의의 현재 프레임을 렌더한다
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
	//현재 플레이 중인 애니메이션 시퀀스를 기억시켜 둔다
	mStrKeyPrevAniSeq = tStrDefaultAniSeq;

	//새로 플레이할 애니매이션 시퀀스를 설정한다
	mStrKeyCurAniSeq = tStrDefaultAniSeq;

	if (mpCurAniSeq)
	{
		//애니메이션 시퀀스 플레이 관련 변수 초기화
		mpCurAniSeq->mCurFrameIndex = 0;
		mpCurAniSeq->mAniTime = 0.0f;
	}
}

void CAnimator::PlayAni(const string& tStrAniSeq)
{
	//현재 플레이 중인 애니메이션 시퀀스를 기억시켜 둔다
	mStrKeyPrevAniSeq = mStrKeyCurAniSeq;

	//새로 플레이할 애니매이션 시퀀스를 설정한다
	mStrKeyCurAniSeq = tStrAniSeq;

	if (mpCurAniSeq)
	{
		//애니메이션 시퀀스 플레이 관련 변수 초기화
		mpCurAniSeq->mCurFrameIndex = 0;
		mpCurAniSeq->mAniTime = 0.0f;
	}
}

void CAnimator::LateUpdate()
{
	//애니메이션 시퀀스 플레이 옵션에 따라서 분기
	switch (mpCurAniSeq->mPlayOption)
	{
	case ANI_PO::LOOP:
	{

	}
	break;
	case ANI_PO::ONCE:
	{
		//현재 플레이 되고 있는 애니메이션 시퀀스가 ONCE 옵션이면
		//마지막 프레임에 도달했을 때
		//이전 에니메이션 시퀀스를 플레이 시작한다.

		if (mpCurAniSeq->mCurFrameIndex == mpCurAniSeq->mTotalFrameCount - 1)
		{
			//여기에 임의의 스프라이트 프레임 유지 시간을 넣어준 이유는,
			//이렇게 하지 않으면 마지막 스프라이트 프레임 이 딱 한 게임 루프 프레임만 랜더되므로
			//우리가 의도한 대로 제대로 랜더되지 않기 때문이다.
			if (mpCurAniSeq->mAniTime >= mpCurAniSeq->mTimeInterval)
			{
				mStrKeyCurAniSeq = mStrKeyPrevAniSeq;

				//애니메이션 시퀀스 플레이 관련 변수 초기화
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