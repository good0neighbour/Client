#pragma once

#include "framework.h"
#include <vector>

#include <fstream>      //<--파일을 흐르는 데이터 개념으로 다루기 위해
#include <strstream>    //

using namespace std;

//클래스 전방선언
class CDxEngine;
class CTextureRyu;


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
    vector<XMFLOAT3> mNormals;      //파일에 기록된 법선 정보 의 데이터들을 담아둘 가변배열
    vector<XMFLOAT2> mUVs;              //파일에 기록된 UV 정보 의 데이터들을 담아둘 가변배열
    vector<XMFLOAT3> mVertices;         //파일에 기록된 정점위치 정보 의 데이터들을 담아둘 가변배열

    vector<int> mIndexVertices;         //파일에 기록된 정점위치의 인덱스 정보 의 데이터들을 담아둘 가변배열
    vector<int> mIndexUVs;              //파일에 기록된 UV 정보의  인덱스 정보의 데이터들을 담아둘 가변배열
    vector<int> mIndexNormals;          //파일에 기록된 법선 정보 인덱스 정보의 데이터들을 담아둘 가변배열

private:
    bool LoadObjFile(string tFileName)
    {
        ifstream tFile(tFileName);

        if (!tFile.is_open())
        {
            //파일이 열리지 않은 경우
            return false;
        }

        //파일이 열린 경우

        //파일의 끝줄까지 반복
        while (!tFile.eof())
        {
            //한 줄씩 내용을 가져온다.
            char tLine[256] = { 0 };
            tFile.getline(tLine, 256);

            //strstream에서 제공하는 파싱기능을 이용하기 위해
            strstream tStr;
            tStr << tLine;

            //불필요한 문자열 처리용
            char tJunk = 0;

            if ('v' == tLine[0])
            {
                //법선 벡터
                if ('n' == tLine[1])
                {
                    XMFLOAT3 tNormal;
                    tStr >> tJunk >> tJunk >> tNormal.x >> tNormal.y >> tNormal.z;
                    //파일에서 가져온 데이터를 그대로 적재함
                    mNormals.push_back(tNormal);
                }
                //텍스쳐 좌표
                if ('t' == tLine[0])
                {
                    XMFLOAT3 tUV;
                    tStr >> tJunk >> tJunk >> tUV.x >> tUV.y;
                    //파일에서 가져온 데이터를 그대로 적재함
                    mNormals.push_back(tUV);
                }
            }
            //정점
            if ('v' == tLine[0])
            {
                if ('n' != tLine[1] && 't' != tLine[1])
                {
                    XMFLOAT3 tPos;
                    tStr >> tJunk >> tPos.x >> tPos.y >> tPos.z;
                    //파일에서 가져온 데이터를 그대로 적재함
                    mVertices.push_back(tPos);
                }
            }
            //면의 인덱스
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

                mIndexUVs.push_back(tIndexVertex[0] - 1);
                mIndexUVs.push_back(tIndexVertex[1] - 1);
                mIndexUVs.push_back(tIndexVertex[2] - 1);
                
                mIndexNormals.push_back(tIndexVertex[0] - 1);
                mIndexNormals.push_back(tIndexVertex[1] - 1);
                mIndexNormals.push_back(tIndexVertex[2] - 1);
            }
        }

        return true;
    }
};

