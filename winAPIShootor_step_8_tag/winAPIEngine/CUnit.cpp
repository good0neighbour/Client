#include "CUnit.h"

#include "CAPIEngine.h"
#include "CTexture.h"

#include "CCollider.h"

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

	mpCollider = nullptr;

	mpTexture = nullptr;

	mIsActive = false;

	mpEngine = nullptr;

	mTag = "";
}

CUnit::~CUnit()
{
	//�浹ü ���� �۾�
	SAFE_DELETE(mpCollider);
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

	mIsActive = t.mIsActive;
	//���� ����
	mpEngine = t.mpEngine;

	//���ָ��� �浹ü�� ���� ������ �ϹǷ� '���� ����'�� �����ؾ� �Ѵ�.
	//���� ����
	//mpCollider = t.mpCollider;
	//���� ���� deep copy
	mpCollider = new CCollider();		//��ü�� ���� �����.
	(*mpCollider) = (*t.mpCollider);	//���� ������ü�κ��� �����´�
	//���� �� �浹ü�� �����Ǿ� ������� ���ο� ���� ��ü�� �浹ü�̴�. ������ ����
	mpCollider->SetOwnerObject(this);

	mTag = t.mTag;
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

	mIsActive = t.mIsActive;

	//���� ����
	mpEngine = t.mpEngine;

	//���� ���� deep copy
	mpCollider = new CCollider();		//��ü�� ���� �����.
	(*mpCollider) = (*t.mpCollider);	//���� ������ü�κ��� �����´�

	//���� �� �浹ü�� �����Ǿ� ������� ���ο� ���� ��ü�� �浹ü�̴�. ������ ����
	mpCollider->SetOwnerObject(this);

	mTag = t.mTag;
}

void CUnit::Create(CAPIEngine* tpEngine)
{
	mpEngine = tpEngine;

	//�浹ü ���� �۾�
	mpCollider = new CCollider();
	mpCollider->Create(this->GetPosition());
	mpCollider->SetOwnerObject(this);	//������ ��ü ����
}
void CUnit::Destroy()
{
	//�浹ü ���� �۾�
	SAFE_DELETE(mpCollider);
}
void CUnit::Update(float tDeltaTime)
{
	if (mIsActive)
	{
		//'���Ϸ� ������ ���й�' �� ���� ��ġ �̵� �ڵ�
		//this->mPosition = this->mPosition + mVelocity * 1.0f;	//������ ��� ����

		this->mPosition = this->mPosition + mVelocity * tDeltaTime;	//�ð� ��� ����

		//�浹ü�� ��ġ ����
		mpCollider->Update();
	}
}
//CAPIEngine���� ���踦 �Ű������� ǥ���ߴ�.
//				<---CUnitŬ������ CAPIEngineŬ������ ���������̴�.
void CUnit::Render()
{	
	if (mIsActive)
	{
		//�ǹ��� ����Ͽ� ����ϱ�
		mWidth = mpTexture->mBitmapInfo.bmWidth;
		mHeight = mpTexture->mBitmapInfo.bmHeight;

		//mDisplayX, mDisplayY ������
		/*mDisplayX = mX - mWidth * mAnchorX;
		mDisplayY = mY - mHeight* mAnchorY;*/
		mDisplayX = mPosition.mX - mWidth * mAnchorX;
		mDisplayY = mPosition.mY - mHeight * mAnchorY;

		mpEngine->DrawTexture(mDisplayX, mDisplayY, mpTexture);

		//DEBUG DRAW
		//mpEngine->DrawCircle(mPosition.mX, mPosition.mY, mRadius);

		//�浹ü ����
		//collider render
		SVector2D tPosition = mpCollider->GetPosition();
		float tWidth = mpCollider->GetWidth();
		float tHeight = mpCollider->GetHeight();
		float tAnchorX = mpCollider->GetAnchorX();
		float tAnchorY = mpCollider->GetAnchorY();

		mpEngine->DrawRect(
			tPosition.mX - tWidth * tAnchorX,
			tPosition.mY - tHeight * tAnchorX,
			tWidth,
			tHeight);
		/*mpEngine->DrawRect(
			tPosition.mX - tWidth * tAnchorX,
			tPosition.mX + tWidth * tAnchorX,
			tPosition.mY - tHeight * tAnchorY,
			tPosition.mY + tHeight * tAnchorY);*/
	}
}