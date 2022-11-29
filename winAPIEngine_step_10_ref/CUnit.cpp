#include "CUnit.h"

#include "CAPIEngine.h"
#include "CTexture.h"

CUnit::CUnit()
	:CObjectRyu(), mRadius(0.0f)
	//:mX(0.0f), mY(0.0f), mRadius(0.0f)
{
	float mDisplayX = 0.0f;
	float mDisplayY = 0.0f;
	float mAnchorX = 0.0f;
	float mAnchorY = 0.0f;
	float mWidth = 0.0f;
	float mHeight = 0.0f;

	CTexture* mpTexture = nullptr;
}
CUnit::~CUnit()
{

}

CUnit::CUnit(const CUnit& t)
{
	//mPosition = t.mPosition;
	CUnit::CUnit(t);

	//mX = t.mX;
	//mY = t.mY;
	mRadius = t.mRadius;

    mDisplayX = t.mDisplayX;
	mDisplayY = t.mDisplayY;
	mAnchorX = t.mAnchorX;
	mAnchorY = t.mAnchorY;
	mWidth = t.mWidth;
	mHeight = t.mHeight;

	//���� ����
	//��Ʈ�� �̹��� �����ʹ� shared resource�� �ٷ�ڴ�.
	//������ ���� �ܰ��� ������ �������̹Ƿ�.
	mpTexture = t.mpTexture;
}
void CUnit::operator=(const CUnit& t)
{
	//mPosition = t.mPosition;
	CUnit::operator=(t);
	//mX = t.mX;
	//mY = t.mY;
	mRadius = t.mRadius;

	mDisplayX = t.mDisplayX;
	mDisplayY = t.mDisplayY;
	mAnchorX = t.mAnchorX;
	mAnchorY = t.mAnchorY;
	mWidth = t.mWidth;
	mHeight = t.mHeight;

	//���� ����
	//��Ʈ�� �̹��� �����ʹ� shared resource�� �ٷ�ڴ�.
	mpTexture = t.mpTexture;
}
//CAPIEngine���� ���踦 �Ű������� ǥ���ߴ�.
//		<---CUnitŬ������ CAPIEngineŬ������ ���������.
void CUnit::Render(CAPIEngine* tpEngine)
{
	//�ǹ��� ����Ͽ� ����ϱ�
	mWidth = mpTexture->mBitmapInfo.bmWidth;
	mHeight = mpTexture->mBitmapInfo.bmHeight;

	//mDisplayX, mDisplayY ������
	//mDisplayX = mX - mWidth * mAnchorX;
	//mDisplayY = mY - mHeight * mAnchorY;
	mDisplayX = mPosition.mX - mWidth * mAnchorX;
	mDisplayY = mPosition.mY - mHeight * mAnchorY;

	tpEngine->DrawTexture(mDisplayX, mDisplayY, mpTexture);

	//tpEngine->DrawCircle(mX, mY, mRadius);
}