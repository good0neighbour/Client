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


    //사각형이니간 삼각형 두개 관한 내용
    mCountVertex = 6;
    mCountIndex = 6;

    //시스템 힙 메모리에 데이터 생성
    tpVertexs = new SBitmapVertex[mCountVertex];
    memset(tpVertexs, 0, sizeof(SBitmapVertex) * mCountVertex);  //메모리 초기화

    //시스템 힙 메모리에 인덱스 데이터 생성
    tpIndexs = new WORD[mCountIndex];
    for (int ti = 0; ti < mCountIndex; ++ti)
    {
        tpIndexs[ti] = ti;      //인덱스 설정
    }



    //주설명
    D3D11_BUFFER_DESC tBd = {};
    tBd.Usage = D3D11_USAGE_DYNAMIC; //버퍼는 기본 용도 버퍼로 사용하겠다.                                                                             //D3D11_USAGE_DEFAULT;    //버퍼는 기본 용도 버퍼로 사용하겠다.
    tBd.ByteWidth = sizeof(SBitmapVertex) * mCountVertex;
    tBd.BindFlags = D3D11_BIND_VERTEX_BUFFER;                                                       //vertex buffer 용도로 만든다. //vertex buffer 용도로 사용하겠다.
    tBd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    //<--응용프로그램 수준에서 ( CPU 에서 ) 직접? 데이터를 써넣는다.              //0;     //CPU의 접근은 불허한다

    //부설명
    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = tpVertexs; //해당 시스템 메모리에 내용을 넘긴다

    //정점버퍼를 생성
    tpDevice->CreateBuffer(&tBd, &InitData, &mpVertexBuffer);

    //======================================================================================================================

    //주설명
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
    UINT stride = sizeof(SBitmapVertex); //buffer를 해석할 때, 정점(실수3개짜리)이 단위가 된다는 것이다.   //메모리를 해석하는 경계
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


ID3D11ShaderResourceView* CBitmapRect::GetTexture() const
{
    return mpTextureRyu->GetTexture();
}


bool CBitmapRect::UpdateBuffers(ID3D11DeviceContext* tpDeviceContext, int tPosX, int tPosY)
{
    //만약 출력 위치가 변경 되지 않았다면, 랜더링 하지 않는다. ( CPU를 이용한 buffer writing 을 따로 하지 않는다. )
    if (tPosX == mPrePosX && tPosY == mPrePosY)
    {
        return true;
    }

    mPrePosX = tPosX;
    mPrePosY = tPosY;

    //출력위치가 변경되었다면, 랜더링 한다. ( CPU를 이용한 buffer writing 을 수행한다. )

    //응용프로그램 수준의 시스템 메모리의 정점 데이터 를 만든다
    SBitmapVertex* tpVertexs = nullptr;
    tpVertexs = new SBitmapVertex[mCountVertex];

    float tLeft = 0.0f;
    float tTop = 0.0f;
    float tRight = 0.0f;
    float tBottom = 0.0f;


    //directX 좌표계로 변환==================
    tLeft = (float)((mScreenWidth / 2.0f) * (-1.0f)) + (float)tPosX;
    tRight = tLeft + (float)mBitmapWidth;

    tTop = (float)((mScreenHeight / 2.0f)) - (float)tPosY;
    tBottom = tTop - (float)mBitmapHeight;
    //===================================

    //첫 번째 삼각형, CW로 정점 나열
    tpVertexs[0].position = XMFLOAT4(tLeft, tTop, 0.0f, 1.0f);
    tpVertexs[0].texcoord = XMFLOAT2(0, 0);

    tpVertexs[1].position = XMFLOAT4(tRight, tBottom, 0.0f, 1.0f);
    tpVertexs[1].texcoord = XMFLOAT2(1, 1);

    tpVertexs[2].position = XMFLOAT4(tLeft, tBottom, 0.0f, 1.0f);
    tpVertexs[2].texcoord = XMFLOAT2(0, 1);

    //두 번째 삼각형, CW로 정점 나열
    tpVertexs[3].position = XMFLOAT4(tLeft, tTop, 0.0f, 1.0f);
    tpVertexs[3].texcoord = XMFLOAT2(0, 0);

    tpVertexs[4].position = XMFLOAT4(tRight, tTop, 0.0f, 1.0f);
    tpVertexs[4].texcoord = XMFLOAT2(1, 0);

    tpVertexs[5].position = XMFLOAT4(tRight, tBottom, 0.0f, 1.0f);
    tpVertexs[5].texcoord = XMFLOAT2(1, 1);

    
    
    //buffer에 써넣는다.

    D3D11_MAPPED_SUBRESOURCE mappedResource;        //셰이더의 정점데이터 접근용 변수
    SBitmapVertex* verticesPtr = nullptr;       //셰이더의 정점데이터를 응용프로그램 수준에서 제어하기 위한 포인터 변수

    //Map/Unmap은 짝을 맞춰줘야만 한다.
    //      Map: buffer에 대한 접근권한 얻기
    //  mappedResource: mpVertexBuffer에 대한 응용프로그램 수준에 추상화된 제어점
    //  verticesPtr: mappedResource에 대한 응용프로그램 수준에 추상화된 제어점
    tpDeviceContext->Map(mpVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    //D3D11_MAP_WRITE_DISCARD 옵션을 주어 mappedResource를 만들면
    //GPU가 임의의 vertex buffer를 참고하여 랜더링할 시
    //CPU가 이를 간섭하지 않고
    //새로운 vertex buffer를 만들어 여기에 데이터를 써넣게 된다.
    //
    //GPU가 이전 vertex buffer를 랜더링하고 나면
    //이제 새로운 vertex buffer를 랜더링한다
    //
    //즉 mappedResource가 중간에서 이러한 작업들을 추상화하여 제공하는 것이다.

    //버퍼를 새로이 생성하는데는 자원이 들지만 랜더링 속도 ( 성능 유지 ) 가 유지된다. 성능 저하가 일어나지 않는다.

        verticesPtr = (SBitmapVertex*)mappedResource.pData;
        //직접 buffer에 시스템 메모리에 있는 데이터를 복사해 넣는다. 시스템 메모리 ---> buffer
        memcpy(verticesPtr, (void*)tpVertexs, sizeof(SBitmapVertex) * mCountVertex);

    //      Unmap: buffer에 대한 접근권한 해제
    tpDeviceContext->Unmap(mpVertexBuffer, 0);


    //응용프로그램 수준의 시스템 메모리의 정점 데이터를 삭제한다
    if (tpVertexs)
    {
        delete[] tpVertexs;
        tpVertexs = nullptr;
    }
    

    return true;
}