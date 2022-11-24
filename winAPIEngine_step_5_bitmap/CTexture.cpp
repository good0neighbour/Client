#include "CTexture.h"

CTexture::CTexture()
	:mthDCMem(nullptr), mthBitmap(nullptr), mthOldBitmap(nullptr)
{
	memset(&mBitmapInfo, 0, sizeof(BITMAP));
}
CTexture::~CTexture()
{
    UnloadTexture();
}

bool CTexture::LoadTexture(HINSTANCE hInst, HDC hDC, LPCWSTR tFileName)
{
    mthDCMem = CreateCompatibleDC(hDC);  //<--Omemory dc를 만든다.

    mthBitmap = (HBITMAP)LoadImage(hInst, tFileName,
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE);   //<--LR~ Load Resource

    mthOldBitmap = (HBITMAP)SelectObject(mthDCMem, mthBitmap);

    //비트맵 정보를 얻어오자
    mBitmapInfo;
    GetObject(mthBitmap, sizeof(mBitmapInfo), &mBitmapInfo);

	return true;
}
void CTexture::UnloadTexture()
{
    SelectObject(mthDCMem, mthOldBitmap); //이전에 사용하던 비트맵으로 설정을 되돌린다
    DeleteObject(mthBitmap);

    DeleteDC(mthDCMem);  //memoryDC를 삭제한다
}