#pragma once

#include <Windows.h>

class CTexture
{
public:
	HDC mthDCMem;
	HBITMAP mthBitmap;
	HBITMAP mthOldBitmap;
	BITMAP mBitmapInfo;

public:
	CTexture();
	~CTexture();

	bool LoadTexture(HINSTANCE hInst, HDC hDC, LPCWSTR tFileName);

private:
	void UnloadTexture();

public:
	bool CreateBackBuffer(HINSTANCE hInst, HDC hDC);
	//메모리DC를 하나 만들고, 백 버퍼 라는 개념으로 삼자
};

