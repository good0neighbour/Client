// winAPIEngine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"

#pragma comment(lib, "winAPIEngine.lib")

#include "CPiece.h"
#include "CSelect.h"

/*
* 이번 스텝에서는 '게임보드'를 만들고 셀들에 퍼즐 피스를 배치하자.
* 
* 이 예시는 3컬러 매치 퍼즐 알고리즘 테스트를 위해
* 
* '격자 형태로 퍼즐 피스를 나열하고(셀들에)
* 플레이어의 선택UI를 표시하는 기능을 가진다'
* 
* 요구사항 가정
* 
* -게임보드는 Grid라고 칭하고 5by5 크기를 가정한다.
* -셀(타일) 1개의 픽셀 단위 크기는 96by96 로 가정
* -게임보드의 시작위치는 left, top 100, 100에서 시작
* 
* 다음의 두 개의 클래스를 준비하자.
* 
* CPiece
* 
*   퍼즐조각<--퍼즐 조각은 색상별로 준비
* 
*   <-- 퍼즐 피스가 그리드에 배치된 상태에서
*   대기애니메이션을 수행한다는 요구사항이 있다고 가정하고
*   퍼즐 피스 하나의 애니메이션 시퀀스 하나를 대응시켜 제작하겠다.
*   (<-- 물론 다르게 설계도 가능하다)
* 
* CSelect
* 
*   임의의 셀을 선택했다는 표시
*/

class CRyuEngine : public CAPIEngine
{
    //자원( 외부 자원 파일 데이터 )
    //대표이미지
    CTexture* mpTexPiece = nullptr;
    CTexture* mpTexUISelect = nullptr;

    //원본객체
    //퍼즐 피스 원본 객체, UISelect 원본 객체
    CUnit* PFPiece = nullptr;
    CUnit* PFUISelect = nullptr;

    //실제객체

    //눈에 보이지 않는 데이터
    
    //퍼즐 피스는검정색을 포함하여 6종이다.
    //  각각의 색상의 퍼즐피스는 애니메이션 시퀀스에 대응시키기로 가정하였다.
    //  애니메이션 시퀀스의 식별자는 문자열이다.
    //  <-- 그래서 해당 값들을 문자열 타입으로 설계했다.
    string mColor[6] = { "BLACK", "PINK", "RED", "GREEN", "BLUE", "YELLOW" };

    //임의의 색상의 퍼즐 피스 25개 배치된 게임보드 속성
    unsigned int mBoarAttrin[5][5] =
    {
        5, 5, 1, 1, 3,
        5, 2, 2, 2, 3,
        5, 4, 2, 3, 2,
        4, 4, 2, 3, 2,
        4, 1, 1, 3, 3
    };

    //선택UI의 위치정보, 행, 열기준 좌표
    int mCurX = 0;
    int mCurY = 0;

    //눈에 보이는 데이터
    //퍼즐 피스 객체
    CPiece* mPieceBoard[5][5] =
    {
        nullptr
    };

    //선택 UI객체
    CSelect* mpUISelect = nullptr;

public:
    CRyuEngine() {};
    virtual ~CRyuEngine() {};

private:
    CRyuEngine(const CRyuEngine& t) = delete;
    CRyuEngine& operator=(const CRyuEngine& t) = delete;

    virtual void OnCreate() override
    {
        CAPIEngine::OnCreate();
    }
    virtual void OnDestroy() override
    {
        CAPIEngine::OnDestroy();
    }

    virtual void OnUpdate(float tDeltaTime) override
    {
        CAPIEngine::OnUpdate(tDeltaTime);

        //render
        this->Clear(0.1f, 0.1f, 0.3f);

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

    //CAPIEngine tEngine;
    CRyuEngine tEngine;
    tEngine.Create(hInstance, nCmdShow);

    MSG msg = { 0 };
    msg = tEngine.Run();

    return (int)msg.wParam;
}


