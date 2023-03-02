#pragma once

#include "framework.h"

#include "CD3D.h"

//ryu_colorkey
#pragma comment(lib, "msimg32.lib")



#define MAX_LOADSTRING 100


class CDxEngine
{
public:
	CDxEngine();
	virtual ~CDxEngine();

private:
	CDxEngine(const CDxEngine&) {};

	CDxEngine& operator=(const CDxEngine&) {};


public:
	BOOL Create(HINSTANCE hInstance, int nCmdShow);
	MSG Run();


	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate(float tDeltaTime);


	void Clear(float tR = 0.0f, float tG = 0.0f, float tB = 0.0f);
	void Clear(XMVECTORF32 tColor);

	void Present();



	


	
	float GetDeltaTime() const
	{
		return mDeltaTime;
	}


	HINSTANCE GetHInst() const
	{
		return hInst;
	}
	HDC GetHDC() const
	{
		return mhDC;
	}

	
	//가짜로 구현
	int ScreenWidth()
	{
		return 800;
	};
	int ScreenHeight()
	{
		return 600;
	};



protected:
	ATOM                MyRegisterClass(HINSTANCE hInstance);
	BOOL                InitInstance(HINSTANCE, int);

	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

protected:
	WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
	WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

	static HINSTANCE hInst;                                // 현재 인스턴스입니다.

	HWND mhWnd;
	HDC mhDC;

private:
	LARGE_INTEGER mSecond;
	LARGE_INTEGER mTime;

	float mDeltaTime;

public:
	CD3D* mpD3D = nullptr;

//public:
//	//dx11
//	D3D_DRIVER_TYPE         mDriverType = D3D_DRIVER_TYPE_NULL;
//	D3D_FEATURE_LEVEL       mFeatureLevel = D3D_FEATURE_LEVEL_11_0;
//	//장치와 장치 컨텍스트 이 두가지는 모든 Direct3D함수들의 인터페이스가 되므로 매우 중요하다 
//	// 이 두가지를 이용하여 대부분의 작업을 수행하게 된다.
//	//Direct3D장치
//	ID3D11Device* mpd3dDevice = nullptr;
//	ID3D11Device1* mpd3dDevice1 = nullptr;
//	//Direct3D장치 컨텍스트 
//	ID3D11DeviceContext* mpImmediateContext = nullptr;      //Direct3D장치 컨텍스트 참조
//	ID3D11DeviceContext1* mpImmediateContext1 = nullptr;
//
//	IDXGISwapChain* mpSwapChain = nullptr;
//	IDXGISwapChain1* mpSwapChain1 = nullptr;
//	ID3D11RenderTargetView* mpRenderTargetView = nullptr;
//
//
//	//depth
//		ID3D11Texture2D* mpDepthStencil = nullptr;
//		ID3D11DepthStencilView* mpDepthStencilView = nullptr;
//
//
//
//public:
//	//dx11
//	HRESULT InitDevice();
//	void CleanupDevice();


};

