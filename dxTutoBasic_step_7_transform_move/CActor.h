#pragma once

#include "framework.h"

class CMesh;
class CMtlTexture;
class CDxEngine;

class CActor
{
public:
	void Create(CDxEngine* tpEngine);
	void Destroy();

	void Update();
	void Render();

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
};

