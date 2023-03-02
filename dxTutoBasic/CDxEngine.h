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

	
	//��¥�� ����
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
	WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
	WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

	static HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.

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
//	//��ġ�� ��ġ ���ؽ�Ʈ �� �ΰ����� ��� Direct3D�Լ����� �������̽��� �ǹǷ� �ſ� �߿��ϴ� 
//	// �� �ΰ����� �̿��Ͽ� ��κ��� �۾��� �����ϰ� �ȴ�.
//	//Direct3D��ġ
//	ID3D11Device* mpd3dDevice = nullptr;
//	ID3D11Device1* mpd3dDevice1 = nullptr;
//	//Direct3D��ġ ���ؽ�Ʈ 
//	ID3D11DeviceContext* mpImmediateContext = nullptr;      //Direct3D��ġ ���ؽ�Ʈ ����
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

