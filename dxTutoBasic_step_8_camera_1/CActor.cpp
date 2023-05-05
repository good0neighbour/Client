#include "CActor.h"


#include "CMtlTexture.h"
#include "CMesh.h"
#include "CDXEngine.h"
#include "ryubuffer.h"


void CActor::Create(CDXEngine* tpEngine, LPCWSTR tFileNameModel, LPCWSTR tpFileNameTexture)
{
    SetEngine(tpEngine);

    //��Ƽ���� ����
    mpMtl = new CMtlTexture();
    mpMtl->Create(mpEngine->GetD3DDevice());
    mpMtl->SetEngine(mpEngine);
    //�޽� ����
    mpMesh = new CMesh();
    /*mpMesh->Create(mpEngine->GetD3DDevice(), L"resources/slimeby.dds");*/
    mpMesh->Create(mpEngine->GetD3DDevice(), tFileNameModel, tpFileNameTexture);
    mpMesh->SetEngine(mpEngine);

    //������� ����
    // for transform
    //�ּ���
    D3D11_BUFFER_DESC tBd = {};
    tBd.Usage = D3D11_USAGE_DEFAULT;    //���۴� �⺻ �뵵 ���۷� ����ϰڴ�.
    tBd.ByteWidth = sizeof(CBTransform);    //�������� ũ��
    tBd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;       //constant buffer �뵵�� �����.
    tBd.CPUAccessFlags = 0;     //CPU�� ������ �����Ѵ�
    //�μ��� �� �ʿ���� UpdateSubResources�� �̿��Ͽ� �� �����Ӹ��� �������� ���� �����Ѵ�
    mpEngine->GetD3DDevice()->CreateBuffer(&tBd, nullptr, &mpCBTransform);

    //���溤�� ( 0,0,1 ) �� �⺻�� ����
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
    //���� ��ǥ�� ����
    mCoordOption = tCoordOption;


    XMFLOAT3 tOld;
    tOld.x = mX;
    tOld.y = mY;
    tOld.z = mZ;

    mPosition = XMLoadFloat3(&tOld);

    //���� ������ �̿��ϱ� ���� XMVECTOR Ÿ���� ���� mPosition, mVelocity �� �̿�
    if (mCoordOption == COORD_OPTION::World)        //world coordinate
    {
        //������ġ = ���� ��ġ + �ӵ�*�ð�����
        mPosition = mPosition + mVelocity * tDeltaTime;
    }
    else                                                                      //local coordinate
    {
        //mForwardVector ���� ���� (0, 0, 1)

        //�������� �Ĺ����� ����
        XMVECTOR b = XMVector4Normalize(mVelocity);
        XMFLOAT4 a;
        
        XMStoreFloat4(&a, b);
        //          a.z�� 1 or -1;
        
        //�ӵ��� ũ�� ����
        XMVECTOR t = XMVector4Length(mVelocity);
        XMFLOAT4 f;
        XMStoreFloat4(&f, t);

        //f.x�� ���ؼ� �̰��� ũ��� ����( x, y, z, w ��� ���� ���� ���´� )

        //������ġ = ���� ��ġ + �ӵ�*�ð�����
        //                                      (0, 0, 1)*(-1)*(50)
        mPosition = mPosition + mForwardVector*a.z*f.x * tDeltaTime;
    }

    XMFLOAT3 tNew;
    XMStoreFloat3(&tNew, mPosition);

    mX = tNew.x;
    mY = tNew.y;
    mZ = tNew.z;

    //=====================================================================

    //���溤�� ( 0,0,1 ) �� �⺻�� ����
    mForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    //mX, mY, mZ�� ������� ���庯ȯ ���( <-- �̵� ��ȯ �� �����ߴ� )�� �����.
    //XMMATRIX tMatWorld = XMMatrixTranslation(mX, mY, mZ);

    mMatTranslate = XMMatrixTranslation(mX, mY, mZ);
    mMatRotation = XMMatrixRotationRollPitchYaw(mPitchX, mYawY, mRollZ);
    mMatScale = XMMatrixScaling(mScaleX, mScaleY, mScaleZ);

    // w = TRSv ���庯ȯ ��� ���� ���� ( ������ ���� )
    // directX �������α׷������� �຤�͸� �������� �Ѵ�
    // �׷��Ƿ� ����� ���� ������ �̷��� ���ߴ�.
    mMatWorld = mMatScale * mMatRotation * mMatTranslate;

    //���溤�Ϳ� ȸ����ȯ�� �����Ͽ� ���ο� ���溤�͸� ����Ѵ�.
    mForwardVector = XMVector3Transform(mForwardVector, mMatRotation);

}
void CActor::Render(XMMATRIX tMatView, XMMATRIX tMatProjection)
{
    ////���溤�� ( 0,0,1 ) �� �⺻�� ����
    //mForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    ////mX, mY, mZ�� ������� ���庯ȯ ���( <-- �̵� ��ȯ �� �����ߴ� )�� �����.
    //XMMATRIX tMatWorld = XMMatrixTranslation(mX, mY, mZ);

    //mMatTranslate = XMMatrixTranslation(mX, mY, mZ);
    //mMatRotation = XMMatrixRotationRollPitchYaw(mPitchX, mYawY, mRollZ);
    //mMatScale = XMMatrixScaling(mScaleX, mScaleY, mScaleZ);

    //// w = TRSv ���庯ȯ ��� ���� ���� ( ������ ���� )
    //// directX �������α׷������� �຤�͸� �������� �Ѵ�
    //// �׷��Ƿ� ����� ���� ������ �̷��� ���ߴ�.
    //tMatWorld = mMatScale * mMatRotation * mMatTranslate;

    ////���溤�Ϳ� ȸ����ȯ�� �����Ͽ� ���ο� ���溤�͸� ����Ѵ�.
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


// 1. ������ ������ ���͸� ���ϰ�
// 2. ���溤�Ϳ� �ش� ������ ���ͻ����� ������ ���ϰ�
// 3. �� ������ ȸ����ȯ����� ����
// 4. ȸ������
void CActor::UpdateLook(XMFLOAT4 tAtPoint)
{
    mForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    //������ ������ ���� ���ϱ�
    //�������� - ��������
    XMVECTOR tSrc = XMVectorSet(mX, mY, mZ, 1.0f);
    XMVECTOR tTarget = XMVectorSet(tAtPoint.x, tAtPoint.y, tAtPoint.z, tAtPoint.w);

    XMVECTOR tDir = tTarget - tSrc;
    tDir = XMVector4Normalize(tDir);

    //�� ���� ���̿� ���� ���ϱ�
    XMVECTOR tAngle = XMVector4AngleBetweenVectors(mForwardVector, tDir);
    XMFLOAT4 tAngleF;
    XMStoreFloat4(&tAngleF, tAngle);

    //ȸ���� ���ϱ�
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



    //���溤�� ( 0,0,1 ) �� �⺻�� ����
    //mForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    mMatTranslate = XMMatrixTranslation(mX, mY, mZ);
    //mMatRotation = XMMatrixRotationY(tAngleF.x);                  //<--- ������ ������, ȸ������ y������ ȸ��

    mMatRotation = XMMatrixRotationAxis(tAxis, tAngleF.x);       //<--- ������ ������, ȸ������ ��Ȳ�� ���� +-y������ ȸ��
    mMatScale = XMMatrixScaling(mScaleX, mScaleY, mScaleZ);

    mMatWorld = mMatScale * mMatRotation * mMatTranslate;

    mForwardVector = XMVector3Transform(mForwardVector, mMatRotation);
}