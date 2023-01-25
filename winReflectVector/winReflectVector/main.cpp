#include "framework.h"
#pragma comment(lib, "winAPIEngine.lib")


class CBox : public CUnit
{
public:
    CBox()
    {
    }
    ~CBox()
    {
    }

    CBox(const CBox& t):CUnit(t)
    {
    }

    void operator=(const CBox& t)
    {
        CUnit::operator=(t);
    }

    virtual CObjectRyu* clone()
    {
        return new CBox(*this);
    }

};


class CRyuEngine : public CAPIEngine
{ 
    CTexture* mpTexture = nullptr;

    CUnit* PFBox = nullptr;

    CBox* mpBox = nullptr;

public:
    CRyuEngine() {};
    virtual ~CRyuEngine() {};

private:
    CRyuEngine(const CRyuEngine& t) = delete;
    CRyuEngine& operator=(const CRyuEngine& t) = delete;

public:
    virtual void OnCreate() override
    {
        CAPIEngine::OnCreate();

        mpTexture = new CTexture();
        mpTexture->LoadTexture(this->GetHInstance(), this->GetHDC(), L"resources/block_5.bmp");
        mpTexture->AddRef();

        PFBox = CreatePrefab<CBox>(mpTexture, 0.5f, 0.5f, SVector2D(0.0f, 0.0f));


        mpBox = InstantObject<CBox>(PFBox);
        mpBox->AddRef();


        mpBox->SetPosition(SVector2D(100.0f, 100.0f));
        mpBox->SetVelocity(SVector2D(0.7f, 0.7f)*250.0f);            //45도 방향 (1/sqrt(2), 1/sqrt(2))
    }
    virtual void OnDestroy() override
    {
        DestroyObject(mpBox);

        DestroyPrefab(PFBox);

        SAFE_RELEASE(mpTexture);

        CAPIEngine::OnDestroy();
    }
    virtual void OnUpdate(float tDeltaTime) override
    {
        CAPIEngine::OnUpdate(tDeltaTime);

        //방향까지 고려하여 버그를 없애자
        if (mpBox->GetPosition().mY > 600.0f && mpBox->GetVelocity().mY > 0.0f)
        {
            //반사벡터를 구해서 적용하자 

            SVector2D tVectorP;     //입사벡터
            SVector2D tVectorR;     //반사벡터
            SVector2D tVectorN;     //법선벡터(평면의 정의 의 요소)
            //하단 평면을 정의
            tVectorN.mX = 0.0f;
            tVectorN.mY = -1.0f;
            tVectorN.Normalize();
            //속도를 가져와 입사벡터로 가정
            tVectorP = mpBox->GetVelocity();

            //반사벡터를 구하자
            tVectorR = tVectorP + tVectorN * (((tVectorP*( -1.0f)).Dot(tVectorN)) * 2.0f);
            //속도를 설정한다
            mpBox->SetVelocity(tVectorR);
        }

        if (mpBox->GetPosition().mY < 0.0f && mpBox->GetVelocity().mY < 0.0f)
        {
            //반사벡터를 구해서 적용하자 

            SVector2D tVectorP;     //입사벡터
            SVector2D tVectorR;     //반사벡터
            SVector2D tVectorN;     //법선벡터(평면의 정의 의 요소)
            //하단 평면을 정의
            tVectorN.mX = 0.0f;
            tVectorN.mY = 1.0f;
            tVectorN.Normalize();
            //속도를 가져와 입사벡터로 가정
            tVectorP = mpBox->GetVelocity();

            //반사벡터를 구하자
            tVectorR = tVectorP + tVectorN * (((tVectorP * (-1.0f)).Dot(tVectorN)) * 2.0f);
            //속도를 설정한다
            mpBox->SetVelocity(tVectorR);
        }

        if (mpBox->GetPosition().mX < 0.0f && mpBox->GetVelocity().mX < 0.0f)
        {
            //반사벡터를 구해서 적용하자 
            SVector2D tVectorP;     //입사벡터
            SVector2D tVectorR;     //반사벡터
            SVector2D tVectorN;     //법선벡터(평면의 정의 의 요소)
            //하단 평면을 정의
            tVectorN.mX = 1.0f;
            tVectorN.mY = 0.0f;
            tVectorN.Normalize();
            //속도를 가져와 입사벡터로 가정
            tVectorP = mpBox->GetVelocity();

            //반사벡터를 구하자
            tVectorR = tVectorP + tVectorN * (((tVectorP * (-1.0f)).Dot(tVectorN)) * 2.0f);
            //속도를 설정한다
            mpBox->SetVelocity(tVectorR);
        }

        if (mpBox->GetPosition().mX > 800.0f && mpBox->GetVelocity().mX > 0.0f)
        {
            //반사벡터를 구해서 적용하자 
            SVector2D tVectorP;     //입사벡터
            SVector2D tVectorR;     //반사벡터
            SVector2D tVectorN;     //법선벡터(평면의 정의 의 요소)
            //하단 평면을 정의
            tVectorN.mX = -1.0f;
            tVectorN.mY = 0.0f;
            tVectorN.Normalize();
            //속도를 가져와 입사벡터로 가정
            tVectorP = mpBox->GetVelocity();

            //반사벡터를 구하자
            tVectorR = tVectorP + tVectorN * (((tVectorP * (-1.0f)).Dot(tVectorN)) * 2.0f);
            //속도를 설정한다
            mpBox->SetVelocity(tVectorR);
        }








        mpBox->Update(tDeltaTime);

        this->Clear(0.1f, 0.1f, 0.3f);  

        mpBox->Render();
                
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


