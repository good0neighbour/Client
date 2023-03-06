#include "CMesh.h"

#include "ryubuffer.h"
#include "CDxEngine.h"

#include "CTextureRyu.h"

void CMesh::Create(ID3D11Device* tpDevice, LPCWSTR tFileName)
{
	CreateBuffers(tpDevice);

    mpTextureRyu = new CTextureRyu();
    mpTextureRyu->Create(tpDevice, tFileName);

}
void CMesh::Destroy()
{

    if (mpTextureRyu)
    {
        mpTextureRyu->Destroy();

        delete mpTextureRyu;
        mpTextureRyu = nullptr;
    }

	DestroyBuffers();
}
void CMesh::Render()
{
	RenderBuffers(mpEngine->mpD3D->GetImmediateDevice());
}



ID3D11ShaderResourceView* CMesh::GetTexture() const
{
    return mpTextureRyu->GetTexture();
}





void CMesh::CreateBuffers(ID3D11Device* tpDevice)
{
    SVertex* tpVertexs = nullptr;
    WORD* tpIndexs = nullptr;

    //������ü�� �����Ͽ� ���� ����
    //mCountVertex = 24;
    //mCountIndex = 36;

    /*
    //���� �����͸� ���޸𸮿� ����
    tpVertexs = new SVertex[mCountVertex]
    {
        { XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f),  XMFLOAT3(0.0f, 1.0f, 0.0f),   XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f),    XMFLOAT3(0.0f, 1.0f, 0.0f),    XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),      XMFLOAT3(0.0f, 1.0f, 0.0f),   XMFLOAT2(0.0f, 1.0f) },
        { XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f),    XMFLOAT3(0.0f, 1.0f, 0.0f),    XMFLOAT2(1.0f, 1.0f) },

        { XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),    XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),     XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),      XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),     XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
        { XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

        { XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
        { XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

        { XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
        { XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },

        { XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
        { XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
    };


    tpIndexs = new WORD[mCountIndex]
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };
    */
    LoadFromObjFile("resources/slime.obj");
    //LoadFromObjFile("resources/test_sphere.obj");
    //���Ͽ��� �ε��� �����͸� �ٰŷ� ���� ����
    mCountVertex = mVertices.size();
    mCountIndex = mIndexVertices.size();

    tpVertexs = new SVertex[mCountVertex];

    //���Ͽ��� �ε��� �����ͷ� ������ ��ġ ������ ����
    for (int ti = 0; ti<mCountVertex; ++ti)
    {
        tpVertexs[ti].position.x = mVertices[ti].x;
        tpVertexs[ti].position.y = mVertices[ti].y;
        tpVertexs[ti].position.z = mVertices[ti].z;
        tpVertexs[ti].position.w = 1.0f;
    }

    //���Ͽ��� �ε��� �����ͷ� ������ uv, normal ������ ����
    //f�� ������ ������ġ�ε���/uv�ε���/normal�ε��� �� ������ �Ǿ������Ƿ�
    //������ ���� ������ �� �ε��� �������� �̿��Ͽ� �����Ѵ�
    //for (int ti = 0; ti < mCountVertex; ++ti)    
    for (int ti = 0; ti < mCountIndex; ++ti)
    {
        int tIndexVertex = mIndexVertices[ti];
        int tIndexUV = mIndexUVs[ti];
        int tIndexNormal = mIndexNormals[ti];


        tpVertexs[tIndexVertex].texcoord = mUVs[tIndexUV];
        tpVertexs[tIndexVertex].normal = mNormals[tIndexNormal];
    }

    tpIndexs = new WORD[mCountIndex];
    for (int ti = 0; ti<mCountIndex; ++ti)
    {
        tpIndexs[ti] = mIndexVertices[ti];
    }




    //�ּ���
    D3D11_BUFFER_DESC tBd = {};
    tBd.Usage = D3D11_USAGE_DEFAULT;            //�⺻�뵵�� 
    tBd.ByteWidth = sizeof(SVertex) * mCountVertex;    //�ش� ���ϱ����� �޸� ũ�⸦ �˸���
    tBd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//vertex buffer�뵵��
    tBd.CPUAccessFlags = 0;     //CPU�� ������ �����Ѵ�

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
void CMesh::DestroyBuffers()
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
void CMesh::RenderBuffers(ID3D11DeviceContext* tpDeviceContext)
{
    UINT stride = sizeof(SVertex);   
    UINT offset = 0;                       

    tpDeviceContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
    tpDeviceContext->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    tpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    //tpDeviceContext->DrawIndexed(36, 0, 0);
}