#include "CBitmapRect.h"

#include "ryubuffer.h"
#include "CDxEngine.h"

#include "CTextureRyu.h"

void CBitmapRect::Create(ID3D11Device* tpDevice, LPCWSTR tFileName, int tScreenWidth, int tScreenHeight, int tBitmapWidth, int tBitmapHeight)
{
    //2d render�� ���� �ʿ��� �ΰ����� ������ ����
    mScreenWidth = tScreenWidth;
    mScreenHeight = tScreenHeight;

    mBitmapWidth = tBitmapWidth;
    mBitmapHeight = tBitmapHeight;

    mPrePosX = -1;
    mPrePosY = -1;



	CreateBuffers(tpDevice);

    mpTextureRyu = new CTextureRyu();
    mpTextureRyu->Create(tpDevice, tFileName);

}
void CBitmapRect::Destroy()
{

    if (mpTextureRyu)
    {
        mpTextureRyu->Destroy();

        delete mpTextureRyu;
        mpTextureRyu = nullptr;
    }

	DestroyBuffers();
}
void CBitmapRect::Render(int tPosX, int tPosY)
{
    UpdateBuffers(mpEngine->mpD3D->GetImmediateDevice(), tPosX, tPosY);

	RenderBuffers(mpEngine->mpD3D->GetImmediateDevice());
}



ID3D11ShaderResourceView* CBitmapRect::GetTexture() const
{
    return mpTextureRyu->GetTexture();
}





void CBitmapRect::CreateBuffers(ID3D11Device* tpDevice)
{
    SBitmapVertex* tpVertexs = nullptr;
    WORD* tpIndexs = nullptr;

    //�簢���� �����Ͽ� ���� ����
    mCountVertex = 6;
    mCountIndex = 6;


    tpVertexs = new SBitmapVertex[mCountVertex];
    memset(tpVertexs, 0, sizeof(SBitmapVertex) * mCountVertex);//�޸� �ʱ�ȭ

    tpIndexs = new WORD[mCountIndex];
    for (int ti = 0; ti < mCountIndex; ++ti)
    {
        tpIndexs[ti] = ti;  //�ε��� ����
    }


    //�ּ���
    D3D11_BUFFER_DESC tBd = {};
    tBd.Usage = D3D11_USAGE_DYNAMIC;            //Dynamic Buffer�뵵��.��������
    tBd.ByteWidth = sizeof(SBitmapVertex) * mCountVertex;    //�ش� ���ϱ����� �޸� ũ�⸦ �˸���
    tBd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//vertex buffer�뵵��
    tBd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     //�������α׷� ���ؿ��� ���� �����͸� ��ִ´�. ( CPU�� ������ ���� �뵵�� ����Ѵ� )

    //�μ���
    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = tpVertexs;       //�ش� �ý��� �޸��� �������� ������ �Ѱ��ش�

    //���� ����vertex buffer �����͸� ����
    tpDevice->CreateBuffer(&tBd, &InitData, &mpVertexBuffer);


    //�ּ���   
    tBd.Usage = D3D11_USAGE_DEFAULT;            //�⺻�뵵�� 
    tBd.ByteWidth = sizeof(WORD) * mCountIndex;    //�ش� ���ϱ����� �޸� ũ�⸦ �˸���
    tBd.BindFlags = D3D11_BIND_INDEX_BUFFER;//index buffer�뵵��
    tBd.CPUAccessFlags = 0;     //CPU�� ������ �����Ѵ�

    //�μ���    
    InitData.pSysMem = tpIndexs;       //�ش� �ý��� �޸��� �������� ������ �Ѱ��ش�

    tpDevice->CreateBuffer(&tBd, &InitData, &mpIndexBuffer);

    //vertex buffer �� index buffer�� �����Ϸ������Ƿ� ���� tpIndexs�� tpVertexs �� �ʿ����
    //�׷��Ƿ� ����( �ý��� �޸� �� �����ʹ� ���� )
    if (nullptr != tpIndexs)
    {
        delete[] tpIndexs;
        tpIndexs = nullptr;
    }

    if (nullptr != tpVertexs)
    {
        delete[] tpVertexs;
        tpVertexs = nullptr;
    }

}
void CBitmapRect::DestroyBuffers()
{
    if (nullptr != mpIndexBuffer)
    {
        mpIndexBuffer->Release();        
    }

    if (nullptr != mpVertexBuffer)
    {
        mpVertexBuffer->Release();
    }
}
void CBitmapRect::RenderBuffers(ID3D11DeviceContext* tpDeviceContext)
{
    //UINT stride = sizeof(SVertex);
    UINT stride = sizeof(SBitmapVertex);
    UINT offset = 0;                       

    tpDeviceContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
    tpDeviceContext->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    tpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    //tpDeviceContext->DrawIndexed(36, 0, 0);
}


void CBitmapRect::UpdateBuffers(ID3D11DeviceContext* tpDeviceContext, int tPosX, int tPosY)
{
    //tPosX, tPosY: ���� ��� ��ġ
    //mPrePosX, mPrePosY: ���� �����ġ
    //���� �����ġ�� ���� ��� ��ġ�� ���ٸ�( ��� ��ġ���� ������ ���ٸ� ) �������� ���� ��ġ�� �ʴ´�
    if (tPosX == mPrePosX && tPosY == mPrePosY)
    {
        return;
    }


    mPrePosX = tPosX;
    mPrePosY = tPosY;


    //��� ��ġ�� �ٲ�����Ƿ� //������ �ڵ�( ���� ���۸� �̿��� ���� �� ���� )�� ��������
    SBitmapVertex* tpVertexs = nullptr;
    tpVertexs = new SBitmapVertex[mCountVertex];
    memset(tpVertexs, 0, sizeof(SBitmapVertex) * mCountVertex);//�޸� �ʱ�ȭ


    //�簢������ �����ϱ� ���� ����������
    float tLeft = 0.0f;
    float tTop = 0.0f;
    float tRight = 0.0f;
    float tBottom = 0.0f;

    



    tLeft = (float)((mScreenWidth / 2.0f) * (11.0f)) + (float)tPosX;
    tRight = tLeft + (float)mBitmapWidth;

    tTop = (float)((mScreenWidth / 2.0f) * (11.0f)) + (float)tPosY;
    tBottom = tTop + (float)mBitmapHeight;

    //���� 6��
    //�簢���� ����
    //ù ��° �ﰢ��, CW�� ���� ����
    tpVertexs[0].position = XMFLOAT4(tLeft, tTop, 0.0f, 1.0f);
    tpVertexs[0].texcoord = XMFLOAT2(0.0f, 0.0f);

    tpVertexs[1].position = XMFLOAT4(tRight, tBottom, 0.0f, 1.0f);
    tpVertexs[1].texcoord = XMFLOAT2(1.0f, 1.0f);

    tpVertexs[2].position = XMFLOAT4(tLeft, tBottom, 0.0f, 1.0f);
    tpVertexs[2].texcoord = XMFLOAT2(0.0f, 1.0f);

    //�� ��° �ﰢ��, CW�� ���� ����
    tpVertexs[3].position = XMFLOAT4(tLeft, tTop, 0.0f, 1.0f);
    tpVertexs[3].texcoord = XMFLOAT2(0.0f, 0.0f);

    tpVertexs[4].position = XMFLOAT4(tRight, tTop, 0.0f, 1.0f);
    tpVertexs[4].texcoord = XMFLOAT2(1.0f, 0.0f);

    tpVertexs[5].position = XMFLOAT4(tRight, tBottom, 0.0f, 1.0f);
    tpVertexs[5].texcoord = XMFLOAT2(1.0f, 1.0f);

    //������� �ϸ� �������α׷� ������ �ý��� �޸𸮿� �����͸� ����� ������ ���̴�.


    //�̷��� ������� �����͸�, buffer�� �������� ���� ��ִ´�.

    D3D11_MAPPED_SUBRESOURCE mappedResource;    //���̴��� ���� �������� ���ٿ� ����. directx���� ����
    SBitmapVertex* verticesPtr = nullptr;       //���̴��� ���� �����͸� �������α׷� ���ؿ��� �����ϱ� ���� ������

    //�׷��� ī���� ��� �������α׷��� ����� �� �ְ�( �ٸ� �۾��� �������� �ʰ� ) �ڹ��踦 ��ٴ�
    tpDeviceContext->Map(mpVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

        //�����͸� ��ִ´�
        //
        // �º�( �������α׷� ������ ������ ) = �캯( �׷��� ��ġ�� vertex buffer�޸� ���� )
        verticesPtr = (SBitmapVertex*)mappedResource.pData;
        //���� �����͸� �����ϸ� �ȴ�.
        memcpy(verticesPtr, tpVertexs, sizeof(SBitmapVertex) * mCountVertex);

    //�׷��� ī���� ��� �������α׷��� ����� �� ����( �ٸ� �۾��� ����ǵ��� ) �ڹ��踦 ����
    tpDeviceContext->Unmap(mpVertexBuffer, 0);






    if (nullptr != tpVertexs)
    {
        delete[] tpVertexs;
        tpVertexs = nullptr;
    }
}