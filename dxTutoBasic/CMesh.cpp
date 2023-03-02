#include "CMesh.h"

#include "ryubuffer.h"
#include "CDxEngine.h"

void CMesh::Create(ID3D11Device* tpDevice)
{
	CreateBuffers(tpDevice);
}
void CMesh::Destroy()
{
	DestroyBuffers();
}
void CMesh::Render()
{
	RenderBuffers(mpEngine->mpD3D->GetImmediateDevice());
}

void CMesh::CreateBuffers(ID3D11Device* tpDevice)
{
    SimpleVertex* tpVertexs = nullptr;
    WORD* tpIndexs = nullptr;

    //정육면체를 가정하여 갯수 설정
    mCountVertex = 24;
    mCountIndex = 36;

    //정점 데이터를 힙메모리에 생성
    tpVertexs = new SimpleVertex[mCountVertex]
    {
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)    , XMFLOAT2(1.0f, 0.0f)  },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)     , XMFLOAT2(0.0f, 0.0f)  },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)      , XMFLOAT2(0.0f, 1.0f)  },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)     , XMFLOAT2(1.0f, 1.0f)  },

        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)  , XMFLOAT2(0.0f, 0.0f)  },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)   , XMFLOAT2(1.0f, 0.0f)  },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)    , XMFLOAT2(1.0f, 1.0f)  },
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)   , XMFLOAT2(0.0f, 1.0f)  },

        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)   , XMFLOAT2(0.0f, 1.0f)  },
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)  , XMFLOAT2(1.0f, 1.0f)  },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)   , XMFLOAT2(1.0f, 0.0f)  },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)    , XMFLOAT2(0.0f, 0.0f)  },

        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)     , XMFLOAT2(1.0f, 1.0f)  },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)    , XMFLOAT2(0.0f, 1.0f)  },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)     , XMFLOAT2(0.0f, 0.0f)  },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)      , XMFLOAT2(1.0f, 0.0f)  },

        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)  , XMFLOAT2(0.0f, 1.0f)  },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)   , XMFLOAT2(1.0f, 1.0f)  },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)    , XMFLOAT2(1.0f, 0.0f)  },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)   , XMFLOAT2(0.0f, 0.0f)  },

        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)    , XMFLOAT2(1.0f, 1.0f)  },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)     , XMFLOAT2(0.0f, 1.0f)  },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)      , XMFLOAT2(0.0f, 0.0f)  },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)     , XMFLOAT2(1.0f, 0.0f)  },
    };


    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * mCountVertex;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;


    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = tpVertexs;

    tpDevice->CreateBuffer(&bd, &InitData, &mpVertexBuffer);


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



    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * mCountIndex;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = tpIndexs;
    tpDevice->CreateBuffer(&bd, &InitData, &mpIndexBuffer);

    //vertex buffer 와 index buffer를 생성 완료했으므로 이제 tpIndexs와 tpVertexs는 필요없다
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
    UINT stride = sizeof(SimpleVertex);   //정점(실수3개짜리)이 단위가 된다는 것이다 
    UINT offset = 0;

    tpDeviceContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);

    tpDeviceContext->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    tpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    tpDeviceContext->DrawIndexed(36, 0, 0);
}