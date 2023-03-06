#pragma once

#include "framework.h"
#include <vector>

#include <fstream>      //<--파일을 흐르는 데이터 개념으로 다루기 위해
#include <strstream>    //<--문자열을 흐르는 데이터 개념으로 다루기 위해


using namespace std;

//클래스 전방선언
class CDxEngine;
class CTextureRyu;


//정점의 '위치/UV/normal' 를 한쌍으로 다루기 위한 구조체
struct TEST
{
    int mFace;      //정점의 위치 정보 인덱스
    int mUV;        //정점의 uv 정보 인덱스
    int mNormal;    //정점의 normal 정보 인덱스
};


class CMesh
{
private:
    ID3D11Buffer* mpVertexBuffer = nullptr;    
    ID3D11Buffer* mpIndexBuffer = nullptr;

    int mCountVertex = 0;       //정점 데이터의 갯수
    int mCountIndex= 0;       //인덱스 데이터의 갯수

    CDxEngine* mpEngine = nullptr;

    CTextureRyu* mpTextureRyu = nullptr;

public:
    CMesh()
    {
        mpVertexBuffer = nullptr;
        mpIndexBuffer = nullptr;
        mCountVertex = 0; 
        mCountIndex = 0;
        mpEngine = nullptr;
    };
    ~CMesh() {};
    //복사는 막는다 
    CMesh(const CMesh& t) = delete;
    void operator=(const CMesh& t) = delete;

    //외부에서 호출될 함수
public:
    void Create(ID3D11Device* tpDevice, LPCWSTR tFileName);
    void Destroy();
    void Render();

    inline void SetEngine(CDxEngine* tpEngine)
    {
        mpEngine = tpEngine;
    }
    //인덱스 데이터의 갯수 리턴 
    inline int GetCountIndex() const
    {
        return mCountIndex;
    }

    ID3D11ShaderResourceView* GetTexture() const;



    //내부에서 호출될 함수
private:
    void CreateBuffers(ID3D11Device* tpDevice);
    void DestroyBuffers();
    void RenderBuffers(ID3D11DeviceContext* tpDeviceContext);



private:
    vector<XMFLOAT3> mNormals;      //'최종적으로 만들어질' 법선 정보 의 데이터들을 담아둘 가변배열
    vector<XMFLOAT2> mUVs;              //'최종적으로 만들어질' UV 정보 의 데이터들을 담아둘 가변배열
    vector<XMFLOAT3> mVertices;         //'최종적으로 만들어질' 정점위치 정보 의 데이터들을 담아둘 가변배열

    vector<int> mIndexVertices;         //'최종적으로 만들어질' 정점위치의 인덱스 정보 의 데이터들을 담아둘 가변배열
    vector<int> mIndexUVs;              //'최종적으로 만들어질' UV 정보의  인덱스 정보의 데이터들을 담아둘 가변배열
    vector<int> mIndexNormals;          //'최종적으로 만들어질' 법선 정보 인덱스 정보의 데이터들을 담아둘 가변배열


    //재가공의 중간단계에 필요한 자료구조들이다 
    vector<TEST> mRYUIndexs;          //파일에 기록된 'vertexIndex/uvIndex/normalIndex'를 한쌍으로 다룰 자료구조
    vector<XMFLOAT3> mTT;           //파일에 기록된 '정점  위치정보'의 데이터들을 담아둘 가변배열
    vector<XMFLOAT2> mAAAA;     //파일에 기록된 'UV텍스처좌표 정보'의 데이터들을 담아둘 가변배열
    vector<XMFLOAT3> mNN;       //파일에 기록된 '법선 정보'의 데이터들을 담아둘 가변배열


private:
    bool LoadFromObjFile(string tFileName)
    {
        ifstream tFile(tFileName);

        if (!tFile.is_open())
        {
            //파일이 열리지 않은 경우
            return false;
        }

        //파일이 열린 경우

        //위치정보/uv정보/normal정보 가 결합된 하나의 정점인덱스 개념의 변수
        int tRealIndex = 0;

        //파일의 끝줄까지 반복
        while (!tFile.eof())
        {
            //한 줄씩 내용을 가져온다.
            char tLine[256] = { 0 };
            tFile.getline(tLine, 256);

            //strstream에서 제공하는 파싱기능을 이용하기 위해 
            strstream tStr;
            tStr << tLine;

            //불필요한 문자 처리용
            char tJunk = 0;

            if ('v' == tLine[0])
            {
                //법선 벡터
                if ('n' == tLine[1])
                {
                    XMFLOAT3 tNormal;
                    tStr >> tJunk >> tJunk >> tNormal.x >> tNormal.y >> tNormal.z;

                    //좌우이므로 x축
                    //좌우 반전을 위해 x값을 뒤집는다.( -1을 곱한다 )
                    tNormal.x = tNormal.x * (-1.0f);

                    //파일에서 가져온 데이터를 그대로 적재함
                    mNN.push_back(tNormal);
                }
                //텍스처 좌표
                if ('t' == tLine[1])
                {
                    XMFLOAT2 tUV;
                    tStr >> tJunk >> tJunk >> tUV.x >> tUV.y;
                    //파일에서 가져온 데이터를 그대로 적재함
                    mAAAA.push_back(tUV);
                }
            }
            //정점
            if ('v' == tLine[0])
            {
                if ( 'n' != tLine[1] && 't' != tLine[1])
                {               
                    XMFLOAT3 tPos;
                    tStr >> tJunk >> tPos.x >> tPos.y >> tPos.z;

                    //좌우이므로 x축
                    //좌우 반전을 위해 x값을 뒤집는다.( -1을 곱한다 )
                    tPos.x = tPos.x * (-1.0f);


                    //파일에서 가져온 데이터를 그대로 적재함
                    mTT.push_back(tPos);
                }
            }
            //면의 인덱스
            if ('f' == tLine[0])
            {
                /*int tIndexVertex[3];
                int tIndexUV[3];
                int tIndexNormal[3];*/

                /*tStr >> tJunk >> 
                    tIndexVertex[0] >> tJunk >> tIndexUV[0]>>tJunk>>tIndexNormal[0]>>
                    tIndexVertex[1] >> tJunk >> tIndexUV[1] >> tJunk >> tIndexNormal[1] >>
                    tIndexVertex[2] >> tJunk >> tIndexUV[2] >> tJunk >> tIndexNormal[2];*/


                /*mIndexVertices.push_back(tIndexVertex[0] - 1);
                mIndexVertices.push_back(tIndexVertex[1] - 1);
                mIndexVertices.push_back(tIndexVertex[2] - 1);

                mIndexUVs.push_back(tIndexUV[0] - 1);
                mIndexUVs.push_back(tIndexUV[1] - 1);
                mIndexUVs.push_back(tIndexUV[2] - 1);

                mIndexNormals.push_back(tIndexNormal[0] - 1);
                mIndexNormals.push_back(tIndexNormal[1] - 1);
                mIndexNormals.push_back(tIndexNormal[2] - 1);*/

                TEST tTest[3];

                /*tStr >> tJunk >>
                    tTest[0].mFace >> tJunk >> tTest[0].mUV >> tJunk >> tTest[0].mNormal >>
                    tTest[1].mFace >> tJunk >> tTest[1].mUV >> tJunk >> tTest[1].mNormal >>
                    tTest[2].mFace >> tJunk >> tTest[2].mUV >> tJunk >> tTest[2].mNormal;*/

                tStr >> tJunk >>
                    tTest[0].mFace >> tJunk >> tTest[0].mUV >> tJunk >> tTest[0].mNormal >>
                    tTest[2].mFace >> tJunk >> tTest[2].mUV >> tJunk >> tTest[2].mNormal >>
                    tTest[1].mFace >> tJunk >> tTest[1].mUV >> tJunk >> tTest[1].mNormal;
                

                /*
                
                좌우를 반전시켰으므로 
                정점의 나열 순서가 바뀌어야만 한다. 
                
                : 인덱싱 순서를 0 1 2 ---> 0 2 1

                0   1

                2

                ------------

                1   0

                2
                
                
                */



                //중간 재가공

                //이미 구축된 인덱스 정보인지 여부
                bool tIsBe = false;

                //첫번째 데이터에 대해서 검토
                for (int ti = 0; ti<mRYUIndexs.size(); ++ti)
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

                //이미 구축된 인덱스 정보가 없다면(새로운 데이터라면), 추가한다
                if(!tIsBe)
                {
                    mRYUIndexs.push_back(tTest[0]);

                    mIndexVertices.push_back(tRealIndex);
                    mIndexUVs.push_back(tRealIndex);
                    mIndexNormals.push_back(tRealIndex);

                    tRealIndex++;       //정점의 인덱스 갯수 증가
                }



                tIsBe = false;

                //두번째 데이터에 대해서 검토
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

                //이미 구축된 인덱스 정보가 없다면(새로운 데이터라면), 추가한다
                if (!tIsBe)
                {
                    mRYUIndexs.push_back(tTest[1]);

                    mIndexVertices.push_back(tRealIndex);
                    mIndexUVs.push_back(tRealIndex);
                    mIndexNormals.push_back(tRealIndex);

                    tRealIndex++;       //정점의 인덱스 갯수 증가
                }



                tIsBe = false;

                //세번째 데이터에 대해서 검토
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

                //이미 구축된 인덱스 정보가 없다면(새로운 데이터라면), 추가한다
                if (!tIsBe)
                {
                    mRYUIndexs.push_back(tTest[2]);

                    mIndexVertices.push_back(tRealIndex);
                    mIndexUVs.push_back(tRealIndex);
                    mIndexNormals.push_back(tRealIndex);

                    tRealIndex++;       //정점의 인덱스 갯수 증가
                }
            }
        }//while

        for (int ti = 0; ti<mRYUIndexs.size(); ++ti)
        {
            //각 데이터 종류별 인덱스를 뽑아낸다
            int tIndexFace = mRYUIndexs[ti].mFace - 1;
            int tIndexUV = mRYUIndexs[ti].mUV - 1;
            int tIndexNormal = mRYUIndexs[ti].mNormal - 1;

            //이것이 바로 
            //실제 랜더링에 참조할 데이터들이다
            mVertices.push_back(mTT[tIndexFace]);
            mUVs.push_back(mAAAA[tIndexUV]);
            mNormals.push_back(mNN[tIndexNormal]);
        }
        /*
        정점의 인덱스 갯수만큼 순회하면서
        해당 정점의 인덱스 들을 구축하기 위해 필요한
        정점의 위치정보, 정점의 uv정보, 정점의 normal정보
        를 추출한다
        */



        return true;
    }

};

