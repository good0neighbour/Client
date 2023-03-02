// winAPIEngine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#pragma comment(lib, "winAPIEngine.lib")

/*
* 소프트웨어 렌더러를 작성해보는 이유는 다음과 같다.
* i)그래픽스 관련 수학에 익숙해지기 위함이다.
* ii)렌더링 파이프라인 과정에 기초적인 이해를 하기 위함이다.
* 
* 이번 스텝에서는
* 삼각형 그리기 코드를 만들고 함수화하여 준비하자.( window api를 사용하여 만들자 )
* <-- 왜냐하면, 픽셀 단위로 삼각형을 그리는 기능이 필요하기 때문이다.
* 
* --> 삼각형은 세 개의 정점으로 이루어진 도형이다.
*   <-- 정점 데이터가 필요하다.
* 
* 선으로 그리기 DrawTriangle
* 면으로 그리기 FillTriangle
* 
* 좌표계는 윈도우 좌표계를 가정하자.
* ( 원점은 좌측 상단, x축은 오른쪽 방향이 양의 방향, y축은 아래 방향이 양의 방향 )
*/

class CRyuEngine : public CAPIEngine
{
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
    }

    virtual void OnDestroy() override
    {

        CAPIEngine::OnDestroy();
    }

    virtual void OnUpdate(float tDeltaTime) override
    {
        CAPIEngine::OnUpdate(tDeltaTime);

        this->Clear(0.1f, 0.1f, 0.3f);

        //step_0
        //window api를 이용하여 삼각형을 그리는 코드를 작성하자

        //선으로 삼각형 그리기
        //정점의 나열의 순서는 (0, 0), (0, 100), (100, 0)
        //( 윈도우 좌표계 기준으로 CCW Counter Clock Wise
        //  일반적인 데카르트 좌표계 기준으로는 CW )

        /*
        //선의 색상, 선의 굵기, 세 점
        float tR = 1.0f;
        float tG = 1.0f;
        float tB = 1.0f;

        int tThickness = 1; //1pixel;

        float P0_x = 0.0f;
        float P0_y = 0.0f;

        float P1_x = 0.0f;
        float P1_y = 100.0f;

        float P2_x = 100.0f;
        float P2_y = 0.0f;

        //wireframe triangle
        //<--색상설정 코드를 이어서 작성하면 됨

        HPEN hPen; //현재 펜 도구
        HPEN hPenOld;   //이전에 사용하던 펜 도구

        hPen = CreatePen(PS_SOLID, tThickness, RGB(tR * 255, tG * 255, tB * 255));

        hPenOld = (HPEN)SelectObject(this->mpBackBuffer->mhDCMem, hPen);

        POINT tPoint = { 0, 0 };
        MoveToEx(this->mpBackBuffer->mhDCMem, P0_x, P0_y, &tPoint);
        LineTo(this->mpBackBuffer->mhDCMem, P1_x, P1_y);
        LineTo(this->mpBackBuffer->mhDCMem, P2_x, P2_y);
        LineTo(this->mpBackBuffer->mhDCMem, P0_x, P0_y);

        hPen = (HPEN)SelectObject(this->mpBackBuffer->mhDCMem, hPenOld);
        DeleteObject(hPen);
        */
        /*
        //면으로 삼각형 그리기
        //선의 색상, 선의 굵기, 세 점
        float tR = 1.0f;
        float tG = 1.0f;
        float tB = 1.0f;

        float P0_x = 0.0f;
        float P0_y = 0.0f;

        float P1_x = 0.0f;
        float P1_y = 100.0f;

        float P2_x = 100.0f;
        float P2_y = 0.0f;

        //정점 3개
        TRIVERTEX vertex[3];
        //첫 번째 정점
        vertex[0].x = P0_x;
        vertex[0].y = P0_y;
        vertex[0].Red = 0xffff * tR;
        vertex[0].Blue = 0xffff * tB;
        vertex[0].Green = 0xffff * tG;
        vertex[0].Alpha = 0x0000;

        //두 번째 정점
        vertex[1].x = P1_x;
        vertex[1].y = P1_y;
        vertex[1].Red = 0xffff * tR;
        vertex[1].Blue = 0xffff * tB;
        vertex[1].Green = 0xffff * tG;
        vertex[1].Alpha = 0x0000;

        //세 번째 정점
        vertex[2].x = P2_x;
        vertex[2].y = P2_y;
        vertex[2].Red = 0xffff * tR;
        vertex[2].Blue = 0xffff * tB;
        vertex[2].Green = 0xffff * tG;
        vertex[2].Alpha = 0x0000;

        //삼각형의 데이터를 색인(인덱스) 정보를 멤버로 가진다.
        GRADIENT_TRIANGLE tTriangle;
        tTriangle.Vertex1 = 0;
        tTriangle.Vertex2 = 1;
        tTriangle.Vertex3 = 2;

        GradientFill(this->mpBackBuffer->mhDCMem, vertex, 3, &tTriangle, 1, GRADIENT_FILL_TRIANGLE);
        */

        //step_1
        DrawTriangle(0, 0, 0, 100, 100, 0);

        FillTriangle(100, 100, 100, 200, 200, 100);

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


