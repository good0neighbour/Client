#include "CTexture.h"


CTexture::CTexture()
	:mhDCMem(nullptr), mhBitmap(nullptr), mhOldBitmap(nullptr) 
{
	memset(&mBitmapInfo, 0, sizeof(BITMAP));
}
CTexture::~CTexture()
{
    UnloadTexture();
}

bool CTexture::LoadTexture(HINSTANCE hInst, HDC hDC, LPCWSTR tFileName)
{
    mhDCMem = CreateCompatibleDC(hDC);     //<--memory dc를 만든다.

    mhBitmap = (HBITMAP)LoadImage(hInst, tFileName,
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE);       //<-- LR~ Load Resource


    mhOldBitmap = (HBITMAP)SelectObject(mhDCMem, mhBitmap);

    //비트맵 정보를 얻어오자
    mBitmapInfo;
    GetObject(mhBitmap, sizeof(mBitmapInfo), &mBitmapInfo);


	return true;
}
void CTexture::UnloadTexture()
{
    SelectObject(mhDCMem, mhOldBitmap);     //이전에 사용하던 비트맵으로 설정을 되돌린다.
    DeleteObject(mhBitmap);

    DeleteDC(mhDCMem);      //memory DC를 삭제한다

}

bool CTexture::CreateBackBuffer(HINSTANCE hInst, HDC hDC)
{
    mhDCMem = CreateCompatibleDC(hDC);     //<--memory dc를 만든다.

    mhBitmap = (HBITMAP)CreateCompatibleBitmap(hDC, 800, 600);  
    //현재화면DC와 크기가 같은 memoryDC에 설정된 비트맵을 하나 만든다


    mhOldBitmap = (HBITMAP)SelectObject(mhDCMem, mhBitmap);

    //비트맵 정보를 얻어오자
    mBitmapInfo;
    GetObject(mhBitmap, sizeof(mBitmapInfo), &mBitmapInfo);


    return true;
}