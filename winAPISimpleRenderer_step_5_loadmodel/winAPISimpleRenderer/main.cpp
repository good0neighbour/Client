#include "framework.h"
#pragma comment(lib, "winAPIEngine.lib")

/*
    
    이번 스텝에서는 
    
        모델을 파일에서부터 로드하여 랜더링하자.

        wavefront obj
        웨이브프론트 사에서 만든 3d model파일의 약자 ---> *.obj

        테스트로 사용할 파일의 정보는 다음과 같다.
        cube_888.obj
        정점의 갯수: 24개
        면의 갯수: 12개

    좌표계는 윈도우 좌표계를 가정하자.
    ( 원점은 좌측 상단, x축은 오른쪽 방향이 양의 방향, y 축은 아래 방향이 양의 방향 )

    행벡터를 기준으로 한다.
*/

#include <fstream>  //파일 스트림으로 다룰 수 있도록 하는 라이브러리
#include <strstream>//문자열 스트림으로 다룰 수 있도록 하는 라이브러리, string stream

#include <vector>   //가변배열
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

    RyuPixel color; //면 단위로 색상정보를 결정할 것이므로 여기에 추가
};
//메쉬Mesh는 삼각형들의 모음(정점의 모음)
struct SRyuMesh
{
    vector<SRyuTriangle> tris;

    bool LoadFromObjFile(string tFileName)
    {
        //input file stream
        ifstream tFile(tFileName);
        if (!tFile.is_open())
        {
            //파일 열기 실패
            return false;
        }

        //파일 열기 성공

        //정점데이터들을 기록해둘 지역변수
        vector<SRyuVector3> tVertices;

        //파일의 끝까지 처리
        while (!tFile.eof())
        {
            //'파일'의 임의의 한 줄의 데이터를 기록해둘 지역변수 tLine
            char tLine[256] = { 0 };
            tFile.getline(tLine, 256);  //파일의 임의의 한 줄을 담는다.

            //tLine에 담긴 한 줄의 데이터를 strstream으로 처리하기 위해 tStr에 담는다.
            strstream tStr;
            tStr << tLine;
            //파싱parsing(문자열을 토큰(구분자) 단위로 해석해서 임의의 중요한 요소들을 추출해내는 작업)
            //파싱을 위해 준비된 strstream의 기능을 사용하기 위함

            //불필요한 문자 하나를 걸러내기 위한 지역변수
            char tJunk;
            //정점 데이터 파싱
            if ('v' == tLine[0])
            {
                SRyuVector3 tPos;
                //한 줄에 담긴 데이터를 파싱한다.( 공백(스페이스바문자)을 토큰으로 삼는다 )
                tStr >> tJunk >> tPos.x >> tPos.y >> tPos.z;

                //파싱의 결과로 나온 정점 데이터를 정점 목록에 담아둔다.
                tVertices.push_back(tPos);
            }
            //면 데이터 파싱
            if ('f' == tLine[0])
            {
                int tFace[3];
                //한 줄에 담긴 데이터를 파싱한다. ( 공백(스페이스바)을 토큰으로 삼는다 )
                tStr >> tJunk >> tFace[0] >> tFace[1] >> tFace[2];

                //obj파일에 기록된 색인은
                // 1부터 시작하는 형식으로 되어있으므로
                // 0부터 시작하는 형식으로 바꿔준다.
                this->tris.push_back(
                    { tVertices[tFace[0] - 1], tVertices[tFace[1] - 1], tVertices[tFace[2] - 1] }
                );
            }
        }

        return true;
    }
};



class CRyuEngine : public CAPIEngine
{ 
    float mTheta = 0.0f;

    //카메라 위치
    SRyuVector3 mMainCameraPos;

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



        //test 외적 
        SRyuVector3 tNormal;    //평면의 법선벡터
        SRyuVector3 tLine_A;    //임의의 두 점으로 만든 한변, 벡터
        SRyuVector3 tLine_B;    //임의의 두 점으로 만든 한변, 벡터

        //테스트용 삼각형(면)
        SRyuTriangle t;
        t.p[0] = { 0.0f, 0.0f, 0.0f };
        t.p[1] = { 0.0f, 1.0f, 0.0f };
        t.p[2] = { 1.0f, 1.0f, 0.0f };

        //A벡터
        tLine_A.x = t.p[1].x - t.p[0].x;
        tLine_A.y = t.p[1].y - t.p[0].y;
        tLine_A.z = t.p[1].z - t.p[0].z;
        //B벡터
        tLine_B.x = t.p[2].x - t.p[0].x;
        tLine_B.y = t.p[2].y - t.p[0].y;
        tLine_B.z = t.p[2].z - t.p[0].z;

        //외적의 오른손 법칙.
        //법선 벡터 구하기
        //B cross A 
        //tNormal.x = tLine_B.y * tLine_A.z - tLine_B.z * tLine_A.y;
        //tNormal.y = tLine_B.z * tLine_A.x - tLine_B.x * tLine_A.z;
        //tNormal.z = tLine_B.x * tLine_A.y - tLine_B.y * tLine_A.x;

        //A cross B 
        tNormal.x = tLine_A.y * tLine_B.z - tLine_A.z * tLine_B.y;
        tNormal.y = tLine_A.z * tLine_B.x - tLine_A.x * tLine_B.z;
        tNormal.z = tLine_A.x * tLine_B.y - tLine_A.y * tLine_B.x;

        //정규화
        //크기를 구함
        // C dot C = ||C|| ^ 2
        float tLength = sqrt(tNormal.x* tNormal.x + tNormal.y * tNormal.y + tNormal.z * tNormal.z);

        tNormal.x = tNormal.x * (1.0f / tLength);
        tNormal.y = tNormal.y * (1.0f / tLength);
        tNormal.z = tNormal.z * (1.0f / tLength);

        //wsprintf는 실수 포맷을 지원하지 않는다.
        /*WCHAR tszTemp[256] = { 0 };
        wsprintf(tszTemp, L"tNormal: %f, %f, %f\n", tNormal.x, tNormal.y, tNormal.z);
        OutputDebugString(tszTemp);*/

        char tszTemp[256] = { 0 };
        printf_s(tszTemp, L"tNormal: %f, %f, %f\n", tNormal.x, tNormal.y, tNormal.z);
        OutputDebugStringA(tszTemp);
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
        //SRyuMesh tMesh;
        //tMesh.tris =
        //{
        //    //정점의 나열 순서
        //    //윈도우 좌표계 기준 CCW로 나열( 일반적인 데카르트 좌표계 기준 CW)
        //    //south
        //    {0.0f, 0.0f, 0.0f,          0.0f, 1.0f, 0.0f,       1.0f, 1.0f, 0.0f},
        //    {1.0f, 1.0f, 0.0f,          1.0f, 0.0f, 0.0f,       0.0f, 0.0f, 0.0f},

        //    //east
        //    {1.0f, 0.0f, 0.0f,          1.0f, 1.0f, 0.0f,       1.0f, 1.0f, 1.0f},
        //    {1.0f, 1.0f, 1.0f,          1.0f, 0.0f, 1.0f,       1.0f, 0.0f, 0.0f},

        //    //north
        //    {0.0f, 0.0f, 1.0f,          1.0f, 0.0f, 1.0f,       1.0f, 1.0f, 1.0f},
        //    {1.0f, 1.0f, 1.0f,          0.0f, 1.0f, 1.0f,       0.0f, 0.0f, 1.0f},

        //    //west
        //    {0.0f, 0.0f, 0.0f,          0.0f, 0.0f, 1.0f,       0.0f, 1.0f, 1.0f},
        //    {0.0f, 1.0f, 1.0f,          0.0f, 1.0f, 0.0f,       0.0f, 0.0f, 0.0f},

        //    //top
        //    {0.0f, 0.0f, 0.0f,          1.0f, 0.0f, 0.0f,       1.0f, 0.0f, 1.0f},
        //    {1.0f, 0.0f, 1.0f,          0.0f, 0.0f, 1.0f,       0.0f, 0.0f, 0.0f},

        //    //bottom
        //    {1.0f, 1.0f, 0.0f,          0.0f, 1.0f, 0.0f,       1.0f, 1.0f, 1.0f},
        //    {0.0f, 1.0f, 1.0f,          1.0f, 1.0f, 1.0f,       0.0f, 1.0f, 0.0f}
        //};

        SRyuMesh tMesh;
        tMesh.LoadFromObjFile("resources/cube_888.obj");

        //월드변환: 이동, 회전, 비례 v' = TRSv

        //비례 변환 행렬
        //단위행렬
        float tScale = 1.0f;//100.0f;
        float tMatScale[4][4] = { 0 };
        tMatScale[0][0] = 1.0f*tScale;
        tMatScale[1][1] = 1.0f * tScale;
        tMatScale[2][2] = 1.0f * tScale;
        tMatScale[3][3] = 1.0f;

        //비례 변환 행렬이 적용된 후에 벡터
        SRyuMesh tMeshScale;
        tMeshScale.tris = tMesh.tris;
        //비례 변환 적용
        for (int ti = 0; ti < 12; ++ti)
        {
            MultiplyMatrixVec(tMesh.tris[ti].p[0], tMeshScale.tris[ti].p[0], tMatScale);
            MultiplyMatrixVec(tMesh.tris[ti].p[1], tMeshScale.tris[ti].p[1], tMatScale);
            MultiplyMatrixVec(tMesh.tris[ti].p[2], tMeshScale.tris[ti].p[2], tMatScale);
        }

        //각도 누적
        mTheta = mTheta + 1.0f * tDeltaTime;

        ////Z축을 회전축으로 하는 회전
        //float tMatRot[4][4] = { 0 };
        //tMatRot[0][0] = cosf(mTheta) ;
        //tMatRot[0][1] = sinf(mTheta);
        //tMatRot[1][0] = -1.0f* sinf(mTheta);
        //tMatRot[1][1] = cosf(mTheta);
        //tMatRot[2][2] = 1.0f;
        //tMatRot[3][3] = 1.0f;

        ////Y축을 회전축으로 하는 회전
        float tMatRot[4][4] = { 0 };
        tMatRot[0][0] = cosf(mTheta);
        tMatRot[0][2] = -1.0f*sinf(mTheta);
        tMatRot[1][1] = 1.0f;
        tMatRot[2][0] = sinf(mTheta);
        tMatRot[2][2] = cosf(mTheta);
        tMatRot[3][3] = 1.0f;

        //X축을 회전축으로 하는 회전
        /*float tMatRot[4][4] = { 0 };
        tMatRot[0][0] = 1.0f;        
        tMatRot[1][1] = cosf(mTheta);
        tMatRot[1][2] = sinf(mTheta);
        tMatRot[2][1] = -1.0f*sinf(mTheta);
        tMatRot[2][2] = cosf(mTheta);
        tMatRot[3][3] = 1.0f;*/

        //회전 변환 행렬이 적용된 후에 벡터
        SRyuMesh tMeshRot;
        tMeshRot.tris = tMeshScale.tris;
        //회전 변환 적용
        for (int ti = 0; ti < 12; ++ti)
        {
            MultiplyMatrixVec(tMeshScale.tris[ti].p[0], tMeshRot.tris[ti].p[0], tMatRot);
            MultiplyMatrixVec(tMeshScale.tris[ti].p[1], tMeshRot.tris[ti].p[1], tMatRot);
            MultiplyMatrixVec(tMeshScale.tris[ti].p[2], tMeshRot.tris[ti].p[2], tMatRot);
        }


        //이동변환

        float tMatTranslate[4][4] = { 0 };
        tMatTranslate[0][0] = 1.0f;
        tMatTranslate[1][1] = 1.0f;
        tMatTranslate[2][2] = 1.0f;
        tMatTranslate[3][3] = 1.0f;

        tMatTranslate[3][0] = 0.0f;
        tMatTranslate[3][1] = -0.5f;
        tMatTranslate[3][2] = 5.0f;     //z축 양의 방향으로 5만큼 이동
        

        //이동 변환 행렬이 적용된 후에 벡터
        SRyuMesh tMeshTranslate;
        tMeshTranslate.tris = tMeshRot.tris;
        //이동 변환 적용
        for (int ti = 0; ti < 12; ++ti)
        {
            MultiplyMatrixVec(tMeshRot.tris[ti].p[0], tMeshTranslate.tris[ti].p[0], tMatTranslate);
            MultiplyMatrixVec(tMeshRot.tris[ti].p[1], tMeshTranslate.tris[ti].p[1], tMatTranslate);
            MultiplyMatrixVec(tMeshRot.tris[ti].p[2], tMeshTranslate.tris[ti].p[2], tMatTranslate);
        }







        //뷰변환



        //투영변환
        //원근 투영 변환 행렬

        //수치는 관찰하기 쉽게 여러 테스트 후 정한 수치이다.

        //정직한 버전
        //float tNear = 0.7f;
        //float tFar = 1.0f;
        //float tR = 1.0f*0.5f;
        //float tL = -1.0f*1.0f * 0.5f;
        //float tT = 0.5f*0.5f;
        //float tB = -1.0f*0.5f * 0.5f;
        //
        //float tMatProj[4][4] = { 0 };
        //tMatProj[0][0] = 2.0f*tNear/(tR - tL);
        //tMatProj[1][1] = 2.0f * tNear / (tT - tB);
        //tMatProj[2][2] = -1.0f*(tFar + tNear)/(tFar - tNear);
        //tMatProj[3][2] = -2.0f*tFar*(tNear/(tFar - tNear));

        //tMatProj[2][3] = 1.0f;      //z축 방향을 양의 방향으로 가정
        //tMatProj[3][3] = 0.0f;

        //tMatProj[2][0] = (tR + tL)/(tR - tL);
        //tMatProj[2][1] = (tT + tB)/(tT - tB);


        //시야각 FOV Field of View: 카메라가 볼 수 있는 시야의 범위를 각으로 나타낸 것
        // 종횡비 Apsect Ratio: 가로길이와 세로길이의 비율
        // 
        //종횡비, 시야각 등의 개념을 도입하여 변형된 식 
        float tNear = 0.1f;
        float tFar = 100.0f;
        float tFov = 90.0f;         //90 degree,  시야각. 세로를 기준으로 하겠다.

        //종횡비
        float tAspectRatio = (float)GetScreenHeight() / (float)GetScreenWidth();
        //float tAspectRatio = (float)GetScreenWidth() / (float)GetScreenHeight();
        //시야각 radian 단위
        float tFovRad = 1.0f / tanf((0.5f * tFov) * (3.14159f / 180.0f));
        

        float tMatProj[4][4] = { 0 };
        tMatProj[0][0] = tAspectRatio*tFovRad;  //높이에 비례하여 너비가 결정되게 함
        tMatProj[1][1] = tFovRad;                       // (2*n)/(t-b) = tFovRad
        tMatProj[2][2] = -1.0f * (tFar + tNear) / (tFar - tNear);
        tMatProj[3][2] = -2.0f * tFar * (tNear / (tFar - tNear));

        tMatProj[2][3] = 1.0f;      //z축 방향을 양의 방향으로 가정
        tMatProj[3][3] = 0.0f;

        tMatProj[2][0] = 0.0f;
        tMatProj[2][1] = 0.0f;


        //투영 변환 행렬이 적용된 후에 벡터
        SRyuMesh tMeshProj;
        tMeshProj.tris = tMeshTranslate.tris;
        
        //for (int ti = 0; ti < 2; ++ti)
        int ti = 0;
        for(auto t:tMeshTranslate.tris)
        {
            //임의의 면(삼각형)에 대한 법선벡터 구하기
            SRyuVector3 tNormal;    //평면의 법선벡터
            SRyuVector3 tLine_A;    //임의의 두 점으로 만든 한변, 벡터
            SRyuVector3 tLine_B;    //임의의 두 점으로 만든 한변, 벡터

            //A벡터
            tLine_A.x = t.p[1].x - t.p[0].x;
            tLine_A.y = t.p[1].y - t.p[0].y;
            tLine_A.z = t.p[1].z - t.p[0].z;
            //B벡터
            tLine_B.x = t.p[2].x - t.p[0].x;
            tLine_B.y = t.p[2].y - t.p[0].y;
            tLine_B.z = t.p[2].z - t.p[0].z;

            //외적의 오른손 법칙.
            //법선 벡터 구하기
            //여기서는 법선벡터를 뒤집었다는 개념으로 만들겠다. 그래서 B cross A를 선택했다.
            //B cross A 
            tNormal.x = tLine_B.y * tLine_A.z - tLine_B.z * tLine_A.y;
            tNormal.y = tLine_B.z * tLine_A.x - tLine_B.x * tLine_A.z;
            tNormal.z = tLine_B.x * tLine_A.y - tLine_B.y * tLine_A.x;

            //정규화
            //크기를 구함
            // C dot C = ||C|| ^ 2
            float tLength = sqrt(tNormal.x * tNormal.x + tNormal.y * tNormal.y + tNormal.z * tNormal.z);

            //크기분의 1로 스칼라곱셈
            tNormal.x = tNormal.x * (1.0f / tLength);
            tNormal.y = tNormal.y * (1.0f / tLength);
            tNormal.z = tNormal.z * (1.0f / tLength);

            //시선벡터 View Vector( 카메라 벡터 )를 구하자.

            //삼각형의 중점
            SRyuVector3 tPosMid;
            tPosMid.x = (t.p[0].x + t.p[1].x + t.p[2].x) / 3.0f;
            tPosMid.y = (t.p[0].y + t.p[1].y + t.p[2].y) / 3.0f;
            tPosMid.z = (t.p[0].z + t.p[1].z + t.p[2].z) / 3.0f;

            //시선벡터
            //삼각형의 중점 - 카메라의 위치
            SRyuVector3 tViewVector;
            tViewVector.x = tPosMid.x - mMainCameraPos.x;
            tViewVector.y = tPosMid.y - mMainCameraPos.y;
            tViewVector.z = tPosMid.z - mMainCameraPos.z;

            tLength = sqrt(tViewVector.x * tViewVector.x + tViewVector.y * tViewVector.y + tViewVector.z * tViewVector.z);

            //크기분의 1로 스칼라곱셈
            tViewVector.x = tViewVector.x * (1.0f / tLength);
            tViewVector.y = tViewVector.y * (1.0f / tLength);
            tViewVector.z = tViewVector.z * (1.0f / tLength);

            float tResultDot = tNormal.x * tViewVector.x + tNormal.y * tViewVector.y + tNormal.z * tViewVector.z;

            //
            //
            //램버트 조명 모델 Rambert Lighting Model
            //내적을 이용하여 광량을 결정하는 고전적인 조명 모델
            SRyuVector3 tLightVec = { 0.0f, 0.0f, 1.0f };   //직사광 모델, 위치는 상관이 없고, 방향만 관계있다.
            //정규화
            tLength = sqrt(tLightVec.x * tLightVec.x + tLightVec.y * tLightVec.y + tLightVec.z * tLightVec.z);
            //크기분의 1로 스칼라곱셈
            tLightVec.x = tLightVec.x * (1.0f / tLength);
            tLightVec.y = tLightVec.y * (1.0f / tLength);
            tLightVec.z = tLightVec.z * (1.0f / tLength);

            //조명벡터 vs 법선벡터 내적 <--면에 드리워지는 광량(빛의 양)
            float tResultLight = tLightVec.x * tNormal.x + tLightVec.y * tNormal.y + tLightVec.z * tNormal.z;
            //[-1, 1]

            //하프 램버트 조명모델 half-Rambert Lighting Model
            //[-1, 1] --> [0, 1], 명암의 단계를 보다 부드럽게 표현하는 기법이다.
            tResultLight = tResultLight * 0.5f + 0.5f;

            //3디 공간상에서 광량을 구해 면 단위로 색상값을 기억시켜둠
            RyuPixel tPixel = GetColor(tResultLight);
            t.color = tPixel;

            if (tResultDot >= 0.0f)
            {
                //투영 변환 적용
                MultiplyMatrixVec(t.p[0], tMeshProj.tris[ti].p[0], tMatProj);
                MultiplyMatrixVec(t.p[1], tMeshProj.tris[ti].p[1], tMatProj);
                MultiplyMatrixVec(t.p[2], tMeshProj.tris[ti].p[2], tMatProj);

                //색상 정보도 (랜더링 파이프라인을 통해 ) 전달
                tMeshProj.tris[ti].color = t.color;

                //투영변환 단계를 거쳤으므로 여기서부터는 2D

                //뷰포트변환 
                //이동
                tMeshProj.tris[ti].p[0].x = tMeshProj.tris[ti].p[0].x + 1.0f;
                tMeshProj.tris[ti].p[0].y = tMeshProj.tris[ti].p[0].y + 1.0f;

                tMeshProj.tris[ti].p[1].x = tMeshProj.tris[ti].p[1].x + 1.0f;
                tMeshProj.tris[ti].p[1].y = tMeshProj.tris[ti].p[1].y + 1.0f;

                tMeshProj.tris[ti].p[2].x = tMeshProj.tris[ti].p[2].x + 1.0f;
                tMeshProj.tris[ti].p[2].y = tMeshProj.tris[ti].p[2].y + 1.0f;

                //stretch
                tMeshProj.tris[ti].p[0].x = tMeshProj.tris[ti].p[0].x * 0.5f * GetScreenWidth();
                tMeshProj.tris[ti].p[0].y = tMeshProj.tris[ti].p[0].y * 0.5f * GetScreenHeight();

                tMeshProj.tris[ti].p[1].x = tMeshProj.tris[ti].p[1].x * 0.5f * GetScreenWidth();
                tMeshProj.tris[ti].p[1].y = tMeshProj.tris[ti].p[1].y * 0.5f * GetScreenHeight();

                tMeshProj.tris[ti].p[2].x = tMeshProj.tris[ti].p[2].x * 0.5f * GetScreenWidth();
                tMeshProj.tris[ti].p[2].y = tMeshProj.tris[ti].p[2].y * 0.5f * GetScreenHeight();

                //래스터라이즈
                //DrawTriangle(tMeshProj.tris[ti].p[0].x, tMeshProj.tris[ti].p[0].y,
                    //tMeshProj.tris[ti].p[1].x, tMeshProj.tris[ti].p[1].y,
                    //tMeshProj.tris[ti].p[2].x, tMeshProj.tris[ti].p[2].y);

                //랜더링 파이프라인을 통해 전달받은 색상값으로 픽셀 단위로 삼각형 렌더링
                FillTriangle(tMeshProj.tris[ti].p[0].x, tMeshProj.tris[ti].p[0].y,
                    tMeshProj.tris[ti].p[1].x, tMeshProj.tris[ti].p[1].y,
                    tMeshProj.tris[ti].p[2].x, tMeshProj.tris[ti].p[2].y,

                    tMeshProj.tris[ti].color);
            }

            ++ti;
        }




        //래스터라이즈 단계
        /*for (auto t:tMeshProj.tris)
        {
            DrawTriangle(
                t.p[0].x, t.p[0].y,
                t.p[1].x, t.p[1].y,
                t.p[2].x, t.p[2].y 
            );
        }*/

                
        this->Present();
    }

    //내적의 결과값은 [-1, 1] ----> [0255]
    //  window api GDI에서 픽셀색상은 음수이면 0으로 취급
    RyuPixel GetColor(float tLuminate)
    {
        RyuPixel tColor;

        tColor.r = tLuminate * 255.0f;
        tColor.g = tLuminate * 255.0f;
        tColor.b = tLuminate * 255.0f;

        return tColor;
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
        if ( 0.0f != tW )
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

    
    CRyuEngine tEngine;
    tEngine.Create(hInstance, nCmdShow);

    MSG msg = { 0 };
    msg = tEngine.Run();

    return (int)msg.wParam;
}



