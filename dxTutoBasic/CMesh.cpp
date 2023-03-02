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

    //������ü�� �����Ͽ� ���� ����
    mCountVertex = 24;
    mCountIndex = 36;

    //���� �����͸� ���޸𸮿� ����
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

    //vertex buffer �� index buffer�� ���� �Ϸ������Ƿ� ���� tpIndexs�� tpVertexs�� �ʿ����
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
    UINT stride = sizeof(SimpleVertex);   //����(�Ǽ�3��¥��)�� ������ �ȴٴ� ���̴� 
    UINT offset = 0;

    tpDeviceContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);

    tpDeviceContext->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    tpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    tpDeviceContext->DrawIndexed(36, 0, 0);
}