#include "CCamera.h"

#include "CActor.h"

//camera_ryu
//부모를 설정하면 부모를 따라간다. follow camera
void CCamera::UpdateMatView()
{
    mMatView = XMMatrixIdentity();
    XMVECTOR tEye = XMVectorSet(mX, mY, mZ, 1.0f);   //카메라의 위치
    XMVECTOR tAt = XMVectorSet(mAtX, mAtY, mAtZ, 1.0f); //바라보는 응시점의 위치
    XMVECTOR tUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //상방벡터

    //radian
    float tPitch = 0.0f;
    float tYaw = 0.0f;
    float tRoll = 0.0f;

    tPitch = mPitchX * (3.141592f / 180.0f);
    tYaw = mYawY * (3.141592f / 180.0f);
    tRoll = mRollZ * (3.141592f / 180.0f);

    XMMATRIX tMatRotation = XMMatrixRotationRollPitchYaw(tPitch, tYaw, tRoll);

    
    tEye = XMVector3TransformCoord(tEye, tMatRotation);     //위치
    tAt = XMVector3TransformCoord(tAt, tMatRotation);           //위치
    tUp = XMVector3Transform(tUp, tMatRotation);                //벡터
    //회전변환행렬까지 적욛된 eye, at, up

    //camera_ryu
    //부모가 있다면, 부모의 월드(모델)변환행렬까지 누적하여 계산하고
    //월드변환행렬
    XMMATRIX tMat = XMMatrixIdentity();
    if (mpParent)
    {
        //부모의 월드변환행렬 얻기
        tMat = mpParent->GetMatWorld();

        tEye = XMVector3TransformCoord(tEye, tMat);     //위치
        tAt = XMVector3TransformCoord(tAt, tMat);           //위치
        //tUp = XMVector3Transform(tUp, tMat);                //벡터
    }
    //그렇지 않다면 자신만 고려한다



    //카메라의 위치, 응시점을 2d 랜더링을 관찰하기 편리하게 수치 조정
    mMatView = XMMatrixLookAtLH(tEye, tAt, tUp);   //뷰변환 행렬을 구해준다 XMMatrixLookAtLH
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




