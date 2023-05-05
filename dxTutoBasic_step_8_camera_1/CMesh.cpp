#include "CMesh.h"

#include "ryubuffer.h"
#include "CDXEngine.h"

#include "CTextureRyu.h"

void CMesh::Create(ID3D11Device* tpDevice, LPCWSTR tFileNameModel, LPCWSTR tFileName)
{
	CreateBuffers(tpDevice, tFileNameModel);

    mpTextureRyu = new CTextureRyu();
    mpTextureRyu->Create(tpDevice, tFileName);
}
void CMesh::Destroy()
{
    mVertices.clear();
    mUVs.clear();
    mNormals.clear();

    mIndexVertices.clear();
    mIndexUVs.clear();
    mIndexNormals.clear();
    
    mNN.clear();
    mRYUIndexs.clear();
    mTT.clear();
    mAAAA.clear();
    //--------------------------


    if (mpTextureRyu)
    {
        mpTextureRyu->Destroy();
    }
	DestroyBuffers();
}
void CMesh::Render()
{
	RenderBuffers(mpEngine->GetImmediateDevice());

}


void CMesh::CreateBuffers(ID3D11Device* tpDevice, LPCWSTR tFileNameModel)
{
    SimpleVertex* tpVertexs = nullptr;
    WORD* tpIndexs = nullptr;

    //=============================================
    //LPCWSTR string ȣȯ�ȵǼ� ���� 3��
    wstring tWFileName = tFileNameModel;
    string tName;
    tName.assign(tWFileName.begin(), tWFileName.end());
    //=============================================

    //���Ͽ��� �����͸� �ε�
    LoadFromObjFile(tName);
    //LoadFromObjFile("resources/test_sphere.obj");
    //���Ͽ��� ������ �����͸� �ٰŷ� ���� ����
    mCountVertex = mVertices.size();
    mCountIndex = mIndexVertices.size();

    //�ý��� �� �޸𸮿� ������ ����
    tpVertexs = new SimpleVertex[mCountVertex];
    //�ý��� �޸𸮿� ���� �����͸� ���Ͽ��� 
    for (int ti = 0; ti < mCountVertex; ++ti)
    {
        tpVertexs[ti].position.x = mVertices[ti].x;
        tpVertexs[ti].position.y = mVertices[ti].y;
        tpVertexs[ti].position.z = mVertices[ti].z;
        tpVertexs[ti].position.w = 1.0f;
    }

    //������ �ٸ� ������(uv, normal)�� ���Ͽ��� ������ �����ͷ� ����
    for (int ti = 0; ti < mCountIndex; ++ti)
    {
        int tIndexVertex                    = mIndexVertices[ti];
        int tIndexUV                        = mIndexUVs[ti];
        int tIndexNormal                    = mIndexNormals[ti];

        tpVertexs[tIndexVertex].texcoord = mUVs[tIndexUV];
        tpVertexs[tIndexVertex].normal = mNormals[tIndexNormal];
    }

    //�ý��� �� �޸𸮿� �ε��� ������ ����
    tpIndexs = new WORD[mCountIndex];
    //�ý��� �޸𸮿� �ε��� �����͸� ���Ͽ��� �о���� �����ͷ� ����
    for (int ti = 0; ti < mCountIndex; ++ti)
    {
        tpIndexs[ti] = mIndexVertices[ti];
    }



    //�ּ���
    D3D11_BUFFER_DESC tBd = {};
    tBd.Usage = D3D11_USAGE_DEFAULT;    //���۴� �⺻ �뵵 ���۷� ����ϰڴ�.
    tBd.ByteWidth = sizeof(SimpleVertex) * mCountVertex;   //�������� ũ��� �̷���. ����ü�̹Ƿ� �ش� ������ 8���̴�.
    tBd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       //vertex buffer �뵵�� �����. //vertex buffer �뵵�� ����ϰڴ�.
    tBd.CPUAccessFlags = 0;     //CPU�� ������ �����Ѵ�

    //�μ���
    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = tpVertexs; //�ش� �ý��� �޸𸮿� ������ �ѱ��

    //�������۸� ����
    tpDevice->CreateBuffer(&tBd, &InitData, &mpVertexBuffer);

    //======================================================================================================================

    //�ּ���
    //D3D11_BUFFER_DESC tBd = {};
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
void CMesh::DestroyBuffers()
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
void CMesh::RenderBuffers(ID3D11DeviceContext* tpDeviceContext)
{
    UINT stride = sizeof(SimpleVertex); //buffer�� �ؼ��� ��, ����(�Ǽ�3��¥��)�� ������ �ȴٴ� ���̴�.   //�޸𸮸� �ؼ��ϴ� ���
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


ID3D11ShaderResourceView* CMesh::GetTexture() const
{
    return mpTextureRyu->GetTexture();
}