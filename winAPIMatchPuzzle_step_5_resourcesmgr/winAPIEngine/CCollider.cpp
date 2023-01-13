#include "CCollider.h"

#include "CUnit.h"


CCollider::CCollider()
	:mPosition(), mAnchorX(0.0f), mAnchorY(0.0f), mWidth(0.0f), mHeight(0.0f), mpObject(nullptr)
{

}
CCollider::~CCollider()
{

}
CCollider::CCollider(const CCollider& t)
{
	mPosition = t.mPosition;

	mAnchorX = t.mAnchorX;
	mAnchorY = t.mAnchorY;

	mWidth = t.mWidth;
	mHeight = t.mHeight;

	mpObject = t.mpObject;
}
void CCollider::operator=(const CCollider& t)
{
	mPosition = t.mPosition;

	mAnchorX = t.mAnchorX;
	mAnchorY = t.mAnchorY;

	mWidth = t.mWidth;
	mHeight = t.mHeight;

	mpObject = t.mpObject;
}

bool CCollider::DoCollision(CCollider* tOther)
{
	bool tResult = false;
	//�̷��� �Լ��� ����wrapping�Ͽ� �����adapter �������� �� ������ ���⼭ ���� �ɼ����� ���� �����ϰ� �ϱ� �����̴�.
	tResult = DoCollisionAABB(tOther);

	return tResult;
}

void CCollider::Create(SVector2D tPosition, float tWidth, float tHeight, float tAnchorX, float tAnchorY)
{
	mPosition = tPosition;
	mWidth = tWidth;
	mHeight = tHeight;
	mAnchorX = tAnchorX;
	mAnchorY = tAnchorY;
}
void CCollider::Update()
{
	//�ڽ��� '������' �� ������ ��ġ�� �޾� �浹ü�� ��ġ�� �����ϰڴ�
	mPosition = mpObject->GetPosition();
}

void CCollider::AddToCollisions(CCollider* tpCollider)
{
	mCollisions.push_back(tpCollider);
}
void CCollider::EraseFromCollisions(CCollider* tpCollider)
{
	list<CCollider*>::iterator tItor;
	for (tItor = mCollisions.begin(); tItor != mCollisions.end(); ++tItor)
	{
		if ((*tItor) == tpCollider)
		{
			mCollisions.erase(tItor);
			break;
		}
	}
}
bool CCollider::DoCheckBeInCollisions(CCollider* tpCollider)
{
	list<CCollider*>::iterator tItor;
	for (tItor = mCollisions.begin(); tItor != mCollisions.end(); ++tItor)
	{
		if ((*tItor) == tpCollider)
		{
			return true;
		}
	}


	return false;
}

bool CCollider::DoCollisionAABB(CCollider* tOther)
{
	//A �簢��
	float tLeftX = 0.0f;
	float tRightX = 0.0f;
	float tTopY = 0.0f;
	float tBottomY = 0.0f;


	tLeftX = mPosition.mX - GetWidth() * GetAnchorX();
	tRightX = mPosition.mX - GetWidth() * GetAnchorX() + GetWidth();
	tTopY = mPosition.mY - GetHeight() * GetAnchorY();
	tBottomY = mPosition.mY - GetHeight() * GetAnchorY() + GetHeight();

	//B �簢��
	float tLeftX_Other = 0.0f;
	float tRightX_Other = 0.0f;
	float tTopY_Other = 0.0f;
	float tBottomY_Other = 0.0f;

	tLeftX_Other = tOther->GetPosition().mX - tOther->GetWidth() * tOther->GetAnchorX();
	tRightX_Other = tOther->GetPosition().mX - tOther->GetWidth() * tOther->GetAnchorX() + tOther->GetWidth();
	tTopY_Other = tOther->GetPosition().mY - tOther->GetHeight() * tOther->GetAnchorY();
	tBottomY_Other = tOther->GetPosition().mY - tOther->GetHeight() * tOther->GetAnchorY() + tOther->GetHeight();

	if (tRightX < tLeftX_Other)
	{
		return false;
	}

	if (tLeftX > tRightX_Other)
	{
		return false;
	}

	if (tBottomY < tTopY_Other)
	{
		return false;
	}

	if (tTopY > tBottomY_Other)
	{
		return false;
	}

	//���� ������ ��� ��ġ�� ������� �Դٸ�, �浹�� ����̴�.
	return true;
}