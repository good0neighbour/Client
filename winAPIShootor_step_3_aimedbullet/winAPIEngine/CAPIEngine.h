#pragma once

#include "framework.h"

#include "SVector2D.h"
#include "ryumacro.h"
#include "CUnit.h"

#define MAX_LOADSTRING 100


class CTexture;

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
	void DrawCircle(float tX, float tY, float tRadius);
	//비트맵 이미지 그리기 함수
	void DrawTexture(float tX, float tY, CTexture* tpTexture);


	//정규화: 크기를 1로 만들어 다루는 개념
	// [0,1] <--폐구간 0에서 1사이의 수치(0과 1 포함)

	//화면 클리어 함수
	void Clear(float tR = 0.0f, float tG = 0.0f, float tB= 0.0f);
	//프레임 화면 제출 함수
	void Present();


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
	static HINSTANCE hInst;                                // 현재 인스턴스입니다.
	WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
	WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


	HDC mhDC;			//현재 화면 DC(의 핸들)
	HWND mhWnd;		//윈도우 핸들


	CTexture* mpBackBuffer = nullptr;


private:
	LARGE_INTEGER mTickPerSecond;	//초당 틱수
	LARGE_INTEGER mTime;				//임의의 시점에 틱수

	float mDeltaTime;		//한 프레임에 걸린 시간

};

