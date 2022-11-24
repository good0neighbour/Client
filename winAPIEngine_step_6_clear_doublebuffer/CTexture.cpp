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
    mthDCMem = CreateCompatibleDC(hDC);  //<--Omemory dc�� �����.

    mthBitmap = (HBITMAP)LoadImage(hInst, tFileName,
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE);   //<--LR~ Load Resource

    mthOldBitmap = (HBITMAP)SelectObject(mthDCMem, mthBitmap);

    //��Ʈ�� ������ ������
    mBitmapInfo;
    GetObject(mthBitmap, sizeof(mBitmapInfo), &mBitmapInfo);

	return true;
}
void CTexture::UnloadTexture()
{
    SelectObject(mthDCMem, mthOldBitmap); //������ ����ϴ� ��Ʈ������ ������ �ǵ�����
    DeleteObject(mthBitmap);

    DeleteDC(mthDCMem);  //memoryDC�� �����Ѵ�
}

bool CTexture::CreateBackBuffer(HINSTANCE hInst, HDC hDC)
{
    mthDCMem = CreateCompatibleDC(hDC);  //<--memory dc�� �����.

    mthBitmap = CreateCompatibleBitmap(hDC, 800, 600);
    //����ȭ��DC�� ũ�Ⱑ ���� memoryDC�� �ϳ� �����

    mthOldBitmap = (HBITMAP)SelectObject(mthDCMem, mthBitmap);

    //��Ʈ�� ������ ������
    mBitmapInfo;
    GetObject(mthBitmap, sizeof(mBitmapInfo), &mBitmapInfo);

    return true;
}