#include "CActor.h"

#include "CBullet.h"

#include <windows.h>


CActor::CActor()
{
    mCurIndexBullet = 0;
}
CActor::~CActor()
{

}

CActor::CActor(const CActor& t)
	:CUnit(t)
{
    mCurIndexBullet = t.mCurIndexBullet;
}
void CActor::operator=(const CActor& t)
{
	CUnit::operator=(t);

    mCurIndexBullet = t.mCurIndexBullet;
}

//void CActor::DoFire(vector<CBullet*>& tBullets)
//{
//    //발사 루틴
//        //		탄환 발사
//        //  i) 탄환의 '발사시작지점'을 설정한다
//        //  ii) 탄환의 '속도'를 설정한다
//        //  iii) 탄환을 '활성화'시킨다
//
//    tBullets[0]->SetPosition(this->GetPosition());  //발사시작지점은 주인공기체의 위치
//    tBullets[0]->SetVelocity(SVector2D(0.0f, -1.0f) * 200.0f);      //벡터의 스칼라곱
//    tBullets[0]->SetIsActive(true);
//}
void CActor::DoFire(vector<CBullet*>& tBullets)
{
    //임의의 인덱스 관찰을 위해 로그 찍기
    WCHAR tszTemp[256] = { 0 };
    wsprintf(tszTemp, L"DoFire %d\n", mCurIndexBullet);
    OutputDebugString(tszTemp);

    //발사 루틴
        //		탄환 발사
        //  i) 탄환의 '발사시작지점'을 설정한다
        //  ii) 탄환의 '속도'를 설정한다
        //  iii) 탄환을 '활성화'시킨다

    tBullets[mCurIndexBullet]->SetPosition(this->GetPosition());  //발사시작지점은 주인공기체의 위치
    tBullets[mCurIndexBullet]->SetVelocity(SVector2D(0.0f, -1.0f) * 200.0f);      //벡터의 스칼라곱
    tBullets[mCurIndexBullet]->SetIsActive(true);

    // 아래는 탄환이라는 객체의 재활용이라고 할 수 있다. 
    // 이렇게 미리 만들어두고 재활용하는 개념을 object pooling 이라고 한다( object pool )
    //구현하는 방법은 여러가지인데 
    //여기서는 간단히 가변배열을 이용하였다

    //메모리 단편화: 전체 비어있는 메모리 용량은 충분한데 메모리가 단편화되어 
    //                      막상 실제로 필요한 연속적인 메모리 공간이 부족한 상태
    //  <-- 동적할당 연산자의 범용성 때문에 일어나는 현상이다.
    //          그러므로, 해결하려면 범용성을 포기한다


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

//CUnit::Update를 override할 것이다.
//위치값 갱신이 되어야 하므로 CUnit::Update도 동작이 되어야 한다.
void CActor::Update(float tDeltaTime)
{
    CUnit::Update(tDeltaTime);
    //부모클래스의 기능은 그대로 가져와 사용한다

    //여기에는 추가된 기능을 구현하자
    // step_0
    //  중심점만 고려
    //좌측 경계 처리
    //if (this->mPosition.mX <= 0.0f )
    //{
    //    this->mPosition.mX = 0.0f;
    //}
    ////우측 경계 처리
    //if (this->mPosition.mX >= 800.0f)
    //{
    //    this->mPosition.mX = 800.0f;
    //}

    // step_1
    //너비도 고려
    ////좌측 경계 처리
    //if (this->mPosition.mX - GetWidth()*GetAnchorX()  <= 0.0f )
    //{
    //    this->mPosition.mX = 0.0f + GetWidth() * GetAnchorX();
    //}
    ////우측 경계 처리
    //if (this->mPosition.mX + GetWidth() * GetAnchorX() >= 800.0f)
    //{
    //    this->mPosition.mX = 800.0f - GetWidth() * GetAnchorX();
    //}

    float tDiff = GetWidth() * GetAnchorX();
    //좌측 경계 처리
    if (this->mPosition.mX - tDiff <= 0.0f)
    {
        this->mPosition.mX = 0.0f + tDiff;
    }
    //우측 경계 처리
    if (this->mPosition.mX + tDiff >= 800.0f)
    {
        this->mPosition.mX = 800.0f - tDiff;
    }

}