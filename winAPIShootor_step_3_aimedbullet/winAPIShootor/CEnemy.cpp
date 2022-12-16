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
    //������ �ε��� ������ ���� �α� ���
    WCHAR tszTemp[256] = { 0 };
    wsprintf(tszTemp, L"DoFire Enemy %d\n", mCurIndexBullet);
    OutputDebugString(tszTemp);

    //�߻� ��ƾ
        //		źȯ �߻�
        //  i) źȯ�� '�߻��������'�� �����Ѵ�
        //  ii) źȯ�� '�ӵ�'�� �����Ѵ�
        //  iii) źȯ�� 'Ȱ��ȭ'��Ų��

    tBullets[mCurIndexBullet]->SetPosition(this->GetPosition());  //�߻���������� ���ΰ���ü�� ��ġ
    tBullets[mCurIndexBullet]->SetVelocity(SVector2D(0.0f, +1.0f) * 200.0f);      //������ ��Į���
    tBullets[mCurIndexBullet]->SetIsActive(true);

    
    if (mCurIndexBullet < tBullets.size() - 1)
    {
        //źȯ ���� ����
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

    //�߰��� ��� �ڵ�
    float tDiff = GetWidth() * GetAnchorX();
    //���� ��� ó��
    if (this->mPosition.mX - tDiff <= 0.0f)
    {
        this->SetVelocity(SVector2D(+1.0f, 0.0f) * 100.0f);
    }
    //���� ��� ó��
    if (this->mPosition.mX + tDiff >= 800.0f)
    {
        this->SetVelocity(SVector2D(-1.0f, 0.0f) * 100.0f);
    }

}