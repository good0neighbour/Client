// winAPIEngine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

/*
    dxTutoBasic step_8 camera_1

    카메라 개념을 클래스화해보자

*/
/*
    created by pokpoongryu
    교사용

    //dxTutoBasic_step_8_camera_1

    이번 예시에서는
    이전 step에서 만든 카메라 클래스를 수정하여
        3인칭 카메라 시점
        1인칭 카메라 시점
    등을 만들어보자

    그 과정에서 변환, 변환의 계층 에 대해서도 심화하여 이해해보자
    ( 주로 CCameraRyu의 Render 부분을 수정해보겠다 )

    i) CCameraRyu


    //camera_ryu


    --부모의 transform이라는 개념이 적용되어야 한다!!!!!!--
    mpParent


    //사실상 두개는 같다
    void CCameraRyu::UpdateMatViewFP();
    void CCameraRyu::UpdateMatView();
*/


#include "framework.h"
#include "CDXEngine.h"
#include <time.h>
#include <vector>
#include <algorithm>

//texture
#include "DDSTextureLoader.h"
#include "ryubuffer.h"
#include "CMesh.h"
#include "CMtlTexture.h"
#include "CBitmapRect.h"
#include "CMtlUnlit.h"

#include "CActor.h"
#include "CCamera.h"

using namespace std;

HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

class CRyuEngine : public CDXEngine
{
    //상수 버퍼(변환행렬 용 )
    ID3D11Buffer* mpCBTransform = nullptr;      //for transform constanct buffer
    ID3D11Buffer* mpCBLight = nullptr;              //for light constant buffer

    //응용프로그램에서 사용할 변환행렬들
    XMMATRIX mMatWorld_0;
    XMMATRIX mMatWorld_1;
    XMMATRIX mMatPerspective;
    XMMATRIX mMatOrthographic;

    XMMATRIX mMatView;


    //카메라 위치
    XMFLOAT4 mCameraPosition;


    CCamera* mpCamera = nullptr;


    CActor* mpSlime = nullptr;          //슬라임 게임 오브젝트
    CActor* mpLand = nullptr;          //바닥

    CActor* mpKnight = nullptr;        //경비병
    CActor* mpKnight_0 = nullptr;        //경비병_0



    CBitmapRect* mpBitmap = nullptr;
    CMtlUnlit* mpMtlUnlit = nullptr;


public:
    CRyuEngine() { OutputDebugStringW(L"CRyuEngine::CRyuEngine\n");  };
    virtual ~CRyuEngine() { OutputDebugStringW(L"CRyuEngine::~CRyuEngine\n");  };

    virtual void OnCreate() override
    {
        CDXEngine::OnCreate();


        CInputMgr::GetInstance()->AddKey("OnMoveLt", VK_LEFT);
        CInputMgr::GetInstance()->AddKey("OnMoveRt", VK_RIGHT);
        CInputMgr::GetInstance()->AddKey("OnMoveForward", VK_UP);
        CInputMgr::GetInstance()->AddKey("OnMoveBackward", VK_DOWN);

        CInputMgr::GetInstance()->AddKey("OnExit", VK_ESCAPE);

        mpSlime = new CActor();
        mpSlime->Create(this, L"resources/slime.obj", L"resources/slimeby.dds");

        mpLand= new CActor();
        mpLand->Create(this, L"resources/simpleworld.obj", L"resources/simpleworld.dds");
        mpLand->SetPosition(0.0f, -10.0f, 0.0f);
        mpLand->SetScale(200.0f, 20.0f, 200.0f);

        mpKnight = new CActor();
        mpKnight->Create(this, L"resources/chr_knight.obj", L"resources/chr_knight.dds");
        mpKnight->SetPosition(100.0f, 0.0f, 100.0f);
        //mpKnight->SetRotation(0.0f, 180.0f, 0.0f);
        mpKnight->SetScale(25.0f, 25.0f, 25.0f);

        mpKnight_0 = new CActor();
        mpKnight_0->Create(this, L"resources/chr_knight.obj", L"resources/chr_knight.dds");
        mpKnight_0->SetPosition(-100.0f, 0.0f, 100.0f);
        //mpKnight_0->SetRotation(0.0f, 90.0f, 0.0f);
        mpKnight_0->SetScale(25.0f, 25.0f, 25.0f);


        RECT tRect{};
        GetClientRect(this->mhWnd, &tRect);
        int tWidth = tRect.right - tRect.left;
        int tHeight = tRect.bottom - tRect.top;

        mpBitmap = new CBitmapRect();
        //mpBitmap->Create(this->GetD3DDevice(), L"resources/testUV256by256.dds", tWidth, tHeight, 128, 128);
        mpBitmap->Create(this->GetD3DDevice(), L"resources/test_title.dds", tWidth, tHeight, 128, 128);
        mpBitmap->SetEngine(this);


        mpMtlUnlit = new CMtlUnlit();
        mpMtlUnlit->Create(this->GetD3DDevice());
        mpMtlUnlit->SetEngine(this);


        //상수버퍼 생성
        // 
        // for transform
        //주설명
        D3D11_BUFFER_DESC tBd = {};
        tBd.Usage = D3D11_USAGE_DEFAULT;    //버퍼는 기본 용도 버퍼로 사용하겠다.
        tBd.ByteWidth = sizeof(CBTransform);    //데이터의 크기
        tBd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;       //constant buffer 용도로 만든다.
        tBd.CPUAccessFlags = 0;     //CPU의 접근은 불허한다
        //부설명 은 필요없다 UpdateSubResources을 이용하여 매 프레임마다 데이터의 값을 갱신한다
        GetD3DDevice()->CreateBuffer(&tBd, nullptr, &mpCBTransform);


        // for light
        //주설명
        D3D11_BUFFER_DESC tBdLight = {};
        tBdLight.Usage = D3D11_USAGE_DEFAULT;    //버퍼는 기본 용도 버퍼로 사용하겠다.
        tBdLight.ByteWidth = sizeof(CBLight);            //데이터의 크기
        tBdLight.BindFlags = D3D11_BIND_CONSTANT_BUFFER;       //constant buffer 용도로 만든다.
        tBdLight.CPUAccessFlags = 0;     //CPU의 접근은 불허한다
        //부설명 은 필요없다 UpdateSubResources을 이용하여 매 프레임마다 데이터의 값을 갱신한다
        GetD3DDevice()->CreateBuffer(&tBd, nullptr, &mpCBLight);




        //camera_ryu
        ////-----3인칭 follow camera 수치 설정-----
        float tLength = 55.0f;        
                mCameraPosition.x = 0.0f;
        mCameraPosition.y = 1.0f*tLength;
        mCameraPosition.z = -1.0f*tLength;
        mCameraPosition.w = 1.0f;

        //-----1인칭 follow camera 수치 설정-----
        /*float tLength = 1.0f;
        mCameraPosition.x = 0.0f;
        mCameraPosition.y = 20.0f;
        mCameraPosition.z = 0.0f;
        mCameraPosition.w = 1.0f;*/

        mpCamera = new CCamera();
        mpCamera->SetPosition(mCameraPosition.x, mCameraPosition.y, mCameraPosition.z);
        mpCamera->SetAtPoint(0.0f, 0.0f, 50.0f);    //fps
        
        ////mpCamera->SetRotation(0.0f, 45.0f, 0.0f);
        ////mpCamera->SetRotation(0.0f, 0.0f, 0.0f);
        ////mpCamera->SetRotation(-45.0f, 0.0f, 0.0f);
        ////mpCamera->SetParent(mpSlime);

        //camera_ryu
        //      follow camera fps
        mpCamera->SetParent(mpSlime);
        //      not follow camera
        //mpCamera->SetParent(nullptr);








        //변환행렬 들 설정
        mMatWorld_0 = XMMatrixIdentity();    //월드변환 행렬을 단위행렬로 설정
        mMatWorld_1 = XMMatrixIdentity();    //월드변환 행렬을 단위행렬로 설정

        mMatOrthographic = XMMatrixIdentity();    //투영변환 행렬을 단위행렬로 설정


        RECT rc;
        GetClientRect(this->mhWnd, &rc);
        unsigned int width = rc.right - rc.left;
        unsigned int height = rc.bottom - rc.top;

        float tNear = 0.01f;
        float tFar = 10000.0f;

        //원근 투영 변환으로 적용
        mMatPerspective = XMMatrixPerspectiveFovLH(XM_PIDIV2, (float)width / (float)height, tNear, tFar);

        float tPixelPerUnit = 1.0f;

        //직교 투영 변환으로 적용
        mMatOrthographic = XMMatrixOrthographicLH(width * tPixelPerUnit, height * tPixelPerUnit, 0.01f, 100.0f);

    };



    virtual void OnDestroy() override
    {

        if (mpCamera)
        {
            delete mpCamera;
            mpCamera = nullptr;
        }

        if (mpMtlUnlit)
        {
            mpMtlUnlit->Destroy();

            delete mpMtlUnlit;
            mpMtlUnlit = nullptr;
        }

        if (mpBitmap)
        {
            mpBitmap->Destroy();

            delete mpBitmap;
            mpBitmap = nullptr;
        }


        if (mpKnight_0)
        {
            mpKnight_0->Destroy();

            delete mpKnight_0;
            mpKnight_0 = nullptr;
        }


        if (mpKnight)
        {
            mpKnight->Destroy();

            delete mpKnight;
            mpKnight = nullptr;
        }



        if (mpLand)
        {
            mpLand->Destroy();

            delete mpLand;
            mpLand = nullptr;
        }

        if (mpSlime)
        {
            mpSlime->Destroy();

            delete mpSlime;
            mpSlime = nullptr;
        }

        if (mpCBLight)
        {
            mpCBLight->Release();
        }

        if (mpCBTransform)
        {
            mpCBTransform->Release();
        }


        CDXEngine::OnDestroy();
    }; 

    virtual void OnUpdate(float tDeltaTime) override
    {
        CDXEngine::OnUpdate(tDeltaTime);


        //key input
        if (CInputMgr::GetInstance()->KeyPress("OnExit"))
        {
            PostQuitMessage(0);
        }

        //====  STEP2   =================================================================
        XMVECTOR tVelocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        //XMVECTOR tDirX = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);           //x축 기저벡터 (1,0,0)
        XMVECTOR tDirZ = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);           //z축 기저벡터 (0,0,1)

        //float tXVal = 0.0f;
        float tZVal = 0.0f;

        //Get axis X
        if (CInputMgr::GetInstance()->KeyPress("OnMoveLt"))
        {
            //tXVal = -1.0f;

            XMFLOAT3 t = mpSlime->GetRotation();

            t.y = t.y - 2.0f * tDeltaTime;

            mpSlime->SetRotation(t.x, t.y, t.z);
        }

        if (CInputMgr::GetInstance()->KeyPress("OnMoveRt"))
        {
            //tXVal = +1.0f;

            XMFLOAT3 t = mpSlime->GetRotation();

            t.y = t.y + 2.0f * tDeltaTime;

            mpSlime->SetRotation(t.x, t.y, t.z);
        }

        //Get axis Z
        if (CInputMgr::GetInstance()->KeyPress("OnMoveForward"))
        {
            tZVal = +1.0f;
        }

        if (CInputMgr::GetInstance()->KeyPress("OnMoveBackward"))
        {
            tZVal = -1.0f;
        }

        //tDirX = tDirX * tXVal;
        tDirZ = tDirZ * tZVal;

       // tVelocity = tDirX + tDirZ;                                  //벡터의 덧셈연산
        tVelocity = tDirZ;                                  //벡터의 덧셈연산
        tVelocity = XMVector3Normalize(tVelocity);      //정규화
        tVelocity = tVelocity * 50.0f;                            //벡터의 스칼라곱 <-- 방향벡터*크기

        mpSlime->SetVelocity(tVelocity);
        mpSlime->Update(tDeltaTime, COORD_OPTION::Self);

        mpLand->Update(tDeltaTime);

        XMFLOAT3 tTarget = mpSlime->GetPosition();
                XMFLOAT4 tTarget4(tTarget.x, tTarget.y, tTarget.z, 1.0f);
                mpKnight->UpdateLook(tTarget4);

        mpKnight_0->Update(tDeltaTime);

        //update

        //회전각도
        static float t = 0.0f;
        t = t + 0.5f * tDeltaTime;

        //상수버퍼에 담아서 랜더링 파이프라인에 보내준다
        mMatWorld_0 = XMMatrixRotationY(t);

        //mMatWorld_1 = XMMatrixRotationY(t*4.0f);

        XMMATRIX tRotation = XMMatrixRotationY(t * 4.0f);
        XMMATRIX tTranslate = XMMatrixTranslation(3.0f, 0.0f, 0.0f);

        //물체의 입장에서 보면 '자전' 운동이 일어난다.
        //mMatWorld_1 = tRotation * tTranslate;               // 행벡터 * 회전변환행렬*이동변환행렬  //회전하고 그리고나서 이동한다


        //행렬의 곱셈은 교환법칙이 성립하지 않는다
        //물체의 입장에서 보면 '공전' 운동이 일어난다
        mMatWorld_1 = tTranslate * tRotation;               // 행벡터 * 이동변환행렬*회전변환행렬  //이동하고 그리고나서 회전한다
        


        //light
        //응용프로그램 수준의 조명 데이터를 만들겠다.
        //직사광(direction light) 개념으로 만들었다. 이것은 태양광을 모사한 것이다.
        //      직사광은 위치는 상관없고, 방향만 중요하다 ( 그렇게 하기 위해서 w를 0으로 했다 )

        //============================  diffuse light  =====================================================
        XMFLOAT4 tLightDir = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);      //조명벡터 전방방향(z축)(벡터라서 0.0f)
        XMFLOAT4 tLightColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);    //붉은색 가정

        //조명벡터를 회전
        XMMATRIX tRotateLight = XMMatrixIdentity();//XMMatrixRotationX(5.0f * t);    //x축을 회전축으로 하는 변환행렬
        XMVECTOR tVectorLight = XMLoadFloat4(&tLightDir);       //XMLoadFloat4: XMFLOAT4 ---> XMVECTOR
        tVectorLight = XMVector3Transform(tVectorLight, tRotateLight);//행렬과 벡터의 곱셈, 결과는 벡터
        //XMVector3TransformCoord   //행렬과 벡터(위치)의 곱셈, 결과는 벡터(위치)
        XMStoreFloat4(&tLightDir, tVectorLight);



        //============================  ambient light  =====================================================
        XMFLOAT4 tAmbientColor = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
        //float tAmbientIntensity = 1.5f;



        //============================  specular light  =====================================================
        XMFLOAT4 tSpecularColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
        float tPower = 1.0f;

        mpCamera->UpdateMatView();                    //뷰변환행렬을 계산(갱신)
        mpCamera->GetViewMatrix(mMatView);      //mMatView에 뷰변환행렬을 설정
        

        this->Clear(Colors::Plum);

        //vertex shade 단계에 vertex shader 객체 설정
        GetImmediateDevice()->VSSetConstantBuffers(0, 1, &mpCBTransform);       //<-- constant buffer slot index 0
        GetImmediateDevice()->VSSetConstantBuffers(1, 1, &mpCBLight);               //<-- constant buffer slot index 1


        //pixel shade 단계에 pixel shader 객체 설정
        GetImmediateDevice()->PSSetConstantBuffers(1, 1, &mpCBLight);       //<-- constant buffer slot index 1
        

        


        //=================================================================================
        //                                                  첫 번쨰 육면체 랜더링 
        CBTransform tCBTransform;           // transform용 상수버퍼 데이터 지역변수
        CBLight tCBLight;                           // light용 상수버퍼 데이터 지역변수

        //tCBTransform.mWorld = XMMatrixTranspose(mMatWorld_0);
        //tCBTransform.mView = XMMatrixTranspose(mMatView);
        //tCBTransform.mProjection = XMMatrixTranspose(mMatPerspective);

        tCBLight.mLightDir = tLightDir;
        tCBLight.mLightColor = tLightColor;
        tCBLight.mAmbientColor = tAmbientColor;
        tCBLight.mCameraPosition = mCameraPosition;
        tCBLight.mSpecularColor = tSpecularColor;
        tCBLight.mSpecularPower = tPower;

            //UpdateSubresource 상수버퍼의 내용을 갱신해주는 함수
            GetImmediateDevice()->UpdateSubresource(mpCBLight, 0, nullptr, &tCBLight, 0, 0);

            //mpActor->Update(tDeltaTime);
            mpSlime->Render(mMatView, mMatPerspective);
            mpLand->Render(mMatView, mMatPerspective);
            mpKnight->Render(mMatView, mMatPerspective);
            mpKnight_0->Render(mMatView, mMatPerspective);

        //=================================================================================


       
                //============================  depthbuffer 비활성 구간  ===================================================== //            OFF
                this->mpD3D->DoTurnOffDepthBuffer();
                //<-- 2D UI를 3D 공간에 있는 물체와 개념적으로 분리하여 다루기 위해서이다.


                XMMATRIX tMatWorldBitmap = XMMatrixIdentity();

                //============================  diffuse light  =====================================================
                XMFLOAT4 tLightDirBitmap                    = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
                XMFLOAT4 tLightColorBitmap                  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                //============================  ambient light  =====================================================
                XMFLOAT4 tAmbientColorBitmap            = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
                //============================  specular light  =====================================================
                XMFLOAT4 tSpecularColorBitmap            = XMFLOAT4(0.0f, 0.0f, 0.0f,  0.0f);
                float tPowerBitmap = 1.0f;


                tCBTransform.mWorld = XMMatrixTranspose(tMatWorldBitmap);
                tCBTransform.mView = XMMatrixTranspose(mMatView);
                tCBTransform.mProjection = XMMatrixTranspose(mMatOrthographic);
                
                tCBLight.mLightDir = tLightDirBitmap;
                tCBLight.mLightColor = tLightColorBitmap;
                tCBLight.mAmbientColor = tAmbientColorBitmap;
                tCBLight.mCameraPosition = mCameraPosition;
                tCBLight.mSpecularColor = tSpecularColorBitmap;
                tCBLight.mSpecularPower = tPowerBitmap;

                //UpdateSubresource 상수버퍼의 내용을 갱신해주는 함수
                GetImmediateDevice()->UpdateSubresource(mpCBTransform, 0, nullptr, &tCBTransform, 0, 0);

                RECT tRect{};
                GetClientRect(this->mhWnd, &tRect);
                int tWidth = tRect.right - tRect.left;
                int tHeight = tRect.bottom - tRect.top;


                mpBitmap->Render(3, 3);
                mpMtlUnlit->Render(mpBitmap->GetCountIndex(), mpBitmap->GetTexture());

                //======================================================================================================= //            ON
                this->mpD3D->DoTurnOnDepthBuffer();





        this->Present();
    };

public:
    CRyuEngine(const CRyuEngine& t) = delete;
    void operator=(const CRyuEngine& t) = delete;

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
    //D3DCompileFromFile : 셰이더 소스코드를 컴파일해주는 함수
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