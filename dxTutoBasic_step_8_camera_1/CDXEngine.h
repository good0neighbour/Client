#pragma once


#include "framework.h"
#include "CD3D.h"

#define MAX_LOADSTRING 100


class CDXEngine
{
public:
	CDXEngine()
	{
		OutputDebugStringW(L"CAPIEngine::CAPIEngine\n");
	};
	virtual ~CDXEngine()
	{
		OutputDebugStringW(L"CAPIEngine::~CAPIEngine\n");
	};	


	CDXEngine(const CDXEngine& t) = delete;	
	void operator=(const CDXEngine& t) = delete;	


public:
	void Create(HINSTANCE hInstance, int nCmdShow);
	MSG Run();


	virtual void OnCreate()
	{
		OutputDebugStringW(L"CAPIEngine::OnCreate\n");
	};
	virtual void OnDestroy()
	{
		OutputDebugStringW(L"CAPIEngine::OnDestroy\n");
	};

	virtual void OnUpdate(float tDeltaTime);
	


	void Clear(float tR = 0.0f, float tG = 0.0f, float tB = 0.0f);
	void Clear(XMVECTORF32 tColor);//directx math vector 32bit
	void Present();



	HINSTANCE GetHInst() const
	{
		return hInst;
	}
	HDC GetHDC() const
	{
		return mhDC;
	}
	float GetDeltaTime() const
	{
		return mDeltaTime;
	}

	//��¥����
	//ȭ���� �ʺ�
	float GetWindowWidth()
	{
		return 800.0f;
	}
	//ȭ���� ����
	float GetWindowHeight()
	{
		return 600.0f;
	}




protected:
	ATOM                MyRegisterClass(HINSTANCE hInstance);
	BOOL                InitInstance(HINSTANCE, int);

	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	//static INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


protected:
	static HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
	WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
	WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.


	HDC mhDC;	//���� ȭ�� DC �ڵ� 
	HWND mhWnd;	//������ �ڵ�
	


	LARGE_INTEGER mSecond;
	LARGE_INTEGER mTime;

	float mDeltaTime;		//������ �ð�: �� �����ӿ� �ɸ��� �ð�



	//CD3DŸ���� ��ü���� directX�� �ʱ�ȭ�� ���� ����� �����Ѵ�
public:
	CD3D* mpD3D = nullptr;

	ID3D11Device* GetD3DDevice() const
	{
		return mpD3D->GetD3DDevice();
	}

	ID3D11DeviceContext* GetImmediateDevice() const
	{
		return mpD3D->GetImmediateDevice();
	}




	//dx11

	//window api �ü���� �ϵ���� �ڿ��� �����ϰ� �������α׷����Դ� �������� ����Ǹ� �����Ѵ�.
	//DirectX: �׷��� ī�忡�� ���������� �����Ͽ� �����ϵ��� ������� ms���� ���� ���̺귯��( Ŭ������ �Լ��� ���� ). �������� �߻�ȭ �����̴�.
	//					�ü������ ���� ��� ���� ���������.
	//		�׷��� ó�� ��ġ( GPU + VRAM + Driver���α׷� )

//public:
//	//����̹�: �ϵ��� ������ ����Ʈ����
//	D3D_DRIVER_TYPE         mDriverType = D3D_DRIVER_TYPE_NULL;
//	D3D_FEATURE_LEVEL       mFeatureLevel = D3D_FEATURE_LEVEL_11_0;
//	//������ ���̷�Ʈ���� ���� ���ؿ� �´� �������<-- feature level
//
//
//	//process: ���α׷��� �������� ��, ������ �帧�� ����� <-- n���� thread�� �����ȴ�
//	//������ thread: ���� �帧�� �ּҴ���
//	//			������� ���μ����� �޸𸮸� �����Ѵ�. ��, ������ ���� ������
//
//	//Device: �׷��� ó�� ��ġ( GPU + VRAM + Driver���α׷� ) �� �߻�ȭ �س��� ��ü, single thread���� ó����ü
//	ID3D11Device* mpd3dDevice = nullptr; //Interface direct 3d 11 device
//	ID3D11Device1* mpd3dDevice1 = nullptr;
//
//	//DeviceContext: �׷��� ó�� ��ġ( GPU + VRAM + Driver���α׷� ) �� �߻�ȭ �س��� ��ü, multi thread���� ó����ü
//	//		( �츮�� �ۼ��ϴ� ���õ鿡���� �̱۽����带 �����ϰ� �ۼ��Ѵ� ) 
//	ID3D11DeviceContext* mpImmediateContext = nullptr;
//	ID3D11DeviceContext1* mpImmediateContext1 = nullptr;
//
//	//����ü����
//	//������۸� ������ �����ϰ� �� �� ���۰� ���� ��ȯ�Ǵ� ������ �߻�ȭ�� ��ü�̴�.
//	//window api�� �������� �ۼ��غ� ���� ���� ������ ���� �ΰ��� �����ϰ�
//	// ( back buffer, front buffer ) back buffer���� front buffer�� �̹��� �����͸� �����ϴ� ���·� �۵��߾��ٸ�
//	//���� DirectX���� �����ϴ� ������۴� ���� ��ȭ�� �������ν�
//	//�� �� ���۰� ���� ��ȯ�ȴ�
//	//�̷��� ������ ��ġ �罽ó�� ����
//	//SwapChain �̶� ���Ǿ���
//	IDXGISwapChain* mpSwapChain = nullptr;
//	IDXGISwapChain1* mpSwapChain1 = nullptr;
//
//	//�������� ��� �̹��� �����͸� ���� �� �ֵ��� �߻�ȭ�� �����̴�.
//	//���� Ÿ�� '��'�� back buffer�ĸ���۸� �̹��� �����͸� �������� �� �ִ� ���·� �߻�ȭ�س��� ��ü�̴�
//	ID3D11RenderTargetView* mpRenderTargetView = nullptr;
//
//	//depth buffer�� ������ ���� ����
//	//DirectX11���� ����/���ٽǹ��۴� �ؽ�ó ���ҽ�(�̹���������)�� �� ������ ����Ѵ�
//	ID3D11Texture2D* mpDepthStencil = nullptr;
//	// ���̹���: ���� ���� �����ѵδ� �޸�. �յ������� �ӽ� ������ ��� �����̴�.
//	// ���ٽǹ���: ������ ȿ���� ����� ���� �޸�
//	//DirectX������ �� �� ���۸� �ϳ��� ���ҽ��� ��� ����Ѵ�. �� �ȼ��� 24+8 = 32 �� ����Ѵ�
//	ID3D11DepthStencilView* mpDepthStencilView = nullptr;
//
//	//'�ؽ�ó'�� ���̴����� �̹��� �����͸� �߻�ȭ�Ͽ� �ٷ�� ����̴�. �鿡 �����ϱ� ���� �ð����� ���� �����̴�.
//	//'��'�� ���̴����� �̹��� �����͸� �߻�ȭ�Ͽ� �ٷ�� ����̴�. �ؽ�ó �� ������ �翬�� �����ϰ� �ű⿡ ������ ���������ο��� �ٷ�� �����ΰ��� ǥ���ϴ� �����̶�� �����̴�.
//
//public:
//	HRESULT InitDevice();
//	void CleanupDevice();
};

