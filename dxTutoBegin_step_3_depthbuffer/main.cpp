#include "framework.h"
//#pragma comment(lib, "winAPIEngine.lib")


#include "CDxEngine.h"

/*
    dxTutoBegin_step_2

    이번 스텝에서는 깊이버퍼 Depth Buffer( Z Buffer )를 적용하자.

    i) 두 번째 정육면체를 렌더링해보자

    ii) 변환행렬을 조작해보자

    iii) 깊이버퍼를 적용해보자.

        앞쪽에 있는 물체가 뒷쪽에 있는 물체를 가릴 수 있는 처리가 필요하다.
        이를 위한 처리 알고리즘에는
        크게 두 가지 방법이 있다.

        a) Z sort: 면 단위다. 정렬해야 하므로 입력데이터(삼각형)가 많아질 수록 정렬에 시간이 오래 걸리게 된다.

        b) Z buffer( Depth Buffer ): 물체까지의 거리(z, depth)를 픽셀 단위로 깊이버퍼에 기억시켜두고
        렌더링시 렌더링하려는 물체의 거리와 이미 깊이버퍼에 기록되어 있는 거리를 비교하여 앞에 있다고 판단되면 렌더링한다.

        깊이버퍼는 [0, 1]로 정규화되어있다.

        z값이 작을 수록 앞에 있는 것이다.

        픽셀단위이므로 소요시간은 O(1)이다.
        <-- 삼각형 전체를 정렬하는 것에 비해 매우 효율적이다.
*/

//선언 
HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

//응용 프로그램 수준에서 정의하였다
//정점의 정의
struct SVertex
{    
    XMFLOAT4 position;          //정점의 위치 정보 xyzw
    XMFLOAT4 color;                 //정점의 색상 정보 rgba
};
//응용 프로그램 수준에서 정의하였다
//시스템 메모리에 들 정의
struct CBTransform
{
    XMMATRIX mWorld;        //월드변환 행렬
    XMMATRIX mView;         //뷰변환 행렬
    XMMATRIX mProjection;   //투영변환 행렬
};

//월드변환 행렬: local 좌표계 상의 정점의 위치 --> world좌표계 상의 정점의 위치
//뷰변환 행렬: world좌표계 상의 정점의 위치 --> view좌표계 상의 정점의 위치
//투영변환 행렬: view좌표계 상의 정점의 위치 --> 정규뷰 기준에 정점의 위치로 변환하고 근평면에 투영한 정점의 위치로 변환한 것

class CRyuEngine : public CDxEngine
{   
    //다음은 각각 vertex shader와 pixel shader객체이다.
    ID3D11VertexShader* mpVertexShader = nullptr;
    ID3D11PixelShader* mpPixelShader = nullptr;

    //응용프로그램 수준에서 넘어온 입력데이터( 기하구조물( 정점들, ... ) 데이터가 
    //렌더링 파이프라인에서 요구하는 정점의 데이터 인지 여부를 검사하기 위한 객체
    ID3D11InputLayout* mpVertexLayout = nullptr;
    
    //buffer <--DirectX에서 정의하고 있는 resource자원 중 하나이다. 
    //렌더링에 사용할 정점데이터들을 담아두는 '연속적인 메모리 블럭' 
    //  !!!그래픽 카드 장치에 VRAM에 있는 것이다.
    ID3D11Buffer* mpVertexBuffer = nullptr;

    //인덱스 버퍼
    ID3D11Buffer* mpIndexBuffer = nullptr;

    //상수 버퍼 ( 변환행렬 용 )
    ID3D11Buffer* mpCBTransform = nullptr;

    //응용프로그램 수준에서 다룰 변환행렬들
    XMMATRIX mMatWorld_0;        //월드변환 행렬 <-- 첫 번째 정육면체 용

    XMMATRIX mMatWorld_1;        //월드변환 행렬 <-- 두 번째 정육면체 용
    
    XMMATRIX mMatView;         //뷰변환 행렬
    XMMATRIX mMatProjection;   //투영변환 행렬

public:
    CRyuEngine() {};
    virtual ~CRyuEngine() {};

private:
    CRyuEngine(const CRyuEngine& t) = delete;
    CRyuEngine& operator=(const CRyuEngine& t) = delete;

public:
    virtual void OnCreate() override
    {
        CDxEngine::OnCreate();


        /*
            hlsl: High Level Shader Language 
                        DirectX에서 제공하는 고수준 셰이더 프로그래밍 언어

            GLSL: OpenGL의 고수준 셰이더 프로그래밍 언어
            CG: 엔비디아 의 셰이더 언어 

                                                Compile
            shader source code file  --->               Byte  Code( 중간 단계의 컴파일 결과물 )
                                                                        Blob에 담긴다               ---> shader object를 만든다. 
        
        */

        //shader object를 만들자 
        ID3DBlob* pVSBlob = nullptr;

        CompileShaderFromFile(L"VertexShader.hlsl", "main", "vs_4_0", &pVSBlob);
        mpd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &mpVertexShader);

        //입력레이아웃 정보 : 응용프로그램에서 정의한 데이터의 구조와 셰이더에서 정의한  데이터의 구조가 합치되는지 판단하기 위한 데이터 
                D3D11_INPUT_ELEMENT_DESC tLayout[] =
                {
                    //시맨틱 이름, 시맨틱 번호, 타입, 정점버퍼메모리의 슬롯 인덱스(0~15), 오프셋, 고급옵션, 고급옵션
                    { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
                    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
                };

                UINT numElements = ARRAYSIZE(tLayout);
                //layout객체 생성
                mpd3dDevice->CreateInputLayout(tLayout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &mpVertexLayout);

        pVSBlob->Release();

        //IA에 layout설정
        //IA는 다이렉트 엑스의 랜더링 파이프라인의 첫 단계(관문)이다.
        //( 응용프로그램에서 넘어온 데이터가 셰이더 입장에서 호환가능한 데이터인지 판별한다 )
        mpImmediateContext->IASetInputLayout(mpVertexLayout);




        ID3DBlob* pPSBlob = nullptr;

        CompileShaderFromFile(L"PixelShader.hlsl", "main", "ps_4_0", &pPSBlob);
        mpd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mpPixelShader);


        pPSBlob->Release();




        //응용프로그램 수준에 정점 데이터를 생성
        //삼각형을 만들자. ( 정점 3개 )
        /*
            DirectX의 좌표계(왼손좌표계)
            기본적으로 화면에서 원점은 화면중앙에 위치하고 
            x축 양의 방향은 우방, 
            y축 양의 방향은 상방이다.
        
            DirectX는 CW 를 전면을 인식한다. 

        */
        //아무 세팅도 없다면, 카메라의 위치는 암묵적으로 원점0,0,0이므로
        //해당 삼각형의 z값을 0으로 하면 관찰되지 않는다
        //그러므로 0.5로 z값을 설정했다. 
        //(응용프로그램이므로 지역변수이므로 System 스택 메모리에 있다)
        //SVertex tVertices[] =
        //{
        //    //CW로 정점을 나열하겠다
        //    //정점이므로 마지막 값을 1로 했다
        //    //xyzw,                             //rgba
        //    XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
        //    XMFLOAT4(0.0f, 1.0f, 0.5f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
        //    XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)
        //};

        //정육면체를 렌더링하기 위한 데이터( 공유되는 정점은 그냥 하나로 포기 )
        SVertex tVertices[] =
        {
            {XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
            {XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
            {XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
            {XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},

            {XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
            {XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
            {XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
            {XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)}
        };

        //주설명
        D3D11_BUFFER_DESC tBd = {};
        tBd.Usage = D3D11_USAGE_DEFAULT;            //기본용도다 
        tBd.ByteWidth = sizeof(SVertex) * 8;    //해당 기하구조의 메모리 크기를 알린다
        tBd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//vertex buffer용도다
        tBd.CPUAccessFlags = 0;     //CPU의 접근은 불허한다

        //부설명
        D3D11_SUBRESOURCE_DATA InitData = {};
        InitData.pSysMem = tVertices;       //해당 시스템 메모리의 데이터의 내용을 넘겨준다

        //정점 버퍼vertex buffer 데이터를 생성
        mpd3dDevice->CreateBuffer(&tBd, &InitData, &mpVertexBuffer);

        UINT stride = sizeof(SVertex);      //주어진 buffer를 해석할때 정점의 갯수 단위를 해석하는 경계 크기
        UINT offset = 0;                        //해당 buffer에 시작점에서 얼만큼 떨어져 있는지 
        //IA에 vertex buffer 설정(입력)
        mpImmediateContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);

        //인덱스 버퍼
        WORD indices[] =
        {
            3, 1, 0, //<--예시, 위쪽 면
            2, 1, 3, //<--예시, 위쪽 면

            0, 5, 4,
            1, 5, 0,

            3, 4, 7,
            0, 4, 3,

            1, 6, 5,
            2, 6, 1,

            2, 7, 6,
            3, 7, 2,

            6, 4, 5,
            7, 4, 6
        };

        //주설명
        //D3D11_BUFFER_DESC tBd = {};
        tBd.Usage = D3D11_USAGE_DEFAULT;            //기본용도다 
        tBd.ByteWidth = sizeof(WORD) * 36;    //해당 기하구조의 메모리 크기를 알린다
        tBd.BindFlags = D3D11_BIND_INDEX_BUFFER;//index buffer용도다
        tBd.CPUAccessFlags = 0;     //CPU의 접근은 불허한다

        //부설명
        //D3D11_SUBRESOURCE_DATA InitData = {};
        InitData.pSysMem = indices;       //해당 시스템 메모리의 데이터의 내용을 넘겨준다

        mpd3dDevice->CreateBuffer(&tBd, &InitData, &mpIndexBuffer);
        //WORD라서 DXGI_FORMAT_R16_UINT 포맷을 설정
        mpImmediateContext->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
        
        //기하구조(도형)의 위상구조를 IA 에 설정(입력)한다
        //임의의 주어진 정점을 가지고 임의의 도형으로 어떻게 해석할 것인가?에 대한 규칙을 정해준다.
        mpImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        /*
        
        topology


        POINTLIST

            모든 정점을 개별으로 그린다

        LINESTRIP

            차례로 연결되는 선분들을 그린다 <-- 한붓그리기한다

        LINELIST

            매 정점 두개( 시점, 종점 )를 이용하여 선분 하나씩을 그린다

        TRIANGLESTRIP

            매 정점 세개를 이용하여 그린다 <-- 한붓그리기한다
            --> 이렇게 그리면, 짝수번째 삼각형은 뒤집힌 형태이지만, directx가 그것을 올바른 형태로 잡아준다

        TRIANGLELIST
        
            매 정점 세개를 이용하여 삼각형을 하나씩 그린다.
        
        */

        //상수 버퍼 생성
        //주설명
        //D3D11_BUFFER_DESC tBd = {};
        tBd.Usage = D3D11_USAGE_DEFAULT;            //기본용도다 
        tBd.ByteWidth = sizeof(CBTransform) * 1;    //해당 상수버퍼의 메모리 크기를 알린다
        tBd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//constant buffer용도다
        tBd.CPUAccessFlags = 0;     //CPU의 접근은 불허한다

        //부설명은 필요없다. UpdateSubResources를 이용하여 매 프레임마다 데이터의 값을 갱신할 것이기 때문이다.
        //D3D11_SUBRESOURCE_DATA InitData = {};
        //InitData.pSysMem = tVertices;       //해당 시스템 메모리의 데이터의 내용을 넘겨준다

        mpd3dDevice->CreateBuffer(&tBd, nullptr, &mpCBTransform);

        //변환행렬들 설정
        //월드변환 행렬
        mMatWorld_0 = XMMatrixIdentity(); //단위행렬로 초기화

        mMatWorld_1 = XMMatrixIdentity(); //단위행렬로 초기화

        //뷰변환 행렬
        mMatView = XMMatrixIdentity();
        
        XMVECTOR tEye = XMVectorSet(0.0f, 2.0f, -5.0f, 1.0f);//카메라의 위치
        XMVECTOR tAt = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);//바라보는 응시점위치
        XMVECTOR tUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);//월드의 상방벡터

        //뷰변환 행렬을 구한다. ~LH Left Hand 왼손좌표계
        mMatView = XMMatrixLookAtLH(tEye, tAt, tUp);

        //투영변환 행렬
        mMatProjection = XMMatrixIdentity();

        RECT rc;
        GetClientRect(this->mhWnd, &rc);    //window api
        unsigned int tWidth = rc.right - rc.left;
        unsigned int tHeight = rc.bottom - rc.top;//윈도우 조표계 상에서 계산

        mMatProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, (float)tWidth / (float)tHeight, 0.01f, 100.0f);
    }
    virtual void OnDestroy() override
    {
        if (mpIndexBuffer)
            mpIndexBuffer->Release();

        if (mpCBTransform)
            mpCBTransform->Release();

        if (mpVertexShader)
            mpVertexShader->Release();
        
        if (mpPixelShader)
            mpPixelShader->Release();

        if (mpVertexLayout)
            mpVertexLayout->Release();

        if (mpVertexBuffer)
            mpVertexBuffer->Release();

        CDxEngine::OnDestroy();
    }
    virtual void OnUpdate(float tDeltaTime) override
    {
        CDxEngine::OnUpdate(tDeltaTime);

        //각도 증가
        static float tTheta = 0.0f;
        tTheta = tTheta + 2.5f * tDeltaTime;
        //y축을 회전축으로 회전하는 회전변환행렬
        mMatWorld_0 = XMMatrixRotationY(tTheta);

        XMMATRIX tRotation = XMMatrixRotationY(tTheta * 1.5f);
        XMMATRIX tTranslation = XMMatrixTranslation(3.0f, 0.0f, 0.0f); //x축으로 3만큼 이동

        //mMatWorld_1 = XMMatrixRotationY(tTheta * 3.0f);

        //DirectX의 응용프로그램 수준의 코드는 '행벡터'를 기준으로 한다.( 그러므로 행렬의 앞에 벡터가 곱해지는 것이다 )
        //mMatWorld_1 = tRotation * tTranslation; //행벡터 * 회전변환행렬 * 이동변환행렬

        //회전하고 나서 그리고 나서 이동한다.

        mMatWorld_1 = tTranslation * tRotation; //행벡터 * 이동변환행렬 * 회전변환행렬

        //이동하고 나서 그리고 회전한다. 회전의 축은 월드 좌표계 상의 y축이다. (정점단위로 적용된다)

        //this->Clear(0.0f, 0.0f, 1.0f);  
        this->Clear(Colors::DarkOrchid);

        //vertex shader 를 vertex shading 단계에 설정, //pixel shader 를 pixel shading 단계에 설정, 
        mpImmediateContext->VSSetShader(mpVertexShader, nullptr, 0);
        //상수버퍼를 vertex shading 단계에 설정
        mpImmediateContext->VSSetConstantBuffers(0, 1, &mpCBTransform);

        mpImmediateContext->PSSetShader(mpPixelShader, nullptr, 0);

        //첫 번째 육면체 렌더링=========================================

        //directX의 응용프로그램에서 시스템 스택에 마련된 상수버퍼 데이터
        //
        //directX 응용프로그램에서는 행렬연산을 '행벡터'를 기준으로 한다
        //hlsl셰이더 프로그램에서는 행렬연산을 '열벡터'를 기준으로 한다
        CBTransform tCB;
        tCB.mWorld = XMMatrixTranspose(mMatWorld_0);
        tCB.mView = XMMatrixTranspose(mMatView);
        tCB.mProjection = XMMatrixTranspose(mMatProjection);

        //셰이더 측에 상수버퍼에 값 갱신
        mpImmediateContext->UpdateSubresource(mpCBTransform, 0, nullptr, &tCB, 0, 0);

        //인덱스 정보를 이용하여 그려라
        mpImmediateContext->DrawIndexed(36, 0, 0);

        //두 번째 정육면체 렌더링=========================================
        //CBTransform tCB;
        tCB.mWorld = XMMatrixTranspose(mMatWorld_1);
        tCB.mView = XMMatrixTranspose(mMatView);
        tCB.mProjection = XMMatrixTranspose(mMatProjection);

        //셰이더 측에 상수버퍼에 값 갱신
        mpImmediateContext->UpdateSubresource(mpCBTransform, 0, nullptr, &tCB, 0, 0);

	    //인덱스 정보를 이용하여 그려라
        mpImmediateContext->DrawIndexed(36, 0, 0);

        //정점버퍼의 내용으로 랜더링할 것을 명령
        //mpImmediateContext->Draw(3, 0);//GPU에 보낼 정점의 갯수, 보내기 시작할 첫번째 정점의 인덱스

                
        this->Present();
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


//정의
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