#pragma once

#include "framework.h"

class CD3D
{
public:	
	D3D_DRIVER_TYPE         mDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       mFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	

	ID3D11Device* mpd3dDevice = nullptr;
	ID3D11Device1* mpd3dDevice1 = nullptr;

	ID3D11DeviceContext* mpImmediateContext = nullptr;
	ID3D11DeviceContext1* mpImmediateContext1 = nullptr;


	IDXGISwapChain* mpSwapChain = nullptr;
	IDXGISwapChain1* mpSwapChain1 = nullptr;

	ID3D11RenderTargetView* mpRenderTargetView = nullptr;
	ID3D11Texture2D* mpDepthStencil = nullptr;
	ID3D11DepthStencilView* mpDepthStencilView = nullptr;


	HWND mhWnd;		//윈도우 핸들

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

