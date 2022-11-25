// winAPIEngine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "winAPIEngine.h"

#include "CAPIEngine.h"

#include "CUnit.h"

#include "CTexture.h"

/*
* 여기서는 CUnit에 Pivot기능을 추가해보자
*/

class CRyuEngine : public CAPIEngine
{
    CUnit* mpUnit = nullptr;

    CTexture* mpTexture = nullptr;

public:
    CRyuEngine() {};
    virtual ~CRyuEngine() {};

    //delete는 해당 함수의 구현을 막는 문법
private:
    CRyuEngine(const CRyuEngine& t) = delete;
    CRyuEngine& operator = (const CRyuEngine& t) = delete;

public:
    virtual void OnCreate() override
    {
        CAPIEngine::OnCreate();

        //todo
        mpUnit = new CUnit();
        mpUnit->SetPosition(800 * 0.5f, 600 - 50 - 100.0f);
        mpUnit->SetRaidus(50.0f);

        mpTexture = new CTexture();
        mpTexture->LoadTexture(hInst, mhDC, L"resources/bongbong_0.bmp");
    }
    virtual void OnDestroy() override
    {
        //todo
        if (nullptr != mpTexture)
        {
            delete mpTexture;
            mpTexture = nullptr;
        }

        CAPIEngine::OnDestroy();
    }
    virtual void OnUpdate() override
    {
        CAPIEngine::OnUpdate();

        //update

        //&비트연산의 이유
        //GetAsyncKeyState의 리턴값은 다음과 같다
        /*
        * 0x0000: 이전에 눌림 없음. 호출시점에 눌림 없음 <-- 안눌림
        * 0x0001: 이전에 눌림 있음. 호출시점에 눌림 없음 <-- 떼어짐
        * 
        * 0x8000: 이전에 눌림 없음. '호출시점에 눌림' 있음 <-- 새롭게 눌림
        * 0x8001: 이전에 눌림 있음. '호출시점에 눌림' 있음 <-- 눌리고 있음
        * 
        * 이것을 알아오기 위함이다.
        * 
        * 비트연산으로 계산하므로 밑에 두 가지 경우가 참이 된다.
        */

        //A키가 눌리고 있다면
        if (GetAsyncKeyState('A') & 0x8000)  //<-- GetAsyncKeyState함수가 호출되는 시점에 A키 눌림이 있다.
        {
            //현재 위치 = 이전위치 + 속도
            mpUnit->mX = mpUnit->mX - 0.1f;
        }
        //D키가 눌리고 있다면
        if (GetAsyncKeyState('D') & 0x8000)  //<-- GetAsyncKeyState함수가 호출되는 시점에 D키 눌림이 있다.
        {
            mpUnit->mX = mpUnit->mX + 0.1f;
        }

        //render
        this->Clear(0.1f, 0.1f, 0.3f);

        //Update Method Pattern: 한 프레임에 일어나는 동작을 함수화하여 게임루프와 결합하여 프로그래밍하는 패턴
        mpUnit->Render(this);

        if (mpTexture)
        {
            /*BitBlt(mhDC,
                mpUnit->mX, mpUnit->mY,
                mpTexture->mBitmapInfo.bmWidth, mpTexture->mBitmapInfo.bmHeight,
                mpTexture->mthDCMem,
                0, 0,
                SRCCOPY);*/

            this->DrawTexture(mpUnit->mX, mpUnit->mY, mpTexture);
        }

        this->Present();
        //BitBlt(mhDC,
        //    0, 0,                           //전면버퍼: 현재화면DC
        //    800, 600,
        //    this->mpBackBuffer->mhDCMem,    //후면버퍼: memoryDC
        //    0, 0,
        //    SRCCOPY);
    }
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    //CAPIEngine tEngine;
    CRyuEngine tEngine;
    tEngine.Create(hInstance, nCmdShow);

    //복사생성자
    //CRyuEngine tA = tEngine;

    //CRyuEngine tB;
    //복사대입연산자
    //tB = tEngine;

    MSG msg = { 0 };
    msg = tEngine.Run();

    return (int)msg.wParam;
}