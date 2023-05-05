#pragma once


#include "framework.h"


class CActor;

class CCamera
{
public:
	void UpdateMatView();
	void GetViewMatrix(XMMATRIX& tMatView);


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

	void SetParent(CActor* tpParent);
	CActor* GetParent();

	void SetAtPoint(float tAtX, float tAtY, float tAtZ)
	{
		mAtX = tAtX;
		mAtY = tAtY;
		mAtZ = tAtZ;
	}


private:
	float mX = 0.0f;
	float mY = 0.0f;
	float mZ = 0.0f;

	float mAtX = 0.0f;
	float mAtY = 0.0f;
	float mAtZ = 0.0f;

	//degree
	float mPitchX = 0.0f;
	float mYawY = 0.0f;
	float mRollZ = 0.0f;

	//뷰변환행렬을 최종적으로 산출해낸다.
	XMMATRIX mMatView;

	//camera_ryu
	CActor* mpParent = nullptr;
};

