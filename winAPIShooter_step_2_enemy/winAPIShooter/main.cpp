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
#include "CEnemy.h"

#include "config.h"

//test
#include <list>
using namespace std;

//소스코드상에서 설정
//winAPIEngine.lib를 링킹과정에서 링크하겠다 라고 컴파일 옵션 지정
#pragma comment(lib, "winAPIEngine.lib")

/*
* 이번 예시에서는 다음의 사항들을 만들어보자.
* 
* i) 적 기체 생성
* 
*   CEnemy클래스
* 
* ii) 적 기체 일반탄환 발사
* 
*       일반탄환 한발
* 
*       연사 <--'타이머' 기능이 필요하다.
* 
*           가) window api 에서 제공하는 타이머를 간단히 살펴보자
*               다음과 같은 문제점 발견
*               <-- 정밀도가 낮다
*               <-- 게임 프로그램 구조를 객체지향으로 처리하는데 문제가 있다.
* 
*           우리는 이미 시간측정을 좀더 정밀하게 하고 있다.
*           나) deltatime 개념을 이용하여 타이머를 직접 제작하도록 하자.
* 
*               deltaTime을 누적하여
                의도하는 일정 시간 간격이 되었는지 판단하는 형태로
*               타이머를 작성하겠다.
* 
* iii) 적 기체 좌우 이동, 좌우 경계 처리
*/

class CRyuEngine : public CAPIEngine
{
    //test
    //CObjectRyu* testObject = nullptr;

    //자원resource
    CTexture* mpTexture = nullptr;
    CTexture* mpTexBullet = nullptr; //탄환 비트맵 이미지 데이터 지원
    CTexture* mpTexEnemy = nullptr; //적 비트맵 데이터 지원

    //prefab
    //원본 객체( 주인공 기체의 원본 객체 )
    CUnit* PFActor = nullptr;
    CUnit* PFBullet = nullptr;  //탄환 원본 객체
    CUnit* PFEnemy = nullptr;  //적 원본 객체

    //object
    CActor* mpActor = nullptr;
    vector<CBullet*> mBullets;  //실제 주인공 기체가 사용할 탄환 객체들

    CEnemy* mpEnemy = nullptr;  //적 기체
    vector<CBullet*> mBulletsEnemy;  //실제 적 기체가 사용할 탄환 객체들

public:
    CRyuEngine() {};
    virtual ~CRyuEngine() {};

    //delete는 해당 함수의 구현을 막는 문법
private:
    CRyuEngine(const CRyuEngine& t) = delete;
    CRyuEngine& operator=(const CRyuEngine& t) = delete;

public:
    static VOID CALLBACK OnTimer_0(HWND thWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
    {
        OutputDebugString(L"=========OnTimer_0, Enemy Fire=======\n");

        //정적staic함수에서 비정적 멤버변수에 접근이 불가능하다.
        //mpEnemy->DoFire(mBulletsEnemy);

        //이런 것은 가능
        //CInputMgr::GetInstance();
    }

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

        mpTexEnemy = new CTexture();
        mpTexEnemy->LoadTexture(hInst, mhDC, L"resources/bongenemy.bmp");

        //원본객체(주인공 기체에 대한 원본 객체 ) 생성
        //PFActor = new CActor();
        //PFActor->SetPosition(800 * 0.5f, 600 - 50 - 100.0f);
        //PFActor->SetRadius(50.0f);
        ////pivot
        //PFActor->SetAnchorPoint(0.5f, 0.5f);
        //PFActor->SetTexture(mpTexture);
        PFActor = CreatePrefab<CActor>(mpTexture, 0.5f, 0.5f, SVector2D(800 * 0.5f, 600 - 50 - 100.0f));

        PFBullet = CreatePrefab<CBullet>(mpTexBullet, 0.5f, 0.5f, SVector2D(800 * 0.5f, 600 - 50 - 100.0f));

        PFEnemy = CreatePrefab<CEnemy>(mpTexEnemy, 0.5f, 0.5f, SVector2D(800 * 0.5f, 100.0f));

        //실제 객체 생성
        //mpActor = static_cast<CActor*>(PFActor->clone());   //원본객체를 복제하여 객체를 생성
        //mpActor->AddRef();

        mpActor = InstantObject<CActor>(PFActor);   //원본객체를 복제하여 객체를 생성
        mpActor->AddRef();

        CBullet* tpBullet = nullptr;
        for (int ti = 0; ti < BULLET_COUNT_MAX; ++ti)
        {
            tpBullet = InstantObject<CBullet>(PFBullet);   //원본객체를 복제하여 객체를 생성
            tpBullet->AddRef();

            tpBullet->SetIsActive(false);   //탄환 객체들은 비활성으로 생성

            mBullets.push_back(tpBullet);
            tpBullet->AddRef();

            tpBullet->Release();
            tpBullet = nullptr;
        }

        mpEnemy = InstantObject<CEnemy>(PFEnemy);   //원본객체를 복제하여 객체를 생성
        mpEnemy->AddRef();

        //적 기체의 일반탄환들
        tpBullet = nullptr;
        for (int ti = 0; ti < BULLET_COUNT_MAX; ++ti)
        {
            tpBullet = InstantObject<CBullet>(PFBullet);   //원본객체를 복제하여 객체를 생성
            tpBullet->AddRef();

            tpBullet->SetIsActive(false);   //탄환 객체들은 비활성으로 생성

            mBulletsEnemy.push_back(tpBullet);
            tpBullet->AddRef();

            tpBullet->Release();
            tpBullet = nullptr;
        }

        //입력 매핑 등록
        CInputMgr::GetInstance()->AddKey("OnMoveLt", 'A');
        CInputMgr::GetInstance()->AddKey("OnMoveRt", 'D');
        CInputMgr::GetInstance()->AddKey("OnFire", VK_SPACE);

        CInputMgr::GetInstance()->AddKey("OnTest", VK_LCONTROL, 'H');

        //test
        //테스트로 한발만 발사해본다.
        //mpEnemy->DoFire(mBulletsEnemy);

        //3초 간격으로 작동하는 윈도우 타이머를 하나 만들고 설정했다.
        //3초를 가정한다
        // 마지막 인자를 널로 하면, 윈도우 메시지를 이용하여 처리하겠다의 의미
        //SetTimer(this->mhWnd, 0, 3000, nullptr);
        
        SetTimer(this->mhWnd, 0, 3000, OnTimer_0);
    }

    virtual void OnDestroy() override
    {
        //0번 타이머 해제
        KillTimer(this->mhWnd, 0);

        //적기체의 일반탄환 객체들 해제
        for (vector<CBullet*>::iterator tItor = mBulletsEnemy.begin(); tItor != mBulletsEnemy.end(); ++tItor)
        {
            SAFE_RELEASE((*tItor)); //(*tItor)-> 이런 형태로 치환하기 위해 괄호를 서줬다.
        }

        DestroyObject<CEnemy>(mpEnemy);

        //실제 객체 소멸
        for (vector<CBullet*>::iterator tItor = mBullets.begin(); tItor != mBullets.end(); ++tItor)
        {
            SAFE_RELEASE((*tItor)); //(*tItor)-> 이런 형태로 치환하기 위해 괄호를 서줬다.
        }

        DestroyObject<CActor>(mpActor);

        //원본 객체 소멸
        DestroyPrefab(PFEnemy);
        DestroyPrefab(PFBullet);
        DestroyPrefab(PFActor);
        
        //자원 해제
        //매크로 함수를 사용하여
        //호출하는 측의 코드가 간략화되었다.
        SAFE_DELETE(mpTexEnemy)
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

            mpActor->DoFire(mBullets);
        }

        if (CInputMgr::GetInstance()->KeyUp("OnTest"))
        {
            OutputDebugString(L"Fire Bullet............VK_LCONTROL  'H'\n");
        }
        
        //시간기반진행
        mpActor->Update(tDeltaTime);
        for (vector<CBullet*>::iterator tItor = mBullets.begin(); tItor != mBullets.end(); ++tItor)
        {
            (*tItor)->Update(tDeltaTime);
        }

        mpEnemy->Update(tDeltaTime);

        //적 기체가 일반탄환을 일정 시간 간격으로 발사

        if (mpEnemy->mTimeTick >= 2.0f)
        {
            //todo 일정시간 간격으로 실행할 코드
            mpEnemy->DoFire(mBulletsEnemy);

            //time tick을 초기 상태로 되돌려줌
            mpEnemy->mTimeTick = 0.0f;
        }
        else
        {
            //delta time을 누적
            mpEnemy->mTimeTick = mpEnemy->mTimeTick + tDeltaTime;
        }

        for (vector<CBullet*>::iterator tItor = mBulletsEnemy.begin(); tItor != mBulletsEnemy.end(); ++tItor)
        {
            (*tItor)->Update(tDeltaTime);
        }

        //render
        this->Clear(0.1f, 0.1f, 0.3f);        
        
        mpActor->Render();

        for (vector<CBullet*>::iterator tItor = mBullets.begin(); tItor != mBullets.end(); ++tItor)
        {
            (*tItor)->Render();
        }
        mpEnemy->Render();
        for (vector<CBullet*>::iterator tItor = mBulletsEnemy.begin(); tItor != mBulletsEnemy.end(); ++tItor)
        {
            (*tItor)->Render();
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


