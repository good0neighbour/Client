#include "CUnit.h"

#include "CAPIEngine.h"
#include "CTexture.h"

CUnit::CUnit()
	:mX(0.0f), mY(0.0f), mRadius(0.0f)
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
	mX = t.mX;
	mY = t.mY;
	mRadius = t.mX;

    mDisplayX = t.mDisplayX;
	mDisplayY = t.mDisplayY;
	mAnchorX = t.mAnchorX;
	mAnchorY = t.mAnchorY;
	mWidth = t.mWidth;
	mHeight = t.mHeight;

	//얕은 복사
	//비트맵 이미지 데이터는 shared resource로 다루겠다.
	//어차피 같은 외관을 가지는 비행기들이므로.
	mpTexture = t.mpTexture;
}
void CUnit::operator=(const CUnit& t)
{
	mX = t.mX;
	mY = t.mY;
	mRadius = t.mX;

	mDisplayX = t.mDisplayX;
	mDisplayY = t.mDisplayY;
	mAnchorX = t.mAnchorX;
	mAnchorY = t.mAnchorY;
	mWidth = t.mWidth;
	mHeight = t.mHeight;

	//얕은 복사
	//비트맵 이미지 데이터는 shared resource로 다루겠다.
	mpTexture = t.mpTexture;
}
//CAPIEngine과의 관계를 매개변수로 표현했다.
//		<---CUnit클래스가 CAPIEngine클래스에 의존관계다.
void CUnit::Render(CAPIEngine* tpEngine)
{
	//피벗을 고려하여 출력하기
	mWidth = mpTexture->mBitmapInfo.bmWidth;
	mHeight = mpTexture->mBitmapInfo.bmHeight;

	//mDisplayX, mDisplayY 결정식
	mDisplayX = mX - mWidth * mAnchorX;
	mDisplayY = mY - mHeight * mAnchorY;

	tpEngine->DrawTexture(mDisplayX, mDisplayY, mpTexture);

	//tpEngine->DrawCircle(mX, mY, mRadius);
}