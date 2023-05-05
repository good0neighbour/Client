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

	//가짜구현
	//화면의 너비
	float GetWindowWidth()
	{
		return 800.0f;
	}
	//화면의 높이
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
	static HINSTANCE hInst;                                // 현재 인스턴스입니다.
	WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
	WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


	HDC mhDC;	//현재 화면 DC 핸들 
	HWND mhWnd;	//윈도우 핸들
	


	LARGE_INTEGER mSecond;
	LARGE_INTEGER mTime;

	float mDeltaTime;		//프레임 시간: 한 프레임에 걸리는 시간



	//CD3D타입의 객체에게 directX의 초기화와 해제 기능을 위임한다
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

	//window api 운영체제가 하드웨어 자원을 관리하고 응용프로그램에게는 간접적인 제어권만 제공한다.
	//DirectX: 그래픽 카드에서 직접적으로 접근하여 제어하도록 만들어진 ms에서 만든 라이브러리( 클래스나 함수의 집합 ). 간접적인 추상화 도구이다.
	//					운영체제보다 빠른 제어를 위해 만들어졌다.
	//		그래픽 처리 장치( GPU + VRAM + Driver프로그램 )

//public:
//	//드라이버: 하드웨어에 밀착된 소프트웨어
//	D3D_DRIVER_TYPE         mDriverType = D3D_DRIVER_TYPE_NULL;
//	D3D_FEATURE_LEVEL       mFeatureLevel = D3D_FEATURE_LEVEL_11_0;
//	//각각의 다이렉트엑스 버전 수준에 맞는 기능집합<-- feature level
//
//
//	//process: 프로그램이 실행중인 것, 실행의 흐름을 만든다 <-- n개의 thread로 구성된다
//	//스레드 thread: 실행 흐름의 최소단위
//	//			스레드는 프로세스의 메모리를 공유한다. 단, 스택은 각자 가진다
//
//	//Device: 그래픽 처리 장치( GPU + VRAM + Driver프로그램 ) 를 추상화 해놓은 객체, single thread용의 처리객체
//	ID3D11Device* mpd3dDevice = nullptr; //Interface direct 3d 11 device
//	ID3D11Device1* mpd3dDevice1 = nullptr;
//
//	//DeviceContext: 그래픽 처리 장치( GPU + VRAM + Driver프로그램 ) 를 추상화 해놓은 객체, multi thread용의 처리객체
//	//		( 우리가 작성하는 예시들에서는 싱글스레드를 가정하고 작성한다 ) 
//	ID3D11DeviceContext* mpImmediateContext = nullptr;
//	ID3D11DeviceContext1* mpImmediateContext1 = nullptr;
//
//	//스왑체인은
//	//더블버퍼링 구조가 존재하고 이 두 버퍼가 서로 교환되는 구조의 추상화된 객체이다.
//	//window api때 수동으로 작성해본 더블 버퍼 구조는 버퍼 두개가 존재하고
//	// ( back buffer, front buffer ) back buffer에서 front buffer로 이미지 데이터를 복사하느 형태로 작동했었다면
//	//여기 DirectX에서 제공하는 더블버퍼는 좀더 진화된 개념으로써
//	//이 두 버퍼가 서로 교환된다
//	//이러한 동작이 마치 사슬처럼 보여
//	//SwapChain 이라 명명되었다
//	IDXGISwapChain* mpSwapChain = nullptr;
//	IDXGISwapChain1* mpSwapChain1 = nullptr;
//
//	//랜더링된 결과 이미지 데이터를 담을 수 있도록 추상화된 개념이다.
//	//랜더 타겟 '뷰'는 back buffer후면버퍼를 이미지 데이터를 랜더링할 수 있는 형태로 추상화해놓은 객체이다
//	ID3D11RenderTargetView* mpRenderTargetView = nullptr;
//
//	//depth buffer의 개념을 위해 선언
//	//DirectX11에서 깊이/스텐실버퍼는 텍스처 리소스(이미지데이터)의 한 종류로 취급한다
//	ID3D11Texture2D* mpDepthStencil = nullptr;
//	// 깊이버퍼: 깊이 값을 기억시켜두는 메모리. 앞뒤판정용 임시 데이터 기록 버퍼이다.
//	// 스텐실버퍼: 임의의 효과를 만들기 위한 메모리
//	//DirectX에서는 이 두 버퍼를 하나의 리소스에 담아 사용한다. 한 픽셀에 24+8 = 32 를 사용한다
//	ID3D11DepthStencilView* mpDepthStencilView = nullptr;
//
//	//'텍스처'란 셰이더에서 이미지 데이터를 추상화하여 다루는 방법이다. 면에 매핑하기 위한 시각에서 보는 개념이다.
//	//'뷰'란 셰이더에서 이미지 데이터를 추상화하여 다루는 방법이다. 텍스처 의 개념은 당연히 포함하고 거기에 랜더링 파이프라인에서 다루는 무엇인가를 표시하는 영역이라는 개념이다.
//
//public:
//	HRESULT InitDevice();
//	void CleanupDevice();
};

