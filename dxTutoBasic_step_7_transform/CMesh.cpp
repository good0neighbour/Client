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

    //정육면체를 가정하여 갯수 설정
    //mCountVertex = 24;
    //mCountIndex = 36;

    /*
    //정점 데이터를 힙메모리에 생성
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
    //파일에서 로드한 데이터를 근거로 갯수 설정
    mCountVertex = mVertices.size();
    mCountIndex = mIndexVertices.size();

    tpVertexs = new SVertex[mCountVertex];

    //파일에서 로드한 데이터로 정점의 위치 데이터 구성
    for (int ti = 0; ti<mCountVertex; ++ti)
    {
        tpVertexs[ti].position.x = mVertices[ti].x;
        tpVertexs[ti].position.y = mVertices[ti].y;
        tpVertexs[ti].position.z = mVertices[ti].z;
        tpVertexs[ti].position.w = 1.0f;
    }

    //파일에서 로드한 데이터로 정점의 uv, normal 데이터 구성
    //f면 정보에 정점위치인덱스/uv인덱스/normal인덱스 로 구성이 되어있으므로
    //임의의 정점 정보를 이 인덱스 정보들을 이용하여 구축한다
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




    //주설명
    D3D11_BUFFER_DESC tBd = {};
    tBd.Usage = D3D11_USAGE_DEFAULT;            //기본용도다 
    tBd.ByteWidth = sizeof(SVertex) * mCountVertex;    //해당 기하구조의 메모리 크기를 알린다
    tBd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//vertex buffer용도다
    tBd.CPUAccessFlags = 0;     //CPU의 접근은 불허한다

    //부설명
    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = tpVertexs;       //해당 시스템 메모리의 데이터의 내용을 넘겨준다

    //정점 버퍼vertex buffer 데이터를 생성
    tpDevice->CreateBuffer(&tBd, &InitData, &mpVertexBuffer);


    //주설명   
    tBd.Usage = D3D11_USAGE_DEFAULT;            //기본용도다 
    tBd.ByteWidth = sizeof(WORD) * mCountIndex;    //해당 기하구조의 메모리 크기를 알린다
    tBd.BindFlags = D3D11_BIND_INDEX_BUFFER;//index buffer용도다
    tBd.CPUAccessFlags = 0;     //CPU의 접근은 불허한다

    //부설명    
    InitData.pSysMem = tpIndexs;       //해당 시스템 메모리의 데이터의 내용을 넘겨준다

    tpDevice->CreateBuffer(&tBd, &InitData, &mpIndexBuffer);

    //vertex buffer 와 index buffer를 생성완료했으므로 이제 tpIndexs와 tpVertexs 는 필요없다
    //그러므로 삭제( 시스템 메모리 측 데이터는 삭제 )
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