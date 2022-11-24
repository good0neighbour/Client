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
	void UnloadTexture();
};

