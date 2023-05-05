#pragma once

#include "framework.h"

class CMesh;
class CMtlTexture;
class CDXEngine;

enum COORD_OPTION
{
	World = 0,			//월드좌표계 world coordinate 기준
	Self,					//로컬좌표계 local coordinate 기준
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
	// Actor 라는 개념을 위해 mesh와 material을 묶었다.
	CMesh* mpMesh = nullptr;				//mesh
	CMtlTexture* mpMtl = nullptr;			//material

	CDXEngine* mpEngine = nullptr;		//engine

private:
	float mX = 0.0f;
	float mY = 0.0f;
	float mZ = 0.0f;

	ID3D11Buffer* mpCBTransform = nullptr;

	//yaw

	//roll:	전방벡터를 회전축으로 회전
	//yaw:	상방벡터를 회전축으로 회전
	//pitch:	우방벡터를 회전축으로 회전

	//회전각도
	float mPitchX = 0.0f;
	float mYawY = 0.0f;
	float mRollZ = 0.0f;

	//비례
	float mScaleX = 1.0f;
	float mScaleY = 1.0f;
	float mScaleZ = 1.0f;

	//전방벡터
	XMVECTOR mForwardVector;

	//이동, 회전, 비례 변환용 변환행렬
	XMMATRIX mMatTranslate;
	XMMATRIX mMatRotation;
	XMMATRIX mMatScale;

	//좌표계 옵션
	COORD_OPTION mCoordOption = COORD_OPTION::World;

	//속도 벡터
	XMVECTOR mVelocity;
	//위치 벡터
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

	//자신의 월드변환행렬을 얻는다
	XMMATRIX GetMatWorld() const
	{
		return mMatWorld;
	}
};