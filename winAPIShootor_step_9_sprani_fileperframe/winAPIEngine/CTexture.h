#pragma once

#include <windows.h>

class CTexture
{
private:	
	HBITMAP mhBitmap;
	HBITMAP mhOldBitmap;

	//다음 두 변수는 노출을 의도했다.
public:
	HDC mhDCMem;
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

