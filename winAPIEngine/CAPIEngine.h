#pragma once

#include "framework.h"

#define MAX_LOADSTRING 100

class CAPIEngine
{
public:
	BOOL Create(HINSTANCE hInstance, int nCmdShow);
	MSG Run();

protected:
	ATOM MyRegisterClass(HINSTANCE hInstance);

	BOOL                InitInstance(HINSTANCE, int);
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


protected:
	static HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
	WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
	WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
};

