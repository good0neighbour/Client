#pragma once

#include "framework.h"

class CD3D
{
private:
	//����̹�: �ϵ��� ������ ����Ʈ����
	D3D_DRIVER_TYPE         mDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       mFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	//������ ���̷�Ʈ���� ���� ���ؿ� �´� �������<-- feature level


	//process: ���α׷��� �������� ��, ������ �帧�� ����� <-- n���� thread�� �����ȴ�
	//������ thread: ���� �帧�� �ּҴ���
	//			������� ���μ����� �޸𸮸� �����Ѵ�. ��, ������ ���� ������

	//Device: �׷��� ó�� ��ġ( GPU + VRAM + Driver���α׷� ) �� �߻�ȭ �س��� ��ü, single thread���� ó����ü
	ID3D11Device* mpd3dDevice = nullptr; //Interface direct 3d 11 device
	ID3D11Device1* mpd3dDevice1 = nullptr;

	//DeviceContext: �׷��� ó�� ��ġ( GPU + VRAM + Driver���α׷� ) �� �߻�ȭ �س��� ��ü, multi thread���� ó����ü
	//		( �츮�� �ۼ��ϴ� ���õ鿡���� �̱۽����带 �����ϰ� �ۼ��Ѵ� ) 
	ID3D11DeviceContext* mpImmediateContext = nullptr;
	ID3D11DeviceContext1* mpImmediateContext1 = nullptr;

	//����ü����
	//������۸� ������ �����ϰ� �� �� ���۰� ���� ��ȯ�Ǵ� ������ �߻�ȭ�� ��ü�̴�.
	//window api�� �������� �ۼ��غ� ���� ���� ������ ���� �ΰ��� �����ϰ�
	// ( back buffer, front buffer ) back buffer���� front buffer�� �̹��� �����͸� �����ϴ� ���·� �۵��߾��ٸ�
	//���� DirectX���� �����ϴ� ������۴� ���� ��ȭ�� �������ν�
	//�� �� ���۰� ���� ��ȯ�ȴ�
	//�̷��� ������ ��ġ �罽ó�� ����
	//SwapChain �̶� ���Ǿ���
	IDXGISwapChain* mpSwapChain = nullptr;
	IDXGISwapChain1* mpSwapChain1 = nullptr;

	//�������� ��� �̹��� �����͸� ���� �� �ֵ��� �߻�ȭ�� �����̴�.
	//���� Ÿ�� '��'�� back buffer�ĸ���۸� �̹��� �����͸� �������� �� �ִ� ���·� �߻�ȭ�س��� ��ü�̴�
	ID3D11RenderTargetView* mpRenderTargetView = nullptr;

	//depth buffer�� ������ ���� ����
	//DirectX11���� ����/���ٽǹ��۴� �ؽ�ó ���ҽ�(�̹���������)�� �� ������ ����Ѵ�
	ID3D11Texture2D* mpDepthStencil = nullptr;
	// ���̹���: ���� ���� �����ѵδ� �޸�. �յ������� �ӽ� ������ ��� �����̴�.
	// ���ٽǹ���: ������ ȿ���� ����� ���� �޸�
	//DirectX������ �� �� ���۸� �ϳ��� ���ҽ��� ��� ����Ѵ�. �� �ȼ��� 24+8 = 32 �� ����Ѵ�
	ID3D11DepthStencilView* mpDepthStencilView = nullptr;

	//'�ؽ�ó'�� ���̴����� �̹��� �����͸� �߻�ȭ�Ͽ� �ٷ�� ����̴�. �鿡 �����ϱ� ���� �ð����� ���� �����̴�.
	//'��'�� ���̴����� �̹��� �����͸� �߻�ȭ�Ͽ� �ٷ�� ����̴�. �ؽ�ó �� ������ �翬�� �����ϰ� �ű⿡ ������ ���������ο��� �ٷ�� �����ΰ��� ǥ���ϴ� �����̶�� �����̴�.

	//Depth Buffer ����

	//depth buffer�� ���� ������ ���� �� ����
	ID3D11DepthStencilState* mpDepthEnableState = nullptr;			//depth buffer Ȱ��ȭ ����
	ID3D11DepthStencilState* mpDepthDisableState = nullptr;			//depth buffer ��Ȱ��ȭ ����




	HWND mhWnd;

public:
	HRESULT Create(HWND thWnd);
	void Destroy();

	void Clear(float tR = 0.0f, float tG = 0.0f, float tB = 0.0f);
	void Clear(XMVECTORF32 tColor);//directx math vector 32bit
	void Present();


	ID3D11Device* GetD3DDevice() const
	{
		return mpd3dDevice;
	}

	ID3D11DeviceContext* GetImmediateDevice() const
	{
		return mpImmediateContext;
	}


	void DoTurnOnDepthBuffer();
	void DoTurnOffDepthBuffer();

};

