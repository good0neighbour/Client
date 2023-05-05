#include "CCamera.h"

#include "CActor.h"

//camera_ryu
//�θ� �����ϸ� �θ� ���󰣴�. follow camera
void CCamera::UpdateMatView()
{
    mMatView = XMMatrixIdentity();
    XMVECTOR tEye = XMVectorSet(mX, mY, mZ, 1.0f);   //ī�޶��� ��ġ
    XMVECTOR tAt = XMVectorSet(mAtX, mAtY, mAtZ, 1.0f); //�ٶ󺸴� �������� ��ġ
    XMVECTOR tUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //��溤��

    //radian
    float tPitch = 0.0f;
    float tYaw = 0.0f;
    float tRoll = 0.0f;

    tPitch = mPitchX * (3.141592f / 180.0f);
    tYaw = mYawY * (3.141592f / 180.0f);
    tRoll = mRollZ * (3.141592f / 180.0f);

    XMMATRIX tMatRotation = XMMatrixRotationRollPitchYaw(tPitch, tYaw, tRoll);

    
    tEye = XMVector3TransformCoord(tEye, tMatRotation);     //��ġ
    tAt = XMVector3TransformCoord(tAt, tMatRotation);           //��ġ
    tUp = XMVector3Transform(tUp, tMatRotation);                //����
    //ȸ����ȯ��ı��� ������ eye, at, up

    //camera_ryu
    //�θ� �ִٸ�, �θ��� ����(��)��ȯ��ı��� �����Ͽ� ����ϰ�
    //���庯ȯ���
    XMMATRIX tMat = XMMatrixIdentity();
    if (mpParent)
    {
        //�θ��� ���庯ȯ��� ���
        tMat = mpParent->GetMatWorld();

        tEye = XMVector3TransformCoord(tEye, tMat);     //��ġ
        tAt = XMVector3TransformCoord(tAt, tMat);           //��ġ
        //tUp = XMVector3Transform(tUp, tMat);                //����
    }
    //�׷��� �ʴٸ� �ڽŸ� ����Ѵ�



    //ī�޶��� ��ġ, �������� 2d �������� �����ϱ� ���ϰ� ��ġ ����
    mMatView = XMMatrixLookAtLH(tEye, tAt, tUp);   //�亯ȯ ����� �����ش� XMMatrixLookAtLH
}

void CCamera::GetViewMatrix(XMMATRIX& tMatView)
{
	tMatView = mMatView;
}

void CCamera::SetParent(CActor* tpParent)
{
    mpParent = tpParent;
}
CActor* CCamera::GetParent()
{
    return mpParent;
}




