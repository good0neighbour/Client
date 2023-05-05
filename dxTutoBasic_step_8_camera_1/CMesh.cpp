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
    //LPCWSTR string 호환안되서 넣음 3줄
    wstring tWFileName = tFileNameModel;
    string tName;
    tName.assign(tWFileName.begin(), tWFileName.end());
    //=============================================

    //파일에서 데이터를 로드
    LoadFromObjFile(tName);
    //LoadFromObjFile("resources/test_sphere.obj");
    //파일에서 가져온 데이터를 근거로 갯수 설정
    mCountVertex = mVertices.size();
    mCountIndex = mIndexVertices.size();

    //시스템 힙 메모리에 데이터 생성
    tpVertexs = new SimpleVertex[mCountVertex];
    //시스템 메모리에 정점 데이터를 파일에서 
    for (int ti = 0; ti < mCountVertex; ++ti)
    {
        tpVertexs[ti].position.x = mVertices[ti].x;
        tpVertexs[ti].position.y = mVertices[ti].y;
        tpVertexs[ti].position.z = mVertices[ti].z;
        tpVertexs[ti].position.w = 1.0f;
    }

    //정점의 다른 정보들(uv, normal)도 파일에서 가져온 데이터로 구축
    for (int ti = 0; ti < mCountIndex; ++ti)
    {
        int tIndexVertex                    = mIndexVertices[ti];
        int tIndexUV                        = mIndexUVs[ti];
        int tIndexNormal                    = mIndexNormals[ti];

        tpVertexs[tIndexVertex].texcoord = mUVs[tIndexUV];
        tpVertexs[tIndexVertex].normal = mNormals[tIndexNormal];
    }

    //시스템 힙 메모리에 인덱스 데이터 생성
    tpIndexs = new WORD[mCountIndex];
    //시스템 메모리에 인덱스 데이터를 파일에서 읽어들인 데이터로 설정
    for (int ti = 0; ti < mCountIndex; ++ti)
    {
        tpIndexs[ti] = mIndexVertices[ti];
    }



    //주설명
    D3D11_BUFFER_DESC tBd = {};
    tBd.Usage = D3D11_USAGE_DEFAULT;    //버퍼는 기본 용도 버퍼로 사용하겠다.
    tBd.ByteWidth = sizeof(SimpleVertex) * mCountVertex;   //데이터의 크기는 이렇다. 육면체이므로 해당 정점이 8개이다.
    tBd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       //vertex buffer 용도로 만든다. //vertex buffer 용도로 사용하겠다.
    tBd.CPUAccessFlags = 0;     //CPU의 접근은 불허한다

    //부설명
    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = tpVertexs; //해당 시스템 메모리에 내용을 넘긴다

    //정점버퍼를 생성
    tpDevice->CreateBuffer(&tBd, &InitData, &mpVertexBuffer);

    //======================================================================================================================

    //주설명
    //D3D11_BUFFER_DESC tBd = {};
    tBd.Usage = D3D11_USAGE_DEFAULT;    //버퍼는 기본 용도 버퍼로 사용하겠다.
    tBd.ByteWidth = sizeof(WORD) * mCountIndex;   //데이터의 크기는 이렇다. 삼각형이므로 해당 정점이 3개이다. //정점 세개 크기의 데이터이다.
    tBd.BindFlags = D3D11_BIND_INDEX_BUFFER;       //index buffer 용도로 만든다. //index buffer 용도로 사용하겠다.
    tBd.CPUAccessFlags = 0;     //CPU의 접근은 불허한다

    //부설명
    //D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = tpIndexs; //해당 시스템 메모리에 내용을 넘긴다

    //index buffer를 생성
    tpDevice->CreateBuffer(&tBd, &InitData, &mpIndexBuffer);


    //vertex buffer, index buffer를 생성완료하였으므로
    //시스템 메모리 측에 데이터는 삭제
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
    UINT stride = sizeof(SimpleVertex); //buffer를 해석할 때, 정점(실수3개짜리)이 단위가 된다는 것이다.   //메모리를 해석하는 경계
    UINT offset = 0;    //얼마나 떨어졌는지, //해석을 시작할 버퍼 메모리의 위치가 시작부분에서 얼만큼 떨어져있는지

    //IA에 VertexBuffer설정
    tpDeviceContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
    //IA에 IndexBuffer설정
    tpDeviceContext->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    //IA에 도형의 위상 topology 설정
    tpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    //인덱스  버퍼를 기반으로 폴리곤을 랜더링하라고 지시한다
    //tpDeviceContext->DrawIndexed(mCountIndex, 0, 0);
}


ID3D11ShaderResourceView* CMesh::GetTexture() const
{
    return mpTextureRyu->GetTexture();
}