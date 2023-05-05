#pragma once

#include "framework.h"

//Ŭ���� ���漱��
class CDxEngine;

class CMtlUnlit
{
private:
    ID3D11VertexShader* mpVertexShader = nullptr;
    ID3D11PixelShader* mpPixelShader = nullptr;

    ID3D11InputLayout* mpVertexLayout = nullptr;

    ID3D11SamplerState* mpSamplerStateLinear = nullptr;



    CDxEngine* mpEngine = nullptr;

public:
    CMtlUnlit()
    {       
    };
    ~CMtlUnlit() {};
    //����� ���´� 
    CMtlUnlit(const CMtlUnlit& t) = delete;
    void operator=(const CMtlUnlit& t) = delete;

    //�ܺο��� ȣ��� �Լ�
public:
    void Create(ID3D11Device* tpDevice);
    void Destroy();
    void Render(int tCountIndex, ID3D11ShaderResourceView* tpTexture);

    inline void SetEngine(CDxEngine* tpEngine)
    {
        mpEngine = tpEngine;
    }

    //���ο��� ȣ��� �Լ�
private:
    void CreateShader(ID3D11Device* tpDevice);
    void DestroyShader();
    void RenderShader(ID3D11DeviceContext* tpDeviceContext, int tCountIndex);

    void SetShaderParameter(ID3D11DeviceContext* tpDeviceContext, ID3D11ShaderResourceView* tpTexture);


    HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
};

