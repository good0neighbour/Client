#pragma once

#include "CObjectRyu.h"

#include <string>
using namespace std;

//Ŭ���� ���漱��
class CAPIEngine;
class CTexture;

class CCollider;

class CAnimator;

class CUnit: public CObjectRyu
{
public:
	CUnit();
	virtual ~CUnit();
	//���������
	CUnit(const CUnit& t);
	//������Կ�����
	void operator=(const CUnit& t);		//���� ������ ������� �ʰڴ�.

public:
	virtual void Create(CAPIEngine* tpEngine);
	virtual void Destroy();
	virtual void Update(float tDeltaTime);
	virtual void Render();

	virtual void LateUpdate() {};

	virtual void OnTimer() {};

	//�浹 ó�� ���α׷� ������ �������� �̿��Ͽ� �ۼ���
	virtual void OnEnterCollision(CCollider* tOther) {};
	virtual void OnStayCollision(CCollider* tOther) {};
	virtual void OnExitCollision(CCollider* tOther) {};

	inline void SetPosition(SVector2D tPosition)
	{
		mPosition = tPosition;
	}
	inline SVector2D GetPosition()
	{
		return mPosition;
	}

	inline void SetPosition(float tX, float tY)
	{
		//mX = tX;
		//mY = tY;
		SVector2D tVector(tX, tY);
		mPosition = tVector;
	}

	inline void SetRadius(float tRadius)
	{
		mRadius = tRadius;
	}

	inline const float GetRadius() const
	{
		return mRadius;
	}

	inline void SetAnchorPoint(float tAnchorX, float tAnchorY)
	{
		mAnchorX = tAnchorX;
		mAnchorY = tAnchorY;
	}
	inline void SetTexture(CTexture* tpTexture)
	{
		mpTexture = tpTexture;
	}

	inline void SetVelocity(SVector2D tVelocity)
	{
		mVelocity = tVelocity;
	}

	inline void SetIsActive(bool tIsActive)
	{
		mIsActive = tIsActive;
	}

	inline bool GetIsActive()
	{
		return mIsActive;
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

	inline void SetTag(string tTag)
	{
		mTag = tTag;
	}
	inline string GetTag() const
	{
		return mTag;
	}

private:
	//2����, �������� ������ �����Ͽ� �����Ϳ� Ÿ���� �������ߴ�.
	SVector2D mVelocity;

	float mRadius = 0.0f;


	//pivot 
	// �ǹ� ������ �޼��ϱ� ���� �ʿ��� �����͵��� ��������� �������.
	//���� ��� ��ġ 
	float mDisplayX = 0.0f;
	float mDisplayY = 0.0f;
	//[0,1]�� ����ȭ�� ���ؿ����� ��ġ
	float mAnchorX = 0.0f;
	float mAnchorY = 0.0f;
	//��Ʈ���� �ʺ�, ����
	float mWidth = 0.0f;
	float mHeight = 0.0f;
	

	//�ܰ��� ��Ÿ�� ��Ʈ�� ������
	CTexture* mpTexture = nullptr;

	//Ȱ��, ��Ȱ�� ���� ����
	bool mIsActive = false;

	//���� ���� ����
	CAPIEngine* mpEngine = nullptr;



public:	//�ϴ� ���� �ۼ��� ���� ����ȭ ����
	//���� ���ø� ���� �浹ü�� �ϳ��� ��������.
	//vector<CCollider*> mColliders;	//���� �������� �Ҽ��� �ִ� 
	CCollider* mpCollider = nullptr;


	//��ü�� ������ �� �ְ� �±� ������ ����ڴ�. 
	//�±״� ���ڿ� Ÿ������ �ϰڴ�. 
	string mTag = "";

	CAnimator* mpAnimator = nullptr;

public:
	//���� ���� �Լ�
	//clone�̶�� ���·� ���¸� �����Ͽ� �����ϴ� ���̴�
	virtual CObjectRyu* clone() = 0;

	CAnimator* CreateAnimation(const string& tId, CAPIEngine* tpEngine);
	void DestroyAnimation();

	void SetDefaultAniSeq(const string& tStrDefaultAniSeq);
	void PlayAni(const string& tStrAniSeq);

};

