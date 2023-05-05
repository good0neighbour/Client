#include "CActor.h"

#include "CMesh.h"
#include "CMtlTexture.h"

#include "CDxEngine.h"
#include "CD3D.h"

#include "ryubuffer.h"


void CActor::Create(CDxEngine* tpEngine, LPCWSTR tFileNameModel, LPCWSTR tFileNameTexture)
{
    SetEngine(tpEngine);

    //머티리얼 
    mpMtlTexture = new CMtlTexture();
    mpMtlTexture->Create(mpEngine->mpD3D->GetD3DDevice());
    mpMtlTexture->SetEngine(mpEngine);

    //메쉬
    mpMesh = new CMesh();
    mpMesh->Create(mpEngine->mpD3D->GetD3DDevice(), tFileNameModel, tFileNameTexture);
    mpMesh->SetEngine(mpEngine);

    //상수버퍼(변환)
    D3D11_BUFFER_DESC tBd = {};
    tBd.Usage = D3D11_USAGE_DEFAULT;            //기본용도다 
    tBd.ByteWidth = sizeof(CBTransform) * 1;    //해당 상수버퍼의 메모리 크기를 알린다
    tBd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//constant buffer용도다
    tBd.CPUAccessFlags = 0;     //CPU의 접근은 불허한다

    mpEngine->mpD3D->GetD3DDevice()->CreateBuffer(&tBd, nullptr, &mpCBTransform);

    //전방벡터 (0,0,1)의 기본값 설정
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

    if (mpMtlTexture)
    {
        mpMtlTexture->Destroy();

        delete mpMtlTexture;
        mpMtlTexture = nullptr;
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

    //이전 프레임에서 결정된 위치 정보를 담아둔다.
    XMFLOAT3 tOld;
    tOld.x = mX;
    tOld.y = mY;
    tOld.z = mZ;
    mPosition = XMLoadFloat3(&tOld);

    //월드좌표계가 기준이라면
    if ( COORD_OPTION::World == mCoordOption )
    {
        //현재위치 = 이전위치 + 속도*시간간격
        mPosition = mPosition + mVelocity * tDeltaTime;
    }
    else //로컬좌표계 기준이라면 
    {
        //방향을 결정( 전방인지 후방인지 결정 )
        XMVECTOR b = XMVector4Normalize(mVelocity);
        XMFLOAT4 a;
        XMStoreFloat4(&a, b);
        //a.z는 +1 or -1

        //속도의 크기(속력) 결정
        XMVECTOR t = XMVector4Length(mVelocity);
        XMFLOAT4 f;
        XMStoreFloat4(&f, t);
        //x,y,z,w 모두 같은 값이 들어온다.
        //<-- f.x를 취하겠다 


        //여기까지 오면 mVelocity의 방향과 크기를 구할 수 있다. 
        //<-- 이것을 mForwardVector와 스칼라곱셈하여 
        //          우리가 원하는 속도를 구하자.

        //현재위치 = 이전위치 + 속도*시간간격
        mPosition = mPosition + mForwardVector* a.z*f.x * tDeltaTime;
    }

    XMFLOAT3 tNew;
    XMStoreFloat3(&tNew, mPosition);

    mX = tNew.x;
    mY = tNew.y;
    mZ = tNew.z;

}
void CActor::Render(XMMATRIX tMatView, XMMATRIX tMatPerspective)
{
    //전방벡터 (0,0,1)의 기본값 설정
    mForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    //월드변환 행렬 , 단위행렬
    XMMATRIX tMatWorld = XMMatrixIdentity();

    //이동 변환행렬
    mMatTranslation = XMMatrixTranslation(mX, mY, mZ);
    //회전 변환행렬
    mMatRotation = XMMatrixRotationRollPitchYaw(mPitchX, mYawY, mRollZ);
    //비례 변환행렬
    mMatScale = XMMatrixScaling(mScaleX, mScaleY, mScaleZ);

    // w = TRSv, 열벡터 기준
    // DirectX의 응용프로그램(C++)수준, 행벡터 기준
    tMatWorld = mMatScale * mMatRotation * mMatTranslation;

    //전방벡터 = 전방벡터*회전행렬
    //  <--- 회전이 적용된 전방벡터를 구하자.
    mForwardVector = XMVector3Transform(mForwardVector, mMatRotation);


    mpEngine->mpD3D->GetImmediateDevice()->VSSetConstantBuffers(0, 1, &mpCBTransform);

    CBTransform tCBTransform;
    tCBTransform.mWorld = XMMatrixTranspose(tMatWorld);
    tCBTransform.mView = XMMatrixTranspose(tMatView);
    tCBTransform.mProjection = XMMatrixTranspose(tMatPerspective);

    mpEngine->mpD3D->GetImmediateDevice()->UpdateSubresource(mpCBTransform, 0, nullptr, &tCBTransform, 0, 0);

    mpMesh->Render();
    mpMtlTexture->Render(mpMesh->GetCountIndex(), mpMesh->GetTexture());

}