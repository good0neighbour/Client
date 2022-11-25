#pragma once

#include "framework.h"
#define MAX_LOADSTRING 100

class CTexture;

class CAPIEngine
{
public:
	CAPIEngine();
	virtual ~CAPIEngine();

	//���� �����Լ����� ������ ���´�
private:
	CAPIEngine(const CAPIEngine& t) = delete;
	CAPIEngine& operator = (const CAPIEngine& t) = delete;

public:
	BOOL Create(HINSTANCE hInstance, int nCmdShow);
	MSG Run();

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate();

	//���׸��� �Լ�
	void DrawCircle(float tX, float tY, float tRadius);
	//��Ʈ�� �̹��� �׸��� �Լ�
	void DrawTexture(float tX, float tY, CTexture* tpTexture);

	//ȭ�� Ŭ���� �Լ�
	void Clear();
	//������ ȭ�� ���� �Լ�
	void Present();

protected:
	ATOM MyRegisterClass(HINSTANCE hInstance);

	BOOL                InitInstance(HINSTANCE, int);
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


protected:
	static HINSTANCE hInst;                         // ���� �ν��Ͻ��Դϴ�.
	WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
	WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

	HDC mhDC;	//���� ȭ�� DC(�� �ڵ�)
	HWND mhWnd;	//������ �ڵ�

	CTexture* mpBackBuffer = nullptr;
};

