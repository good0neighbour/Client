#include "CBitmapRect.h"

#include "ryubuffer.h"
#include "CDxEngine.h"

#include "CTextureRyu.h"

void CBitmapRect::Create(ID3D11Device* tpDevice, LPCWSTR tFileName, int tScreenWidth, int tScreenHeight, int tBitmapWidth, int tBitmapHeight)
{
    //2d render를 위해 필요한 부가적인 데이터 설정
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

    //사각형을 가정하여 갯수 설정
    mCountVertex = 6;
    mCountIndex = 6;


    tpVertexs = new SBitmapVertex[mCountVertex];
    memset(tpVertexs, 0, sizeof(SBitmapVertex) * mCountVertex);//메모리 초기화

    tpIndexs = new WORD[mCountIndex];
    for (int ti = 0; ti < mCountIndex; ++ti)
    {
        tpIndexs[ti] = ti;  //인덱스 설정
    }


    //주설명
    D3D11_BUFFER_DESC tBd = {};
    tBd.Usage = D3D11_USAGE_DYNAMIC;            //Dynamic Buffer용도다.동적버퍼
    tBd.ByteWidth = sizeof(SBitmapVertex) * mCountVertex;    //해당 기하구조의 메모리 크기를 알린다
    tBd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//vertex buffer용도다
    tBd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     //응용프로그램 수준에서 직접 데이터를 써넣는다. ( CPU의 접근은 쓰기 용도로 허용한다 )

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
    //tPosX, tPosY: 현재 출력 위치
    //mPrePosX, mPrePosY: 이전 출력위치
    //현재 출력위치와 이전 출력 위치가 같다면( 출력 위치값이 변경이 없다면 ) 랜더링을 새로 고치지 않는다
    if (tPosX == mPrePosX && tPosY == mPrePosY)
    {
        return;
    }


    mPrePosX = tPosX;
    mPrePosY = tPosY;


    //출력 위치가 바뀌었으므로 //렌더링 코드( 동적 버퍼를 이용한 직접 값 갱신 )를 실행하자
    SBitmapVertex* tpVertexs = nullptr;
    tpVertexs = new SBitmapVertex[mCountVertex];
    memset(tpVertexs, 0, sizeof(SBitmapVertex) * mCountVertex);//메모리 초기화


    //사각영역을 지정하기 위한 지역변수들
    float tLeft = 0.0f;
    float tTop = 0.0f;
    float tRight = 0.0f;
    float tBottom = 0.0f;

    



    tLeft = (float)((mScreenWidth / 2.0f) * (11.0f)) + (float)tPosX;
    tRight = tLeft + (float)mBitmapWidth;

    tTop = (float)((mScreenWidth / 2.0f) * (11.0f)) + (float)tPosY;
    tBottom = tTop + (float)mBitmapHeight;

    //정점 6개
    //사각형을 만듦
    //첫 번째 삼각형, CW로 정점 나열
    tpVertexs[0].position = XMFLOAT4(tLeft, tTop, 0.0f, 1.0f);
    tpVertexs[0].texcoord = XMFLOAT2(0.0f, 0.0f);

    tpVertexs[1].position = XMFLOAT4(tRight, tBottom, 0.0f, 1.0f);
    tpVertexs[1].texcoord = XMFLOAT2(1.0f, 1.0f);

    tpVertexs[2].position = XMFLOAT4(tLeft, tBottom, 0.0f, 1.0f);
    tpVertexs[2].texcoord = XMFLOAT2(0.0f, 1.0f);

    //두 번째 삼각형, CW로 정점 나열
    tpVertexs[3].position = XMFLOAT4(tLeft, tTop, 0.0f, 1.0f);
    tpVertexs[3].texcoord = XMFLOAT2(0.0f, 0.0f);

    tpVertexs[4].position = XMFLOAT4(tRight, tTop, 0.0f, 1.0f);
    tpVertexs[4].texcoord = XMFLOAT2(1.0f, 0.0f);

    tpVertexs[5].position = XMFLOAT4(tRight, tBottom, 0.0f, 1.0f);
    tpVertexs[5].texcoord = XMFLOAT2(1.0f, 1.0f);

    //여기까지 하면 응용프로그램 수준의 시스템 메모리에 데이터를 만들고 설정한 것이다.


    //이렇게 만들어진 데이터를, buffer에 데이터의 값을 써넣는다.

    D3D11_MAPPED_SUBRESOURCE mappedResource;    //셰이더의 정점 제이터의 접근용 변수. directx에서 제공
    SBitmapVertex* verticesPtr = nullptr;       //셰이더의 정점 데이터를 응용프로그램 수준에서 제어하기 위한 포인터

    //그래픽 카드의 제어를 응용프로그램이 사용할 수 있게( 다른 작업이 방해하지 않게 ) 자물쇠를 잠근다
    tpDeviceContext->Map(mpVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

        //데이터를 써넣는다
        //
        // 좌변( 응용프로그램 수준의 포인터 ) = 우변( 그래픽 장치의 vertex buffer메모리 참조 )
        verticesPtr = (SBitmapVertex*)mappedResource.pData;
        //이제 데이터를 복사하면 된다.
        memcpy(verticesPtr, tpVertexs, sizeof(SBitmapVertex) * mCountVertex);

    //그래픽 카드의 제어를 응용프로그램이 사용할 수 없게( 다른 작업이 진행되도록 ) 자물쇠를 연다
    tpDeviceContext->Unmap(mpVertexBuffer, 0);






    if (nullptr != tpVertexs)
    {
        delete[] tpVertexs;
        tpVertexs = nullptr;
    }
}