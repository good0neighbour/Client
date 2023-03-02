#include "framework.h"
//#pragma comment(lib, "winAPIEngine.lib")


#include "CDxEngine.h"

#include "DDSTextureLoader.h"

#include "ryubuffer.h"
#include "CMesh.h"

#include "CMtlTexture.h"

/*
    3d 기반의 2d 랜더링하기
    <-- 2d UI, 2d Sprite, text rendering


    i) 직교 투영 orthographic projection

        perspective projection 원근투영 vs orthographic projection 직교투영


    ii) Depth Buffer( Z Buffer ) 해제


    iii) 동적 정점 버퍼 Dynamic Vertex Buffer를 이용


        가) vertex buffer
            우리가 지금 정적 버퍼vertex buffer를 이용하고 있는 방법

            응용프로그램 수준에서 데이터를 만들고 IA에 입력한다.
            (<-- 그러면 GPU는 CPU의 간섭을 받지 않고 해당 데이터vertex buffer를 이용하여 랜더링한다.
            이렇게 하는 이유는, 전체적인 랜더링 파이프라인관점에서 보면 CPU의 간섭이 있을 시 전체 랜더링 속도가 느려지기 때문이다.
            왜냐하면, CPU와 GPU는 처리 속도가 다르기 때문이다.)

            한번 만든 데이터는 바뀌지 않는다

        ------------------------------------------------------------
        CPU                             GPU
        ------------------------------------------------------------
        시스템 메모리     --->        --IA에 전달 --> vertex biffer
        ------------------------------------------------------------

        나) constant buffer

            UpdateSubresource 함수를 이용하여 업데이트한다
            한 프레임당 한 번씩 갱신되어야 하는 자원들을 위해 만들어진 구조다.
            상수버퍼 자체는 바뀌지 않는다. 값이 갱신되는 것이다.

        ------------------------------------------------------------
        CPU                                                          GPU
        ------------------------------------------------------------
        시스템 메모리     시스템 메모리(상수버퍼용)      --->        상수버퍼
        ------------------------------------------------------------



        vs

        Dynamic Buffer옵션으로 설정한 vertex buffer

        :CPU가 직접 데이터를 그래픽 처리 장치에 써넣는다. 그렇게 하기위한 구조가 제공된다.
        Map/ UnMap 을 사용하여 데이터를 갱신한다.

        <-- 정점 버퍼의 데이터의 값이
        응용프로그램 수준에서
        지속적으로 끊임없이 갱신(변경)되어야 하는 경우를 위해서 이런 옵션이 존재한다
        예) 스프라이트 애니메이션의 경우 정점 버퍼의 uv좌표 정보가 계속 변경되어야만 한다.


        ------------------------------------------------------------
        CPU                                                          GPU
        ------------------------------------------------------------
        시스템 메모리     --> mapResources    --->    직접데이터갱신 vertex buffer
        ------------------------------------------------------------

        그러므로, 당연히 IA에 설정하여 렌더링하도록 만들어진 vertex buffer 보다는 기본적으로 느리다
        하지만, 성능상 붙이기 없도록 옵션이 존재한다.
*/





//선언 
HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);


class CRyuEngine : public CDxEngine
{   
    CMesh* mpMesh = nullptr;
    CMtlTexture* mpMtlTexture = nullptr;

    
    //상수 버퍼( 변환행렬 용 )
    ID3D11Buffer* mpCBTransform = nullptr;

    //응용프로그램 수준에서 다룰 변환행렬들
    XMMATRIX mMatWorld_0;            //월드변환 행렬 <--- 첫번째 정육면체 용
    XMMATRIX mMatWorld_1;       //월드변환 행렬 <--- 두번째 정육면체 용
    XMMATRIX mMatView;              //뷰변환 행렬
    XMMATRIX mMatProjection;       //투영변환 행렬 



    //카메라 위치
    XMFLOAT4 mCameraPosition;


  
    //ID3D11ShaderResourceView* mpTexture = nullptr;
  
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




        mpMtlTexture = new CMtlTexture();
        mpMtlTexture->Create(mpD3D->GetD3DDevice());
        mpMtlTexture->SetEngine(this);


        mpMesh = new CMesh();
        //mpMesh->Create(mpD3D->GetD3DDevice(), L"resources/testUV256by256.dds");
        mpMesh->Create(mpD3D->GetD3DDevice(), L"resources/slimeby.dds");
        //mpMesh->Create(mpD3D->GetD3DDevice(), L"resources/seafloor.dds");
        mpMesh->SetEngine(this);

        

        //텍스처 이미지 데이터를 파일에서 로드하여  mpTexture에 담음
        //CreateDDSTextureFromFile(mpD3D->GetD3DDevice(), L"resources/testUV256by256.dds", nullptr, &mpTexture);



        //상수 버퍼 생성
        //주설명
        D3D11_BUFFER_DESC tBd = {};
        tBd.Usage = D3D11_USAGE_DEFAULT;            //기본용도다 
        tBd.ByteWidth = sizeof(CBTransform) * 1;    //해당 상수버퍼의 메모리 크기를 알린다
        tBd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//constant buffer용도다
        tBd.CPUAccessFlags = 0;     //CPU의 접근은 불허한다

        //부설명 은 필요없다. 
        // UpdateSubResources을 이용하여 매 프레임마다 데이터의 값을 갱신할 것이기 때문이다.
        // 
        //D3D11_SUBRESOURCE_DATA InitData = {};
        //InitData.pSysMem = tVertices;       //해당 시스템 메모리의 데이터의 내용을 넘겨준다

        mpD3D->GetD3DDevice()->CreateBuffer(&tBd, nullptr, &mpCBTransform);



        //변환행렬들 설정 
        //월드변환 행렬
        mMatWorld_0 = XMMatrixIdentity();     //단위행렬로 초기화

        mMatWorld_1 = XMMatrixIdentity();     //단위행렬로 초기화




        //카메라 arm의 길이
        float tLength = 15.0f;//2.0f;//15.0f;

        mCameraPosition.x = 0.0f;
        mCameraPosition.y = 1.0f*tLength;
        mCameraPosition.z = -1.0f * tLength;
        mCameraPosition.w = 1.0f;



        //뷰변환 행렬
        mMatView = XMMatrixIdentity();

        //XMVECTOR tEye = XMVectorSet(0.0f, 2.0f, -5.0f, 1.0f); //카메라의 위치
        XMVECTOR tEye = XMVectorSet(mCameraPosition.x, mCameraPosition.y, mCameraPosition.z, 1.0f); //카메라의 위치
        XMVECTOR tAt = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f); //바라보는 응시점위치
        XMVECTOR tUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //월드에 상방벡터

        //뷰변환 행렬을 구한다. ~LH Left Hand 왼손좌표계
        mMatView = XMMatrixLookAtLH(tEye, tAt, tUp);


        //투영변환 행렬 

        //원근 투영 변환 행렬 만들기
        //mMatProjection = XMMatrixIdentity();

        //RECT rc = { 0 };
        //GetClientRect(this->mhWnd, &rc);    //window api
        //unsigned int tWidth = rc.right - rc.left;
        //unsigned int tHeight = rc.bottom - rc.top;//윈도우 좌표계 상에서 계산

        //mMatProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, (float)tWidth / (float)tHeight, 0.01f, 100.0f);

        //직교 퉁영 변환 행렬 만들기 
        mMatProjection = XMMatrixIdentity();

        RECT rc = { 0 };
        GetClientRect(this->mhWnd, &rc);    //window api
        unsigned int tWidth = rc.right - rc.left;
        unsigned int tHeight = rc.bottom - rc.top;//윈도우 좌표계 상에서 계산

        //픽셀 당 유닛 
        //float tPixelPerUnit = 1.0f;
        float tPixelPerUnit = 0.05f;
        //1유닛당 몇개의 픽셀이 들어있는가?

        mMatProjection = XMMatrixOrthographicLH(tPixelPerUnit*tWidth, tPixelPerUnit * tHeight, 0.01f, 100.0f);
    }
    virtual void OnDestroy() override
    {        
        if (mpMtlTexture)
        {
            mpMtlTexture->Destroy();

            delete mpMtlTexture;
            mpMtlTexture = nullptr;
        }

        if (mpMesh)
        {
            mpMesh->Destroy();

            delete mpMesh;
            mpMesh = nullptr;
        }

    

        CDxEngine::OnDestroy();
    }
    virtual void OnUpdate(float tDeltaTime) override
    {
        CDxEngine::OnUpdate(tDeltaTime);

        //각도 증가
        static float tTheta = 0.0f;
        tTheta = tTheta + 0.5f * tDeltaTime;
        //y축을 회전축으로 회전하는 회전변환행렬
        //mMatWorld_0 = XMMatrixRotationY(tTheta);


        XMMATRIX tRotation = XMMatrixRotationY(tTheta * 2.0f);
        //XMMATRIX tTranslation = XMMatrixTranslation(3.0f, 0.0f, 0.0f);  //x축으로 3만큼 이동
        XMMATRIX tTranslation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);  //z축으로 20만큼 이동

        //mMatWorld_1 = XMMatrixRotationY(tTheta*3.0f);

        //DirectX의 응용프로그램 수준의 코드는 '행벡터'를 기준으로 한다.( 그러므로 행렬의 앞에 벡터가 곱해지는 것이다 )
        //mMatWorld_1 = tRotation * tTranslation;     //행벡터 * 회전변환행렬*이동변환행렬
        mMatWorld_0 = tRotation * tTranslation;     //행벡터 * 회전변환행렬*이동변환행렬

        //회전하고 나서 그리고나서 이동한다. 회전의 축은 월드좌표계 상의 y축이다. (정점단위로 적용된다)

        //mMatWorld_1 = tTranslation* tRotation;     //행벡터 *이동변환행렬* 회전변환행렬

        //이동하고 나서 그리고 회전한다. 회전의 축은 월드좌표계 상의 y축이다. (정점단위로 적용된다)


        //빛 정보 생성
        //난반사 관련 조명 정보 
        XMFLOAT4 tLightDir = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);//xyzw 빛의 방향은 전방(z+)
        XMFLOAT4 tLightColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);//rgba

        //주변광 관련 조명 정보 
        XMFLOAT4 tAmbientColor = XMFLOAT4(0.1f, 0.1f, 0.3f, 1.0f);//rgba

        //정반사광 관련 조명 정보
        XMFLOAT4 tSpecularColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);//rgba
        float tPower = 20.0f;//1.0f;//8.0f;


        // XMFLOAT4 --> XMVECTOR --> XMFLOAT4
        //XMMATRIX tRotateLight = XMMatrixRotationX(0.5f * tTheta);//x축 회전축으로 회전변환행렬
        //XMVECTOR tVectorLight = XMLoadFloat4(&tLightDir);

        //tVectorLight = XMVector3Transform(tVectorLight, tRotateLight);

        //XMStoreFloat4(&tLightDir, tVectorLight);

        //XMVECTOR는  GPU연산에 최적화되어 만들어진 타입이다.
        //그래서 컴파일 후에 16바이트 단위로 정렬되도록 만들어진다.
        //그런데 이 정렬은 지역변수, 매개변수, 전역변수 등에만 적용되고 멤버변수에는 적용되지 않는다고 한다.
        // ------> 그래서, 응용프로그램 코드에 멤버변수는 주로 XMFLOAT계열이 사용되었다. <----- 
        /*
            //XMLoadFloat4:
                                        XMFLOAT4 ---> XMVECTOR

            //XMStoreFloat4:
                                        XMVECTOR ----> XMFLOAT4
        */





        this->Clear(Colors::DarkOrchid);

      
        mpD3D->GetImmediateDevice()->VSSetConstantBuffers(0, 1, &mpCBTransform);
        mpD3D->GetImmediateDevice()->PSSetConstantBuffers(0, 1, &mpCBTransform);

        //첫번째 육면체 랜더링==================================
        
        //directX의 응용프로그램 에서 시스템 스택에 마련된 상수버퍼 데이터 
        // 
        //directX 응용프로그램에서는 행렬연산을 '행벡터'를 기준으로 한다
        //hlsl셰이더 프로그램에서는 행렬연산을 '열벡터'를 기준으로 한다
        CBTransform tCB;
        tCB.mWorld = XMMatrixTranspose(mMatWorld_0);
        tCB.mView = XMMatrixTranspose(mMatView);
        tCB.mProjection = XMMatrixTranspose(mMatProjection);
        //빛 정보를 상수버퍼를 매개로 셰이더 측에 전달하기 위해 설정
        //난반사광 관련 정보 전달 
        tCB.mLightDir = tLightDir;
        tCB.mLightColor = tLightColor;
        //주변광 관련 정보 전달
        tCB.mAmbientColor = tAmbientColor;
        //정반사광 관련 정보 전달
        tCB.mCameraPosition = mCameraPosition;
        tCB.mSpecularColor = tSpecularColor;
        tCB.mSpecularPower = tPower;
        

        //셰이더 측에 상수버퍼에 값 갱신
        mpD3D->GetImmediateDevice()->UpdateSubresource(mpCBTransform, 0, nullptr, &tCB, 0, 0);

    


        mpMesh->Render();
        mpMtlTexture->Render(mpMesh->GetCountIndex(), mpMesh->GetTexture());

        //================================
        // 2D render 모드를 위해, 깊이버퍼를 끈다
        mpD3D->DoTurnOffDepthBuffer();




        // 2D render 모드를 위해, 깊이버퍼를 켠다
        mpD3D->DoTurnOnDepthBuffer();
        //================================

                
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