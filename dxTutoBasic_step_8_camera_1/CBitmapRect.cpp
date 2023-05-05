#include "CBitmapRect.h"

#include "ryubuffer.h"
#include "CDXEngine.h"

#include "CTextureRyu.h"

void CBitmapRect::Create(ID3D11Device* tpDevice, LPCWSTR tFileName, int tScreenWidth, int tScreenHeight, int tBitmapWidth, int tBitmapHeight)
{
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
    }
	DestroyBuffers();
}
void CBitmapRect::Render(int tPosX, int tPosY)
{
    UpdateBuffers(mpEngine->GetImmediateDevice(), tPosX, tPosY);
	RenderBuffers(mpEngine->GetImmediateDevice());

}


void CBitmapRect::CreateBuffers(ID3D11Device* tpDevice)
{
    SBitmapVertex* tpVertexs = nullptr;
    WORD* tpIndexs = nullptr;


    //�簢���̴ϰ� �ﰢ�� �ΰ� ���� ����
    mCountVertex = 6;
    mCountIndex = 6;

    //�ý��� �� �޸𸮿� ������ ����
    tpVertexs = new SBitmapVertex[mCountVertex];
    memset(tpVertexs, 0, sizeof(SBitmapVertex) * mCountVertex);  //�޸� �ʱ�ȭ

    //�ý��� �� �޸𸮿� �ε��� ������ ����
    tpIndexs = new WORD[mCountIndex];
    for (int ti = 0; ti < mCountIndex; ++ti)
    {
        tpIndexs[ti] = ti;      //�ε��� ����
    }



    //�ּ���
    D3D11_BUFFER_DESC tBd = {};
    tBd.Usage = D3D11_USAGE_DYNAMIC; //���۴� �⺻ �뵵 ���۷� ����ϰڴ�.                                                                             //D3D11_USAGE_DEFAULT;    //���۴� �⺻ �뵵 ���۷� ����ϰڴ�.
    tBd.ByteWidth = sizeof(SBitmapVertex) * mCountVertex;
    tBd.BindFlags = D3D11_BIND_VERTEX_BUFFER;                                                       //vertex buffer �뵵�� �����. //vertex buffer �뵵�� ����ϰڴ�.
    tBd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    //<--�������α׷� ���ؿ��� ( CPU ���� ) ����? �����͸� ��ִ´�.              //0;     //CPU�� ������ �����Ѵ�

    //�μ���
    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = tpVertexs; //�ش� �ý��� �޸𸮿� ������ �ѱ��

    //�������۸� ����
    tpDevice->CreateBuffer(&tBd, &InitData, &mpVertexBuffer);

    //======================================================================================================================

    //�ּ���
    tBd.Usage = D3D11_USAGE_DEFAULT;    //���۴� �⺻ �뵵 ���۷� ����ϰڴ�.
    tBd.ByteWidth = sizeof(WORD) * mCountIndex;   //�������� ũ��� �̷���. �ﰢ���̹Ƿ� �ش� ������ 3���̴�. //���� ���� ũ���� �������̴�.
    tBd.BindFlags = D3D11_BIND_INDEX_BUFFER;       //index buffer �뵵�� �����. //index buffer �뵵�� ����ϰڴ�.
    tBd.CPUAccessFlags = 0;     //CPU�� ������ �����Ѵ�

    //�μ���
    //D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = tpIndexs; //�ش� �ý��� �޸𸮿� ������ �ѱ��

    //index buffer�� ����
    tpDevice->CreateBuffer(&tBd, &InitData, &mpIndexBuffer);


    //vertex buffer, index buffer�� �����Ϸ��Ͽ����Ƿ�
    //�ý��� �޸� ���� �����ʹ� ����
    if (tpIndexs)
    {
        delete[] tpIndexs;
        tpIndexs = nullptr;
    }

    if (tpVertexs)
    {
        delete[] tpVertexs;
        tpVertexs = nullptr;
    }


}
void CBitmapRect::DestroyBuffers()
{
    if (mpIndexBuffer)
    {
        mpIndexBuffer->Release();
    }

    if (mpVertexBuffer)
    {
        mpVertexBuffer->Release();
    }
}
void CBitmapRect::RenderBuffers(ID3D11DeviceContext* tpDeviceContext)
{
    UINT stride = sizeof(SBitmapVertex); //buffer�� �ؼ��� ��, ����(�Ǽ�3��¥��)�� ������ �ȴٴ� ���̴�.   //�޸𸮸� �ؼ��ϴ� ���
    UINT offset = 0;    //�󸶳� ����������, //�ؼ��� ������ ���� �޸��� ��ġ�� ���ۺκп��� ��ŭ �������ִ���

    //IA�� VertexBuffer����
    tpDeviceContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
    //IA�� IndexBuffer����
    tpDeviceContext->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    //IA�� ������ ���� topology ����
    tpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    //�ε���  ���۸� ������� �������� �������϶�� �����Ѵ�
    //tpDeviceContext->DrawIndexed(mCountIndex, 0, 0);
}


ID3D11ShaderResourceView* CBitmapRect::GetTexture() const
{
    return mpTextureRyu->GetTexture();
}


bool CBitmapRect::UpdateBuffers(ID3D11DeviceContext* tpDeviceContext, int tPosX, int tPosY)
{
    //���� ��� ��ġ�� ���� ���� �ʾҴٸ�, ������ ���� �ʴ´�. ( CPU�� �̿��� buffer writing �� ���� ���� �ʴ´�. )
    if (tPosX == mPrePosX && tPosY == mPrePosY)
    {
        return true;
    }

    mPrePosX = tPosX;
    mPrePosY = tPosY;

    //�����ġ�� ����Ǿ��ٸ�, ������ �Ѵ�. ( CPU�� �̿��� buffer writing �� �����Ѵ�. )

    //�������α׷� ������ �ý��� �޸��� ���� ������ �� �����
    SBitmapVertex* tpVertexs = nullptr;
    tpVertexs = new SBitmapVertex[mCountVertex];

    float tLeft = 0.0f;
    float tTop = 0.0f;
    float tRight = 0.0f;
    float tBottom = 0.0f;


    //directX ��ǥ��� ��ȯ==================
    tLeft = (float)((mScreenWidth / 2.0f) * (-1.0f)) + (float)tPosX;
    tRight = tLeft + (float)mBitmapWidth;

    tTop = (float)((mScreenHeight / 2.0f)) - (float)tPosY;
    tBottom = tTop - (float)mBitmapHeight;
    //===================================

    //ù ��° �ﰢ��, CW�� ���� ����
    tpVertexs[0].position = XMFLOAT4(tLeft, tTop, 0.0f, 1.0f);
    tpVertexs[0].texcoord = XMFLOAT2(0, 0);

    tpVertexs[1].position = XMFLOAT4(tRight, tBottom, 0.0f, 1.0f);
    tpVertexs[1].texcoord = XMFLOAT2(1, 1);

    tpVertexs[2].position = XMFLOAT4(tLeft, tBottom, 0.0f, 1.0f);
    tpVertexs[2].texcoord = XMFLOAT2(0, 1);

    //�� ��° �ﰢ��, CW�� ���� ����
    tpVertexs[3].position = XMFLOAT4(tLeft, tTop, 0.0f, 1.0f);
    tpVertexs[3].texcoord = XMFLOAT2(0, 0);

    tpVertexs[4].position = XMFLOAT4(tRight, tTop, 0.0f, 1.0f);
    tpVertexs[4].texcoord = XMFLOAT2(1, 0);

    tpVertexs[5].position = XMFLOAT4(tRight, tBottom, 0.0f, 1.0f);
    tpVertexs[5].texcoord = XMFLOAT2(1, 1);

    
    
    //buffer�� ��ִ´�.

    D3D11_MAPPED_SUBRESOURCE mappedResource;        //���̴��� ���������� ���ٿ� ����
    SBitmapVertex* verticesPtr = nullptr;       //���̴��� ���������͸� �������α׷� ���ؿ��� �����ϱ� ���� ������ ����

    //Map/Unmap�� ¦�� ������߸� �Ѵ�.
    //      Map: buffer�� ���� ���ٱ��� ���
    //  mappedResource: mpVertexBuffer�� ���� �������α׷� ���ؿ� �߻�ȭ�� ������
    //  verticesPtr: mappedResource�� ���� �������α׷� ���ؿ� �߻�ȭ�� ������
    tpDeviceContext->Map(mpVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    //D3D11_MAP_WRITE_DISCARD �ɼ��� �־� mappedResource�� �����
    //GPU�� ������ vertex buffer�� �����Ͽ� �������� ��
    //CPU�� �̸� �������� �ʰ�
    //���ο� vertex buffer�� ����� ���⿡ �����͸� ��ְ� �ȴ�.
    //
    //GPU�� ���� vertex buffer�� �������ϰ� ����
    //���� ���ο� vertex buffer�� �������Ѵ�
    //
    //�� mappedResource�� �߰����� �̷��� �۾����� �߻�ȭ�Ͽ� �����ϴ� ���̴�.

    //���۸� ������ �����ϴµ��� �ڿ��� ������ ������ �ӵ� ( ���� ���� ) �� �����ȴ�. ���� ���ϰ� �Ͼ�� �ʴ´�.

        verticesPtr = (SBitmapVertex*)mappedResource.pData;
        //���� buffer�� �ý��� �޸𸮿� �ִ� �����͸� ������ �ִ´�. �ý��� �޸� ---> buffer
        memcpy(verticesPtr, (void*)tpVertexs, sizeof(SBitmapVertex) * mCountVertex);

    //      Unmap: buffer�� ���� ���ٱ��� ����
    tpDeviceContext->Unmap(mpVertexBuffer, 0);


    //�������α׷� ������ �ý��� �޸��� ���� �����͸� �����Ѵ�
    if (tpVertexs)
    {
        delete[] tpVertexs;
        tpVertexs = nullptr;
    }
    

    return true;
}