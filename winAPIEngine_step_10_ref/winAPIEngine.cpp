// winAPIEngine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "winAPIEngine.h"

#include "CAPIEngine.h"

#include "CUnit.h"

#include "CTexture.h"

//test
#include <list>
using namespace std;

/*
* 여기서는 CUnit에 Pivot기능을 추가해보자
*/

class CRyuEngine : public CAPIEngine
{
    //test
    //CObjectRyu* testObject = nullptr;

    CTexture* mpTexture = nullptr;
    CUnit* mpUnit = nullptr;

public:
    CRyuEngine() {};
    virtual ~CRyuEngine() {};

    //delete는 해당 함수의 구현을 막는 문법
private:
    CRyuEngine(const CRyuEngine& t) = delete;
    CRyuEngine& operator = (const CRyuEngine& t) = delete;

public:
    virtual void OnCreate() override
    {
        CAPIEngine::OnCreate();

        //test
        //testObject = new CObjectRyu();
        //testObject->AddRef();
        //
        //list<CObjectRyu*> testObjectList;
        //testObjectList.push_back(testObject);
        //testObject->AddRef();
        //
        //testObject->Release();

        //todo
        mpTexture = new CTexture();
        mpTexture->LoadTexture(hInst, mhDC, L"resources/bongbong_0.bmp");

        mpUnit = new CUnit();
        mpUnit->AddRef();
        mpUnit->SetPosition(800 * 0.5f, 600 - 50 - 100.0f);
        mpUnit->SetRaidus(50.0f);
        //pivot
        mpUnit->SetAnchorPoint(0.5f, 0.5f);
        mpUnit->SetTexture(mpTexture);
    }
    virtual void OnDestroy() override
    {
        //todo
        if (nullptr != mpTexture)
        {
            delete mpTexture;
            mpTexture = nullptr;
        }
        if (nullptr != mpUnit)
        {
            mpUnit->Release();
            //delete mpUnit;
            mpUnit = nullptr;
        }

        //if (nullptr != testObject)
        //{
        //    testObject->Release();
        //    testObject = nullptr;
        //}

        CAPIEngine::OnDestroy();
    }
    virtual void OnUpdate() override
    {
        CAPIEngine::OnUpdate();

        //update

        //&비트연산의 이유
        //GetAsyncKeyState의 리턴값은 다음과 같다
        /*
        * 0x0000: 이전에 눌림 없음. 호출시점에 눌림 없음 <-- 안눌림
        * 0x0001: 이전에 눌림 있음. 호출시점에 눌림 없음 <-- 떼어짐
        * 
        * 0x8000: 이전에 눌림 없음. '호출시점에 눌림' 있음 <-- 새롭게 눌림
        * 0x8001: 이전에 눌림 있음. '호출시점에 눌림' 있음 <-- 눌리고 있음
        * 
        * 이것을 알아오기 위함이다.
        * 
        * 비트연산으로 계산하므로 밑에 두 가지 경우가 참이 된다.
        */

        //A키가 눌리고 있다면
        SVector2D tVelocity;
        if (GetAsyncKeyState('A') & 0x8000)  //<-- GetAsyncKeyState함수가 호출되는 시점에 A키 눌림이 있다.
        {
            //'오일러 축차적 적분법' 에 의한 위치 이동 코드
            //mpUnit->mPosition.mX = mpUnit->mPosition.mX - 0.1f;
            //순수한 방향의 속도 설정 (-1, 0)
            tVelocity.mX = -1.0f;
            tVelocity.mY = 0.0f;
        }
        //D키가 눌리고 있다면
        if (GetAsyncKeyState('D') & 0x8000)  //<-- GetAsyncKeyState함수가 호출되는 시점에 D키 눌림이 있다.
        {
            //mpUnit->mPosition.mX = mpUnit->mPosition.mX + 0.1f;
            //순수한 방향의 속도 설정 (+1, 0)
            tVelocity.mX = +1.0f;
            tVelocity.mY = 0.0f;
        }

        //속도에 의한 이동 코드
        mpUnit->mPosition = mpUnit->mPosition + tVelocity * 0.1f * 1.0f;

        //render
        this->Clear(0.1f, 0.1f, 0.3f);

        //Update Method Pattern: 한 프레임에 일어나는 동작을 함수화하여 게임루프와 결합하여 프로그래밍하는 패턴
        mpUnit->Render(this);

        this->Present();
    }
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    //CAPIEngine tEngine;
    CRyuEngine tEngine;
    tEngine.Create(hInstance, nCmdShow);

    //복사생성자
    //CRyuEngine tA = tEngine;

    //CRyuEngine tB;
    //복사대입연산자
    //tB = tEngine;

    MSG msg = { 0 };
    msg = tEngine.Run();

    return (int)msg.wParam;
}