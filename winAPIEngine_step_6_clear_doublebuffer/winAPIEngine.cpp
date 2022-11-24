// winAPIEngine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "winAPIEngine.h"

#include "CAPIEngine.h"

#include "CUnit.h"

#include "CTexture.h"

/*
* 여기에는 화면 그리기에 대해서 정리해보자
* 
* 이전까지 작성한 예시를 종작시켜보면
* 두 가지 문제점이 있다.
* 
* -----첫째, 비트맵이 지나간 자리에서 그려진 흔적이 남는다.
* -----둘째, 깜빡이는 현상이 일어난다.
* 
* 첫째, 흔적이 남는 것은 필요한 부분만 갱신하므로 당연한 것인데
* 이것은 화면 전체를 새로운 색으로 채워넣는 것으로 하여 해결하자
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
            mpUnit->mX = mpUnit->mX - 0.01f;
        }
        //D키가 눌리고 있다면
        if (GetAsyncKeyState('D') & 0x8000)  //<-- GetAsyncKeyState함수가 호출되는 시점에 D키 눌림이 있다.
        {
            mpUnit->mX = mpUnit->mX + 0.01f;
        }

        //render
        this->Clear();

        //Update Method Pattern: 한 프레임에 일어나는 동작을 함수화하여 게임루프와 결합하여 프로그래밍하는 패턴
        mpUnit->Render(this);

        if (mpTexture)
        {
            BitBlt(mhDC,
                mpUnit->mX, mpUnit->mY,
                mpTexture->mBitmapInfo.bmWidth, mpTexture->mBitmapInfo.bmHeight,
                mpTexture->mthDCMem,
                0, 0,
                SRCCOPY);
        }

        //todo
        //this->DrawCircle(200.0f, 200.0f, 100.0f);

        //test
        //window api를 이용하여 비트맵 출력해보기

        //비트맵은 '현재화면DC'에 직접적으로 출력할 수 없다.
        //<-- 현재화면 DC에 출력할 수 없으므로 현재화면DC와 호환되는 memoryDC를 만들어 그곳에 비트맵을 설정한다

        //비트맵을 현재화면DC에 출력하는 절차
        // i) memory dc를 만든다.
        // ii) 비트맵을 로드한다('파일'에서 데이터를 로드하여 '메모리에 생성한다) 리턴값은 '비트맵 핸들'이다.
        // iii) 비트맵 핸들을 memory dc에 설정한다

        //          그린다.
        //          <--memoryDC에 설정된 비트맵(픽셀의 2차원 배열로 표시된 이미지 데이터)데이터를 현재화면DC로 복사

        // i)memory dc의 비트맵 핸들 설정을 해제한다
        // ii)비트맵 핸들을 삭제한다(생성한 비트맵 자원 해제)
        // iii)memory dc도 삭제한다

        //HDC thDCMem = CreateCompatibleDC(mhDC);  //<--Omemory dc를 만든다.

        //HBITMAP thBitmap = (HBITMAP)LoadImage(hInst, L"resources/bongbong_0.bmp",
        //    IMAGE_BITMAP,
        //    0, 0,
        //    LR_LOADFROMFILE);   //<--LR~ Load Resource

        //HBITMAP  thOldBitmap = (HBITMAP)SelectObject(thDCMem, thBitmap);

        //    //비트맵 정보를 얻어오자
        //    BITMAP tBitmapInfo;
        //    GetObject(thBitmap, sizeof(tBitmapInfo), &tBitmapInfo);

        ////비트맵 데이터 전송함수
        //BitBlt(mhDC,    //현재화면DC
        //    100, 100,   //<--출력위치
        //    tBitmapInfo.bmWidth, tBitmapInfo.bmHeight,  //<-- 너비, 높이
        //    thDCMem,
        //    0, 0,
        //    SRCCOPY);

        //SelectObject(thDCMem, thOldBitmap); //이전에 사용하던 비트맵으로 설정을 되돌린다
        //DeleteObject(thBitmap);

        //DeleteDC(thDCMem);  //memoryDC를 삭제한다

        //현재 화면 DC와는 동작방식이 다르다
        //현재 화면 DC는 얻어오고, 놓아주는 개념이다.
        //반면에 여기서 사용하고 있는 memoryDC는 생성하고 해제하는 개념이다.
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