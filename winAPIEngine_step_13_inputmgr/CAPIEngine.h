#pragma once

#include "framework.h"
#define MAX_LOADSTRING 100

class CTexture;

class CAPIEngine
{
public:
	CAPIEngine();
	virtual ~CAPIEngine();

	//다음 복사함수들의 구현을 막는다
private:
	CAPIEngine(const CAPIEngine& t) = delete;
	CAPIEngine& operator=(const CAPIEngine& t) = delete;

public:
	BOOL Create(HINSTANCE hInstance, int nCmdShow);
	MSG Run();

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate();

	//원그리기 함수
	void DrawCircle(float tX, float tY, float tRadius);
	//비트맵 이미지 그리기 함수
	void DrawTexture(float tX, float tY, CTexture* tpTexture);

	//정규화: 크기를 1로 만들어 다루는 개념
	// [0,1] <--폐구간 0에서 1사이의 수치(0과 1 포함)

	//화면 클리어 함수
	void Clear(float tR = 0.0f, float tG = 0.0f, float tB= 0.0f);
	//프레임 화면 제출 함수
	void Present();

protected:
	ATOM MyRegisterClass(HINSTANCE hInstance);

	BOOL                InitInstance(HINSTANCE, int);
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

protected:
	static HINSTANCE hInst;                                // 현재 인스턴스입니다.
	WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
	WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

	HDC mhDC;			//현재 화면 DC(의 핸들)
	HWND mhWnd;		//윈도우 핸들

	CTexture* mpBackBuffer = nullptr;
};

