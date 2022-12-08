#include "CUnit.h"

#include "CAPIEngine.h"
#include "CTexture.h"

CUnit::CUnit()
	:CObjectRyu(), mRadius(0.0f)
	//:mX(0.0f), mY(0.0f), mRadius(0.0f)
{
	mDisplayX = 0.0f;
	mDisplayY = 0.0f;
	mAnchorX = 0.0f;
	mAnchorY = 0.0f;
	mWidth = 0.0f;
	mHeight = 0.0f;

	mpTexture = nullptr;
}
CUnit::~CUnit()
{

}

CUnit::CUnit(const CUnit& t)
	:CObjectRyu(t)
{
	//mPosition = t.mPosition;
	//CUnit::CUnit(t);
	//CObjectRyu::CObjectRyu(t);

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

void CUnit::operator=(const CUnit& t)
{
	//mPosition = t.mPosition;
	//CUnit::operator=(t);
	CObjectRyu::operator=(t);

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
	//��Ʈ�� �̹��� �����ʹ� shared resource(�����ڿ�)�� �ٷ�ڴ�.
	//������ ���� �ܰ��� ������ �������̹Ƿ�.
	mpTexture = t.mpTexture;
}

void  CUnit::Update(float tDeltaTime)
{
	//'���Ϸ� ������ ���й�' �� ���� ��ġ �̵� �ڵ�
	//this->mPosition = this->mPosition + mVelocity * 1.0f;		//������ ��� ����

	this->mPosition = this->mPosition + mVelocity * tDeltaTime;	//�ð� ��� ����
}

//CAPIEngine���� ���踦 �Ű������� ǥ���ߴ�.
//				<---CUnitŬ������ CAPIEngineŬ������ ���������̴�.
void CUnit::Render(CAPIEngine* tpEngine)
{	
	//�ǹ��� ����Ͽ� ����ϱ�
	mWidth = mpTexture->mBitmapInfo.bmWidth;
	mHeight = mpTexture->mBitmapInfo.bmHeight;

	//mDisplayX, mDisplayY ������
	/*mDisplayX = mX - mWidth * mAnchorX;
	mDisplayY = mY - mHeight* mAnchorY;*/
	mDisplayX = mPosition.mX - mWidth * mAnchorX;
	mDisplayY = mPosition.mY - mHeight * mAnchorY;

	tpEngine->DrawTexture(mDisplayX, mDisplayY, mpTexture);

	//DEBUG DRAW
	//tpEngine->DrawCircle(mX, mY, mRadius);
}