#pragma once

#include "framework.h"

#define MAX_LOADSTRING 100

class CAPIEngine
{
public:
	CAPIEngine();
	virtual ~CAPIEngine();

	//다음 복사함수들의 구현을 막는다
private:
	CAPIEngine(const CAPIEngine& t) = delete;
	CAPIEngine& operator = (const CAPIEngine& t) = delete;

public:
	BOOL Create(HINSTANCE hInstance, int nCmdShow);
	MSG Run();

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate();

protected:
	ATOM MyRegisterClass(HINSTANCE hInstance);

	BOOL                InitInstance(HINSTANCE, int);
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


protected:
	static HINSTANCE hInst;                                // 현재 인스턴스입니다.
	WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
	WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
};

