#include "CEnemy.h"

#include "CBullet.h"

#include <windows.h>

#include "CEfxMgr.h"
#include "CExplosion.h"

CEnemy::CEnemy()
{
    mCurIndexBullet = 0;
    mTimeTick = 0.0f;
}
CEnemy::~CEnemy()
{

}

CEnemy::CEnemy(const CEnemy& t)
    :CUnit(t)
{
    mCurIndexBullet = t.mCurIndexBullet;
    mTimeTick = t.mTimeTick;
}
void CEnemy::operator=(const CEnemy& t)
{
    CUnit::operator=(t);

    mCurIndexBullet = t.mCurIndexBullet;
    mTimeTick = t.mTimeTick;
}

void CEnemy::DoFire(vector<CBullet*>& tBullets)
{
    //임의의 인덱스 관찰을 위해 로그 찍기
    WCHAR tszTemp[256] = { 0 };
    wsprintf(tszTemp, L"DoFire Enemy %d\n", mCurIndexBullet);
    OutputDebugString(tszTemp);

    //발사 루틴
        //		탄환 발사
        //  i) 탄환의 '발사시작지점'을 설정한다
        //  ii) 탄환의 '속도'를 설정한다
        //  iii) 탄환을 '활성화'시킨다

    tBullets[mCurIndexBullet]->SetPosition(this->GetPosition());  //발사시작지점은 주인공기체의 위치
    tBullets[mCurIndexBullet]->SetVelocity(SVector2D(0.0f, +1.0f) * 200.0f);      //벡터의 스칼라곱
    tBullets[mCurIndexBullet]->SetIsActive(true);

    
    if (mCurIndexBullet < tBullets.size() - 1)
    {
        //탄환 순번 증가
        ++mCurIndexBullet;
    }
    else
    {
        mCurIndexBullet = 0;
    }
}

void CEnemy::DoFireAimed(vector<CBullet*>& tBullets, CUnit* tpUnit)
{
    //발사 루틴
        //		탄환 발사
        //  i) 탄환의 '발사시작지점'을 설정한다
        //  ii) 탄환의 '속도'를 설정한다
        //  iii) 탄환을 '활성화'시킨다

    tBullets[mCurIndexBullet]->SetPosition(this->GetPosition());  //발사시작지점은 주인공기체의 위치
    

    //조준탄환의 속도를 구하자

    //임의의 방향의 임의의 크기의 벡터 = 목적지점 - 시작지점
    SVector2D tStartP = this->GetPosition();
    SVector2D tTargetP = tpUnit->GetPosition();

    SVector2D tVector = tTargetP - tStartP;

    SVector2D tUnitVector = tVector.Normalize();        //정규화

    float tScalarSpeed = 200.0f;
    SVector2D tVelocity = tUnitVector * tScalarSpeed;       //벡터의 스칼라곱
    //속도 구함

    tBullets[mCurIndexBullet]->SetVelocity(tVelocity);      

    tBullets[mCurIndexBullet]->SetIsActive(true);


    if (mCurIndexBullet < tBullets.size() - 1)
    {
        //탄환 순번 증가
        ++mCurIndexBullet;
    }
    else
    {
        mCurIndexBullet = 0;
    }
}

void CEnemy::DoFireCircled(vector<CBullet*>& tBullets)
{
    //발사 루틴
        //		탄환 발사
        //  i) 탄환의 '발사시작지점'을 설정한다
        //  ii) 탄환의 '속도'를 설정한다
        //  iii) 탄환을 '활성화'시킨다

    //8발 동시 발사
    for (int ti = mCurIndexBullet; ti < mCurIndexBullet + 8; ++ti)
    {
        tBullets[ti]->SetPosition(this->GetPosition());  //발사시작지점은 주인공기체의 위치


        //속도 벡터의 각각의 구성성분
        float tAngleDegree = 45.0f * ti;        //45도 간격
        //단위벡터이므로 r은 1.0f
        float tDirX = 1.0f*cosf(tAngleDegree*(3.14159f/180.0f));    //degree --> radian
        float tDirY = 1.0f*sinf(tAngleDegree * (3.14159f / 180.0f));    //degree --> radian

        tBullets[ti]->SetVelocity(SVector2D(tDirX, tDirY)*200.0f);     

        tBullets[ti]->SetIsActive(true);
    }


    if (mCurIndexBullet < tBullets.size() - 8 )//1)
    {
        //탄환 순번 증가
        mCurIndexBullet +=8;
    }
    else
    {
        mCurIndexBullet = 0;
    }
}

void CEnemy::OnEnterCollision(CCollider* tOther)
{
    //태그 기능 적용
    if ("tagEnemy" == this->GetTag() && "tagActorBullet" == tOther->GetOwnerObject()->GetTag())
    {
        OutputDebugString(L"CEnemy::OnEnterCollision\n");

        //충돌처리

        //적 비활성화
        this->SetIsActive(false);

        //상대 도 비활성화( 여기서는 주인공기체의 일반탄환 )
        tOther->GetOwnerObject()->SetIsActive(false);

        //해당 위치에, 폭발 이펙트 애니메이션 활성화
        CEfxMgr::GetInstance()->mpExplosion->SetPosition(this->GetPosition());
        CEfxMgr::GetInstance()->mpExplosion->SetIsActive(true);
    }
};

void CEnemy::OnTimer()
{
    OutputDebugString(L"OnTimer Enemy Fire\n");
}


void CEnemy::Update(float tDeltaTime)
{
    CUnit::Update(tDeltaTime);

    //추가된 기능 코딩
    float tDiff = GetWidth() * GetAnchorX();
    //좌측 경계 처리
    if (this->mPosition.mX - tDiff <= 0.0f)
    {
        this->SetVelocity(SVector2D(+1.0f, 0.0f) * 100.0f);
    }
    //우측 경계 처리
    if (this->mPosition.mX + tDiff >= 800.0f)
    {
        this->SetVelocity(SVector2D(-1.0f, 0.0f) * 100.0f);
    }

}