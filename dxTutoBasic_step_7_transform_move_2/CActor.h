#pragma once

#include "framework.h"

class CMesh;
class CMtlTexture;
class CDxEngine;

//열거 형
enum COORD_OPTION
{
	World = 0,				//월드 좌표계 world coordinate 기준
	Self						//로컬 좌표계 local coordinate 기준
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
	//actor라는 개념을 위해 mesh, material개념을 묶었다.
	CMesh* mpMesh = nullptr;
	CMtlTexture* mpMtlTexture = nullptr;

	CDxEngine* mpEngine = nullptr;

private:
	//위치 정보
	float mX = 0.0f;
	float mY = 0.0f;
	float mZ = 0.0f;

	ID3D11Buffer* mpCBTransform = nullptr;


private:
	
	//회전 
	//회전각도 의 개념으로 다음 변수들을 만들었다. 
	// pitch: 우방 축을 회전축으로 하는 회전
	// yaw: 상방 축을 회전축으로 하는 회전
	// roll: 전방 축을 회전축으로 하는 회전
	float mPitchX = 0.0f;
	float mYawY = 0.0f;
	float mRollZ = 0.0f;


	//비례
	float mScaleX = 1.0f;
	float mScaleY = 1.0f;
	float mScaleZ = 1.0f;

	//전방벡터 
	XMVECTOR mForwardVector;


	//좌표계 옵션 
	COORD_OPTION mCoordOption = COORD_OPTION::World;		//기본은 월드좌표계 기준

	//속도
	XMVECTOR mVelocity;

	//위치 <--벡터 형식 
	XMVECTOR mPosition;

	//이동, 회전, 비례 에 관한 변환행렬 
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

