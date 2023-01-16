// winAPIEngine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#pragma comment(lib, "winAPIEngine.lib")

#include "CPiece.h"
#include "CSelect.h"

#include <time.h>
#include <stack>
using namespace std;

/*
* 이번 스텝에서는
* 
*   ResourcesMgr을 이용한 형태로 코드를 '모두 정리하여' 수정하자.
*   <-- 이렇게 얘기했지만 사실 기존에 CTexture::LoadTexture함수를 호출받아
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
    unsigned int mBoardAttrib[5][5] =
    {
        5, 5, 1, 1, 3,
        5, 2, 2, 2, 3,
        5, 4, 2, 3, 2,
        4, 4, 2, 3, 2,
        4, 1, 1, 3, 3
    };
    //체크보드
    unsigned int mCheckVisit[5][5] =
    {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    //선택UI의 위치정보, 행, 열기준 좌표
    int mCurX = 0;
    int mCurY = 0;

    typedef stack<int> CIntStack;	//행, 열 위치 정보를 기억시켜둘 자료구조
    CIntStack mIntStack;	//행, 열 위치 정보를 기억시켜둘 자료구조

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

        CPathMgr::GetInstance()->MakePath(RSC, L"resources");

        //난수의 씨앗을 뿌린다
        srand(time(nullptr));

        //키입력 매핑값 등록
        CInputMgr::GetInstance()->AddKey("OnMoveLt", VK_LEFT);
        CInputMgr::GetInstance()->AddKey("OnMoveRt", VK_RIGHT);
        CInputMgr::GetInstance()->AddKey("OnMoveUp", VK_UP);
        CInputMgr::GetInstance()->AddKey("OnMoveDown", VK_DOWN);

        CInputMgr::GetInstance()->AddKey("OnSelectHit", VK_SPACE);

        //  '파일'에서 자원 데이터를 로드(메모리)하여 자원관리자에 등록
        //======================================================
        //외부자원 로드
        //대표이미지
        //mpTexPiece = new CTexture();
        //mpTexPiece->LoadTexture(hInst, mhDC, L"resources/block_0.bmp");
        CTexture* tpTex = nullptr;

        for (int ti = 0; ti < 6; ++ti)
        {
            WCHAR szTemp[256] = { 0 };
            wsprintf(szTemp, L"block_%d.bmp", ti);

            //자원의 식별자도 색상 식별자와 똑같이 맞추도록 하겠다
            tpTex = CResourcesMgr::GetInstance()->LoadTexture(mColor[ti], szTemp, RSC);

            if (!tpTex)
            {
                OutputDebugString(L"resources load failed.\n");
                return;
            }
        }

        tpTex = CResourcesMgr::GetInstance()->LoadTexture("select_0", L"select_0.bmp", RSC);
        if (!tpTex)
        {
            OutputDebugString(L"resources load failed.\n");
            return;
        }

        tpTex = CResourcesMgr::GetInstance()->LoadTexture("select_1", L"select_1.bmp", RSC);
        if (!tpTex)
        {
            OutputDebugString(L"resources load failed.\n");
            return;
        }
        //======================================================
        
        //실제로 사용하기 위해 자원관리자에 질의하여 자원데이터(메모리)에 접근하는 부분
        //mpTexUISelect = CResourcesMgr::GetInstance()->FindTexture("select_0");
        //mpTexUISelect = FIND_TEX("select_0");

        //퍼즐 피스의 색상종류를 애니메이션 시퀀스에 대응시킴
        PFPiece = CreatePrefab<CPiece>(FIND_TEX("block_0"), 0.5f, 0.5f, SVector2D(0.0f, 0.0f));
        CAnimator* tpAnimPiece = PFPiece->CreateAnimation("AnimPiece", this);
        tpAnimPiece->AddAniSeq("BLACK", 0.0f, 1, L"resources/block_0", ANI_PO::LOOP, ANI_SO::SHEET_FILE);
        tpAnimPiece->AddAniSeq("PINK", 0.0f, 1, L"resources/block_1", ANI_PO::LOOP, ANI_SO::SHEET_FILE);
        tpAnimPiece->AddAniSeq("RED", 0.0f, 1, L"resources/block_2", ANI_PO::LOOP, ANI_SO::SHEET_FILE);
        tpAnimPiece->AddAniSeq("GREEN", 0.0f, 1, L"resources/block_3", ANI_PO::LOOP, ANI_SO::SHEET_FILE);
        tpAnimPiece->AddAniSeq("BLUE", 0.0f, 1, L"resources/block_4", ANI_PO::LOOP, ANI_SO::SHEET_FILE);
        tpAnimPiece->AddAniSeq("YELLOW", 0.0f, 1, L"resources/block_5", ANI_PO::LOOP, ANI_SO::SHEET_FILE);
        tpAnimPiece->SetDefaultAniSeq("BLACK");

        PFUISelect = CreatePrefab<CSelect>(FIND_TEX("select_0"), 0.5f, 0.5f, SVector2D(0.0f, 0.0f));
        //'대기', '클릭' 두 경우의 애니메이션 시퀀스를 만든다
        CAnimator* tpAnimSelect = PFUISelect->CreateAnimation("AnimSelect", this);
        tpAnimSelect->AddAniSeq("select_0", 0.0f, 1, L"resources/select_0", ANI_PO::LOOP, ANI_SO::SHEET_FILE);
        tpAnimSelect->AddAniSeq("select", 0.3f, 2, L"resources/select", ANI_PO::LOOP, ANI_SO::FRAME_FILE);
        //tpAnimSelect->AddAniSeq("SELECT", 0.3f, 2, L"resources/select", ANI_PO::ONCE, ANI_SO::FRAME_FILE);
        //tpAnimSelect->SetDefaultAniSeq("IDLE");
        tpAnimSelect->SetDefaultAniSeq("select");

        //실제 객체 생성
        for (int tRow = 0; tRow < 5; ++tRow)
        {
            for (int tCol = 0; tCol < 5; ++tCol)
            {
                mPieceBoard[tRow][tCol] = InstantObject<CPiece>(PFPiece);
                mPieceBoard[tRow][tCol]->AddRef();
            }
        }
        //선택UI객체
        mpUISelect = InstantObject<CSelect>(PFUISelect);
        mpUISelect->AddRef();
    }

    virtual void OnDestroy() override
    {
        DestroyObject(mpUISelect);

        for (int tRow = 0; tRow < 5; ++tRow)
        {
            for (int tCol = 0; tCol < 5; ++tCol)
            {
                DestroyObject(mPieceBoard[tRow][tCol]);
            }
        }

        DestroyPrefab(PFUISelect);
        DestroyPrefab(PFPiece);

        //이 코드는 이제 필요없다. 왜냐하면 자원관리자에 의해 이 자원이 관리되기 때문이다.
        //SAFE_DELETE(mpTexUISelect);

        SAFE_DELETE(mpTexPiece);

        CAPIEngine::OnDestroy();
    }

    virtual void OnUpdate(float tDeltaTime) override
    {
        CAPIEngine::OnUpdate(tDeltaTime);

        //키입력에 따른 조작 <-- 나중에는 마우스 처리로 교체 예정
        //경걔처리는 생략하겠습니다. 넣으시면 더 좋아요.
        if (CInputMgr::GetInstance()->KeyUp("OnMoveLt"))
        {
            mCurX = mCurX - 1;
        }

        if (CInputMgr::GetInstance()->KeyUp("OnMoveRt"))
        {
            mCurX = mCurX + 1;
        }

        if (CInputMgr::GetInstance()->KeyUp("OnMoveUp"))
        {
            mCurY = mCurY - 1;
        }

        if (CInputMgr::GetInstance()->KeyUp("OnMoveDown"))
        {
            mCurY = mCurY + 1;
        }

        if (CInputMgr::GetInstance()->KeyUp("OnSelectHit"))
        {
            OutputDebugString(L"==========OnSelectHit==========\n");

            int tColorIndex = mBoardAttrib[mCurY][mCurX];

            WCHAR szTemp[256] = { 0 };
            wsprintf(szTemp, L"color index: %d\n", tColorIndex);
            OutputDebugString(szTemp);

            int tPieceCount = DoCheckCellAttrib(mCurX, mCurY, tColorIndex);

            if (tPieceCount >= 3)
            {
                WCHAR szTemp_0[256] = { 0 };
                wsprintf(szTemp_0, L"continuous color pieces count: %d\n", tPieceCount);
                OutputDebugString(szTemp_0);

                //ClearPieces

                for (int tRow = 0; tRow < 5; ++tRow)
                {
                    for (int tCol = 0; tCol < 5; ++tCol)
                    {
                        //선별한 퍼즐 피스라면
                        if (1 == mCheckVisit[tRow][tCol])
                        {
                            //해당 퍼즐 피스들을 삭제
                            mBoardAttrib[tRow][tCol] = 0;
                        }
                    }
                }

                //SpawnNewPiece;
                for (int tRow = 0; tRow < 5; ++tRow)
                {
                    for (int tCol = 0; tCol < 5; ++tCol)
                    {
                        if (0 == mBoardAttrib[tRow][tCol])
                        {
                            //우리가 원하는 퍼즐 조각의 색상은 [1, 5]까지다.
                            int tRandColor = rand() % 5 + 1;

                            mBoardAttrib[tRow][tCol] = tRandColor;
                        }
                    }
                }
            }
        }

        //update
        int tBoardStartX = 100;
        int tBoardStartY = 100;

        for (int tRow = 0; tRow < 5; ++tRow)
        {
            for (int tCol = 0; tCol < 5; ++tCol)
            {


                //행, 열 단위의 위치값 ---> 픽셀 단위의 위치값
                int tX = tBoardStartX + tCol * 96;
                int tY = tBoardStartY + tRow * 96;

                //해당 퍼즐 피스가 위치할 셀의 위치값을 픽셀 단위로 구하여 설정
                mPieceBoard[tRow][tCol]->SetPosition(SVector2D((float)tX, (float)tY));

                //어느 색상의 퍼즐 피스인가 결정
                int tColorIndex = mBoardAttrib[tRow][tCol];

                string tColorString = mColor[tColorIndex];

                mPieceBoard[tRow][tCol]->PlayAni(tColorString);
            }
        }

        int tX = tBoardStartX + mCurX * 96;    //<-- mCurX 는 열
        int tY = tBoardStartY + mCurY * 96;    //<-- mCurY 는 행
        mpUISelect->SetPosition(SVector2D(tX, tY));
        mpUISelect->Update(tDeltaTime);

        this->Clear(0.1f, 0.1f, 0.3f);

        //셀들에 배치된 퍼즐 피스들
        for (int tRow = 0; tRow < 5; ++tRow)
        {
            for (int tCol = 0; tCol < 5; ++tCol)
            {
                mPieceBoard[tRow][tCol]->Render();
            }
        }
        //선택UI 렌더
        mpUISelect->Render();

        this->Present();
    }

    /*
    * 이 함수의 기능은
    * 임의의 색상이 연속되어 나오는지 검사하는 기능이다.
    * 
    * 이를 위하여 해당 게임 알고리즘에서 검토해야 하는 것은 세 가지 정도다.
    * 
    * i) 색상을 검사한 곳은 검사했다고 '체크'해둔다.<-- 이미 체크해둔 곳은 체크하지 않는다
    * ii) 검사한 셀이 다른 색이라면 '체크'하지 않는다
    * iii) 검사한 셀이 경계를 벗어난 곳이라면 '체크'하지 않는다
    */
    int DoCheckCellAttrib(int tCol, int tRow, int tColorIndex)
    {
        int tResult = 0;

        // 검사 여부 체크 보드
        //0: 체크 안됨, 1: 체크됨
        /*unsigned int mCheckVisit[5][5] =
        {
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0
        };*/

        //clean checkboard
        for (int tRow = 0; tRow < 5; ++tRow)
        {
            for (int tCol = 0; tCol < 5; ++tCol)
            {
                mCheckVisit[tRow][tCol] = 0;
            }
        }

        //clean stack
        while (!mIntStack.empty())
        {
            mIntStack.pop();
        }

        //build stack

        //seed
        //자료구조에 위치 정보 1개 넣는다
        mIntStack.push(tCol);
        mIntStack.push(tRow);

        //스택 자료구조에 행, 열 위치정보가 하나라도 있다면
        while (!mIntStack.empty())
        {
            //LIFO
            tRow = mIntStack.top();
            mIntStack.pop();

            tCol = mIntStack.top();
            mIntStack.pop();

            //=================================
            //상하 경계 처리, 경계를 벗어났으면 체크하지 않는다
            if (tRow < 0 || tRow > 4)
            {
                continue;
            }
            //좌우 경걔처리, 경계를 벗어났으면 체크하지 않는다
            if (tCol < 0 || tCol > 4)
            {
                continue;
            }

            //다른 색상이라면, 체크하지 않는다
            if (tColorIndex != mBoardAttrib[tRow][tCol])
            {
                continue;
            }


            //이미 체크해둔 것이라면, 체크하지 않는다
            if (1 == mCheckVisit[tRow][tCol])
            {
                continue;
            }

            //=================================

            WCHAR szTemp[256] = { 0 };
            wsprintf(szTemp, L"remember CELL: %d, %d\n", tRow, tCol);
            OutputDebugString(szTemp);

            //체크한다.
            mCheckVisit[tRow][tCol] = 1;

            //해당 색상의 퍼즐 조각의 갯수를 카운트한다.
            tResult++;

            //상하좌우 사방의 위치 정보를 스택 자료구조에 넣는다.
            //좌
            mIntStack.push(tCol - 1);
            mIntStack.push(tRow);
            //우
            mIntStack.push(tCol + 1);
            mIntStack.push(tRow);
            //상
            mIntStack.push(tCol);
            mIntStack.push(tRow - 1);
            //하
            mIntStack.push(tCol);
            mIntStack.push(tRow + 1);
        }

        return tResult;
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


