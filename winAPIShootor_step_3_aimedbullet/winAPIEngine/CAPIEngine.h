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
	void DrawCircle(float tX, float tY, float tRadius);
	//��Ʈ�� �̹��� �׸��� �Լ�
	void DrawTexture(float tX, float tY, CTexture* tpTexture);


	//����ȭ: ũ�⸦ 1�� ����� �ٷ�� ����
	// [0,1] <--�󱸰� 0���� 1������ ��ġ(0�� 1 ����)

	//ȭ�� Ŭ���� �Լ�
	void Clear(float tR = 0.0f, float tG = 0.0f, float tB= 0.0f);
	//������ ȭ�� ���� �Լ�
	void Present();


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
	static HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
	WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
	WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.


	HDC mhDC;			//���� ȭ�� DC(�� �ڵ�)
	HWND mhWnd;		//������ �ڵ�


	CTexture* mpBackBuffer = nullptr;


private:
	LARGE_INTEGER mTickPerSecond;	//�ʴ� ƽ��
	LARGE_INTEGER mTime;				//������ ������ ƽ��

	float mDeltaTime;		//�� �����ӿ� �ɸ� �ð�

};

