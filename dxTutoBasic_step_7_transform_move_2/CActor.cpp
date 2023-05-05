#include "CActor.h"

#include "CMesh.h"
#include "CMtlTexture.h"

#include "CDxEngine.h"
#include "CD3D.h"

#include "ryubuffer.h"


void CActor::Create(CDxEngine* tpEngine, LPCWSTR tFileNameModel, LPCWSTR tFileNameTexture)
{
    SetEngine(tpEngine);

    //��Ƽ���� 
    mpMtlTexture = new CMtlTexture();
    mpMtlTexture->Create(mpEngine->mpD3D->GetD3DDevice());
    mpMtlTexture->SetEngine(mpEngine);

    //�޽�
    mpMesh = new CMesh();
    mpMesh->Create(mpEngine->mpD3D->GetD3DDevice(), tFileNameModel, tFileNameTexture);
    mpMesh->SetEngine(mpEngine);

    //�������(��ȯ)
    D3D11_BUFFER_DESC tBd = {};
    tBd.Usage = D3D11_USAGE_DEFAULT;            //�⺻�뵵�� 
    tBd.ByteWidth = sizeof(CBTransform) * 1;    //�ش� ��������� �޸� ũ�⸦ �˸���
    tBd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//constant buffer�뵵��
    tBd.CPUAccessFlags = 0;     //CPU�� ������ �����Ѵ�

    mpEngine->mpD3D->GetD3DDevice()->CreateBuffer(&tBd, nullptr, &mpCBTransform);

    //���溤�� (0,0,1)�� �⺻�� ����
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
    //���� ��ǥ�� ����
    mCoordOption = tCoordOption;

    //���� �����ӿ��� ������ ��ġ ������ ��Ƶд�.
    XMFLOAT3 tOld;
    tOld.x = mX;
    tOld.y = mY;
    tOld.z = mZ;
    mPosition = XMLoadFloat3(&tOld);

    //������ǥ�谡 �����̶��
    if ( COORD_OPTION::World == mCoordOption )
    {
        //������ġ = ������ġ + �ӵ�*�ð�����
        mPosition = mPosition + mVelocity * tDeltaTime;
    }
    else //������ǥ�� �����̶�� 
    {
        //������ ����( �������� �Ĺ����� ���� )
        XMVECTOR b = XMVector4Normalize(mVelocity);
        XMFLOAT4 a;
        XMStoreFloat4(&a, b);
        //a.z�� +1 or -1

        //�ӵ��� ũ��(�ӷ�) ����
        XMVECTOR t = XMVector4Length(mVelocity);
        XMFLOAT4 f;
        XMStoreFloat4(&f, t);
        //x,y,z,w ��� ���� ���� ���´�.
        //<-- f.x�� ���ϰڴ� 


        //������� ���� mVelocity�� ����� ũ�⸦ ���� �� �ִ�. 
        //<-- �̰��� mForwardVector�� ��Į������Ͽ� 
        //          �츮�� ���ϴ� �ӵ��� ������.

        //������ġ = ������ġ + �ӵ�*�ð�����
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
    //���溤�� (0,0,1)�� �⺻�� ����
    mForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    //���庯ȯ ��� , �������
    XMMATRIX tMatWorld = XMMatrixIdentity();

    //�̵� ��ȯ���
    mMatTranslation = XMMatrixTranslation(mX, mY, mZ);
    //ȸ�� ��ȯ���
    mMatRotation = XMMatrixRotationRollPitchYaw(mPitchX, mYawY, mRollZ);
    //��� ��ȯ���
    mMatScale = XMMatrixScaling(mScaleX, mScaleY, mScaleZ);

    // w = TRSv, ������ ����
    // DirectX�� �������α׷�(C++)����, �຤�� ����
    tMatWorld = mMatScale * mMatRotation * mMatTranslation;

    //���溤�� = ���溤��*ȸ�����
    //  <--- ȸ���� ����� ���溤�͸� ������.
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