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
	//������ �ִϸ��̼� �������̸� ���� ���� ��Ȱ��ȭ
	if (mpAnimator)
	{
		if (mpAnimator->mpCurAniSeq->mCurFrameIndex == mpAnimator->mpCurAniSeq->mTotalFrameCount - 1)
		{
			this->SetIsActive(false);
		}
	}
}