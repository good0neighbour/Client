#pragma once

#include "framework.h"

#include "SVector2D.h"
#include "ryumacro.h"
#include "CUnit.h"
#include "CCollisionMgr.h"

//TransparentBlt함수의 정의를 링킹시키기 위해 필요
#pragma comment(lib, "msimg32.lib")




#define MAX_LOADSTRING 100


class CTexture;

//32bit 색상 정보 구조체
struct RyuPixel
{
	//uint8_t unsigned char 8비트
	uint8_t r = 255;
	uint8_t g = 255;
	uint8_t b = 255;
	uint8_t a = 255;
};

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
	virtual void OnUpdate(float tDeltaTime);

	//원그리기 함수
	void DrawCircle(float tX, float tY, float tRadius, float tR = 1.0f, float tG = 0.0f, float tB = 0.0f);
	void DrawRect(float tX, float tY, float tWidth,float tHeight, float tR = 1.0f, float tG = 0.0f, float tB = 0.0f);
	//비트맵 이미지 그리기 함수
	//투명 컬러키로 하얀색을 지정, 매개변수 기본값 문법을 이용
	void DrawTexture(float tX, float tY, CTexture* tpTexture, COLORREF tColorKey = RGB(255,0,255));

	void DrawTexturePartial(float tX, float tY, CTexture* tpTexture,int tRow, int tCol, int tIndex, COLORREF tColorKey = RGB(255, 0, 255));

	//2D 픽셀 단위에 공간에 삼각형을 그린다. 그러므로 위치값의 타입디자인은 int
	void DrawTriangle(int x_0, int y_0,int x_1, int y_1,int x_2, int y_2,
		float tR = 1.0f, float tG = 1.0f, float tB = 1.0f);
	
	void FillTriangle(int x_0, int y_0, int x_1, int y_1, int x_2, int y_2,
		float tR = 1.0f, float tG = 1.0f, float tB = 1.0f);
	
	void FillTriangle(int x_0, int y_0, int x_1, int y_1, int x_2, int y_2,
		RyuPixel tColor);




	//정규화: 크기를 1로 만들어 다루는 개념
	// [0,1] <--폐구간 0에서 1사이의 수치(0과 1 포함)

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


	//원본객체 생성 함수
	//CreatePrefab에서는 임의의 원본객체를 생성하기 위해 
	//범용적으로 다뤄 코드 표현을 일반화하기 위해 템플릿을 사용하였다.
	//		프리팹 생성과 해지에는 ref 개념을 적용하지 않겠다.
	template<typename T>
	T* CreatePrefab(CTexture* tpTex, float tAnchorX, float tAnchorY, SVector2D tPosition)
	{
		T* tpPrefab = new T();

		tpPrefab->Create(this);						//<--
		tpPrefab->SetPosition(tPosition);
		
		tpPrefab->SetRadius(50.0f);
		
		tpPrefab->SetAnchorPoint(tAnchorX, tAnchorY);
		tpPrefab->SetTexture(tpTex);

		tpPrefab->SetIsActive(true);			//원본객체 생성시 활성화 상태로 만듦

		return tpPrefab;
	}

	//원본객체 해제 함수
	void DestroyPrefab(CUnit*& tPrefab)
	{
		tPrefab->Destroy();

		SAFE_DELETE(tPrefab)
	}


	//실제 객체 생성 함수
	template<typename T>
	T* InstantObject(CUnit* tpPrefab)
	{
		T* tpUnit = nullptr;

		tpUnit = static_cast<T*>(tpPrefab->clone());
		//tpUnit은 지역변수이다.
		//참조가 만들어졌다가 지역변수 소멸로 사라지므로 상쇄된다.
		//그래서 코드는 생략하였다
		/*tpUnit->AddRef();
		tpUnit->Release();*/

		//모든 유닛들을 충돌관리자에 생성시 등록하도록 함
		CCollisionMgr::GetInstance()->AddUnit(tpUnit);


		return tpUnit;
	}

	//실제 객체 해제 함수
	template<typename T>
	void DestroyObject(T*& tUnit)
	{
		tUnit->Destroy();

		SAFE_RELEASE(tUnit)
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

	//편의를 위해 public으로 풀겠다.
public:
	CTexture* mpBackBuffer = nullptr;


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

};

