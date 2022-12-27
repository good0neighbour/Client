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
#include "CAnimator.h"

//test
#include <list>
using namespace std;

//소스코드상에서 설정
//winAPIEngine.lib를 링킹과정에서 링크하겠다 라고 컴파일 옵션 지정
#pragma comment(lib, "winAPIEngine.lib")

/*
    이번 예시에서는 다음의 사항들을 만들어보자.

    Animator생성 코드를
    프리팹(원본 객체)쪽으로 옮기고 테스트하자.

    i) mpAnimator가 유효한지 여부에 따라 deep copy를 하였다.
    ii) mpAnimator에 소유자를 지정했다.
    iii) 애니메이션 시뭔스에 텍스쳐에 레퍼런스 카운팅을 적용하였다.
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

    //스프라이트 애니메이션 테스트
    //CTexture* mTexs[2] = { nullptr, nullptr };

    //float mTimeInterval = 0.0f; //스프라이트 프레임 시간 간격

    //int mTotalFrameCount = 0;   //스프라이트 프레임 총 개수

    //int mCurFrameIndex = 0; //현재 스프라이트 프레임 번호
    //float mAniTime = 0.0f;  //애니메이션 진행 시간<-- 델타타임을 누적할 것임

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

    //CEnemy* mpEnemyAimed = nullptr;      //조준 탄환객체을 발사할 적 객체
    //vector<CBullet*> mBulletsEnemyAimed;      //조준 탄환객체들

    //CEnemy* mpEnemyCircled = nullptr;      //원형객체을 발사할 적 객체
    //vector<CBullet*> mBulletsEnemyCircled;      //원형 탄환객체들

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
        //~A 가 붙은 OutputDebugString은 아스키코드용

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

        /*mTexs[0] = new CTexture();
        mTexs[0]->LoadTexture(hInst, mhDC, L"resources/bongbong_0.bmp");
        mTexs[1] = new CTexture();
        mTexs[1]->LoadTexture(hInst, mhDC, L"resources/bongbong_1.bmp");*/

        //원본객체(주인공 기체에 대한 원본 객체 ) 생성
        //PFActor = new CActor();        
        //PFActor->SetPosition(800 * 0.5f, 600 - 50 - 100.0f);
        //    PFActor->SetRadius(50.0f);
        ////pivot
        //PFActor->SetAnchorPoint(0.5f, 0.5f);
        //PFActor->SetTexture(mpTexture);
        PFActor = CreatePrefab<CActor>(mpTexture, 0.5f, 0.5f, SVector2D(800 * 0.5f, 600 - 50 - 100.0f));
        //이제 Animator객체를 추가하는 것을 프리팹 쪽으로 옮겼다.
        CAnimator* tpAnimActor = PFActor->CreateAnimator("AnimActor", this);
        tpAnimActor->AddAniSeq("ani_idle_actor", 1.0f, 2, L"resources/bongbong");

        PFBullet = CreatePrefab<CBullet>(mpTexBullet, 0.5f, 0.5f, SVector2D(800 * 0.5f, 600 - 50 - 100.0f));

        PFEnemy = CreatePrefab<CEnemy>(mpTexEnemy, 0.5f, 0.5f, SVector2D(800 * 0.5f, 100.0f));

        //실제 객체 생성
        //mpActor = static_cast<CActor*>(PFActor->clone());               //원본객체를 복제하여 객체를 생성
        //mpActor->AddRef();

        mpActor = InstantObject<CActor>(PFActor);               //원본객체를 복제하여 객체를 생성
        mpActor->AddRef();

        /*CAnimator* tpAnimActor = mpActor->CreateAnimator("AnimActor", this);
        tpAnimActor->AddAniSeq("ani_idle_actor", 1.0f, 2, L"resources/bongbong");*/

        CBullet* tpBullet = nullptr;
        for (int ti = 0; ti < BULLET_COUNT_MAX; ++ti)
        {
            tpBullet = InstantObject<CBullet>(PFBullet);               //원본객체를 복제하여 객체를 생성
                tpBullet->AddRef();

                //
                //CCollisionMgr::GetInstance()->AddUnit(tpBullet);
                tpBullet->SetTag("tagActorBullet");

            tpBullet->SetIsActive(false);           //탄환객체들은 비활성으로 생성

            mBullets.push_back(tpBullet);
            tpBullet->AddRef();

                tpBullet->Release();
                tpBullet = nullptr;
        }

        mpEnemy = InstantObject<CEnemy>(PFEnemy);               //원본객체를 복제하여 객체를 생성
        mpEnemy->AddRef();

        //
        //CCollisionMgr::GetInstance()->AddUnit(mpEnemy);
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

        //mpEnemyAimed = InstantObject<CEnemy>(PFEnemy);               //원본객체를 복제하여 객체를 생성
        //mpEnemyAimed->AddRef();

        //for (int ti = 0; ti < BULLET_COUNT_MAX; ++ti)
        //{
        //    tpBullet = InstantObject<CBullet>(PFBullet);               //원본객체를 복제하여 객체를 생성
        //    tpBullet->AddRef();

        //    tpBullet->SetIsActive(false);           //탄환객체들은 비활성으로 생성

        //    mBulletsEnemyAimed.push_back(tpBullet);
        //    tpBullet->AddRef();

        //    tpBullet->Release();
        //    tpBullet = nullptr;
        //}

        //mpEnemyCircled = InstantObject<CEnemy>(PFEnemy);               //원본객체를 복제하여 객체를 생성
        //mpEnemyCircled->AddRef();

        ////8발씩 BULLET_COUNT_MAX벌 준비
        //for (int ti = 0; ti < 8 * BULLET_COUNT_MAX; ++ti)
        //{
        //    tpBullet = InstantObject<CBullet>(PFBullet);               //원본객체를 복제하여 객체를 생성
        //    tpBullet->AddRef();

        //    tpBullet->SetIsActive(false);           //탄환객체들은 비활성으로 생성

        //    mBulletsEnemyCircled.push_back(tpBullet);
        //    tpBullet->AddRef();

        //    tpBullet->Release();
        //    tpBullet = nullptr;
        //}

        //입력 매핑 등록
        CInputMgr::GetInstance()->AddKey("OnMoveLt", 'A');
        CInputMgr::GetInstance()->AddKey("OnMoveRt", 'D');
        CInputMgr::GetInstance()->AddKey("OnFire", VK_SPACE);        

        CInputMgr::GetInstance()->AddKey("OnTest",  VK_LCONTROL, 'H');

        //게임 프로그램 시작시, 오른쪽 방향으로 이동 시작
        //mpEnemy->SetVelocity(SVector2D(+1.0f, 0.0f) * 100.0f);

        //mpEnemyCircled->SetVelocity(SVector2D(-1.0f, 0.0f) * 50.0f);
    }

    virtual void OnDestroy() override
    {
        //0번 타이머 해제
        //KillTimer(this->mhWnd, 0);

        /*if (nullptr != mpTexture)
        {
            delete mpTexture;
            mpTexture = nullptr;
        }*/
        //적기체의 조준탄환 객체들 해제
        //for (vector<CBullet*>::iterator tItor = mBulletsEnemyCircled.begin(); tItor != mBulletsEnemyCircled.end(); ++tItor)
        //{
        //    SAFE_RELEASE((*tItor));       //(*tItor)-> 이런 형태로 치환하기 위해 괄호를 써줬다.            
        //}

        //DestroyObject<CEnemy>(mpEnemyCircled);

        ////적기체의 조준탄환 객체들 해제
        //for (vector<CBullet*>::iterator tItor = mBulletsEnemyAimed.begin(); tItor != mBulletsEnemyAimed.end(); ++tItor)
        //{
        //    SAFE_RELEASE((*tItor));       //(*tItor)-> 이런 형태로 치환하기 위해 괄호를 써줬다.            
        //}

        //DestroyObject<CEnemy>(mpEnemyAimed);

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

        /*SAFE_DELETE(mTexs[1]);
        SAFE_DELETE(mTexs[0]);*/

        //자원 해제 
        SAFE_DELETE(mpTexEnemy)
        SAFE_DELETE(mpTexBullet)
        SAFE_DELETE(mpTexture)

        CAPIEngine::OnDestroy();
    }

    virtual void OnUpdate(float tDeltaTime) override
    {
        CAPIEngine::OnUpdate(tDeltaTime);

        //충돌 게임 주인공기체의 일반탄환들 vs 적 기체 1개
        //원 대 원

        /*
        *   D > A.r + B.r   충돌이 아니다
        *
        *   D == A.r + B.r   충돌이다
        *
        *   D < A.r + B.r   충돌이다
        */
        //for (vector<CBullet*>::iterator tItor = mBullets.begin(); tItor != mBullets.end(); ++tItor)
        //{
        //    // 둘 다 활성화 된 상태인 경우
        //    if ((*tItor)->GetIsActive())
        //    {
        //        if (mpEnemy->GetIsActive())
        //        {
        //            //원래는 tDistance 를 구하기 위해 제곱근을 구해야 하지만
        //            //어차피 양수끼리의 비교이므로, 양변을 제곱하여 제곱근을 없앴다.
        //            //( 양수끼리의 비교라면, 제곱해도 양변의 등식은 변하지 않으므로 )

        //            //D 두 중심점 사이의 거리
        //            float tAdd = ((*tItor)->GetRadius() + mpEnemy->GetRadius()) * ((*tItor)->GetRadius() + mpEnemy->GetRadius());
        //            //여기서는 두 점 사이의 거리 구하기 공식으로 하였다.( 내적을 이용한 방법도 있다 )
        //            float tDistance = ((*tItor)->GetPosition().mX - mpEnemy->GetPosition().mX) * ((*tItor)->GetPosition().mX - mpEnemy->GetPosition().mX) + ((*tItor)->GetPosition().mY - mpEnemy->GetPosition().mY) * ((*tItor)->GetPosition().mY - mpEnemy->GetPosition().mY);

        //            if (tDistance <= tAdd)
        //            {
        //                OutputDebugString(L"Collision\n");

        //                //충돌 처리

        //                //탄환 제거
        //                (*tItor)->SetIsActive(false);

        //                //적 제거
        //                mpEnemy->SetIsActive(false);

        //                break;
        //            }
        //        }
        //    }
        //}

        //AABB
        //for (vector<CBullet*>::iterator tItor = mBullets.begin(); tItor != mBullets.end(); ++tItor)
        //{
        //    if ((*tItor)->GetIsActive())
        //    {
        //        if (mpEnemy->GetIsActive())
        //        {
        //            bool tIsCollision = (*tItor)->Intersects((*mpEnemy));
        //            if (tIsCollision)
        //            {
        //                //충돌 처리

        //                //탄환 제거
        //                (*tItor)->SetIsActive(false);

        //                //적 제거
        //                mpEnemy->SetIsActive(false);
        //                break;
        //            }
        //        }
        //    }
        //}

        //for (vector<CBullet*>::iterator tItor = mBullets.begin(); tItor != mBullets.end(); ++tItor)
        //{
        //    if ((*tItor)->GetIsActive())
        //    {
        //        if (mpEnemy->GetIsActive())
        //        {
        //            bool tIsCollision = (*tItor)->mpCollider->DoCollision(mpEnemy->mpCollider);
        //            if (tIsCollision)
        //            {
        //                //충돌 처리

        //                //탄환 제거
        //                (*tItor)->SetIsActive(false);
        //                //적 제거
        //                mpEnemy->SetIsActive(false);

        //                break;
        //            }
        //        }
        //    }
        //}

        //mTimerForEnemy.Update(tDeltaTime);

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
        //    //순수한 방향의 속도 설정 (-1,0)
        //    tVelocity.mX = -1.0f;
        //    tVelocity.mY = 0.0f;            
        //    mpUnit->SetVelocity(tVelocity*0.1f);

        //    OutputDebugString(L"key input A\n");
        //}        
        //if (GetAsyncKeyState('D') & 0x8000) 
        //{
        //    //순수한 방향의 속도 설정 (+1,0)
        //    tVelocity.mX = +1.0f;
        //    tVelocity.mY = 0.0f;
        //    
        //    mpUnit->SetVelocity(tVelocity*0.1f);

        //    OutputDebugString(L"key input D\n");
        //}

        //if (GetAsyncKeyState(VK_SPACE) & 0x8000)
        //{
        //    OutputDebugString(L"Fire Bullet!.........VK_SPACE\n");
        //}

        //mpUnit->Update();

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
            OutputDebugString(L"Fire Bullet!.........VK_SPACE\n");

            mpActor->DoFire(mBullets);
        }

        if (CInputMgr::GetInstance()->KeyUp("OnTest"))
        {
           OutputDebugString(L"OnTest!.........VK_LCONTROL      'H'\n");
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
            //mpEnemy->DoFire(mBulletsEnemy);

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

        //mpEnemyAimed->Update(tDeltaTime);
        ////적 기체가 일반탄환을 일정시간 간격으로 발사
        //if (mpEnemyAimed->mTimeTick >= 1.0f)
        //{
        //    //todo 일정시간 간격으로 실행할 코드
        //    mpEnemyAimed->DoFireAimed(mBulletsEnemyAimed, mpActor);

        //    //time tick을 초기 상태로 되돌려줌
        //    mpEnemyAimed->mTimeTick = 0.0f;
        //}
        //else
        //{
        //    //delta time 을 누적
        //    mpEnemyAimed->mTimeTick = mpEnemyAimed->mTimeTick + tDeltaTime;
        //}

        //for (vector<CBullet*>::iterator tItor = mBulletsEnemyAimed.begin(); tItor != mBulletsEnemyAimed.end(); ++tItor)
        //{
        //    (*tItor)->Update(tDeltaTime);
        //}

        //mpEnemyCircled->Update(tDeltaTime);
        ////적 기체가 일반탄환을 일정시간 간격으로 발사
        //if (mpEnemyCircled->mTimeTick >= 3.0f)
        //{
        //    //todo 일정시간 간격으로 실행할 코드
        //    mpEnemyCircled->DoFireCircled(mBulletsEnemyCircled);

        //    //time tick을 초기 상태로 되돌려줌
        //    mpEnemyCircled->mTimeTick = 0.0f;
        //}
        //else
        //{
        //    //delta time 을 누적
        //    mpEnemyCircled->mTimeTick = mpEnemyCircled->mTimeTick + tDeltaTime;
        //}

        //for (vector<CBullet*>::iterator tItor = mBulletsEnemyCircled.begin(); tItor != mBulletsEnemyCircled.end(); ++tItor)
        //{
        //    (*tItor)->Update(tDeltaTime);
        //}

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

        /*mpEnemyAimed->Render();
        for (vector<CBullet*>::iterator tItor = mBulletsEnemyAimed.begin(); tItor != mBulletsEnemyAimed.end(); ++tItor)
        {
            (*tItor)->Render();
        }

        mpEnemyCircled->Render();
        for (vector<CBullet*>::iterator tItor = mBulletsEnemyCircled.begin(); tItor != mBulletsEnemyCircled.end(); ++tItor)
        {
            (*tItor)->Render();
        }*/

        //test data setting
        //mTimeInterval = 1.0f;
        //mTotalFrameCount = 2;
        ////애니메이션 프레임 진행 루틴
        //if (mAniTime >= mTimeInterval)
        //{
        //    //todo
        //    //스프라이트 프레임의 번호를 하나씩 증가시킨다.
        //    if (mCurFrameIndex < mTotalFrameCount - 1)
        //    {
        //        mCurFrameIndex++;
        //    }
        //    else
        //    {
        //        mCurFrameIndex = 0;
        //    }

        //    mAniTime = 0.0f;
        //}
        //else
        //{
        //    mAniTime = mAniTime + tDeltaTime;
        //}

        //this->DrawTexture(800.0f * 0.5f, 600.0f * 0.5f, mTexs[mCurFrameIndex]);
    
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


