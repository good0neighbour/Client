#include "CEnemy.h"

#include "CBullet.h"

#include <windows.h>

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