#include "CActor.h"

#include "CMesh.h"
#include "CMtlTexture.h"

#include "CDxEngine.h"
#include "CD3D.h"

#include "ryubuffer.h"


void CActor::Create(CDxEngine* tpEngine)
{
    SetEngine(tpEngine);

    //머티리얼 
    mpMtlTexture = new CMtlTexture();
    mpMtlTexture->Create(mpEngine->mpD3D->GetD3DDevice());
    mpMtlTexture->SetEngine(mpEngine);

    //메쉬
    mpMesh = new CMesh();
    mpMesh->Create(mpEngine->mpD3D->GetD3DDevice(), L"resources/slimeby.dds");
    mpMesh->SetEngine(mpEngine);

    //상수버퍼(변환)
    D3D11_BUFFER_DESC tBd = {};
    tBd.Usage = D3D11_USAGE_DEFAULT;            //기본용도다 
    tBd.ByteWidth = sizeof(CBTransform) * 1;    //해당 상수버퍼의 메모리 크기를 알린다
    tBd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//constant buffer용도다
    tBd.CPUAccessFlags = 0;     //CPU의 접근은 불허한다

    mpEngine->mpD3D->GetD3DDevice()->CreateBuffer(&tBd, nullptr, &mpCBTransform);
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

    }

    XMFLOAT3 tNew;
    XMStoreFloat3(&tNew, mPosition);

    mX = tNew.x;
    mY = tNew.y;
    mZ = tNew.z;

}
void CActor::Render(XMMATRIX tMatView, XMMATRIX tMatPerspective)
{
    //이동변환 행렬 , 지역변수
    XMMATRIX tMatWorld = XMMatrixTranslation(mX, mY, mZ);




    mpEngine->mpD3D->GetImmediateDevice()->VSSetConstantBuffers(0, 1, &mpCBTransform);

    CBTransform tCBTransform;
    tCBTransform.mWorld = XMMatrixTranspose(tMatWorld);
    tCBTransform.mView = XMMatrixTranspose(tMatView);
    tCBTransform.mProjection = XMMatrixTranspose(tMatPerspective);

    mpEngine->mpD3D->GetImmediateDevice()->UpdateSubresource(mpCBTransform, 0, nullptr, &tCBTransform, 0, 0);

    mpMesh->Render();
    mpMtlTexture->Render(mpMesh->GetCountIndex(), mpMesh->GetTexture());

}