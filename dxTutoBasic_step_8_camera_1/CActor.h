#pragma once

#include "framework.h"

class CMesh;
class CMtlTexture;
class CDXEngine;

enum COORD_OPTION
{
	World = 0,			//������ǥ�� world coordinate ����
	Self,					//������ǥ�� local coordinate ����
};

class CActor
{
public:


	void Create(CDXEngine* tpEngine, LPCWSTR tFileNameModel, LPCWSTR tpFileNameTexture);
	void Destroy();

	void Update(float tDeltaTime, COORD_OPTION tCoordOption = COORD_OPTION::World);
	void Render(XMMATRIX tMatView, XMMATRIX tMatProjection);



	void SetEngine(CDXEngine* tpEngine)
	{
		mpEngine = tpEngine;
	};


	void SetPosition(float tX, float tY, float tZ)
	{
		mX = tX;
		mY = tY;
		mZ = tZ;
	}

	XMFLOAT3 GetPosition()
	{
		XMFLOAT3 tResult(mX, mY, mZ);

		return tResult;
	}


private:
	// Actor ��� ������ ���� mesh�� material�� ������.
	CMesh* mpMesh = nullptr;				//mesh
	CMtlTexture* mpMtl = nullptr;			//material

	CDXEngine* mpEngine = nullptr;		//engine

private:
	float mX = 0.0f;
	float mY = 0.0f;
	float mZ = 0.0f;

	ID3D11Buffer* mpCBTransform = nullptr;

	//yaw

	//roll:	���溤�͸� ȸ�������� ȸ��
	//yaw:	��溤�͸� ȸ�������� ȸ��
	//pitch:	��溤�͸� ȸ�������� ȸ��

	//ȸ������
	float mPitchX = 0.0f;
	float mYawY = 0.0f;
	float mRollZ = 0.0f;

	//���
	float mScaleX = 1.0f;
	float mScaleY = 1.0f;
	float mScaleZ = 1.0f;

	//���溤��
	XMVECTOR mForwardVector;

	//�̵�, ȸ��, ��� ��ȯ�� ��ȯ���
	XMMATRIX mMatTranslate;
	XMMATRIX mMatRotation;
	XMMATRIX mMatScale;

	//��ǥ�� �ɼ�
	COORD_OPTION mCoordOption = COORD_OPTION::World;

	//�ӵ� ����
	XMVECTOR mVelocity;
	//��ġ ����
	XMVECTOR mPosition;

	XMMATRIX mMatWorld;

public:
	void SetRotation(float tPitchX, float tYawY, float tRollZ)
	{
		mPitchX = tPitchX;
		mYawY = tYawY;
		mRollZ = tRollZ;
	}

	XMFLOAT3 GetRotation()
	{
		XMFLOAT3 tResult(mPitchX, mYawY, mRollZ);

		return tResult;
	}

	void SetVelocity(XMVECTOR tVelocity)
	{
		mVelocity = tVelocity;
	}

	void SetScale(float tScaleX, float tScaleY, float tScaleZ)
	{
		mScaleX = tScaleX;
		mScaleY = tScaleY;
		mScaleZ = tScaleZ;
	}

	XMFLOAT3 GetScale()
	{
		XMFLOAT3 tResult;

		tResult.x = mScaleX;
		tResult.y = mScaleY;
		tResult.z = mScaleZ;

		return tResult;
	}

	//
	void UpdateLook(XMFLOAT4 tAtPoint);

	//�ڽ��� ���庯ȯ����� ��´�
	XMMATRIX GetMatWorld() const
	{
		return mMatWorld;
	}
};