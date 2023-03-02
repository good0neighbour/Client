#pragma once

#include "framework.h"

#define MAX_LOADSTRING 100

class CDxEngine
{
public:
	CDxEngine();
	virtual ~CDxEngine();

	//���� �����Լ����� ������ ���´�
private:
	CDxEngine(const CDxEngine& t) = delete;
	CDxEngine& operator=(const CDxEngine& t) = delete;

public:
	BOOL Create(HINSTANCE hInstance, int nCmdShow);
	MSG Run();


	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate(float tDeltaTime);

	//ȭ�� Ŭ���� �Լ�
	void Clear(float tR = 0.0f, float tG = 0.0f, float tB= 0.0f);
	//������ ȭ�� ���� �Լ�
	void Present();

	inline int GetScreenWidth()
	{
		RECT tRect = { 0 };
		GetClientRect(mhWnd, &tRect);

		int tW = tRect.right - tRect.left;

		return tW;
	}

	inline int GetScreenHeight()
	{
		RECT tRect = { 0 };
		GetClientRect(mhWnd, &tRect);

		int tH = tRect.bottom - tRect.top;

		return tH;
	}

protected:
	ATOM MyRegisterClass(HINSTANCE hInstance);

	BOOL                InitInstance(HINSTANCE, int);
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


protected:
	HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
	
	WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
	WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.


	HDC mhDC;			//���� ȭ�� DC(�� �ڵ�)
	HWND mhWnd;		//������ �ڵ�

private:
	LARGE_INTEGER mTickPerSecond;	//�ʴ� ƽ��
	LARGE_INTEGER mTime;				//������ ������ ƽ��

	float mDeltaTime;		//�� �����ӿ� �ɸ� �ð�

public:
	HINSTANCE GetHInstance()
	{
		return hInst;
	}
	HDC GetHDC()
	{
		return mhDC;
	}

	float GetDeltaTime()
	{
		return mDeltaTime;
	}

	//window sdk <--- sdk Software Development Kit
	//window api: �ü��(system software)�� �ϵ���� �ڿ��� �����ϰ� �������α׷����Դ� �������� �����(handle)�� �����Ѵ�.
	//
	// GDI: Graphic Device Interface: window api���� �׷��� �ý����� �����ϴ� �߻�ȭ�� ����
	//
	//directX sdk : �׷��� ó�� ��ġ�� ���������� �����Ͽ� �����ϵ��� ������� MS�翡�� ���� ���̺귯��, �������� �߻�ȭ����.
	//	window �ü������ �����ϴ� (���� ��� GDI )�׷��� ó�� �ý��ۺ��� �ſ� ���� ��� ���� ���������.
	//		�׷��� ó�� ��ġ( GPU + VRAM + DRIVER���α׷�) <-- ��)�׷��� ī��

	//		Driver: �ϵ��� ������ ���α׷�. �ϵ��� low���ؿ��� ���� �����Ѵ�.
	//			�ü���� �ϵ���� ���̿��� �����Ѵ�
	//			DirectX�� �ϵ���� ���̿��� �����Ѵ�
public:
	//����̹� ����
	D3D_DRIVER_TYPE         mDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       mFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	//������ ���̷�Ʈ���� ���� ���ؿ� �´� ������� <-- feature level

	// process:���α׷��� �������� ��. ������ �帧 <--n���� ������thread�� ��������ִ�.
	// thread: �����帧�� �ּҴ���.
	//		������� ���μ����� �޸𸮸� �����Ѵ�. ��, ������ ���� ������.

	/*
	* device:
	*	single thread �� ó�� ��ü
	* 
	*	single thread���� ó���ؾ��ϴ� �͵��� �ٷ�� ��ü�̴�
	* <-- �ַ� �ڿ�resources�� �ٷ��
	*	buffer, texture, shader, ...
	* 
	* device context:
	*	multi thread �� ó�� ��ü
	* 
	*		render state, draw primitive <--�ַ� ��ɿ� ���� �͵��� �ٷ��.
	*/

	//Device ��ġ(�׷��� ó�� ��ġ)
	ID3D11Device* mpd3dDevice = nullptr;
	ID3D11Device1* mpd3dDevice1 = nullptr;

	//Device Context ��ġ(�׷��� ó�� ��ġ)
	ID3D11DeviceContext* mpImmediateContext = nullptr;
	ID3D11DeviceContext1* mpImmediateContext1 = nullptr;

	//���� ü��
	//Double buffer ������ �����ϰ�, �� �� ���۰� ���� ��ȯ�Ǵ� ������ �߻�ȭ�� ��ü�̴�.
	//window api�� �н��� ����� ���Ͽ� ���ڸ�, �������� �ۼ��ߴ� ���� ���� ������ ���� �� ���� �����ϰ�
	//(back buffer, front buffer) back buffer���� front buffer�� �̹��� �����͸� �����ϴ� �����̾��ٸ�,
	//
	//DirectX���� �����ϴ� ������۴� ���� ��ȭ�� ��������
	//�� �� ���۰� ���� ��ȯ�� �ݺ��Ѵ�.
	//�̷��� ������ ��ġ �罽ó�� ������
	//SwapChain�̶�� �Ѵ�.
	IDXGISwapChain* mpSwapChain = nullptr;
	IDXGISwapChain1* mpSwapChain1 = nullptr;

	//�������� ��� �̹��� �����͸� ���� �� �ִ� �߻�ȭ�� �����̴�
	//����ü���� ����۸� ������ �����.
	//����ü���� �����backbuffer�� D3D�������� ���� �߻�ȭ�� �����̴�.
	ID3D11RenderTargetView* mpRenderTargetView = nullptr;
	//View <-- �Ϲ������� ��� ������ �����͸� �鿩�ٺ� �� �ִ� �߻�ȭ�� �����̴�.

	public:
		HRESULT InitDevice();
		void CleanupDevice();
};

