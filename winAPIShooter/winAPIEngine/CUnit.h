#pragma once

#include "CObjectRyu.h"

//Ŭ���� ���漱��
class CAPIEngine;
class CTexture;

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
	void Update(float tDeltaTime);
	void Render(CAPIEngine* tpEngine);

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

//public:
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

public:
	//���� ���� �Լ�
	//clone�̶�� ���·� ���¸� �����Ͽ� �����ϴ� ���̴�
	virtual CObjectRyu* clone() = 0;
};

