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

void CEnemy::DoFireAimed(vector<CBullet*>& tBullets, CUnit* tpUnit)
{
    //�߻� ��ƾ
        //		źȯ �߻�
        //  i) źȯ�� '�߻��������'�� �����Ѵ�
        //  ii) źȯ�� '�ӵ�'�� �����Ѵ�
        //  iii) źȯ�� 'Ȱ��ȭ'��Ų��

    tBullets[mCurIndexBullet]->SetPosition(this->GetPosition());  //�߻���������� ���ΰ���ü�� ��ġ
    

    //����źȯ�� �ӵ��� ������

    //������ ������ ������ ũ���� ���� = �������� - ��������
    SVector2D tStartP = this->GetPosition();
    SVector2D tTargetP = tpUnit->GetPosition();

    SVector2D tVector = tTargetP - tStartP;

    SVector2D tUnitVector = tVector.Normalize();        //����ȭ

    float tScalarSpeed = 200.0f;
    SVector2D tVelocity = tUnitVector * tScalarSpeed;       //������ ��Į���
    //�ӵ� ����

    tBullets[mCurIndexBullet]->SetVelocity(tVelocity);      

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

void CEnemy::DoFireCircled(vector<CBullet*>& tBullets)
{
    //�߻� ��ƾ
        //		źȯ �߻�
        //  i) źȯ�� '�߻��������'�� �����Ѵ�
        //  ii) źȯ�� '�ӵ�'�� �����Ѵ�
        //  iii) źȯ�� 'Ȱ��ȭ'��Ų��

    //8�� ���� �߻�
    for (int ti = mCurIndexBullet; ti < mCurIndexBullet + 8; ++ti)
    {
        tBullets[ti]->SetPosition(this->GetPosition());  //�߻���������� ���ΰ���ü�� ��ġ


        //�ӵ� ������ ������ ��������
        float tAngleDegree = 45.0f * ti;        //45�� ����
        //���������̹Ƿ� r�� 1.0f
        float tDirX = 1.0f*cosf(tAngleDegree*(3.14159f/180.0f));    //degree --> radian
        float tDirY = 1.0f*sinf(tAngleDegree * (3.14159f / 180.0f));    //degree --> radian

        tBullets[ti]->SetVelocity(SVector2D(tDirX, tDirY)*200.0f);     

        tBullets[ti]->SetIsActive(true);
    }


    if (mCurIndexBullet < tBullets.size() - 8 )//1)
    {
        //źȯ ���� ����
        mCurIndexBullet +=8;
    }
    else
    {
        mCurIndexBullet = 0;
    }
}

void CEnemy::OnEnterCollision(CCollider* tOther)
{
    //�±� ��� ����
    if ("tagEnemy" == this->GetTag() && "tagActorBullet" == tOther->GetOwnerObject()->GetTag())
    {
        OutputDebugString(L"CEnemy::OnEnterCollision\n");

        //�浹ó��

        //�� ��Ȱ��ȭ
        this->SetIsActive(false);

        //��� �� ��Ȱ��ȭ( ���⼭�� ���ΰ���ü�� �Ϲ�źȯ )
        tOther->GetOwnerObject()->SetIsActive(false);

        //�ش� ��ġ��, ���� ����Ʈ �ִϸ��̼� Ȱ��ȭ
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