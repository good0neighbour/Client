/*
    created by pokpoongryu

    dxEngine_step_5_1 texmapping

    i) 텍스쳐 매핑

    //texmapping

    텍스쳐+컬러+법선 까지 적용한 버전을 만든다 
    
*/

#include "CDxEngine.h"
#include <stdio.h>

#include <d3dcompiler.h>
#include <directxmath.h>

#include "DDSTextureLoader.h"

struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT3 Normal;
    XMFLOAT2 Tex;       
};



//용도별로 여러 개의 상수버퍼를 만들어 적용할 수도 있다
//월드 뷰 프로젝션 행렬
struct ConstantBuffer
{
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;

    XMFLOAT4 vLightDir;
    XMFLOAT4 vLightColor;
};



class CRyuEngine : public CDxEngine
{
    ID3D11VertexShader* mpVertexShader = nullptr;
    ID3D11PixelShader* mpPixelShader = nullptr;

    ID3D11InputLayout* mpVertexLayout = nullptr;

    ID3D11Buffer* mpVertexBuffer = nullptr;  //vertex buffer이다. 이번 예시에서는 삼각형을 만들 것이다 

        
        ID3D11Buffer* mpIndexBuffer = nullptr;




    
    ID3D11Buffer* mpConstantBuffer = nullptr;//상수 버퍼
    //변환 행렬
    XMMATRIX mMatWorld_0;

    //translate
    XMMATRIX mMatWorld_1;

    XMMATRIX mMatView;
    XMMATRIX mMatProjection;



    //texmapping
    ID3D11ShaderResourceView* g_pTextureRV = nullptr;
    ID3D11SamplerState* g_pSamplerLinear = nullptr;

    //XMFLOAT4                            g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);




public:
    CRyuEngine() {};
    virtual ~CRyuEngine() {};

    virtual void OnCreate() override
    {
        CDxEngine::OnCreate();




        HRESULT hr = S_OK;
        //ryu
        // 여기서 VS, PS 는 함수 이름이다 szEntryPoint라는 이름으로 매개변수화되어 있다
        // Compile the vertex shader
        ID3DBlob* pVSBlob = nullptr;
        /*hr = CompileShaderFromFile(L"dxEngine_step_1.fxh", "VS", "vs_4_0", &pVSBlob);*/
        hr = CompileShaderFromFile(L"dxEngine_step_5_VS.hlsl", "main", "vs_4_0", &pVSBlob);
        if (FAILED(hr))
        {
            MessageBox(nullptr,
                L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
            //return hr;
        }

        // Create the vertex shader
        hr = mpd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &mpVertexShader);
        if (FAILED(hr))
        {
            pVSBlob->Release();
            //return hr;
        }

        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        UINT numElements = ARRAYSIZE(layout);

        hr = mpd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
            pVSBlob->GetBufferSize(), &mpVertexLayout);
        pVSBlob->Release();
//        if (FAILED(hr))
//            return hr;

        mpImmediateContext->IASetInputLayout(mpVertexLayout);







        // Compile the pixel shader
        ID3DBlob* pPSBlob = nullptr;
        hr = CompileShaderFromFile(L"dxEngine_step_5_PS.hlsl", "main", "ps_4_0", &pPSBlob);
        if (FAILED(hr))
        {
            MessageBox(nullptr,
                L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
            //return hr;
        }

        // Create the pixel shader
        hr = mpd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mpPixelShader);
        pPSBlob->Release();
        //if (FAILED(hr))
          //  return hr;




        SimpleVertex vertices[] =
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

        
        D3D11_BUFFER_DESC bd            = {};
        bd.Usage                        = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SimpleVertex) * 24;
        bd.BindFlags                    = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags               = 0;


        D3D11_SUBRESOURCE_DATA InitData     = {};
        InitData.pSysMem                    = vertices;

        hr = mpd3dDevice->CreateBuffer(&bd, &InitData, &mpVertexBuffer);
        /*if (FAILED(hr))
            return hr;*/



        //정점 버퍼의 단위와 오프셋을 설정한다
        UINT stride = sizeof(SimpleVertex);   //정점(실수3개짜리)이 단위가 된다는 것이다 
        UINT offset = 0;

        mpImmediateContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);


                        WORD indices[] =
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
                        bd.ByteWidth = sizeof(WORD) * 36;
                        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
                        bd.CPUAccessFlags = 0;
                        InitData.pSysMem = indices;
                        hr = mpd3dDevice->CreateBuffer(&bd, &InitData, &mpIndexBuffer);
                        /*if (FAILED(hr))
                            return hr;*/

                        mpImmediateContext->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

        mpImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        
        bd.Usage            = D3D11_USAGE_DEFAULT;
        bd.ByteWidth        = sizeof(ConstantBuffer);
        bd.BindFlags        = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags   = 0;
        hr = mpd3dDevice->CreateBuffer(&bd, nullptr, &mpConstantBuffer);
        /*if (FAILED(hr))
            return hr;*/

        hr = CreateDDSTextureFromFile(mpd3dDevice, L"resources/testUV256by256.dds", nullptr, &g_pTextureRV);

        D3D11_SAMPLER_DESC sampDesc = {};
        sampDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampDesc.MinLOD         = 0;
        sampDesc.MaxLOD         = D3D11_FLOAT32_MAX;
        hr = mpd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);






        mMatWorld_0 = XMMatrixIdentity();   //단위행렬
                
                XMVECTOR Eye = XMVectorSet(0.0f, 2.0f, -5.0f, 0.0f);
                XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
                XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

        mMatView = XMMatrixLookAtLH(Eye, At, Up);   //뷰변환행렬 얻기

        RECT rc;
        GetClientRect(mhWnd, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;
        //원근 투영 변환 행렬 얻기
        mMatProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f); 

        mMatWorld_1 = XMMatrixIdentity();

    }
    virtual void OnDestroy() override
    {
        
        if (mpConstantBuffer) mpConstantBuffer->Release();


        if (mpVertexBuffer) mpVertexBuffer->Release();

        if (mpVertexLayout) mpVertexLayout->Release();
        if (mpVertexShader) mpVertexShader->Release();
        if (mpPixelShader) mpPixelShader->Release();


        CDxEngine::OnDestroy();
    }
    virtual void OnUpdate(float tDeltaTime) override
    {
        CDxEngine::OnUpdate(tDeltaTime);

        static float t = 0.0f;
        t = t + 0.5f*tDeltaTime;

        
        mMatWorld_0 = XMMatrixRotationY(t);

        
        XMMATRIX mOrbit         = XMMatrixRotationY(t * 1.0f);
        XMMATRIX mTranslate     = XMMatrixTranslation(0.0f, 0.0f, 10.0f);
        //mMatWorld_1             = mTranslate * mOrbit;     //이동하고 회전 한 경우
        mMatWorld_1             = mOrbit*mTranslate;       //회전하고 이동 한 경우








        //조명에 대한 정보를 설정한다
        XMFLOAT4 vLightDirs =
        {
            XMFLOAT4(0.77f, -0.77f, 0.0f, 1.0f),
        };
        XMFLOAT4 vLightColors =
        {
            XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),     
        };

        XMVECTOR vLightDir      = XMLoadFloat4(&vLightDirs);        //float4 --> vector
        vLightDir               = vLightDir* (-1.0f);
        XMStoreFloat4(&vLightDirs, vLightDir);                  //vector --> float4




        ConstantBuffer cb;
        cb.mWorld       = XMMatrixTranspose(mMatWorld_0);
        cb.mView        = XMMatrixTranspose(mMatView);
        cb.mProjection  = XMMatrixTranspose(mMatProjection);
        cb.vLightDir    = vLightDirs;
        cb.vLightColor  = vLightColors;

        ConstantBuffer cb2;
        cb2.mWorld      = XMMatrixTranspose(mMatWorld_1);
        cb2.mView       = XMMatrixTranspose(mMatView);
        cb2.mProjection = XMMatrixTranspose(mMatProjection);
        cb2.vLightDir   = vLightDirs;
        cb2.vLightColor = vLightColors;






        this->Clear(Colors::MidnightBlue);
        



        






        mpImmediateContext->VSSetShader(mpVertexShader, nullptr, 0);
        mpImmediateContext->VSSetConstantBuffers(0, 1, &mpConstantBuffer);

        mpImmediateContext->PSSetShader(mpPixelShader, nullptr, 0);
        mpImmediateContext->PSSetConstantBuffers(0, 1, &mpConstantBuffer);
        //
        mpImmediateContext->PSSetShaderResources(0, 1, &g_pTextureRV);
        mpImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);






        
        mpImmediateContext->UpdateSubresource(mpConstantBuffer, 0, nullptr, &cb, 0, 0);
        mpImmediateContext->DrawIndexed(36, 0, 0); 


        mpImmediateContext->UpdateSubresource(mpConstantBuffer, 0, nullptr, &cb2, 0, 0);
        mpImmediateContext->DrawIndexed(36, 0, 0);


        this->Present();
    }






    HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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
    
};



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    CRyuEngine tEngine;
    tEngine.Create(hInstance, nCmdShow);


    MSG msg = { 0 };

    msg = tEngine.Run();

    return (int)msg.wParam;
}

