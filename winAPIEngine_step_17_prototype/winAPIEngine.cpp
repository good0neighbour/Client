// winAPIEngine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "winAPIEngine.h"

#include "CAPIEngine.h"

#include "CUnit.h"

#include "CTexture.h"

#include "ryumacro.h"

#include "CInputMgr.h"

#include "CActor.h"

//test
#include <list>
using namespace std;

/*
* 이번 예시에서는
* '프로토타입 패턴'을 적용해보기로 한다. <-- 객체 생성 문제에 관한 패턴 중 하나다.
* 
* Prototype Pattern
* 
*   Prefab 원본객체
*   라는 개념을 만들고
* 
*   이것을 '복제'하여
*   새로운 객체를 생성하는 형태를 취하는 패턴
* 
*   clone()함수를 만드는 특징이 드러난다.
* 
* 프로토타입 패턴의 목적
* i)임의의 데이터 값들을 가지는 객체를 '손쉽게' 만들어내는 것이 가능하다 <--이게 가장 주요한 목적
* ii)또한, 객체를 생성하는 부분의 코드를 보다 일반화하여 표현하는 것이 가능하다.
*/

class CRyuEngine : public CAPIEngine
{
    //test
    //CObjectRyu* testObject = nullptr;

    //원본 객체( 주인공 기체의 원본 객체 )
    CUnit* PFActor = nullptr;

    CTexture* mpTexture = nullptr;
    CActor* mpActor = nullptr;

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

        //원본객체(주인공 기체에 대한 원본 객체 ) 생성
        PFActor = new CActor();
        PFActor->SetPosition(800 * 0.5f, 600 - 50 - 100.0f);
        PFActor->SetRadius(50.0f);
        //pivot
        PFActor->SetAnchorPoint(0.5f, 0.5f);
        PFActor->SetTexture(mpTexture);

        //실제 객체 생성
        mpActor = static_cast<CActor*>(PFActor->clone());   //원본객체를 복제하여 객체를 생성
        mpActor->AddRef();

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
        //실제 객체 소멸
        SAFE_RELEASE(mpActor)

        //원본 객체 소멸
        SAFE_DELETE(PFActor)

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
        mpActor->SetVelocity(tVelocity);
        if (CInputMgr::GetInstance()->KeyPress("OnMoveLt"))
        {
            //'오일러 축차적 적분법' 에 의한 위치 이동 코드
            //mpUnit->mPosition.mX = mpUnit->mPosition.mX - 0.1f;
            //순수한 방향의 속도 설정 (-1,0)
            tVelocity.mX = -1.0f;
            tVelocity.mY = 0.0f;
        
            //mpUnit->SetVelocity(tVelocity * 0.1f);
            mpActor->SetVelocity(tVelocity * 350.0f);    //초당 350PIXEL
        
            OutputDebugString(L"key input A\n");
        }

        if (CInputMgr::GetInstance()->KeyPress("OnMoveRt"))
        {
            //mpUnit->mPosition.mX = mpUnit->mPosition.mX + 0.1f;
            //순수한 방향의 속도 설정 (+1,0)
            tVelocity.mX = +1.0f;
            tVelocity.mY = 0.0f;
        
            //mpUnit->SetVelocity(tVelocity * 0.1f);
            mpActor->SetVelocity(tVelocity * 350.0f);    //초당 350PIXEL
        
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
        mpActor->Update(tDeltaTime);

        //render
        this->Clear(0.1f, 0.1f, 0.3f);        
        
        mpActor->Render(this);
                
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


