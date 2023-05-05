#pragma once

#include "framework.h"

class CTextureRyu
{
public:
	CTextureRyu()
	{
		mpTexture = nullptr;
	}
	CTextureRyu(const CTextureRyu& t) = delete;
	void operator=(const CTextureRyu& t) = delete;

	void Create(ID3D11Device* tpDevice, LPCWSTR tFileName);
	void Destroy();

	ID3D11ShaderResourceView* GetTexture() const
	{
		return mpTexture;
	}
private:
	ID3D11ShaderResourceView* mpTexture = nullptr;
};

