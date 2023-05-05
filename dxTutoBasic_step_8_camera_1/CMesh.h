#pragma once

#include "framework.h"
#include <vector>
#include <string>

#include <fstream>      //<--파일을 다루기 위해
#include <strstream>    //<-- 파싱(parsing 임의의 토큰을 기반으로 구성요소들을 분리해내는 것)을 위한 여러 기능을 제공하므로 이것을 사용
class CDXEngine;
class CTextureRyu;


//정점의 '위치/UV/normal' 를 한쌍으로 다루기 위한 구조체
struct TEST
{
    int mFace;
    int mUV;
    int mNormal;
};



class CMesh
{
public:
    CMesh()
    {
        mpVertexBuffer = nullptr;
        mpIndexBuffer = nullptr;

        mCountVertex = 0;
        mCountIndex = 0;

        mpEngine = nullptr;
    }
    //복사는 막는다
    CMesh(const CMesh& t) = delete;
    void operator=(const CMesh& t) = delete;


    //외부에서 호출될 함수들
public:
    void Create(ID3D11Device* tpDevice, LPCWSTR tFileNameModel, LPCWSTR tFileName);
    void Destroy();
    void Render();

    void SetEngine(CDXEngine* tpEngine)
    {
        mpEngine = tpEngine;
    }

    //인덱스데이터 갯수 리턴
    int GetCountIndex() const
    {
        return mCountIndex;
    }

    ID3D11ShaderResourceView* GetTexture() const;


    //내부에서 호출될 함수들
private:
    void CreateBuffers(ID3D11Device* tpDevice, LPCWSTR tFileName);
    void DestroyBuffers();
    void RenderBuffers(ID3D11DeviceContext* tpDeviceContext);


private:
    ID3D11Buffer* mpVertexBuffer = nullptr;
    ID3D11Buffer* mpIndexBuffer = nullptr;

    int mCountVertex = 0;       //정점 데이터의 갯수
    int mCountIndex = 0;        //인덱스 데이터의 갯수


    //엔진 참조 변수
    CDXEngine* mpEngine = nullptr;



    CTextureRyu* mpTextureRyu = nullptr;

    //new
private:
    //--------------------------
    vector<XMFLOAT3> mVertices;         //'최종적으로' 만들어질 정점 위치 데이터를 담아둘 자료구조
    vector<XMFLOAT2> mUVs;              //'최종적으로' 만들어질 UV 데이터를 담아둘 자료구조
    vector<XMFLOAT3> mNormals;          //'최종적으로' 만들어질 법선 데이터를 담아둘 자료구조

    vector<int> mIndexVertices;             //'최종적으로' 만들어질 정점의 인덱스 데이터를 담아둘 자료구조
    vector<int> mIndexUVs;             //'최종적으로' 만들어질 UV의 인덱스 데이터를 담아둘 자료구조
    vector<int> mIndexNormals;             //'최종적으로' 만들어질 법선의 인덱스 데이터를 담아둘 자료구조
    //--------------------------

    //재가공의 중간단계에 필요한 자료구조들이다
    vector<TEST> mRYUIndexs;            //파일에 기록된 'vertexIndex/UVIndex/NormalIndex'를 한쌍으로 다룰 자료구조
    vector<XMFLOAT3> mTT;               //파일에 기록된 '정점 정보'의 데이터들을 담아둘 가변배열
    vector<XMFLOAT2> mAAAA;         //파일에 기록된 'UV텍스처좌표 정보'의 데이터들을 담아둘 가변배열
    vector<XMFLOAT3> mNN;           //파일에 기록된 '정점 정보'의 데이터들을 담아둘 가변배열


    //obj 파일로드 함수
    bool  LoadFromObjFile(string tFileName)
    {
        ifstream tFile(tFileName);

        if (!tFile.is_open())
        {
            //파일이 열리지 않음
            return false;
        }

        //파일이 열림

        //인덱스 카운트 용 변수
        int tIndexReal = 0;

        //파일의 끝까지 반복
        while (!tFile.eof())
        {
            //'파일'의 한 라인의 데이터를 기억해둘 지역'변수'.
            char tLine[256] = { 0 };
            tFile.getline(tLine, 256);

            //strstream 타입으로 지역변수를 하나 선언하였다.
            //파싱(parsing 임의의 토큰을 기반으로 구성요소들을 분리해내는 것)을 위한 여러 기능을 제공하므로 이것을 사용
            strstream tStr;
            tStr << tLine;

            //불필요한 문자 하나를 걸러내기 위한 지역변수
            char tJunk;

            if ('v' == tLine[0])
            {
                if ('n' == tLine[1])
                {
                    XMFLOAT3 tNormal;
                    tStr >> tJunk >> tJunk >> tNormal.x >> tNormal.y >> tNormal.z;

                    //좌우이므로 x축
                    //좌우 반전 값을 적용한다 ---> -1을 곱한다
                    tNormal.x = tNormal.x * (-1.0f);

                    mNN.push_back(tNormal);
                }
                //텍스처 좌표
                if ('t' == tLine[1])
                {
                    XMFLOAT2 tUV;
                    tStr >> tJunk >> tJunk >> tUV.x >> tUV.y;

                    mAAAA.push_back(tUV);
                }
            }

            //정점
            if ('v' == tLine[0])
            {
                if ('n' != tLine[1] && 't' != tLine[1])
                {
                    XMFLOAT3 tPos;
                    tStr >> tJunk >> tPos.x >> tPos.y >> tPos.z;

                    //좌우이므로 x축
                    //좌우 반전 값을 적용한다 ---> -1을 곱한다
                    tPos.x = tPos.x * (-1.0f);

                    mTT.push_back(tPos);
                }
            }

            //면
            if ('f' == tLine[0])
            {
                /*int tIndexVertex[3];
                int tIndexUV[3];
                int tIndexNormal[3];*/
                TEST tTest[3];

                /*tStr >> tJunk >>
                    tTest[0].mFace >> tJunk >> tTest[0].mUV >> tJunk >> tTest[0].mNormal >>
                    tTest[1].mFace >> tJunk >> tTest[1].mUV >> tJunk >> tTest[1].mNormal >>
                    tTest[2].mFace >> tJunk >> tTest[2].mUV >> tJunk >> tTest[2].mNormal;*/

                //좌우 반전을 고려하여
                //인덱싱 순서를 바꿔준다
                // 0 1 2 -----> 0 2 1
                tStr >> tJunk >>
                    tTest[0].mFace >> tJunk >> tTest[0].mUV >> tJunk >> tTest[0].mNormal >>
                    tTest[2].mFace >> tJunk >> tTest[2].mUV >> tJunk >> tTest[2].mNormal >>
                    tTest[1].mFace >> tJunk >> tTest[1].mUV >> tJunk >> tTest[1].mNormal;


                //중간 재가공. 해당 단계를 거치고 나야 모든 인덱스 정보가 제대로 설정된다

                bool tIsBe = false;

                //면을 이루는 첫번째(0번째) 정점의 데이터의 경우를 처리
                //순차검색
                //이전에 구축된 인덱스 데이터라면 여기서 체크
                for (int ti = 0; ti < mRYUIndexs.size(); ++ti)
                {
                    if (tTest[0].mFace == mRYUIndexs[ti].mFace &&
                        tTest[0].mUV == mRYUIndexs[ti].mUV &&
                        tTest[0].mNormal == mRYUIndexs[ti].mNormal)
                    {
                        mIndexVertices.push_back(ti);
                        mIndexUVs.push_back(ti);
                        mIndexNormals.push_back(ti);

                        tIsBe = true;
                        break;
                    }
                }

                //해당 자료구조에 포함되어 있지 않은 경우 ( 새로운 것이라면 ) 추가해야징
                if (!tIsBe)
                {
                    mRYUIndexs.push_back(tTest[0]);

                    mIndexVertices.push_back(tIndexReal);
                    mIndexUVs.push_back(tIndexReal);
                    mIndexNormals.push_back(tIndexReal);

                    tIndexReal++;
                }

                //================================================================================

                tIsBe = false;

                //면을 이루는 첫번째(1번째) 정점의 데이터의 경우를 처리
                //순차검색
                //이전에 구축된 인덱스 데이터라면 여기서 체크
                for (int ti = 0; ti < mRYUIndexs.size(); ++ti)
                {
                    if (tTest[1].mFace == mRYUIndexs[ti].mFace &&
                        tTest[1].mUV == mRYUIndexs[ti].mUV &&
                        tTest[1].mNormal == mRYUIndexs[ti].mNormal)
                    {
                        mIndexVertices.push_back(ti);
                        mIndexUVs.push_back(ti);
                        mIndexNormals.push_back(ti);

                        tIsBe = true;
                        break;
                    }
                }

                //해당 자료구조에 포함되어 있지 않은 경우 ( 새로운 것이라면 ) 추가해야징
                if (!tIsBe)
                {
                    mRYUIndexs.push_back(tTest[1]);

                    mIndexVertices.push_back(tIndexReal);
                    mIndexUVs.push_back(tIndexReal);
                    mIndexNormals.push_back(tIndexReal);

                    tIndexReal++;
                }

                //================================================================================

                tIsBe = false;

                //면을 이루는 첫번째(2번째) 정점의 데이터의 경우를 처리
                //순차검색
                //이전에 구축된 인덱스 데이터라면 여기서 체크
                for (int ti = 0; ti < mRYUIndexs.size(); ++ti)
                {
                    if (tTest[2].mFace == mRYUIndexs[ti].mFace &&
                        tTest[2].mUV == mRYUIndexs[ti].mUV &&
                        tTest[2].mNormal == mRYUIndexs[ti].mNormal)
                    {
                        mIndexVertices.push_back(ti);
                        mIndexUVs.push_back(ti);
                        mIndexNormals.push_back(ti);

                        tIsBe = true;
                        break;
                    }
                }

                //해당 자료구조에 포함되어 있지 않은 경우 ( 새로운 것이라면 ) 추가해야징
                if (!tIsBe)
                {
                    mRYUIndexs.push_back(tTest[2]);

                    mIndexVertices.push_back(tIndexReal);
                    mIndexUVs.push_back(tIndexReal);
                    mIndexNormals.push_back(tIndexReal);

                    tIndexReal++;
                }




                /*
                //mIndexVertices.push_back(tIndexVertex[0] - 1);
                //mIndexVertices.push_back(tIndexVertex[1] - 1);
                //mIndexVertices.push_back(tIndexVertex[2] - 1);

                //mIndexUVs.push_back(tIndexUV[0] - 1);
                //mIndexUVs.push_back(tIndexUV[1] - 1);
                //mIndexUVs.push_back(tIndexUV[2] - 1);

                //mIndexNormals.push_back(tIndexNormal[0] - 1);
                //mIndexNormals.push_back(tIndexNormal[1] - 1);
                //mIndexNormals.push_back(tIndexNormal[2] - 1);
                */
            }//f
        }//while



        for (int ti = 0; ti < mRYUIndexs.size(); ++ti)
        {
            //각 데이터 종류별 인덱스를 뽑아낸다
            int tIndexFace          = mRYUIndexs[ti].mFace - 1;
            int tIndexUV            = mRYUIndexs[ti].mUV - 1;
            int tIndexNormal        = mRYUIndexs[ti].mNormal - 1;

            //이것이 바로
            //실제 랜더링에 참조할 데이터들이다
            mVertices.push_back(mTT[tIndexFace]);
            mUVs.push_back(mAAAA[tIndexUV]);
            mNormals.push_back(mNN[tIndexNormal]);
        }




        return true;
    }





    //old
    /*
private:
    vector<XMFLOAT3> mNormals;      //파일에 기록된 '법선 정보'의 데이터들을 담아둘 가변배열
    vector<XMFLOAT2> mUVs;              //파일에 기록된 '텍스처좌표 정보'의 데이터들을 담아둘 가변배열
    vector<XMFLOAT3> mVertices;         //파일에 기록된 '정점 정보'의 데이터들을 담아둘 가변배열

    vector<int> mIndexVertices;         //파일에 기록된 '정점 인덱스' 정보의 데이터들을 담아둘 가변배열
    vector<int> mIndexUVs;              //파일에 기록된 텍스처 '좌표 인덱스' 정보의 데이터들을 담아둘 가변배열
    vector<int> mIndexNormals;          //파일에 기록된 '법선 인덱스' 정보의 데이터들을 담아둘 가변배열

    //obj 파일 로드 함수
    bool  LoadFromObjFile(string tFileName)
    {
        ifstream tFile(tFileName);

        if (!tFile.is_open())
        {
            //파일이 열리지 않음
            return false;
        }

        //파일이 열림

        //파일의 끝까지 반복
        while (!tFile.eof())
        {
            //'파일'의 한 라인의 데이터를 기억해둘 지역'변수'.
            char tLine[256] = { 0 };
            tFile.getline(tLine, 256);

            //strstream 타입으로 지역변수를 하나 선언하였다.
            //파싱(parsing 임의의 토큰을 기반으로 구성요소들을 분리해내는 것)을 위한 여러 기능을 제공하므로 이것을 사용
            strstream tStr;
            tStr << tLine;

            //불필요한 문자 하나를 걸러내기 위한 지역변수
            char tJunk;

            if ('v' == tLine[0])
            {
                if ('n' == tLine[1])
                {
                    XMFLOAT3 tNormal;
                    tStr >> tJunk >> tJunk >> tNormal.x >> tNormal.y >> tNormal.z;

                    mNormals.push_back(tNormal);
                }
                //텍스처 좌표
                if ('t' == tLine[1])
                {
                    XMFLOAT2 tUV;
                    tStr >> tJunk >> tJunk >> tUV.x >> tUV.y;

                    mUVs.push_back(tUV);
                }
            }

            //정점
            if ('v' == tLine[0])
            {
                if ('n' != tLine[1] && 't' != tLine[1])
                {
                    XMFLOAT3 tPos;
                    tStr >> tJunk >> tPos.x >> tPos.y >> tPos.z;

                    //좌우로 뒤집었다.
                    //tPos.x = (-1.0f) * tPos.x;

                    mVertices.push_back(tPos);
                }
            }

            //면
            if ('f' == tLine[0])
            {
                int tIndexVertex[3];
                int tIndexUV[3];
                int tIndexNormal[3];

                tStr >> tJunk >>
                    tIndexVertex[0] >> tJunk >> tIndexUV[0] >> tJunk >> tIndexNormal[0] >>
                    tIndexVertex[1] >> tJunk >> tIndexUV[1] >> tJunk >> tIndexNormal[1] >>
                    tIndexVertex[2] >> tJunk >> tIndexUV[2] >> tJunk >> tIndexNormal[2];

                mIndexVertices.push_back(tIndexVertex[0] - 1);
                mIndexVertices.push_back(tIndexVertex[1] - 1);
                mIndexVertices.push_back(tIndexVertex[2] - 1);

                mIndexUVs.push_back(tIndexUV[0] - 1);
                mIndexUVs.push_back(tIndexUV[1] - 1);
                mIndexUVs.push_back(tIndexUV[2] - 1);

                mIndexNormals.push_back(tIndexNormal[0] - 1);
                mIndexNormals.push_back(tIndexNormal[1] - 1);
                mIndexNormals.push_back(tIndexNormal[2] - 1);
            }
        }//while
        return true;
    }
    */
};










/*
//랜더링에 사용할 resource자원 <-- directx 에서 정의하고 있다.
//정점 버퍼의 참조이다. 이것은 그래픽 처리 장치에 만들어진 변수(메모리)를 제어하는 것이다.
ID3D11Buffer* mpVertexBuffer = nullptr;

//인덱스 버퍼 ( 글카에 있다 )
//정점 버퍼가 실제 정점(꼭지점)의 값의 집합이라면
//인덱스 버퍼의 내용은 그러한 꼭지점 들을 삼각형을 이루는 세 점을 나열 순서에 따라 나열해놓은 정보를 말한다
//이러한 표현이 가능한 것은 임의의 폴리곤(예를 들면 여기서는 정육면체)의 임의의 점들은 서로 그 값이 같으므로
//공유하기 때문이다.
ID3D11Buffer* mpIndexBuffer = nullptr;

*/