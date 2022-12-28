#include "CUnit.h"

#include "CAPIEngine.h"
#include "CTexture.h"

#include "CCollider.h"

#include "CAnimator.h"

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

	mIsActive = false;

	mpEngine = nullptr;

	mpCollider = nullptr;

	mTag = "";

	mpAnimator = nullptr;
}
CUnit::~CUnit()
{
	//�浹ü ���� �۾�
	SAFE_DELETE(mpCollider);

	SAFE_DELETE(mpAnimator);
}

CUnit::CUnit(const CUnit& t)
	:CObjectRyu(t)
{
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
	(*mpCollider) = (*t.mpCollider);		//���� ������ü�κ��� �����´�
	//���� �� �浹ü�� �����Ǿ� ������� ���ο� ���� ��ü�� �浹ü�̴�. ������ ����
	mpCollider->SetOwnerObject(this);

	mTag = t.mTag;

	//ryu
	// shallow copy
	//mpAnimator = t.mpAnimator;
	// deep copy
	//������ü�� mpAniamtor��ü�� ��ȿ�ϰ� ���� ��츸 deep copy
	if (t.mpAnimator)
	{
		mpAnimator = new CAnimator();
		(*mpAnimator) = (*t.mpAnimator);
		mpAnimator->SetOwnerObject(this);
	}
}
void CUnit::operator=(const CUnit& t)
{
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

	//���ָ��� �浹ü�� ���� ������ �ϹǷ� '���� ����'�� �����ؾ� �Ѵ�.
	//���� ����
	//mpCollider = t.mpCollider;
	//���� ���� deep copy
	mpCollider = new CCollider();		//��ü�� ���� �����.
	(*mpCollider) = (*t.mpCollider);		//���� ������ü�κ��� �����´�

	//���� �� �浹ü�� �����Ǿ� ������� ���ο� ���� ��ü�� �浹ü�̴�. ������ ����
	mpCollider->SetOwnerObject(this);

	mTag = t.mTag;

	//ryu
	//shallow copy
	//mpAnimator = t.mpAnimator;
	// deep copy
	//������ü�� mpAniamtor��ü�� ��ȿ�ϰ� ���� ��츸 deep copy
	if (t.mpAnimator)
	{
		mpAnimator = new CAnimator();
		(*mpAnimator) = (*t.mpAnimator);
		mpAnimator->SetOwnerObject(this);
	}
}

void CUnit::Create(CAPIEngine* tpEngine)
{
	mpEngine = tpEngine;

	//�浹ü ���� �۾�
	mpCollider = new CCollider();
	mpCollider->Create(this->GetPosition());
	mpCollider->SetOwnerObject(this);		//������ ��ü ����
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
		//�ǹ��� �����Ͽ� ����ϱ�
		mWidth = mpTexture->mBitmapInfo.bmWidth;
		mHeight = mpTexture->mBitmapInfo.bmHeight;

		//mDisplayX, mDisplayY ������
		/*mDisplayX = mX - mWidth * mAnchorX;
		mDisplayY = mY - mHeight* mAnchorY;*/
		mDisplayX = mPosition.mX - mWidth * mAnchorX;
		mDisplayY = mPosition.mY - mHeight * mAnchorY;

		//�ִϸ��̼� ������ ������ �ִϸ����ͷ� ����
		//�׷��� ������ ��ǥ�̹����� ����
		if (nullptr != mpAnimator)
		{
			mpAnimator->UpdateAnimation(mpEngine->GetDeltaTime());
			mpAnimator->Render(mDisplayX, mDisplayY);
		}
		else
		{
			//��ǥ�̹���
			mpEngine->DrawTexture(mDisplayX, mDisplayY, mpTexture);
		}
	}
}

CAnimator* CUnit::CreateAnimation(const string& tId, CAPIEngine* tpEngine)
{
	mpAnimator = new CAnimator();

	mpAnimator->SetId(tId);
	mpAnimator->Create(tpEngine);

	return mpAnimator;
}
void CUnit::DestroyAnimation()
{
	if (mpAnimator)
	{
		mpAnimator->Destroy();
	}

	SAFE_DELETE(mpAnimator)
}
//wrapping CUnit::SetDefaultAniSeq�� ����
void CUnit::SetDefaultAniSeq(const string& tStrDefaultAniSeq)
{
	//mpAnimator�� �ش� ����� �����Ͽ���.
	mpAnimator->SetDefaultAniSeq(tStrDefaultAniSeq);
}
void CUnit::PlayAni(const string& tStrAniSeq)
{
	//mpAnimator�� �ش� ����� �����Ͽ���.
	mpAnimator->PlayAni(tStrAniSeq);
}