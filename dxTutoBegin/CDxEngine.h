#pragma once

#include "framework.h"

#define MAX_LOADSTRING 100

class CDxEngine
{
public:
	CDxEngine();
	virtual ~CDxEngine();

	//다음 복사함수들의 구현을 막는다
private:
	CDxEngine(const CDxEngine& t) = delete;
	CDxEngine& operator=(const CDxEngine& t) = delete;

public:
	BOOL Create(HINSTANCE hInstance, int nCmdShow);
	MSG Run();


	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate(float tDeltaTime);

	//화면 클리어 함수
	void Clear(float tR = 0.0f, float tG = 0.0f, float tB= 0.0f);
	//프레임 화면 제출 함수
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
	HINSTANCE hInst;                                // 현재 인스턴스입니다.
	
	WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
	WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


	HDC mhDC;			//현재 화면 DC(의 핸들)
	HWND mhWnd;		//윈도우 핸들

private:
	LARGE_INTEGER mTickPerSecond;	//초당 틱수
	LARGE_INTEGER mTime;				//임의의 시점에 틱수

	float mDeltaTime;		//한 프레임에 걸린 시간

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
	//window api: 운영체제(system software)가 하드웨어 자원을 관리하고 응용프로그램에게는 간접적인 제어권(handle)만 제공한다.
	//
	// GDI: Graphic Device Interface: window api에서 그래픽 시스템을 관장하는 추상화된 도구
	//
	//directX sdk : 그래픽 처리 장치에 직접적으로 접근하여 제어하도록 만들어진 MS사에서 만든 라이브러리, 간접적인 추상화도구.
	//	window 운영체제에서 제공하는 (예를 들면 GDI )그래픽 처리 시스템보다 매우 빠른 제어를 위해 만들어진다.
	//		그래픽 처리 장치( GPU + VRAM + DRIVER프로그램) <-- 예)그래픽 카드

	//		Driver: 하드웨어에 밀착된 프로그램. 하드웨어를 low수준에서 직접 제어한다.
	//			운영체제와 하드웨어 사이에서 동작한다
	//			DirectX와 하드웨어 사이에서 동작한다
public:
	//드라이버 종류
	D3D_DRIVER_TYPE         mDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       mFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	//각각의 다이렉트엑스 버전 수준에 맞는 기능집합 <-- feature level

	// process:프로그램이 실행중인 것. 실행의 흐름 <--n개의 스레드thread로 만들어져있다.
	// thread: 실행흐름의 최소단위.
	//		스레드는 프로세스의 메모리를 공유한다. 단, 스택은 각자 가진다.

	/*
	* device:
	*	single thread 용 처리 객체
	* 
	*	single thread에서 처리해야하는 것들을 다루는 객체이다
	* <-- 주로 자원resources을 다룬다
	*	buffer, texture, shader, ...
	* 
	* device context:
	*	multi thread 용 처리 객체
	* 
	*		render state, draw primitive <--주로 명령에 관한 것들을 다룬다.
	*/

	//Device 장치(그래픽 처리 장치)
	ID3D11Device* mpd3dDevice = nullptr;
	ID3D11Device1* mpd3dDevice1 = nullptr;

	//Device Context 장치(그래픽 처리 장치)
	ID3D11DeviceContext* mpImmediateContext = nullptr;
	ID3D11DeviceContext1* mpImmediateContext1 = nullptr;

	//스왑 체인
	//Double buffer 구조가 존재하고, 이 두 버퍼가 서로 교환되는 구조의 추상화된 객체이다.
	//window api때 학습한 내용과 비교하여 보자면, 수동으로 작성했던 더블 버퍼 구조는 버퍼 두 개가 존재하고
	//(back buffer, front buffer) back buffer에서 front buffer로 이미지 데이터를 복사하는 형식이었다면,
	//
	//DirectX에서 제공하는 더블버퍼는 좀더 진화된 개념으로
	//이 두 버퍼가 서로 교환을 반복한다.
	//이러한 동작이 마치 사슬처럼 보여서
	//SwapChain이라고 한다.
	IDXGISwapChain* mpSwapChain = nullptr;
	IDXGISwapChain1* mpSwapChain1 = nullptr;

	//렌더링된 결과 이미지 데이터를 담을 수 있는 추상화된 개념이다
	//스왑체인의 백버퍼를 가지고 만든다.
	//스왑체인의 백버퍼backbuffer를 D3D관점에서 보는 추상화된 개념이다.
	ID3D11RenderTargetView* mpRenderTargetView = nullptr;
	//View <-- 일반적으로 어떠한 임의의 데이터를 들여다볼 수 있는 추상화된 개념이다.

	public:
		HRESULT InitDevice();
		void CleanupDevice();
};

