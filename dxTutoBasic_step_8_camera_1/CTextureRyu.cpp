#include "CTextureRyu.h"

#include "DDSTextureLoader.h"

void CTextureRyu::Create(ID3D11Device* tpDevice, LPCWSTR tFileName)
{
	CreateDDSTextureFromFile(tpDevice, tFileName, nullptr, &mpTexture);
}

void CTextureRyu::Destroy()
{
	if (mpTexture)
	{
		mpTexture->Release();
	}
}