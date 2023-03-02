#pragma once

#include "framework.h"

#include "SVector2D.h"
#include "ryumacro.h"
#include "CUnit.h"
#include "CCollisionMgr.h"

//TransparentBlt�Լ��� ���Ǹ� ��ŷ��Ű�� ���� �ʿ�
#pragma comment(lib, "msimg32.lib")




#define MAX_LOADSTRING 100


class CTexture;

//32bit ���� ���� ����ü
struct RyuPixel
{
	//uint8_t unsigned char 8��Ʈ
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

	//���� �����Լ����� ������ ���´�
private:
	CAPIEngine(const CAPIEngine& t) = delete;
	CAPIEngine& operator=(const CAPIEngine& t) = delete;

public:
	BOOL Create(HINSTANCE hInstance, int nCmdShow);
	MSG Run();


	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate(float tDeltaTime);

	//���׸��� �Լ�
	void DrawCircle(float tX, float tY, float tRadius, float tR = 1.0f, float tG = 0.0f, float tB = 0.0f);
	void DrawRect(float tX, float tY, float tWidth,float tHeight, float tR = 1.0f, float tG = 0.0f, float tB = 0.0f);
	//��Ʈ�� �̹��� �׸��� �Լ�
	//���� �÷�Ű�� �Ͼ���� ����, �Ű����� �⺻�� ������ �̿�
	void DrawTexture(float tX, float tY, CTexture* tpTexture, COLORREF tColorKey = RGB(255,0,255));

	void DrawTexturePartial(float tX, float tY, CTexture* tpTexture,int tRow, int tCol, int tIndex, COLORREF tColorKey = RGB(255, 0, 255));

	//2D �ȼ� ������ ������ �ﰢ���� �׸���. �׷��Ƿ� ��ġ���� Ÿ�Ե������� int
	void DrawTriangle(int x_0, int y_0,int x_1, int y_1,int x_2, int y_2,
		float tR = 1.0f, float tG = 1.0f, float tB = 1.0f);
	
	void FillTriangle(int x_0, int y_0, int x_1, int y_1, int x_2, int y_2,
		float tR = 1.0f, float tG = 1.0f, float tB = 1.0f);
	
	void FillTriangle(int x_0, int y_0, int x_1, int y_1, int x_2, int y_2,
		RyuPixel tColor);




	//����ȭ: ũ�⸦ 1�� ����� �ٷ�� ����
	// [0,1] <--�󱸰� 0���� 1������ ��ġ(0�� 1 ����)

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


	//������ü ���� �Լ�
	//CreatePrefab������ ������ ������ü�� �����ϱ� ���� 
	//���������� �ٷ� �ڵ� ǥ���� �Ϲ�ȭ�ϱ� ���� ���ø��� ����Ͽ���.
	//		������ ������ �������� ref ������ �������� �ʰڴ�.
	template<typename T>
	T* CreatePrefab(CTexture* tpTex, float tAnchorX, float tAnchorY, SVector2D tPosition)
	{
		T* tpPrefab = new T();

		tpPrefab->Create(this);						//<--
		tpPrefab->SetPosition(tPosition);
		
		tpPrefab->SetRadius(50.0f);
		
		tpPrefab->SetAnchorPoint(tAnchorX, tAnchorY);
		tpPrefab->SetTexture(tpTex);

		tpPrefab->SetIsActive(true);			//������ü ������ Ȱ��ȭ ���·� ����

		return tpPrefab;
	}

	//������ü ���� �Լ�
	void DestroyPrefab(CUnit*& tPrefab)
	{
		tPrefab->Destroy();

		SAFE_DELETE(tPrefab)
	}


	//���� ��ü ���� �Լ�
	template<typename T>
	T* InstantObject(CUnit* tpPrefab)
	{
		T* tpUnit = nullptr;

		tpUnit = static_cast<T*>(tpPrefab->clone());
		//tpUnit�� ���������̴�.
		//������ ��������ٰ� �������� �Ҹ�� ������Ƿ� ���ȴ�.
		//�׷��� �ڵ�� �����Ͽ���
		/*tpUnit->AddRef();
		tpUnit->Release();*/

		//��� ���ֵ��� �浹�����ڿ� ������ ����ϵ��� ��
		CCollisionMgr::GetInstance()->AddUnit(tpUnit);


		return tpUnit;
	}

	//���� ��ü ���� �Լ�
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
	HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
	
	WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
	WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.


	HDC mhDC;			//���� ȭ�� DC(�� �ڵ�)
	HWND mhWnd;		//������ �ڵ�

	//���Ǹ� ���� public���� Ǯ�ڴ�.
public:
	CTexture* mpBackBuffer = nullptr;


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

};

