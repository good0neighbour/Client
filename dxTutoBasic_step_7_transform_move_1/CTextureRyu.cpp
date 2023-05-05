#include "CTextureRyu.h"

#include "DDSTextureLoader.h"


void CTextureRyu::Create(ID3D11Device* tpDevice, LPCWSTR tFileName)
{
	//텍스처 이미지 데이터를 파일에서 로드하여  mpTexture에 담음
	CreateDDSTextureFromFile(tpDevice, tFileName, nullptr, &mpTexture);
}
void CTextureRyu::Destroy()
{
	if (mpTexture)
		mpTexture->Release();
}