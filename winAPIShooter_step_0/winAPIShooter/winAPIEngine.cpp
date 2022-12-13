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
#include "CBullet.h"

//test
#include <list>
using namespace std;

//소스코드상에서 설정
//winAPIEngine.lib를 링킹과정에서 링크하겠다 라고 컴파일 옵션 지정
#pragma comment(lib, "winAPIEngine.lib")

/*
* 컴파일 compile: source code file을 object file로 만드는 것
* 링킹 linking: 서로 다른 파일에 존재하는 프로그램의 부품(함수, 변수)등의 연결을 짓는 작업
* 
* 분할 컴파일: 클래스 등의 부품 단위로 소스코드 파일을 분리하여 작성하고 컴파일하는 것
* 분할 컴파일의 목적
*   i) 전체 빌드 시간을 단축하기 위해서 <--원래 주요 목적
*   ii) 파일 단위로 나눈면 작업 분담하기 좋다.
* 
* ============================================================
*                          빌드 build
* ============================================================
*      전처리단계       컴파일       링킹
*      (텍스트 치환)
*      매크로 문법
*            ----> *.cpp ----> *.obj ----> *.exe              ---> 실행run
*            ----> *.cpp ----> *.obj       *.lib
*            ...                           *.dll
*            ----> *.cpp ----> *.obj
* 
*                              *.lib                               *.dll
* ============================================================
* 
* exe: 실행파일.   자체적으로 메모리로 올려서 실행이 가능한 프로그램 파일
* 
* lib: static library 파일        <-- 쉽게 이야기하면 클래스, 함수, 변수 등등이 들어있는 '바이너리 단위의 재사용' 모듈
* dll: dynamic link library 파일  <-- 쉽게 이야기하면 클래스, 함수, 변수 등등이 들어있는 '바이너리 단위의 재사용' 모듈
* 
* lib <-- 컴파일 타임에 링킹된다. 정적static 링크 라이브러리 파일
* dll <-- 런 타임에 링킹된다. 다이나믹dynamic 링크 라이브러리 파일
*/

class CRyuEngine : public CAPIEngine
{
    //test
    //CObjectRyu* testObject = nullptr;

    //자원resource
    CTexture* mpTexture = nullptr;
    CTexture* mpTexBullet = nullptr; //탄환 비트맵 이미지 데이터 지원

    //원본 객체( 주인공 기체의 원본 객체 )
    CUnit* PFActor = nullptr;
    CUnit* PFBullet = nullptr;  //탄환 원본 객체

    //object
    CActor* mpActor = nullptr;
    vector<CBullet*> mBullets;  //실제 주인공 기체가 사용할 탄환 객체들

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

        //자원 로드 부분
        //todo
        mpTexture = new CTexture();
        mpTexture->LoadTexture(hInst, mhDC, L"resources/bongbong_0.bmp");

        mpTexBullet = new CTexture();
        mpTexBullet->LoadTexture(hInst, mhDC, L"resources/bongbullet.bmp");

        //원본객체(주인공 기체에 대한 원본 객체 ) 생성
        //PFActor = new CActor();
        //PFActor->SetPosition(800 * 0.5f, 600 - 50 - 100.0f);
        //PFActor->SetRadius(50.0f);
        ////pivot
        //PFActor->SetAnchorPoint(0.5f, 0.5f);
        //PFActor->SetTexture(mpTexture);
        PFActor = CreatePrefab<CActor>(mpTexture, 0.5f, 0.5f, SVector2D(800 * 0.5f, 600 - 50 - 100.0f));

        PFBullet = CreatePrefab<CBullet>(mpTexBullet, 0.5f, 0.5f, SVector2D(800 * 0.5f, 600 - 50 - 100.0f));

        //실제 객체 생성
        //mpActor = static_cast<CActor*>(PFActor->clone());   //원본객체를 복제하여 객체를 생성
        //mpActor->AddRef();

        mpActor = InstantObject<CActor>(PFActor);   //원본객체를 복제하여 객체를 생성
        mpActor->AddRef();

        CBullet* tpBullet = nullptr;
        for (int ti = 0; ti < 10; ++ti)
        {
            tpBullet = InstantObject<CBullet>(PFBullet);   //원본객체를 복제하여 객체를 생성
            tpBullet->AddRef();

            mBullets.push_back(tpBullet);
            tpBullet->AddRef();

            tpBullet->Release();
            tpBullet = nullptr;
        }

        //입력 매핑 등록
        CInputMgr::GetInstance()->AddKey("OnMoveLt", 'A');
        CInputMgr::GetInstance()->AddKey("OnMoveRt", 'D');
        CInputMgr::GetInstance()->AddKey("OnFire", VK_SPACE);

        CInputMgr::GetInstance()->AddKey("OnTest", VK_LCONTROL, 'H');
    }

    virtual void OnDestroy() override
    {
        //실제 객체 소멸
        vector<CBullet*>::iterator tItor;
        for (tItor = mBullets.begin(); tItor != mBullets.end(); ++tItor)
        {
            SAFE_RELEASE((*tItor)); //(*tItor)-> 이런 형태로 치환하기 위해 괄호를 서줬다.
        }

        DestroyObject<CActor>(mpActor);

        //원본 객체 소멸
        DestroyPrefab(PFBullet);
        DestroyPrefab(PFActor);
        
        //자원 해제
        //매크로 함수를 사용하여
        //호출하는 측의 코드가 간략화되었다.
        SAFE_DELETE(mpTexBullet)
        SAFE_DELETE(mpTexture)
        
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

        vector<CBullet*>::iterator tItor;
        for (tItor = mBullets.begin(); tItor != mBullets.end(); ++tItor)
        {
            (*tItor)->Render(this);
        }
                
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


