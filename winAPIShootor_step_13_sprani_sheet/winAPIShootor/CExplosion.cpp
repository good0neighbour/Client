#include "CExplosion.h"

#include "CAnimator.h"

CExplosion::CExplosion()
{

}
CExplosion::~CExplosion()
{

}

CExplosion::CExplosion(const CExplosion& t)
	:CUnit(t)
{

}
void CExplosion::operator=(const CExplosion& t)
{
	CUnit::operator=(t);
}

void CExplosion::LateUpdate()
{
	//마지막 애니메이션 프레임이면 폭발 유닛 비활성화
	if (mpAnimator)
	{
		if (mpAnimator->mpCurAniSeq->mCurFrameIndex == mpAnimator->mpCurAniSeq->mTotalFrameCount - 1)
		{
			this->SetIsActive(false);
		}
	}
}