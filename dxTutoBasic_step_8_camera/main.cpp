#include "framework.h"
//#pragma comment(lib, "winAPIEngine.lib")


#include "CDxEngine.h"

#include "DDSTextureLoader.h"

#include "ryubuffer.h"
#include "CMesh.h"
#include "CMtlTexture.h"

#include "CBitmapRect.h"
#include "CMtlUnlit.h"

#include "CActor.h"

#include "CInputMgr.h"

/*
    이번 예시에서는
*/





//선언 
HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);


class CRyuEngine : public CDxEngine
{   
    /*CMesh* mpMesh = nullptr;
    CMtlTexture* mpMtlTexture = nullptr;*/

    CActor* mpSlime = nullptr;      //슬라임

    CActor* mpLand = nullptr;       //바닥

    CActor* mpKnight_0 = nullptr;   //경비병

    
    //상수 버퍼( 변환행렬 용 )
    ID3D11Buffer* mpCBTransform = nullptr;      //transform 용 상수버퍼
    ID3D11Buffer* mpCBLight = nullptr;              //light 용 상수버퍼 

    //응용프로그램 수준에서 다룰 변환행렬들
    XMMATRIX mMatWorld_0;            //월드변환 행렬 <--- 첫번째 정육면체 용
    XMMATRIX mMatWorld_1;       //월드변환 행렬 <--- 두번째 정육면체 용
    XMMATRIX mMatView;              //뷰변환 행렬



    XMMATRIX mMatViewUI;              //뷰변환 행렬


    XMMATRIX mMatPerspective;       //원근 투영변환 행렬 
    XMMATRIX mMatOrthographic;       //직교 투영변환 행렬 



    //카메라 위치
    XMFLOAT4 mCameraPosition;


    CBitmapRect* mpBitmap = nullptr;
    CMtlUnlit* mpMtlUnlit = nullptr;


  
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

        CInputMgr::GetInstance()->AddKey("OnMoveLt", VK_LEFT);
        CInputMgr::GetInstance()->AddKey("OnMoveRt", VK_RIGHT);
        CInputMgr::GetInstance()->AddKey("OnMoveForward", VK_UP);
        CInputMgr::GetInstance()->AddKey("OnMoveBackward", VK_DOWN);


        //mpMtlTexture = new CMtlTexture();
        //mpMtlTexture->Create(mpD3D->GetD3DDevice());
        //mpMtlTexture->SetEngine(this);


        //mpMesh = new CMesh();
        ////mpMesh->Create(mpD3D->GetD3DDevice(), L"resources/testUV256by256.dds");
        //mpMesh->Create(mpD3D->GetD3DDevice(), L"resources/slimeby.dds");
        ////mpMesh->Create(mpD3D->GetD3DDevice(), L"resources/seafloor.dds");
        //mpMesh->SetEngine(this);

        mpSlime = new CActor();
        mpSlime->Create(this, L"resources/slime.obj", L"resources/slimeby.dds");
        
        mpLand = new CActor();
        mpLand->Create(this, L"resources/test_cube.obj", L"resources/slimeby.dds");
        mpLand->SetPosition(0.0f, 0.0f, 0.0f);
        mpLand->SetScale(2000.0f, 1.0f, 2000.0f);

        mpKnight_0 = new CActor();
        mpKnight_0->Create(this, L"resources/chr_knight.obj", L"resources/slimeby.dds");
        mpKnight_0->SetPosition(100.0f, 0.0f, 100.0f);
        mpKnight_0->SetScale(25.0f, 25.0f, 25.0f);


        int tScreenWidth = this->GetScreenWidth();
        int tScreenHeight = this->GetScreenHeight();

        mpBitmap = new CBitmapRect();
        mpBitmap->Create(mpD3D->GetD3DDevice(), L"resources/testUV256by256.dds",
            tScreenWidth, tScreenHeight,
            128, 128);
            //tScreenWidth * 0.5f, tScreenHeight * 0.5f);
        mpBitmap->SetEngine(this);

        mpMtlUnlit = new CMtlUnlit();
        mpMtlUnlit->Create(mpD3D->GetD3DDevice());
        mpMtlUnlit->SetEngine(this);

        

        //텍스처 이미지 데이터를 파일에서 로드하여  mpTexture에 담음
        //CreateDDSTextureFromFile(mpD3D->GetD3DDevice(), L"resources/testUV256by256.dds", nullptr, &mpTexture);



        //상수 버퍼 생성
        //주설명
        D3D11_BUFFER_DESC tBd = {};
        tBd.Usage = D3D11_USAGE_DEFAULT;            //기본용도다 
        tBd.ByteWidth = sizeof(CBTransform) * 1;    //해당 상수버퍼의 메모리 크기를 알린다
        tBd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//constant buffer용도다
        tBd.CPUAccessFlags = 0;     //CPU의 접근은 불허한다

        mpD3D->GetD3DDevice()->CreateBuffer(&tBd, nullptr, &mpCBTransform);


        D3D11_BUFFER_DESC tBdLight = {};
        tBdLight.Usage = D3D11_USAGE_DEFAULT;            //기본용도다 
        tBdLight.ByteWidth = sizeof(CBLight) * 1;    //해당 상수버퍼의 메모리 크기를 알린다
        tBdLight.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//constant buffer용도다
        tBdLight.CPUAccessFlags = 0;     //CPU의 접근은 불허한다

        mpD3D->GetD3DDevice()->CreateBuffer(&tBdLight, nullptr, &mpCBLight);






        //변환행렬들 설정 
        //월드변환 행렬
        mMatWorld_0 = XMMatrixIdentity();     //단위행렬로 초기화

        mMatWorld_1 = XMMatrixIdentity();     //단위행렬로 초기화




        //카메라 arm의 길이
        float tLength = 55.0f;//15.0f;//2.0f;//15.0f;

        mCameraPosition.x = 0.0f;
        mCameraPosition.y = 1.0f*tLength;
        //test
        //mCameraPosition.y = 0.0f * tLength;
        mCameraPosition.z = -1.0f * tLength;
        mCameraPosition.w = 1.0f;



        //뷰변환 행렬
        mMatView = XMMatrixIdentity();

        //XMVECTOR tEye = XMVectorSet(0.0f, 2.0f, -5.0f, 1.0f); //카메라의 위치
        XMVECTOR tEye = XMVectorSet(mCameraPosition.x, mCameraPosition.y, mCameraPosition.z, 1.0f); //카메라의 위치
        XMVECTOR tAt = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); //바라보는 응시점위치
        XMVECTOR tUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //월드에 상방벡터

        //뷰변환 행렬을 구한다. ~LH Left Hand 왼손좌표계
        mMatView = XMMatrixLookAtLH(tEye, tAt, tUp);




        //UI용 뷰변환행렬
        mMatViewUI = XMMatrixIdentity();

        XMVECTOR tEyeUI = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f); //카메라의 위치        
        XMVECTOR tAtUI = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); //바라보는 응시점위치
        XMVECTOR tUpUI = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //월드에 상방벡터
        
        mMatViewUI = XMMatrixLookAtLH(tEyeUI, tAtUI, tUpUI);




        //투영변환 행렬 

        //원근 투영 변환 행렬 만들기
        //mMatProjection = XMMatrixIdentity();

        //RECT rc = { 0 };
        //GetClientRect(this->mhWnd, &rc);    //window api
        //unsigned int tWidth = rc.right - rc.left;
        //unsigned int tHeight = rc.bottom - rc.top;//윈도우 좌표계 상에서 계산

        //mMatProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, (float)tWidth / (float)tHeight, 0.01f, 100.0f);


        RECT rc = { 0 };
        GetClientRect(this->mhWnd, &rc);    //window api
        unsigned int tWidth = rc.right - rc.left;
        unsigned int tHeight = rc.bottom - rc.top;//윈도우 좌표계 상에서 계산

        //원근 투영 변환 행렬 만들기 
        mMatPerspective = XMMatrixIdentity();
        mMatPerspective = XMMatrixPerspectiveFovLH(XM_PIDIV2, (float)tWidth / (float)tHeight, 0.01f, 1000.0f);

        //직교 투영 변환 행렬 만들기 
        mMatOrthographic = XMMatrixIdentity();
        float tPixelPerUnit = 1.0f;
        mMatOrthographic = XMMatrixOrthographicLH(tPixelPerUnit *tWidth, tPixelPerUnit * tHeight, 0.01f, 100.0f);



    }
    virtual void OnDestroy() override
    {       

        if (mpCBLight)
        {
            mpCBLight->Release();
        }

        if (mpCBTransform)
        {
            mpCBTransform->Release();
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


       /* if (mpMtlTexture)
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
        }*/

        if (mpSlime)
        {
            mpSlime->Destroy();

            delete mpSlime;
            mpSlime = nullptr;
        }

        if (mpLand)
        {
            mpLand->Destroy();

            delete mpLand;
            mpLand = nullptr;
        }

        if (mpKnight_0)
        {
            mpKnight_0->Destroy();

            delete mpKnight_0;
            mpKnight_0 = nullptr;
        }

    

        CDxEngine::OnDestroy();
    }
    virtual void OnUpdate(float tDeltaTime) override
    {
        CDxEngine::OnUpdate(tDeltaTime);

        /*
        //zx평면에서의 월드좌표계 기준에 이동을 수행하겠다
        if (CInputMgr::GetInstance()->KeyPress("OnMoveLt"))
        {
            //XMFLOAT3 tPos = mpSlime->GetPosition();
            //tPos.x = tPos.x - 50.0f * tDeltaTime;
            //mpSlime->SetPosition(tPos.x, tPos.y, tPos.z);

            //속도는 (-50,0,0)으로 설정
            XMFLOAT4 f = XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
            XMVECTOR v = XMLoadFloat4(&f);
            mpSlime->SetVelocity(v * 50.0f);
        }

        if (CInputMgr::GetInstance()->KeyPress("OnMoveRt"))
        {
            XMFLOAT3 tPos = mpSlime->GetPosition();

            tPos.x = tPos.x + 50.0f * tDeltaTime;

            mpSlime->SetPosition(tPos.x, tPos.y, tPos.z);
        }

        if (CInputMgr::GetInstance()->KeyPress("OnMoveForward"))
        {
            XMFLOAT3 tPos = mpSlime->GetPosition();

            tPos.z = tPos.z + 50.0f * tDeltaTime;

            mpSlime->SetPosition(tPos.x, tPos.y, tPos.z);
        }

        if (CInputMgr::GetInstance()->KeyPress("OnMoveBackward"))
        {
            XMFLOAT3 tPos = mpSlime->GetPosition();

            tPos.z = tPos.z - 50.0f * tDeltaTime;

            mpSlime->SetPosition(tPos.x, tPos.y, tPos.z);
        }
        */

        //step_0
        //속도는 (0,0,0)으로 설정
        //XMFLOAT4 f = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
        //XMVECTOR v = XMLoadFloat4(&f);
        //mpSlime->SetVelocity(v);
        //
        ////zx평면에서의 월드좌표계 기준에 이동을 수행하겠다
        //if (CInputMgr::GetInstance()->KeyPress("OnMoveLt"))
        //{
        //    //속도는 (-50,0,0)으로 설정
        //    XMFLOAT4 f = XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
        //    XMVECTOR v = XMLoadFloat4(&f);
        //    mpSlime->SetVelocity(v*50.0f);               
        //}

        //if (CInputMgr::GetInstance()->KeyPress("OnMoveRt"))
        //{
        //    //속도는 (50,0,0)으로 설정
        //    XMFLOAT4 f = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
        //    XMVECTOR v = XMLoadFloat4(&f);
        //    mpSlime->SetVelocity(v * 50.0f);
        //}

        //if (CInputMgr::GetInstance()->KeyPress("OnMoveForward"))
        //{
        //    //속도는 (0,0,50)으로 설정
        //    XMFLOAT4 f = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
        //    XMVECTOR v = XMLoadFloat4(&f);
        //    mpSlime->SetVelocity(v * 50.0f);
        //}

        //if (CInputMgr::GetInstance()->KeyPress("OnMoveBackward"))
        //{
        //    //속도는 (0,0,-50)으로 설정
        //    XMFLOAT4 f = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
        //    XMVECTOR v = XMLoadFloat4(&f);
        //    mpSlime->SetVelocity(v * 50.0f);
        //}

        //mpSlime->Update(tDeltaTime);


        //step_1
        //각각의  축입력 개념을 구현
        //XMVECTOR tVelocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        //XMVECTOR tDirX = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);       //x축 기저벡터 (1,0,0)
        //XMVECTOR tDirZ = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);       //z축 기저벡터 (0,0,1)

        ////zx평면에서의 월드좌표계 기준에 이동을 수행하겠다
        //float tXVal = 0.0f;         //x축 선상에서의 입력값
        //if (CInputMgr::GetInstance()->KeyPress("OnMoveLt"))
        //{            
        //    tXVal = -1.0f;
        //}

        //if (CInputMgr::GetInstance()->KeyPress("OnMoveRt"))
        //{            
        //    tXVal = 1.0f;
        //}

        //float tZVal = 0.0f;         //z축 선상에서의 입력값
        //if (CInputMgr::GetInstance()->KeyPress("OnMoveForward"))
        //{            
        //    tZVal = 1.0f;
        //}

        //if (CInputMgr::GetInstance()->KeyPress("OnMoveBackward"))
        //{
        //    tZVal = -1.0f;
        //}

        //tDirX = tDirX* tXVal;           //x축 선상에서의 방향 구함
        //tDirZ = tDirZ * tZVal;           //x축 선상에서의 방향 구함

        //tVelocity = tDirX + tDirZ;              //zx평면에서의 방향 구함
        //tVelocity = XMVector4Normalize(tVelocity);      //정규화

        //tVelocity = tVelocity * 50.0f;              //속력 적용


        //mpSlime->SetVelocity(tVelocity);

        //mpSlime->Update(tDeltaTime);


        //step_2
        //전후이동(축입력) + 좌우선회
        XMVECTOR tVelocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        
        XMVECTOR tDirZ = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);       //z축 기저벡터 (0,0,1)

        //zx평면에서의 월드좌표계 기준에 이동을 수행하겠다
        
        if (CInputMgr::GetInstance()->KeyPress("OnMoveLt"))
        {
            XMFLOAT3 t = mpSlime->GetRotation();
            t.y = t.y - 2.0f * tDeltaTime;
            mpSlime->SetRotation(t.x, t.y, t.z);                
        }

        if (CInputMgr::GetInstance()->KeyPress("OnMoveRt"))
        {
            XMFLOAT3 t = mpSlime->GetRotation();
            t.y = t.y + 2.0f * tDeltaTime;
            mpSlime->SetRotation(t.x, t.y, t.z);
        }

        float tZVal = 0.0f;         //z축 선상에서의 입력값
        if (CInputMgr::GetInstance()->KeyPress("OnMoveForward"))
        {
            tZVal = 1.0f;
        }

        if (CInputMgr::GetInstance()->KeyPress("OnMoveBackward"))
        {
            tZVal = -1.0f;
        }

        tDirZ = tDirZ * tZVal;           //x축 선상에서의 방향 구함

        tVelocity = tDirZ;              //zx평면에서의 방향 구함
        tVelocity = XMVector4Normalize(tVelocity);      //정규화

        tVelocity = tVelocity * 50.0f;              //속력 적용

        mpSlime->SetVelocity(tVelocity);
        //로컬좌표계 기준으로 갱신
        mpSlime->Update(tDeltaTime,COORD_OPTION::Self);





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



        //빛 정보 생성
        //난반사 관련 조명 정보 
        XMFLOAT4 tLightDir = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);//xyzw 빛의 방향은 전방(z+)
        XMFLOAT4 tLightColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);//rgba

        //주변광 관련 조명 정보 
        XMFLOAT4 tAmbientColor = XMFLOAT4(0.1f, 0.1f, 0.3f, 1.0f);//rgba

        //정반사광 관련 조명 정보
        XMFLOAT4 tSpecularColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);//rgba
        float tPower = 20.0f;//1.0f;//8.0f;







        this->Clear(Colors::DarkOrchid);

      
        //mpD3D->GetImmediateDevice()->VSSetConstantBuffers(0, 1, &mpCBTransform);    //<-- transform용 상수버퍼 slot index 0
        mpD3D->GetImmediateDevice()->VSSetConstantBuffers(1, 1, &mpCBLight);    //<-- light용 상수버퍼 slot index 1

        //mpD3D->GetImmediateDevice()->PSSetConstantBuffers(0, 1, &mpCBTransform);
        mpD3D->GetImmediateDevice()->PSSetConstantBuffers(1, 1, &mpCBLight);        //<-- light용 상수버퍼 slot index 1






        //CBTransform tCBTransform;           //transform 용 상수버퍼 데이터 지역변수
        CBLight tCBLight;                           //light 용 상수버퍼 데이터 지역변수

        /*tCBTransform.mWorld = XMMatrixTranspose(mMatWorld_0);
        tCBTransform.mView = XMMatrixTranspose(mMatView);
        tCBTransform.mProjection = XMMatrixTranspose(mMatPerspective);*/

        //빛 정보를 상수버퍼를 매개로 셰이더 측에 전달하기 위해 설정
        //난반사광 관련 정보 전달 
        tCBLight.mLightDir = tLightDir;
        tCBLight.mLightColor = tLightColor;
        //주변광 관련 정보 전달
        tCBLight.mAmbientColor = tAmbientColor;
        //정반사광 관련 정보 전달
        tCBLight.mCameraPosition = mCameraPosition;
        tCBLight.mSpecularColor = tSpecularColor;
        tCBLight.mSpecularPower = tPower;
        
        //셰이더 측에 상수버퍼에 값 갱신
        //mpD3D->GetImmediateDevice()->UpdateSubresource(mpCBTransform, 0, nullptr, &tCBTransform, 0, 0);
        mpD3D->GetImmediateDevice()->UpdateSubresource(mpCBLight, 0, nullptr, &tCBLight, 0, 0);
           

        //mpMesh->Render();
        //mpMtlTexture->Render(mpMesh->GetCountIndex(), mpMesh->GetTexture());

        mpSlime->Render(mMatView, mMatPerspective);


        mpLand->Render(mMatView, mMatPerspective);
        mpKnight_0->Render(mMatView, mMatPerspective);

        //===========================
        // 2D render 모드를 위해, 깊이버퍼를 끈다
        mpD3D->DoTurnOffDepthBuffer();

                //ui용 상수버퍼 를 Vertex Shading 단계에 설정
                mpD3D->GetImmediateDevice()->VSSetConstantBuffers(0, 1, &mpCBTransform);

                //단위행렬
                XMMATRIX tMatWorldBitmap = XMMatrixIdentity();                

                CBTransform tCBBitmapTransform;
                //CBLight tCBBitmapLight;


                tCBBitmapTransform.mWorld = XMMatrixTranspose(tMatWorldBitmap);
                tCBBitmapTransform.mView = XMMatrixTranspose(mMatViewUI);
                tCBBitmapTransform.mProjection = XMMatrixTranspose(mMatOrthographic);
                ////난반사광 관련 정보 전달 
                //tCBBitmapLight.mLightDir = tLightDir;
                //tCBBitmapLight.mLightColor = tLightColor;
                ////주변광 관련 정보 전달
                //tCBBitmapLight.mAmbientColor = tAmbientColor;
                ////정반사광 관련 정보 전달
                //tCBBitmapLight.mCameraPosition = mCameraPosition;
                //tCBBitmapLight.mSpecularColor = tSpecularColor;
                //tCBBitmapLight.mSpecularPower = tPower;

                //셰이더 측에 상수버퍼에 값 갱신
                mpD3D->GetImmediateDevice()->UpdateSubresource(mpCBTransform, 0, nullptr, &tCBBitmapTransform, 0, 0);

                mpBitmap->Render(0, 0);
                //mpBitmap->Render(400 - 64, 300 + 32);
                mpMtlUnlit->Render(mpBitmap->GetCountIndex(), mpBitmap->GetTexture());
                //mpMtlTexture->Render(mpBitmap->GetCountIndex(), mpBitmap->GetTexture());

        // 3D render 모드를 위해, 깊이버퍼를 켠다
        mpD3D->DoTurnOnDepthBuffer();
        //===========================

                
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