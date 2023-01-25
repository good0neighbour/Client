#include <iostream>

using namespace std;

//3by3 행렬을 정의했다 
struct SMatrix3x3
{
    float m[3][3];
};

//3by1, qprxjsms dufqprxjfh wjddmlgkrpTek
struct SVec
{
    float v[3][1];
};

//영벡터 만들기
void Zero(SVec& tV)
{
    tV.v[0][0] = 0.0f;
    tV.v[1][0] = 0.0f;
    tV.v[2][0] = 0.0f;
}

//영행렬 만들기
void Zero(SMatrix3x3& tMat)
{
    tMat.m[0][0] = tMat.m[0][1] = tMat.m[0][2] = 0.0f;
    tMat.m[1][0] = tMat.m[1][1] = tMat.m[1][2] = 0.0f;
    tMat.m[2][0] = tMat.m[2][1] = tMat.m[2][2] = 0.0f;
}

//단위행렬
void Identity(SMatrix3x3& tMat)
{
    tMat.m[0][0] = 1.0f; tMat.m[0][1] = 0.0f; tMat.m[0][2] = 0.0f;
    tMat.m[1][0] = 0.0f; tMat.m[1][1] = 1.0f; tMat.m[1][2] = 0.0f;
    tMat.m[2][0] = 0.0f; tMat.m[2][1] = 0.0f; tMat.m[2][2] = 1.0f;
}

//행렬끼리의 곱셈(정방행렬을 가정)
void MatrixMultiply(SMatrix3x3& tResult, SMatrix3x3& tA, SMatrix3x3& tB)
{
    //행렬끼리의 곱셈의 정의에 따라 만듦
    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            tResult.m[r][c] = tA.m[r][0] * tB.m[0][c] + tA.m[r][1] * tB.m[1][c] + tA.m[r][2] * tB.m[2][c];
        }
    }
}

//행렬식
// 여인수 전개(라플라스 전개)에 의한 방식으로 구하자
float Determinent(SMatrix3x3& tM)
{
    float tDet = 0.0f;

    tDet = tM.m[0][0] * (tM.m[1][1] * tM.m[2][2] - tM.m[1][2] * tM.m[2][1]) -
        tM.m[1][0] * (tM.m[0][1] * tM.m[2][2] - tM.m[0][2] * tM.m[2][1]) +
        tM.m[2][0] * (tM.m[0][1] * tM.m[1][2] - tM.m[0][2] * tM.m[1][1]);

    return tDet;
}

//역행렬
float Invert(SMatrix3x3& tResult, SMatrix3x3& tMatIn)
{
    float tDet = 0.0f;

    //행렬식을 먼저 구해본다.
    tDet = Determinent(tMatIn);

    //abs절대값 구하기 함수
    if (abs(tDet - 0.0f) < FLT_EPSILON)
    {
        cout << "역행렬이 존재하지 않는다" << endl;
        return tDet;
    }

    //행렬식 값이 0이 아니면, 역행렬이 존재하므로 구한다.
    //inverse determinent       1.0f / tDet
    float iDet = 1.0f / tDet;
    //adjoint A를 구한다
    // i)전치한다
    // ii)'여인수행렬'을 구한다

    tResult.m[0][0] = (tMatIn.m[1][1] * tMatIn.m[2][2] - tMatIn.m[1][2] * tMatIn.m[2][1]) * iDet;
    tResult.m[1][0] = (tMatIn.m[2][0] * tMatIn.m[1][2] - tMatIn.m[1][0] * tMatIn.m[2][2]) * iDet;
    tResult.m[2][0] = (tMatIn.m[1][0] * tMatIn.m[2][1] - tMatIn.m[2][0] * tMatIn.m[1][1]) * iDet;

    tResult.m[0][1] = (tMatIn.m[2][1] * tMatIn.m[0][2] - tMatIn.m[0][1] * tMatIn.m[2][2]) * iDet;
    tResult.m[1][1] = (tMatIn.m[0][0] * tMatIn.m[2][2] - tMatIn.m[2][0] * tMatIn.m[0][2]) * iDet;
    tResult.m[2][1] = (tMatIn.m[0][1] * tMatIn.m[2][0] - tMatIn.m[0][0] * tMatIn.m[2][1]) * iDet;

    tResult.m[0][2] = (tMatIn.m[0][1] * tMatIn.m[1][2] - tMatIn.m[0][2] * tMatIn.m[1][1]) * iDet;
    tResult.m[1][2] = (tMatIn.m[0][2] * tMatIn.m[1][0] - tMatIn.m[0][0] * tMatIn.m[1][2]) * iDet;
    tResult.m[2][2] = (tMatIn.m[0][0] * tMatIn.m[1][1] - tMatIn.m[0][1] * tMatIn.m[1][0]) * iDet;
}

//행렬과 벡터의 곱셈
//  열벡터 기준으로 한다. 행렬과 벡터의 곱셈의 결과는 벡터다.
void MultiplyMatrixVec(SVec& tResult, SMatrix3x3& tMat, SVec& tVec)
{
    tResult.v[0][0] = tMat.m[0][0] * tVec.v[0][0] + tMat.m[0][1] * tVec.v[1][0] + tMat.m[0][2] * tVec.v[2][0];
    //tResult.v[1][0] = ;
    //tResult.v[2][0] = ;
}

void DisplayVec(SVec& tVec)
{
    for (int tRow = 0; tRow < 3; ++tRow)
    {
        cout << tVec.v[tRow][0] << "\t";
    }
}

void DisplayMatrix(SMatrix3x3& tMat)
{
    for (int tRow = 0; tRow < 3; ++tRow)
    {
        for (int tCol = 0; tCol < 3; ++tCol)
        {
            cout << tMat.m[tRow][tCol] << "\t";
        }

        cout << endl;
    }
};

int main()
{
    //step_0

    SVec tVecA;
    Zero(tVecA);
    DisplayVec(tVecA);

    cout << endl;

    SMatrix3x3 tMatA;
    Zero(tMatA);
    DisplayMatrix(tMatA);

    Identity(tMatA);
    DisplayMatrix(tMatA);

    cout << endl;

    //step_1
    //행렬끼리의 곱셈
    /*
    * 행렬의 곱셈은 교환법칙이 성립하지 않는다
    * | 1 0 || 1 1 | = | 1 1 |
    * | 1 2 || 0 0 |   | 1 1 |
    * 
    * | 1 1 || 1 0 | = | 2 2 |
    * | 0 0 || 1 2 |   | 0 0 |
    */
    SMatrix3x3 tResult;
    Zero(tResult);

    SMatrix3x3 tX =
    {
        1.0f, 0.0f, 0.0f, 
        1.0f, 2.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    };

    SMatrix3x3 tY =
    {
        1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    };

    cout << endl;

    MatrixMultiply(tResult, tX, tY);
    DisplayMatrix(tResult);

    cout << endl;

    MatrixMultiply(tResult, tY, tX);
    DisplayMatrix(tResult);

    //step_2
    //행렬식

    SMatrix3x3 tMatC =
    {
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f, 9.0f
    };

    float tDet = Determinent(tMatC);

    cout << "tMatC의 행렬식:" << tDet << endl;

    //step_3
    //역행렬

    SMatrix3x3 tInverse;
    Zero(tInverse);
    SMatrix3x3 tH =
    {
        1.0f, -1.0f, 1.0f,
        0.0f, 2.0f, 1.0f,
        1.0f, 3.0f, 0.0f
    };

    float tDet_0 = Invert(tInverse, tH);

    DisplayMatrix(tInverse);

    return 0;
}