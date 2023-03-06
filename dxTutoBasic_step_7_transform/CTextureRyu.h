#pragma once

#include "framework.h"

class CTextureRyu
{
private:
    ID3D11ShaderResourceView* mpTexture = nullptr;


public:
    CTextureRyu()
    {
    };
    ~CTextureRyu() {};
    //����� ���´� 
    CTextureRyu(const CTextureRyu& t) = delete;
    void operator=(const CTextureRyu& t) = delete;

    //�ܺο��� ȣ��� �Լ�
public:
    void Create(ID3D11Device* tpDevice, LPCWSTR tFileName);
    void Destroy();

    inline ID3D11ShaderResourceView* GetTexture() const
    {
        return mpTexture;
    }
};

