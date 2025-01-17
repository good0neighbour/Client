﻿// winAPIEngine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "winAPIEngine.h"

#include "CAPIEngine.h"

#include "CUnit.h"

/*
* i) CUnit 클래스를 만들어 추가한다.
*   CUnit클래스를 만들어 '게임오브젝트'를 묘사해보자.
* 
* ii) CUnit클래스의 외관은 일단 도형 원으로 취급하자.
* 
* iii) 이제 CRyuEngine의 적절한 함수정의 부분들에
*   CUnit 객체를 맏르고 호출하여 동작시켜본다.
*   <--- Update Method Pattern 개념을 살펴본다
*       (<-- Game Loop Pattern 개념도 살펴본다)
*/

class CRyuEngine : public CAPIEngine
{
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

        //todo
        mpUnit = new CUnit();
        mpUnit->SetPosition(800 * 0.5f, 600 - 50 - 30.0f);
        mpUnit->SetRaidus(50.0f);
    }
    virtual void OnDestroy() override
    {
        //todo
        if (mpUnit)
        {
            delete mpUnit;
            mpUnit = nullptr;
        }

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
        if (GetAsyncKeyState('A') & 0x8000)  //<-- GetAsyncKeyState함수가 호출되는 시점에 A키 눌림이 있다.
        {
            //현재 위치 = 이전위치 + 속도
            mpUnit->mX = mpUnit->mX - 0.01f;
        }
        //D키가 눌리고 있다면
        if (GetAsyncKeyState('D') & 0x8000)  //<-- GetAsyncKeyState함수가 호출되는 시점에 D키 눌림이 있다.
        {
            mpUnit->mX = mpUnit->mX + 0.01f;
        }

        //render
        //Update Method Pattern: 한 프레임에 일어나는 동작을 함수화하여 게임루프와 결합하여 프로그래밍하는 패턴
        mpUnit->Render(this);

        //todo
        //this->DrawCircle(200.0f, 200.0f, 100.0f);
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