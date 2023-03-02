// winAPIEngine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#pragma comment(lib, "winAPIEngine.lib")

/*
* 이번 스텝에서는
* 
* 최종적으로는
* 원근투영변환 적용을 하도록 하자.
* 거기에 더해, 월드 변환 중 회전변환행렬, 이동변환행렬도 적용해보도록 하자.
* 
* 좌표계는 윈도우 좌표계를 가정하자.
* ( 원점은 좌측 상단, x축은 오른쪽 방향이 양의 방향, y축은 아래 방향이 양의 방향 )
* 
* 행벡터를 기준으로 한다.
*/

#include <vector> //가변배열
using namespace std;

//3차원 공간에서 벡터와 점(위치)를 다루기 위해 정의
struct SRyuVector3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

//삼각형: 정점 3개의 모음
struct SRyuTriangle
{
    SRyuVector3 p[3];
};

//메쉬Mesh는 삼각형들의 모음(정점의 모음)
struct SRyuMesh
{
    vector<SRyuTriangle> tris;
};

class CRyuEngine : public CAPIEngine
{
    float mTheta = 0.0f;

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

        //모델을 만들자
        SRyuMesh tMesh;
        tMesh.tris =
        {
            //정점의 나열 순서
            //윈도우 좌표계 기준 CCW로 나열( 일반적인 데카르트 좌표계 기준 CW)
            { 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f }
        };

        //월드변환: 이동, 화전, 비례 v' = TRSv

        //비례 변환 행렬
        //단위행렬
        float tScale = 100.0f;
        float tMatScale[4][4] = { 0 };
        tMatScale[0][0] = 1.0f * tScale;
        tMatScale[1][1] = 1.0f * tScale;
        tMatScale[2][2] = 1.0f * tScale;
        tMatScale[3][3] = 1.0f;

        //비례 변환 행렬이 적용된 후의 벡터
        SRyuMesh tMeshScale;
        tMeshScale.tris =
        {
            { 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f }
        };

        //비례변환 적용
        for (int ti = 0; ti < 2; ++ti)
        {
            MultiplyMatrixVec(tMesh.tris[ti].p[0], tMeshScale.tris[ti].p[0], tMatScale);
            MultiplyMatrixVec(tMesh.tris[ti].p[1], tMeshScale.tris[ti].p[1], tMatScale);
            MultiplyMatrixVec(tMesh.tris[ti].p[2], tMeshScale.tris[ti].p[2], tMatScale);
        }

        //각도 누적
        mTheta = mTheta + 1.0f * tDeltaTime;

        float tMatRot[4][4] = { 0 };
        tMatRot[0][0] = cosf(mTheta);
        tMatRot[0][1] = sinf(mTheta);
        tMatRot[1][0] = -1.0f * sinf(mTheta);
        tMatRot[1][1] = cosf(mTheta);
        tMatRot[2][2] = 1.0f;
        tMatRot[3][3] = 1.0f;

        //회전 변환 행렬이 적용된 후의 벡터
        SRyuMesh tMeshRot;
        tMeshRot.tris =
        {
            { 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f }
        };

        //회전변환 적용
        for (int ti = 0; ti < 2; ++ti)
        {
            MultiplyMatrixVec(tMeshScale.tris[ti].p[0], tMeshRot.tris[ti].p[0], tMatRot);
            MultiplyMatrixVec(tMeshScale.tris[ti].p[1], tMeshRot.tris[ti].p[1], tMatRot);
            MultiplyMatrixVec(tMeshScale.tris[ti].p[2], tMeshRot.tris[ti].p[2], tMatRot);
        }

        //뷰변환

        //투영변환
        //투영 변환 행렬
        //단위행렬
        float tMatProj[4][4] = { 0 };
        tMatProj[0][0] = 1.0f;
        tMatProj[1][1] = 1.0f;
        tMatProj[2][2] = 1.0f;
        tMatProj[3][3] = 1.0f;

        //투영 변환 행렬이 적용된 후의 벡터
        SRyuMesh tMeshProj;
        tMeshProj.tris =
        {
            { 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f }
        };

        //투영변환 적용
        for (int ti = 0; ti < 2; ++ti)
        {
            MultiplyMatrixVec(tMeshRot.tris[ti].p[0], tMeshProj.tris[ti].p[0], tMatProj);
            MultiplyMatrixVec(tMeshRot.tris[ti].p[1], tMeshProj.tris[ti].p[1], tMatProj);
            MultiplyMatrixVec(tMeshRot.tris[ti].p[2], tMeshProj.tris[ti].p[2], tMatProj);
        }

        //래스터라이즈 단계
        for (auto t : tMeshProj.tris)
        {
            DrawTriangle(
                t.p[0].x, t.p[0].y,
                t.p[1].x, t.p[1].y,
                t.p[2].x, t.p[2].y
            );
        }

        this->Present();
    }

    //'행벡터 기준'으로 작성하겠다.
    //행렬과 벡터의 곱셈, 결과는 벡터
    void MultiplyMatrixVec(SRyuVector3& tIn, SRyuVector3& tOut, float tM[][4])
    {
        tOut.x = tIn.x * tM[0][0] + tIn.y * tM[1][0] + tIn.z * tM[2][0] + 1.0f * tM[3][0];
        tOut.y = tIn.x * tM[0][1] + tIn.y * tM[1][1] + tIn.z * tM[2][1] + 1.0f * tM[3][1];
        tOut.z = tIn.x * tM[0][2] + tIn.y * tM[1][2] + tIn.z * tM[2][2] + 1.0f * tM[3][2];

        float tW = tIn.x * tM[0][3] + tIn.y * tM[1][3] + tIn.z * tM[2][3] + 1.0f * tM[3][3];

        //수학에서 동차좌표의 개념에 의하여 원래 차원으로 돌아가도록 만들어주는 표현이다
        //그래픽스 이론으로 보면, 임의의 값에 대하여 변형된 공간의 관련값을 원래대로 돌려준다고 보는 것이 타당하다.
        if (0.0f != tW)
        {
            tOut.x = tOut.x / tW;
            tOut.y = tOut.y / tW;
            tOut.z = tOut.z / tW;
        }
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


