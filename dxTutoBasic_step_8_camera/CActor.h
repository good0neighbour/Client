#pragma once

#include "framework.h"

class CMesh;
class CMtlTexture;
class CDxEngine;

//���� ��
enum COORD_OPTION
{
	World = 0,				//���� ��ǥ�� world coordinate ����
	Self						//���� ��ǥ�� local coordinate ����
};



class CActor
{
public:
	void Create(CDxEngine* tpEngine, LPCWSTR tFileNameModel, LPCWSTR tFileNameTexture);
	void Destroy();

	void Update(float tDeltaTime, COORD_OPTION tCoordOption = COORD_OPTION::World);
	void Render(XMMATRIX tMatView, XMMATRIX tMatPerspective);

	inline void SetEngine(CDxEngine* tpEngine)
	{
		mpEngine = tpEngine;
	}
	inline void SetPosition(float tX, float tY, float tZ)
	{
		mX = tX;
		mY = tY;
		mZ = tZ;
	}
	inline XMFLOAT3 GetPosition()
	{
		XMFLOAT3 tResult(mX, mY, mZ);

		return tResult;
	}


private:
	//actor��� ������ ���� mesh, material������ ������.
	CMesh* mpMesh = nullptr;
	CMtlTexture* mpMtlTexture = nullptr;

	CDxEngine* mpEngine = nullptr;

private:
	//��ġ ����
	float mX = 0.0f;
	float mY = 0.0f;
	float mZ = 0.0f;

	ID3D11Buffer* mpCBTransform = nullptr;


private:
	
	//ȸ�� 
	//ȸ������ �� �������� ���� �������� �������. 
	// pitch: ��� ���� ȸ�������� �ϴ� ȸ��
	// yaw: ��� ���� ȸ�������� �ϴ� ȸ��
	// roll: ���� ���� ȸ�������� �ϴ� ȸ��
	float mPitchX = 0.0f;
	float mYawY = 0.0f;
	float mRollZ = 0.0f;


	//���
	float mScaleX = 1.0f;
	float mScaleY = 1.0f;
	float mScaleZ = 1.0f;

	//���溤�� 
	XMVECTOR mForwardVector;


	//��ǥ�� �ɼ� 
	COORD_OPTION mCoordOption = COORD_OPTION::World;		//�⺻�� ������ǥ�� ����

	//�ӵ�
	XMVECTOR mVelocity;

	//��ġ <--���� ���� 
	XMVECTOR mPosition;

	//�̵�, ȸ��, ��� �� ���� ��ȯ��� 
	XMMATRIX mMatTranslation;
	XMMATRIX mMatRotation;
	XMMATRIX mMatScale;

public:
	inline void SetRotation(float tPitchX, float tYawY, float tRollZ)
	{
		mPitchX = tPitchX;
		mYawY = tYawY;
		mRollZ = tRollZ;
	}

	inline XMFLOAT3 GetRotation()
	{
		XMFLOAT3 tResult(mPitchX, mYawY, mRollZ);

		return tResult;
	}

	inline void SetVelocity(XMVECTOR tVelocity)
	{
		mVelocity = tVelocity;
	}




	inline void SetScale(float tScaleX, float tScaleY, float tScaleZ)
	{
		mScaleX = tScaleX;
		mScaleY = tScaleY;
		mScaleZ = tScaleZ;
	}

	inline XMFLOAT3 GetScale()
	{
		XMFLOAT3 tResult(mScaleX, mScaleY, mScaleZ);

		return tResult;
	}

};

