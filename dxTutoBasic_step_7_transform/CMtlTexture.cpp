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


    //입력레이아웃 정보 : 응용프로그램에서 정의한 데이터의 구조와 셰이더에서 정의한  데이터의 구조가 합치되는지 판단하기 위한 데이터 
    D3D11_INPUT_ELEMENT_DESC tLayout[] =
    {
        //시맨틱 이름, 시맨틱 번호, 타입, 정점버퍼메모리의 슬롯 인덱스(0~15), 오프셋, 고급옵션, 고급옵션
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
        //16+12 = 28 offset
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0}

        //{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
        //COLOR는 float가 4개이므로 오프셋을 16으로 설정
    };

    UINT numElements = ARRAYSIZE(tLayout);
    //layout객체 생성
    tpDevice->CreateInputLayout(tLayout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &mpVertexLayout);

    pVSBlob->Release();

    




    ID3DBlob* pPSBlob = nullptr;

    CompileShaderFromFile(L"PixelShader.hlsl", "main", "ps_4_0", &pPSBlob);
    tpDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mpPixelShader);


    pPSBlob->Release();





    D3D11_SAMPLER_DESC tSampleDesc = {};
    tSampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;          //선형보간 MIN:축소 MAG:확대 MIP: 해상도수준별 텍스처 
    tSampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;         //UV반복처리는 되풀이, 경계를 넘어갔을 시 처리는 반복
    tSampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;         //<--3차원 텍스처 개념도 있다
    tSampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;       //샘플링과정에서 비교함수는 따로 없음
    tSampleDesc.MinLOD = 0;                    //LOD에 최소값 level of detail, LOD단계 수준
    tSampleDesc.MaxLOD = D3D11_FLOAT32_MAX;    //LOD에 최대값

    //샘플러 객체 생성
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