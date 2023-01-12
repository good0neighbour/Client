#pragma once

#include <windows.h>

#include "CRef.h"

class CTexture:public CRef
{
private:	
	HBITMAP mhBitmap;
	HBITMAP mhOldBitmap;

	//���� �� ������ ������ �ǵ��ߴ�.
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
	//�޸�DC�� �ϳ� �����, �� ���� ��� �������� ����
};

