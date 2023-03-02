#pragma once

#include "framework.h"

//클래스 전방선언
class CDxEngine;
class CTextureRyu;


class CMesh
{
private:
    ID3D11Buffer* mpVertexBuffer = nullptr;    
    ID3D11Buffer* mpIndexBuffer = nullptr;

    int mCountVertex = 0;       //정점 데이터의 갯수
    int mCountIndex= 0;       //인덱스 데이터의 갯수

    CDxEngine* mpEngine = nullptr;

    CTextureRyu* mpTextureRyu = nullptr;

public:
    CMesh()
    {
        mpVertexBuffer = nullptr;
        mpIndexBuffer = nullptr;
        mCountVertex = 0; 
        mCountIndex = 0;
        mpEngine = nullptr;
    };
    ~CMesh() {};
    //복사는 막는다 
    CMesh(const CMesh& t) = delete;
    void operator=(const CMesh& t) = delete;

    //외부에서 호출될 함수
public:
    void Create(ID3D11Device* tpDevice, LPCWSTR tFileName);
    void Destroy();
    void Render();

    inline void SetEngine(CDxEngine* tpEngine)
    {
        mpEngine = tpEngine;
    }
    //인덱스 데이터의 갯수 리턴 
    inline int GetCountIndex() const
    {
        return mCountIndex;
    }

    ID3D11ShaderResourceView* GetTexture() const;



    //내부에서 호출될 함수
private:
    void CreateBuffers(ID3D11Device* tpDevice);
    void DestroyBuffers();
    void RenderBuffers(ID3D11DeviceContext* tpDeviceContext);
};

