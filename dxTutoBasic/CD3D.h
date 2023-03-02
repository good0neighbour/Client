#pragma once

#include "framework.h"

class CD3D
{
public:
	HWND mhWnd;
	//dx11
	D3D_DRIVER_TYPE         mDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       mFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	//장치와 장치 컨텍스트 이 두가지는 모든 Direct3D함수들의 인터페이스가 되므로 매우 중요하다 
	// 이 두가지를 이용하여 대부분의 작업을 수행하게 된다.
	//Direct3D장치
	ID3D11Device* mpd3dDevice = nullptr;
	ID3D11Device1* mpd3dDevice1 = nullptr;
	//Direct3D장치 컨텍스트 
	ID3D11DeviceContext* mpImmediateContext = nullptr;      //Direct3D장치 컨텍스트 참조
	ID3D11DeviceContext1* mpImmediateContext1 = nullptr;

	IDXGISwapChain* mpSwapChain = nullptr;
	IDXGISwapChain1* mpSwapChain1 = nullptr;
	ID3D11RenderTargetView* mpRenderTargetView = nullptr;


	//depth
	ID3D11Texture2D* mpDepthStencil = nullptr;
	ID3D11DepthStencilView* mpDepthStencilView = nullptr;

public:
	HRESULT Create(HWND thWnd);
	void Destroy();

	void Clear(float tR = 0.0f, float tG = 0.0f, float tB = 0.0f);
	void Clear(XMVECTORF32 tColor);
	void Present();

	inline ID3D11Device* GetD3DDevice() const
	{
		return mpd3dDevice;
	}

	inline ID3D11DeviceContext* GetImmediateDevice() const
	{
		return mpImmediateContext;
	}
};

