#include "CActor.h"


#include "CMtlTexture.h"
#include "CMesh.h"
#include "CDXEngine.h"
#include "ryubuffer.h"


void CActor::Create(CDXEngine* tpEngine, LPCWSTR tFileNameModel, LPCWSTR tpFileNameTexture)
{
    SetEngine(tpEngine);

    //머티리얼 생성
    mpMtl = new CMtlTexture();
    mpMtl->Create(mpEngine->GetD3DDevice());
    mpMtl->SetEngine(mpEngine);
    //메쉬 생성
    mpMesh = new CMesh();
    /*mpMesh->Create(mpEngine->GetD3DDevice(), L"resources/slimeby.dds");*/
    mpMesh->Create(mpEngine->GetD3DDevice(), tFileNameModel, tpFileNameTexture);
    mpMesh->SetEngine(mpEngine);

    //상수버퍼 생성
    // for transform
    //주설명
    D3D11_BUFFER_DESC tBd = {};
    tBd.Usage = D3D11_USAGE_DEFAULT;    //버퍼는 기본 용도 버퍼로 사용하겠다.
    tBd.ByteWidth = sizeof(CBTransform);    //데이터의 크기
    tBd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;       //constant buffer 용도로 만든다.
    tBd.CPUAccessFlags = 0;     //CPU의 접근은 불허한다
    //부설명 은 필요없다 UpdateSubResources을 이용하여 매 프레임마다 데이터의 값을 갱신한다
    mpEngine->GetD3DDevice()->CreateBuffer(&tBd, nullptr, &mpCBTransform);

    //전방벡터 ( 0,0,1 ) 로 기본값 설정
    mForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
}
void CActor::Destroy()
{
    if (mpMesh)
    {
        mpMesh->Destroy();

        delete mpMesh;
        mpMesh = nullptr;
    }

    if (mpMtl)
    {
        mpMtl->Destroy();

        delete mpMtl;
        mpMtl = nullptr;
    }

    if (mpCBTransform)
    {
        mpCBTransform->Release();
    }
}

void CActor::Update(float tDeltaTime, COORD_OPTION tCoordOption)
{
    //기준 좌표계 설정
    mCoordOption = tCoordOption;


    XMFLOAT3 tOld;
    tOld.x = mX;
    tOld.y = mY;
    tOld.z = mZ;

    mPosition = XMLoadFloat3(&tOld);

    //벡터 연산을 이용하기 위해 XMVECTOR 타입의 변수 mPosition, mVelocity 를 이용
    if (mCoordOption == COORD_OPTION::World)        //world coordinate
    {
        //현재위치 = 이전 위치 + 속도*시간간격
        mPosition = mPosition + mVelocity * tDeltaTime;
    }
    else                                                                      //local coordinate
    {
        //mForwardVector 전방 벡터 (0, 0, 1)

        //전방인지 후방인지 결정
        XMVECTOR b = XMVector4Normalize(mVelocity);
        XMFLOAT4 a;
        
        XMStoreFloat4(&a, b);
        //          a.z는 1 or -1;
        
        //속도의 크기 결정
        XMVECTOR t = XMVector4Length(mVelocity);
        XMFLOAT4 f;
        XMStoreFloat4(&f, t);

        //f.x를 취해서 이것을 크기로 하자( x, y, z, w 모두 같은 값이 들어온다 )

        //현재위치 = 이전 위치 + 속도*시간간격
        //                                      (0, 0, 1)*(-1)*(50)
        mPosition = mPosition + mForwardVector*a.z*f.x * tDeltaTime;
    }

    XMFLOAT3 tNew;
    XMStoreFloat3(&tNew, mPosition);

    mX = tNew.x;
    mY = tNew.y;
    mZ = tNew.z;

    //=====================================================================

    //전방벡터 ( 0,0,1 ) 로 기본값 설정
    mForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    //mX, mY, mZ를 기반으로 월드변환 행렬( <-- 이동 변환 만 수행했다 )을 만든다.
    //XMMATRIX tMatWorld = XMMatrixTranslation(mX, mY, mZ);

    mMatTranslate = XMMatrixTranslation(mX, mY, mZ);
    mMatRotation = XMMatrixRotationRollPitchYaw(mPitchX, mYawY, mRollZ);
    mMatScale = XMMatrixScaling(mScaleX, mScaleY, mScaleZ);

    // w = TRSv 월드변환 행렬 곱셈 순서 ( 열벡터 기준 )
    // directX 응용프로그램에서는 행벡터를 기준으로 한다
    // 그러므로 행렬의 곱셈 순서는 이렇게 정했다.
    mMatWorld = mMatScale * mMatRotation * mMatTranslate;

    //전방벡터에 회전변환을 적요하여 새로운 전방벡터를 계산한다.
    mForwardVector = XMVector3Transform(mForwardVector, mMatRotation);

}
void CActor::Render(XMMATRIX tMatView, XMMATRIX tMatProjection)
{
    ////전방벡터 ( 0,0,1 ) 로 기본값 설정
    //mForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    ////mX, mY, mZ를 기반으로 월드변환 행렬( <-- 이동 변환 만 수행했다 )을 만든다.
    //XMMATRIX tMatWorld = XMMatrixTranslation(mX, mY, mZ);

    //mMatTranslate = XMMatrixTranslation(mX, mY, mZ);
    //mMatRotation = XMMatrixRotationRollPitchYaw(mPitchX, mYawY, mRollZ);
    //mMatScale = XMMatrixScaling(mScaleX, mScaleY, mScaleZ);

    //// w = TRSv 월드변환 행렬 곱셈 순서 ( 열벡터 기준 )
    //// directX 응용프로그램에서는 행벡터를 기준으로 한다
    //// 그러므로 행렬의 곱셈 순서는 이렇게 정했다.
    //tMatWorld = mMatScale * mMatRotation * mMatTranslate;

    ////전방벡터에 회전변환을 적요하여 새로운 전방벡터를 계산한다.
    //mForwardVector = XMVector3Transform(mForwardVector, mMatRotation);


    //=======================================================================================================
    
    mpEngine->GetImmediateDevice()->VSSetConstantBuffers(0, 1, &mpCBTransform);

    CBTransform tCBTransform;
    tCBTransform.mWorld = XMMatrixTranspose(mMatWorld);
    tCBTransform.mView = XMMatrixTranspose(tMatView);
    tCBTransform.mProjection = XMMatrixTranspose(tMatProjection);

    mpEngine->GetImmediateDevice()->UpdateSubresource(mpCBTransform, 0, nullptr, &tCBTransform, 0, 0);

    mpMesh->Render();
    mpMtl->Render(mpMesh->GetCountIndex(), mpMesh->GetTexture());
}


// 1. 임의의 방향의 벡터를 구하고
// 2. 전방벡터와 해당 임의의 벡터사이의 각도를 구하고
// 3. 그 각도로 회전변환행렬을 구해
// 4. 회전하자
void CActor::UpdateLook(XMFLOAT4 tAtPoint)
{
    mForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    //임의의 방향의 벡터 구하기
    //목적지점 - 시작지점
    XMVECTOR tSrc = XMVectorSet(mX, mY, mZ, 1.0f);
    XMVECTOR tTarget = XMVectorSet(tAtPoint.x, tAtPoint.y, tAtPoint.z, tAtPoint.w);

    XMVECTOR tDir = tTarget - tSrc;
    tDir = XMVector4Normalize(tDir);

    //두 벡터 사이에 각도 구하기
    XMVECTOR tAngle = XMVector4AngleBetweenVectors(mForwardVector, tDir);
    XMFLOAT4 tAngleF;
    XMStoreFloat4(&tAngleF, tAngle);

    //회전축 구하기
    XMVECTOR cross = XMVector3Cross(mForwardVector, tDir);
    XMFLOAT4 crossF;
    XMStoreFloat4(&crossF, cross);

                    XMVECTOR length = XMVector3Length(cross);
                    XMFLOAT4 lengthF;
                    XMStoreFloat4(&lengthF, length);
                    if (lengthF.x <= 0.0f)
                    {
                        crossF.x = 0.0f;
                        crossF.y = 1.0f;
                        crossF.z = 0.0f;
                        crossF.w = 0.0f;
                    }

                    XMVECTOR tAxis = XMVectorSet(crossF.x, crossF.y, crossF.z, crossF.w);



    //전방벡터 ( 0,0,1 ) 로 기본값 설정
    //mForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    mMatTranslate = XMMatrixTranslation(mX, mY, mZ);
    //mMatRotation = XMMatrixRotationY(tAngleF.x);                  //<--- 임의의 각도로, 회전축은 y축으로 회전

    mMatRotation = XMMatrixRotationAxis(tAxis, tAngleF.x);       //<--- 임의의 각도로, 회전축은 상황에 따라 +-y축으로 회전
    mMatScale = XMMatrixScaling(mScaleX, mScaleY, mScaleZ);

    mMatWorld = mMatScale * mMatRotation * mMatTranslate;

    mForwardVector = XMVector3Transform(mForwardVector, mMatRotation);
}