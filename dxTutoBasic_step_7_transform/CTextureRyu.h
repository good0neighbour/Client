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
    //복사는 막는다 
    CTextureRyu(const CTextureRyu& t) = delete;
    void operator=(const CTextureRyu& t) = delete;

    //외부에서 호출될 함수
public:
    void Create(ID3D11Device* tpDevice, LPCWSTR tFileName);
    void Destroy();

    inline ID3D11ShaderResourceView* GetTexture() const
    {
        return mpTexture;
    }
};

