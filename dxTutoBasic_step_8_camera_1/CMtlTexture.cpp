#include "CMtlTexture.h"

#include "CDXEngine.h"

void CMtlTexture::Create(ID3D11Device* tpDevice)
{
    CreateShader(tpDevice);
}

void CMtlTexture::Destroy()
{
    DestroyShader();
}

void CMtlTexture::Render(int tCountIndex, ID3D11ShaderResourceView* tpTexture)
{
    SetShaderParameter(mpEngine->GetImmediateDevice(), tpTexture);

    RenderShader(mpEngine->GetImmediateDevice(), tCountIndex);
}

//=================================================================

void CMtlTexture::CreateShader(ID3D11Device* tpDevice)
{
    ID3DBlob* pVSBlob = nullptr;

    CompileShaderFromFile(L"VertexShader.hlsl", "main", "vs_4_0", &pVSBlob);
    tpDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &mpVertexShader);

    //�Է� ���̾ƿ� ����
    D3D11_INPUT_ELEMENT_DESC tLayout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    UINT numElements = ARRAYSIZE(tLayout);

    //�Է·��̾ƿ� ��ü�� ����, �̰��� �����ϴµ� pVSBlob�� �̿��Ѵ�
    tpDevice->CreateInputLayout(tLayout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &mpVertexLayout);

    pVSBlob->Release();




    //pixel shader object ����
    ID3DBlob* pPSBlob = nullptr;

    CompileShaderFromFile(L"PixelShader.hlsl", "main", "ps_4_0", &pPSBlob);
    tpDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mpPixelShader);

    pPSBlob->Release();


    //���÷� ��ü�� �����
        //����
    D3D11_SAMPLER_DESC sampDesc = {};
    //'Ȯ��', '���', '�Ӹ�' ó�� �� ��� ���������� ����ϰڴ� ��� �ǹ�
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;          //�������� MIN:��� MAG:Ȯ�� MIP: �ػ󵵼��غ� �ؽ�ó
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;         //UV�ݺ�ó���� ��Ǯ��, ��踦 �Ѿ�� �� ó���� �ݺ�
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;         //<--3���� �ؽ�ó ���䵵 �ִ�
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;       //���ø��������� ���Լ��� ���� ����
    sampDesc.MinLOD = 0;                    //LOD�� �ּڰ� level of detail, LOD�ܰ� ����
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;    //LOD�� �ִ�

    tpDevice->CreateSamplerState(&sampDesc, &mpSamplerStateLinear);
}

void CMtlTexture::DestroyShader()
{
    if (mpVertexLayout)
    {
        mpVertexLayout->Release();
    }

    if (mpPixelShader)
    {
        mpPixelShader->Release();
    }

    if (mpVertexShader)
    {
        mpVertexShader->Release();
    }
}

void CMtlTexture::SetShaderParameter(ID3D11DeviceContext* tpDeviceContext, ID3D11ShaderResourceView* tpTexture)
{
    tpDeviceContext->PSSetShaderResources(0, 1, &tpTexture);
}

void CMtlTexture::RenderShader(ID3D11DeviceContext* tpDeviceContext, int tCountIndex)
{
    tpDeviceContext->IASetInputLayout(mpVertexLayout);

    tpDeviceContext->VSSetShader(mpVertexShader, nullptr, 0);
    tpDeviceContext->PSSetShader(mpPixelShader, nullptr, 0);

    tpDeviceContext->PSSetSamplers(0, 1, &mpSamplerStateLinear);

    tpDeviceContext->DrawIndexed(tCountIndex, 0, 0);
}


HRESULT CMtlTexture::CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;

    // Disable optimizations to further improve shader debugging
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ID3DBlob* pErrorBlob = nullptr;
    //D3DCompileFromFile : ���̴� �ҽ��ڵ带 ���������ִ� �Լ�
    hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
            pErrorBlob->Release();
        }
        return hr;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}