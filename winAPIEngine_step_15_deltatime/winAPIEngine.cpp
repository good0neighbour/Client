﻿// winAPIEngine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
// 현재위치 = 이전위치 + 속도 * 시간간격
// this->mPosition = this->mPosition + mVelocity;
// 
// 시간간격<---- 1프레임
// 
// 프레임 기반으로 진행되는 게임 월드에서의 속도 예
// 
// 초당 프레임
// : 1초에 프레임 갯수가 몇개이냐
// 
// 현재위치 = 이전위치 + 50 * 1프레임
// 
// Frames per second
// 
// 예) 10fps
//      1초에 50 * 10 = 500
// 
// 예) 100fps
//      1초에 50 * 100 = 5000
//

#include "framework.h"
#include "winAPIEngine.h"

#include "CAPIEngine.h"

#include "CUnit.h"

#include "CTexture.h"

#include "ryumacro.h"

#include "CInputMgr.h"

//test
#include <list>
using namespace std;

/*
* 이번 예시에서는
* '시간 기반 진행'을 위한 구조를
* 작성해보자.
* 
*   프레임 기반 진행 vs 시간 기반 진행
* 
* 프레임 기반 진행의 문제점<--- 컴퓨터의 성능에 따라 게임 월드의 진행 속도가 달라진다
* ---> 실제 시간 기반 진행
* 
* <--- 시간 기반 진행 으로 만드려면 무슨 개념이 필요한가?
*   ---> N프레임 : 1초 = 1프레임 : x초
*   x = 1/N( 여기서 N은 초당 프레임 )
*   즉, 실제 시간기반으로 진행하려면 시간 간격의 기준이 프레임이 아니라
*   1초를 잘게 쪼갠 실제 시간이 필요하다.
*   -->즉, '한 프레임에 걸리는 시간'을 측정하면 된다.
* 
* 예) 초당프레임이 10fps인 시스템이 있다면
*       pos = pos + 50 * (1 / 10)
* 
*       50 * (1 / 10) * 10 = 50 <--1초에 실제로 진행된 수치
* 
*   초당프레임이 100fps인 시스템이 있다면
*       pos = pos + 50 * (1 / 100)
* 
*       50 * (1 / 100) * 100 = 50 <--1초에 실제로 진행된 수치
* 
*   --------------------------------------------
* 
*   TimeTick :시스템이 구동되고 나서부터 시스템은 일정 카운트를 센다. 이것을 tick이라고 한다.
*   <-- 이 tick을 가지고 시간측정을 하자.
* 
*   timeGetTime()
*   GetTickCount()
* 
*   <-- 정밀도가 1/1000초 정도이다. 이것은 비교적 부정확하므로 사용하지 않겠다.
*   --> 클럭 장치에 의해 틱을 센다
* 
*   QueryPerformanceCounter
* 
*   <-- 정밀도가 1/1,000,000초 정도이다. 이것은 비교적 정확하므로 이것을 사용하겠다.
*   --> CPU의 주파수에 기반하여 틱을 센다
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
    CRyuEngine& operator=(const CRyuEngine& t) = delete;

public:
    virtual void OnCreate() override
    {
        CAPIEngine::OnCreate();

        //test
        RYU_INT tA = 3;
        RYU_INT tB = 2;
        RYU_INT tResult = 0;

        tResult = tA + tB;

        char tszTemp[256] = { 0 };  //c style 문자열
        sprintf_s(tszTemp, "tResult: %d\n", tResult);
        OutputDebugStringA(tszTemp);
        //~A가 붙은 OutputDebugString은 아스키코드용

            //test
            //testObject = new CObjectRyu();
            //testObject->AddRef();

            //list<CObjectRyu*> testObjectList;
            //testObjectList.push_back(testObject);   //주소값이 복사되었으므로 참조가 하나 증가되어야 한다.
            //testObject->AddRef();

            //testObject->Release();      //리스트가 지역변수이므로 해당 지역을 벗어나면 소멸된다. 이를 고려하여 참조 감소

        //todo
        mpTexture = new CTexture();
        mpTexture->LoadTexture(hInst, mhDC, L"resources/bongbong_0.bmp");

        mpUnit = new CUnit();
        mpUnit->AddRef();//
        mpUnit->SetPosition(800 * 0.5f, 600 - 50 - 100.0f);
        mpUnit->SetRadius(50.0f);
        //pivot
        mpUnit->SetAnchorPoint(0.5f, 0.5f);
        mpUnit->SetTexture(mpTexture);

        //입력 매핑 등록
        CInputMgr::GetInstance()->AddKey("OnMoveLt", 'A');
        CInputMgr::GetInstance()->AddKey("OnMoveRt", 'D');
        CInputMgr::GetInstance()->AddKey("OnFire", VK_SPACE);

        CInputMgr::GetInstance()->AddKey("OnTest", VK_LCONTROL, 'H');
    }

    virtual void OnDestroy() override
    {
        /*if (nullptr != mpTexture)
        {
            delete mpTexture;
            mpTexture = nullptr;
        }*/
        //매크로 함수를 사용하여
        //호출하는 측의 코드가 간략화되었다.
        SAFE_DELETE(mpTexture)

        //todo
        /*if (nullptr != mpUnit)
        {
            mpUnit->Release();
            //delete mpUnit;
            mpUnit = nullptr;
        }*/
        SAFE_RELEASE(mpUnit);

        //if (nullptr != testObject)
        //{
        //    testObject->Release();
        //    testObject = nullptr;
        //}
        
        CAPIEngine::OnDestroy();
    }
    virtual void OnUpdate(float tDeltaTime) override
    {
        CAPIEngine::OnUpdate(tDeltaTime);

        //update

         //&비트연산의 이유
        // GetAsyncKeyState 의 리턴값은 다음과 같다.
        /*
            0x0000: 이전에 눌림 없음. 호출시점에 눌림 없음 <-- 안눌림
            0x0001: 이전에 눌림 있음. 호출시점에 눌림 없음 <-- 떼어짐

            0x8000: 이전에 눌림 없음. '호출시점에 눌림' 있음 <-- 새롭게 눌림
            0x8001: 이전에 눌림 있음. '호출시점에 눌림' 있음 <-- 눌리고있음

            이것을 알아오기 위함이다.

            비트연산으로 계산하므로 밑에 두가지경우가 참이된다.
        */

        //A키가 눌리고 있다면
        //SVector2D tVelocity;
        //mpUnit->SetVelocity(tVelocity);
        //if (GetAsyncKeyState('A') & 0x8000)
        //{
        //    //'오일러 축차적 적분법' 에 의한 위치 이동 코드
        //    //mpUnit->mPosition.mX = mpUnit->mPosition.mX - 0.1f;
        //    //순수한 방향의 속도 설정 (-1,0)
        //    tVelocity.mX = -1.0f;
        //    tVelocity.mY = 0.0f;

        //    mpUnit->SetVelocity(tVelocity * 0.1f);
        //    
        //    OutputDebugString(L"key input A\n");
        //}        
        //if (GetAsyncKeyState('D') & 0x8000)
        //{
        //    //mpUnit->mPosition.mX = mpUnit->mPosition.mX + 0.1f;
        //    //순수한 방향의 속도 설정 (+1,0)
        //    tVelocity.mX = +1.0f;
        //    tVelocity.mY = 0.0f;
        //    
        //    mpUnit->SetVelocity(tVelocity * 0.1f);

        //    OutputDebugString(L"key input D\n");
        //}        
        //if (GetAsyncKeyState(VK_SPACE) & 0x8000) 
        //{
        //    OutputDebugString(L"Fire Bullet............VK_SPACE\n");
        //}

        //mpUnit->Update();

        SVector2D tVelocity;
        mpUnit->SetVelocity(tVelocity);
        if (CInputMgr::GetInstance()->KeyPress("OnMoveLt"))
        {
            //'오일러 축차적 적분법' 에 의한 위치 이동 코드
            //mpUnit->mPosition.mX = mpUnit->mPosition.mX - 0.1f;
            //순수한 방향의 속도 설정 (-1,0)
            tVelocity.mX = -1.0f;
            tVelocity.mY = 0.0f;
        
            //mpUnit->SetVelocity(tVelocity * 0.1f);
            mpUnit->SetVelocity(tVelocity * 350.0f);    //초당 350PIXEL
        
            OutputDebugString(L"key input A\n");
        }

        if (CInputMgr::GetInstance()->KeyPress("OnMoveRt"))
        {
            //mpUnit->mPosition.mX = mpUnit->mPosition.mX + 0.1f;
            //순수한 방향의 속도 설정 (+1,0)
            tVelocity.mX = +1.0f;
            tVelocity.mY = 0.0f;
        
            //mpUnit->SetVelocity(tVelocity * 0.1f);
            mpUnit->SetVelocity(tVelocity * 350.0f);    //초당 350PIXEL
        
            OutputDebugString(L"key input D\n");
        }

        if (CInputMgr::GetInstance()->KeyUp("OnFire"))
        {
            OutputDebugString(L"Fire Bullet............VK_SPACE\n");
        }

        if (CInputMgr::GetInstance()->KeyUp("OnTest"))
        {
            OutputDebugString(L"Fire Bullet............VK_LCONTROL  'H'\n");
        }
        
        //시간기반진행
        mpUnit->Update(tDeltaTime);

        //render
        this->Clear(0.1f, 0.1f, 0.3f);        
        
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


