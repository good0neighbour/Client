#pragma once

#include "framework.h"

//Ŭ���� ���漱��
class CDxEngine;
class CTextureRyu;


class CMesh
{
private:
    ID3D11Buffer* mpVertexBuffer = nullptr;    
    ID3D11Buffer* mpIndexBuffer = nullptr;

    int mCountVertex = 0;       //���� �������� ����
    int mCountIndex= 0;       //�ε��� �������� ����

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
    //����� ���´� 
    CMesh(const CMesh& t) = delete;
    void operator=(const CMesh& t) = delete;

    //�ܺο��� ȣ��� �Լ�
public:
    void Create(ID3D11Device* tpDevice, LPCWSTR tFileName);
    void Destroy();
    void Render();

    inline void SetEngine(CDxEngine* tpEngine)
    {
        mpEngine = tpEngine;
    }
    //�ε��� �������� ���� ���� 
    inline int GetCountIndex() const
    {
        return mCountIndex;
    }

    ID3D11ShaderResourceView* GetTexture() const;



    //���ο��� ȣ��� �Լ�
private:
    void CreateBuffers(ID3D11Device* tpDevice);
    void DestroyBuffers();
    void RenderBuffers(ID3D11DeviceContext* tpDeviceContext);
};

