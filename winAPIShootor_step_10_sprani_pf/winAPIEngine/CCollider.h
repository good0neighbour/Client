#pragma once

#include "SVector2D.h"

//�߰�, ������ ������ �ڷᱸ���� ����
#include <list>
using namespace std;

//'�浹ü' Ŭ����
// �浹 �˻縦 �����ϴ� Ŭ����
//	DoCollision�Լ�

class CUnit;

class CCollider
{
public:
	CCollider();
	~CCollider();

	CCollider(const CCollider& t);

	void operator=(const CCollider& t);

public:
	bool DoCollision(CCollider* tOther);

	void Create(SVector2D tPosition, float tWidth = 64.0f, float tHeight = 64.0f, float tAnchorX = 0.5f, float tAnchorY = 0.5f);

	void Update();

	//�浹ü ��Ͽ� �浹ü ���
	void AddToCollisions(CCollider* tpCollider);
	//�浹ü ��Ͽ��� �浹ü �������
	void EraseFromCollisions(CCollider* tpCollider);
	//������ �浹ü�� �浹ü ��Ͽ� ����ִ��� ����
	bool DoCheckBeInCollisions(CCollider* tpCollider);

	inline SVector2D GetPosition()
	{
		return mPosition;
	}

	inline float GetAnchorX()
	{
		return mAnchorX;
	}
	inline float GetAnchorY()
	{
		return mAnchorY;
	}
	inline float GetWidth()
	{
		return mWidth;
	}
	inline float GetHeight()
	{
		return mHeight;
	}

	inline void SetOwnerObject(CUnit* tpObject)
	{
		mpObject = tpObject;
	}

	inline CUnit* GetOwnerObject()
	{
		return mpObject;
	}

private:
	bool DoCollisionAABB(CCollider* tOther);

private:
	SVector2D mPosition;

	//[0,1]�� ����ȭ�� ���ؿ����� ��ġ
	float mAnchorX = 0.0f;
	float mAnchorY = 0.0f;

	float mWidth = 0.0f;
	float mHeight = 0.0f;

	//�� �浹ü�� ������ ��ü ����
	CUnit* mpObject = nullptr;

	//�浹ü ��� ����: �� �浹ü�� �浹�� �浹ü���� ����صα� ���� �ڷᱸ��
	//	<-- �浹���¸� �����ϱ� ���� �뵵�� �������.
	//	---> �浹����: �浹����, �浹 ��, �浹 ����
	list<CCollider*> mCollisions;
};

