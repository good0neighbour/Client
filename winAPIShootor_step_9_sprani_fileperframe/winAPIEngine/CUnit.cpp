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
	//충돌체 해제 작업
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
	//얕은 복사
	//비트맵 이미지 데이터는 shared resource로 다루겠다.
	mpTexture = t.mpTexture;

	mIsActive = t.mIsActive;
	//얕은 복사
	mpEngine = t.mpEngine;

	//유닛마다 충돌체는 각자 가져야 하므로 '깊은 복사'를 수행해야 한다.
	//얕은 복사
	//mpCollider = t.mpCollider;
	//깊은 복사 deep copy
	mpCollider = new CCollider();		//객체를 새로 만든다.
	(*mpCollider) = (*t.mpCollider);	//값은 원본객체로부터 가져온다
	//이제 이 충돌체는 복제되어 만들어진 새로운 유닛 객체의 충돌체이다. 소유자 지정
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

	//얕은 복사
	//비트맵 이미지 데이터는 shared resource(공유자원)로 다루겠다.
	//어차피 같은 외관을 가지는 비행기들이므로.
	mpTexture = t.mpTexture;

	mIsActive = t.mIsActive;

	//얕은 복사
	mpEngine = t.mpEngine;

	//깊은 복사 deep copy
	mpCollider = new CCollider();		//객체를 새로 만든다.
	(*mpCollider) = (*t.mpCollider);	//값은 원본객체로부터 가져온다

	//이제 이 충돌체는 복제되어 만들어진 새로운 유닛 객체의 충돌체이다. 소유자 지정
	mpCollider->SetOwnerObject(this);

	mTag = t.mTag;
}

void CUnit::Create(CAPIEngine* tpEngine)
{
	mpEngine = tpEngine;

	//충돌체 생성 작업
	mpCollider = new CCollider();
	mpCollider->Create(this->GetPosition());
	mpCollider->SetOwnerObject(this);	//소유자 객체 지정
}
void CUnit::Destroy()
{
	//충돌체 해제 작업
	SAFE_DELETE(mpCollider);
}
void CUnit::Update(float tDeltaTime)
{
	if (mIsActive)
	{
		//'오일러 축차적 적분법' 에 의한 위치 이동 코드
		//this->mPosition = this->mPosition + mVelocity * 1.0f;	//프레임 기반 진행

		this->mPosition = this->mPosition + mVelocity * tDeltaTime;	//시간 기반 진행

		//충돌체의 위치 갱신
		mpCollider->Update();
	}
}
//CAPIEngine과의 관계를 매개변수로 표현했다.
//				<---CUnit클래스가 CAPIEngine클래스에 의존관계이다.
void CUnit::Render()
{	
	if (mIsActive)
	{
		//피벗을 고려하여 출력하기
		mWidth = mpTexture->mBitmapInfo.bmWidth;
		mHeight = mpTexture->mBitmapInfo.bmHeight;

		//mDisplayX, mDisplayY 결정식
		/*mDisplayX = mX - mWidth * mAnchorX;
		mDisplayY = mY - mHeight* mAnchorY;*/
		mDisplayX = mPosition.mX - mWidth * mAnchorX;
		mDisplayY = mPosition.mY - mHeight * mAnchorY;

		mpEngine->DrawTexture(mDisplayX, mDisplayY, mpTexture);

		//DEBUG DRAW
		//mpEngine->DrawCircle(mPosition.mX, mPosition.mY, mRadius);

		//충돌체 렌더
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