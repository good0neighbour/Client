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
//    //�߻� ��ƾ
//        //		źȯ �߻�
//        //  i) źȯ�� '�߻��������'�� �����Ѵ�
//        //  ii) źȯ�� '�ӵ�'�� �����Ѵ�
//        //  iii) źȯ�� 'Ȱ��ȭ'��Ų��
//
//    tBullets[0]->SetPosition(this->GetPosition());  //�߻���������� ���ΰ���ü�� ��ġ
//    tBullets[0]->SetVelocity(SVector2D(0.0f, -1.0f) * 200.0f);      //������ ��Į���
//    tBullets[0]->SetIsActive(true);
//}
void CActor::DoFire(vector<CBullet*>& tBullets)
{
    //������ �ε��� ������ ���� �α� ���
    WCHAR tszTemp[256] = { 0 };
    wsprintf(tszTemp, L"DoFire %d\n", mCurIndexBullet);
    OutputDebugString(tszTemp);

    //�߻� ��ƾ
        //		źȯ �߻�
        //  i) źȯ�� '�߻��������'�� �����Ѵ�
        //  ii) źȯ�� '�ӵ�'�� �����Ѵ�
        //  iii) źȯ�� 'Ȱ��ȭ'��Ų��

    tBullets[mCurIndexBullet]->SetPosition(this->GetPosition());  //�߻���������� ���ΰ���ü�� ��ġ
    tBullets[mCurIndexBullet]->SetVelocity(SVector2D(0.0f, -1.0f) * 200.0f);      //������ ��Į���
    tBullets[mCurIndexBullet]->SetIsActive(true);

    // �Ʒ��� źȯ�̶�� ��ü�� ��Ȱ���̶�� �� �� �ִ�. 
    // �̷��� �̸� �����ΰ� ��Ȱ���ϴ� ������ object pooling �̶�� �Ѵ�( object pool )
    //�����ϴ� ����� ���������ε� 
    //���⼭�� ������ �����迭�� �̿��Ͽ���

    //�޸� ����ȭ: ��ü ����ִ� �޸� �뷮�� ����ѵ� �޸𸮰� ����ȭ�Ǿ� 
    //                      ���� ������ �ʿ��� �������� �޸� ������ ������ ����
    //  <-- �����Ҵ� �������� ���뼺 ������ �Ͼ�� �����̴�.
    //          �׷��Ƿ�, �ذ��Ϸ��� ���뼺�� �����Ѵ�


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

//CUnit::Update�� override�� ���̴�.
//��ġ�� ������ �Ǿ�� �ϹǷ� CUnit::Update�� ������ �Ǿ�� �Ѵ�.
void CActor::Update(float tDeltaTime)
{
    CUnit::Update(tDeltaTime);
    //�θ�Ŭ������ ����� �״�� ������ ����Ѵ�

    //���⿡�� �߰��� ����� ��������
    // step_0
    //  �߽����� ���
    //���� ��� ó��
    //if (this->mPosition.mX <= 0.0f )
    //{
    //    this->mPosition.mX = 0.0f;
    //}
    ////���� ��� ó��
    //if (this->mPosition.mX >= 800.0f)
    //{
    //    this->mPosition.mX = 800.0f;
    //}

    // step_1
    //�ʺ� ���
    ////���� ��� ó��
    //if (this->mPosition.mX - GetWidth()*GetAnchorX()  <= 0.0f )
    //{
    //    this->mPosition.mX = 0.0f + GetWidth() * GetAnchorX();
    //}
    ////���� ��� ó��
    //if (this->mPosition.mX + GetWidth() * GetAnchorX() >= 800.0f)
    //{
    //    this->mPosition.mX = 800.0f - GetWidth() * GetAnchorX();
    //}

    float tDiff = GetWidth() * GetAnchorX();
    //���� ��� ó��
    if (this->mPosition.mX - tDiff <= 0.0f)
    {
        this->mPosition.mX = 0.0f + tDiff;
    }
    //���� ��� ó��
    if (this->mPosition.mX + tDiff >= 800.0f)
    {
        this->mPosition.mX = 800.0f - tDiff;
    }

}