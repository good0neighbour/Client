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

#include "CRyuTimer.h"

#include "CCollider.h"
#include "CCollisionMgr.h"

//test
#include "CAniSeq.h"
#include "CAnimator.h"

//test
#include <list>
using namespace std;

//소스코드상에서 설정
//winAPIEngine.lib를 링킹과정에서 링크하겠다 라고 컴파일 옵션 지정
#pragma comment(lib, "winAPIEngine.lib")

/*
    이번 예시에서는 다음의 사항들을 만들어보자.

    i) 컬러키 적용
        전통적인 2D 게임에서 사용하는 투명 컬러키는 자홍색(255,0,255)이다.
        이 예시에서는 하얀색(255,255,255)을 투명 컬러키로 사용하였다.

        win API에서 제공하는
            'TransparentBlt'함수를 이용하겠다.

    ii) 스프라이트 시트 개념을 고려하여 적용하겠다

    스프라이트 시트를 구성하는 방법은 크게 구가지다.
    가) 각각의 프레임의 위치, 크기가 제각각인 경우
        장점: 스프라이트 시트의 빈 공간이 소모되지 않게 채울 수 있다.
        단점: 임의의 프레임을 가져오기 위해 별도의 정보(위치, 크기(너비, 높이)) 구성 파일이 필요하다

    나) 프레임의 크기를 통일하여 작성하는 경우
        장점: 부가정보가 필요없다.
        단점: 해당 규격을 지켜야만 한다.

        -->우리는 여기서 나)의 방법을 사용하겠다.

    explosionFull.bmp 리소스를 가정한다.
    2048 * 1024의 크기를 가진다.
    4행 8열의 스프라이트 프레임들
    이 하나의 임지 파일로 구성되어 있다.( 4 * 8 = 32 프레임의 구성 )
*/
/*
* sprite animation
* 
* i) 파일 단위 스프라이트 프레임을 가정
* 
* ii) 스프라이트 시트를 가정
* 
* n개의 스프라이트 프레임이 하나의 파일에 담겨있는 이미지 파일---> sprite sheet
*/

class CRyuEngine : public CAPIEngine
{
    //test
    CRyuTimer mTimerForEnemy;

    //test
    //CObjectRyu* testObject = nullptr;

    //자원resource
    CTexture* mpTexture = nullptr;
    CTexture* mpTexBullet = nullptr;    //탄환 비트맵 이미지 데이터 자원
    CTexture* mpTexEnemy = nullptr;     //적 비트맵 데이터 자원

    //prefab
    //원본 객체( 주인공 기체의 원본 객체 )
    CUnit* PFActor = nullptr;
    CUnit* PFBullet = nullptr;          //탄환 원본 객체
    CUnit* PFEnemy = nullptr;           //적 원본 객체

    //object
    CActor* mpActor = nullptr;
    vector<CBullet*> mBullets;      //실제 주인공 기체가 사용할 탄환객체들 

    CEnemy* mpEnemy = nullptr;      //적 객체
    vector<CBullet*> mBulletsEnemy;      //실제 적 기체가 사용할 일반탄환객체들    

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
        //OutputDebugString(L"==========OnTimer_0, Enemy Fire!!!=======\n");

        //정적static함수에서 비정적 멤버변수에 접근이 불가하다.
        //mpEnemy->DoFire(mBulletsEnemy);

        //이런 것은 가능
        //CInputMgr::GetInstance();
    }

    virtual void OnCreate() override
    {
        CAPIEngine::OnCreate();

            //test
        RYU_INT tA = 3;
        RYU_INT tB = 2;
        RYU_INT tResult = 0;

        tResult = tA + tB;
        
        char tszTemp[256] = { 0 };      //c style 문자열
        sprintf_s(tszTemp, "tResult: %d\n", tResult);
        OutputDebugStringA(tszTemp);
        
        //자원 로드 부분
        //todo
        mpTexture = new CTexture();
        mpTexture->LoadTexture(hInst, mhDC, L"resources/bongbong_0.bmp");

        mpTexBullet = new CTexture();
        mpTexBullet->LoadTexture(hInst, mhDC, L"resources/bongbullet.bmp");

        mpTexEnemy = new CTexture();
        //대표이미지 교체
        //mpTexEnemy->LoadTexture(hInst, mhDC, L"resources/bongenemy.bmp");
        mpTexEnemy->LoadTexture(hInst, mhDC, L"resources/paladin_idle_0.bmp");

        PFActor = CreatePrefab<CActor>(mpTexture, 0.5f, 0.5f, SVector2D(800 * 0.5f, 600 - 50 - 100.0f));
        //이제  Animator객체를 추가하는 것을 프리팹 쪽으로 옮겼다.
        CAnimator* tpAnimActor = PFActor->CreateAnimation("AnimActor", this);
        tpAnimActor->AddAniSeq("ani_idle_actor", 1.0f, 2, L"resources/bongbong");
        tpAnimActor->AddAniSeq("ani_super_actor", 0.5f, 2, L"resources/bongbong_super");        //테스트용 애니시퀀스

        //기본 애니메이션 시퀀스 지정
        //tpAnimActor->mStrKeyCurAniSeq = "ani_idle_actor";
        tpAnimActor->SetDefaultAniSeq("ani_idle_actor");

        PFBullet = CreatePrefab<CBullet>(mpTexBullet, 0.5f, 0.5f, SVector2D(800 * 0.5f, 600 - 50 - 100.0f));

        PFEnemy = CreatePrefab<CEnemy>(mpTexEnemy, 0.5f, 0.5f, SVector2D(800 * 0.5f, 100.0f));
        CAnimator* tpAnimEnemy = PFEnemy->CreateAnimation("AnimEnemy", this);
        tpAnimEnemy->AddAniSeq("ani_idle_enemy", 0.03f, 7, L"resources/paladin_idle");
        //tpAnimEnemy->AddAniSeq("ani_attack_enemy", 0.2f, 11, L"resources/paladin_attack",ANI_PO::ONCE);
        tpAnimEnemy->AddAniSeq("ani_attack_enemy", 0.05f, 11, L"resources/paladin_attack", ANI_PO::ONCE);

        //기본 애니메이션 시퀀스 지정
        tpAnimEnemy->SetDefaultAniSeq("ani_idle_enemy");
        




        //실제 객체 생성
        //mpActor = static_cast<CActor*>(PFActor->clone());               //원본객체를 복제하여 객체를 생성
        //mpActor->AddRef();

        mpActor = InstantObject<CActor>(PFActor);               //원본객체를 복제하여 객체를 생성
        mpActor->AddRef();
        
        CBullet* tpBullet = nullptr;
        for (int ti = 0; ti < BULLET_COUNT_MAX; ++ti)
        {
            tpBullet = InstantObject<CBullet>(PFBullet);               //원본객체를 복제하여 객체를 생성
                tpBullet->AddRef();

                tpBullet->SetTag("tagActorBullet");

            tpBullet->SetIsActive(false);           //탄환객체들은 비활성으로 생성

            mBullets.push_back(tpBullet);
            tpBullet->AddRef();

                tpBullet->Release();
                tpBullet = nullptr;
        }

        mpEnemy = InstantObject<CEnemy>(PFEnemy);               //원본객체를 복제하여 객체를 생성
        mpEnemy->AddRef();

        mpEnemy->SetTag("tagEnemy");

        //적 기체의 일반탄환들
        tpBullet = nullptr;
        for (int ti = 0; ti < BULLET_COUNT_MAX; ++ti)
        {
            tpBullet = InstantObject<CBullet>(PFBullet);               //원본객체를 복제하여 객체를 생성
            tpBullet->AddRef();

            tpBullet->SetIsActive(false);           //탄환객체들은 비활성으로 생성

            mBulletsEnemy.push_back(tpBullet);
            tpBullet->AddRef();

            tpBullet->Release();
            tpBullet = nullptr;
        }

        //입력 매핑 등록
        CInputMgr::GetInstance()->AddKey("OnMoveLt", 'A');
        CInputMgr::GetInstance()->AddKey("OnMoveRt", 'D');
        CInputMgr::GetInstance()->AddKey("OnFire", VK_SPACE);        

        CInputMgr::GetInstance()->AddKey("OnTest",  VK_LCONTROL, 'H');
    }
    virtual void OnDestroy() override
    {
        //적기체의 일반탄환 객체들 해제
        for (vector<CBullet*>::iterator tItor = mBulletsEnemy.begin(); tItor != mBulletsEnemy.end(); ++tItor)
        {
            SAFE_RELEASE((*tItor));       //(*tItor)-> 이런 형태로 치환하기 위해 괄호를 써줬다.            
        }

        DestroyObject<CEnemy>(mpEnemy);

        //실제 객체 소멸        
        for (vector<CBullet*>::iterator tItor = mBullets.begin(); tItor != mBullets.end(); ++tItor)
        {
            SAFE_RELEASE( (*tItor) );       //(*tItor)-> 이런 형태로 치환하기 위해 괄호를 써줬다.            
        }

        DestroyObject<CActor>(mpActor);

        //원본 객체 소멸
        DestroyPrefab(PFEnemy);
        DestroyPrefab(PFBullet);
        DestroyPrefab(PFActor);

        //자원 해제 
        SAFE_DELETE(mpTexEnemy)
        SAFE_DELETE(mpTexBullet)
        SAFE_DELETE(mpTexture)

        CAPIEngine::OnDestroy();
    }
    virtual void OnUpdate(float tDeltaTime) override
    {
        CAPIEngine::OnUpdate(tDeltaTime);

        SVector2D tVelocity;
        mpActor->SetVelocity(tVelocity);
       if( CInputMgr::GetInstance()->KeyPress("OnMoveLt") )
        {
            //순수한 방향의 속도 설정 (-1,0)
            tVelocity.mX = -1.0f;
            tVelocity.mY = 0.0f;
            //mpUnit->SetVelocity(tVelocity * 0.1f);
            mpActor->SetVelocity(tVelocity * 350.0f);    //초당 350PIXEL

            OutputDebugString(L"key input A\n");
        }
       if (CInputMgr::GetInstance()->KeyPress("OnMoveRt"))
        {
            //순수한 방향의 속도 설정 (+1,0)
            tVelocity.mX = +1.0f;
            tVelocity.mY = 0.0f;

            //mpUnit->SetVelocity(tVelocity * 0.1f);
            mpActor->SetVelocity(tVelocity * 350.0f);    //초당 350PIXEL

            OutputDebugString(L"key input D\n");
        }

       if (CInputMgr::GetInstance()->KeyUp("OnFire"))
        {          
            mpActor->DoFire(mBullets);
            
            mpActor->PlayAni("ani_super_actor");
        }

       if (CInputMgr::GetInstance()->KeyUp("OnTest"))
       {        
           mpActor->PlayAni("ani_idle_actor");
       }

       //시간기반진행
        mpActor->Update(tDeltaTime);
        for (vector<CBullet*>::iterator tItor = mBullets.begin(); tItor != mBullets.end(); ++tItor)
        {
            (*tItor)->Update(tDeltaTime);
        }

        mpEnemy->Update(tDeltaTime);

        //적 기체가 일반탄환을 일정시간 간격으로 발사

        if ( mpEnemy->mTimeTick >= 2.0f )
        {
            //todo 일정시간 간격으로 실행할 코드
            mpEnemy->DoFire(mBulletsEnemy);
             
            mpEnemy->PlayAni("ani_attack_enemy");

            //time tick을 초기 상태로 되돌려줌
            mpEnemy->mTimeTick = 0.0f;
        }
        else
        {
            //delta time 을 누적
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


