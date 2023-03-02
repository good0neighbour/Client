#include "CMtlTexture.h"


#include "CDxEngine.h"
#include "CD3D.h"

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
	SetShaderParameter(mpEngine->mpD3D->GetImmediateDevice(), tpTexture);

	RenderShader(mpEngine->mpD3D->GetImmediateDevice(), tCountIndex);
}

void CMtlTexture::CreateShader(ID3D11Device* tpDevice)
{
    ID3DBlob* pVSBlob = nullptr;

    CompileShaderFromFile(L"VertexShader.hlsl", "main", "vs_4_0", &pVSBlob);
    tpDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &mpVertexShader);


    //�Է·��̾ƿ� ���� : �������α׷����� ������ �������� ������ ���̴����� ������  �������� ������ ��ġ�Ǵ��� �Ǵ��ϱ� ���� ������ 
    D3D11_INPUT_ELEMENT_DESC tLayout[] =
    {
        //�ø�ƽ �̸�, �ø�ƽ ��ȣ, Ÿ��, �������۸޸��� ���� �ε���(0~15), ������, ��޿ɼ�, ��޿ɼ�
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
        //16+12 = 28 offset
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0}

        //{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
        //COLOR�� float�� 4���̹Ƿ� �������� 16���� ����
    };

    UINT numElements = ARRAYSIZE(tLayout);
    //layout��ü ����
    tpDevice->CreateInputLayout(tLayout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &mpVertexLayout);

    pVSBlob->Release();

    




    ID3DBlob* pPSBlob = nullptr;

    CompileShaderFromFile(L"PixelShader.hlsl", "main", "ps_4_0", &pPSBlob);
    tpDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mpPixelShader);


    pPSBlob->Release();





    D3D11_SAMPLER_DESC tSampleDesc = {};
    tSampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;          //�������� MIN:��� MAG:Ȯ�� MIP: �ػ󵵼��غ� �ؽ�ó 
    tSampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;         //UV�ݺ�ó���� ��Ǯ��, ��踦 �Ѿ�� �� ó���� �ݺ�
    tSampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;         //<--3���� �ؽ�ó ���䵵 �ִ�
    tSampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;       //���ø��������� ���Լ��� ���� ����
    tSampleDesc.MinLOD = 0;                    //LOD�� �ּҰ� level of detail, LOD�ܰ� ����
    tSampleDesc.MaxLOD = D3D11_FLOAT32_MAX;    //LOD�� �ִ밪

    //���÷� ��ü ����
    tpDevice->CreateSamplerState(&tSampleDesc, &mpSamplerStateLinear);
}
void CMtlTexture::DestroyShader()
{
    if (mpSamplerStateLinear)
        mpSamplerStateLinear->Release();


    if (mpVertexShader)
        mpVertexShader->Release();

    if (mpPixelShader)
        mpPixelShader->Release();

    if (mpVertexLayout)
        mpVertexLayout->Release();
}
void CMtlTexture::RenderShader(ID3D11DeviceContext* tpDeviceContext, int tCountIndex)
{    
    tpDeviceContext->IASetInputLayout(mpVertexLayout);

    tpDeviceContext->VSSetShader(mpVertexShader, nullptr, 0);
    tpDeviceContext->PSSetShader(mpPixelShader, nullptr, 0);
    tpDeviceContext->PSSetSamplers(0, 1, &mpSamplerStateLinear);



    tpDeviceContext->DrawIndexed(tCountIndex, 0, 0);
}

void CMtlTexture::SetShaderParameter(ID3D11DeviceContext* tpDeviceContext, ID3D11ShaderResourceView* tpTexture)
{
    tpDeviceContext->PSSetShaderResources(0, 1, &tpTexture);
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