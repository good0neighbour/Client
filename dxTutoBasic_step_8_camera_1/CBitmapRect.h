#pragma once

#include "framework.h"
#include <vector>
#include <string>

//#include <fstream>      //<--파일을 다루기 위해
//#include <strstream>    //<-- 파싱(parsing 임의의 토큰을 기반으로 구성요소들을 분리해내는 것)을 위한 여러 기능을 제공하므로 이것을 사용

using namespace std;

class CDXEngine;
class CTextureRyu;


/*
        실시간으로 사각형을 만들고
        텍스처를 매핑할 것이다.


*/



//2d 객체를 위한 정점 구조체
struct SBitmapVertex
{
    XMFLOAT4 position;      //정점의 위치 정보 xyzw
    XMFLOAT2 texcoord;      //정점의 텍스처 좌표(uv) 정보
};


//정점의 '위치/UV/normal' 를 한쌍으로 다루기 위한 구조체
//struct TEST
//{
//    int mFace;
//    int mUV;
//    int mNormal;
//};


class CBitmapRect
{
public:
    CBitmapRect()
    {
        mpVertexBuffer = nullptr;
        mpIndexBuffer = nullptr;

        mCountVertex = 0;
        mCountIndex = 0;

        mpEngine = nullptr;
    }
    //복사는 막는다
    CBitmapRect(const CBitmapRect& t) = delete;
    void operator=(const CBitmapRect& t) = delete;


    //외부에서 호출될 함수들
public:
    void Create(ID3D11Device* tpDevice, LPCWSTR tFileName, int tScreenWidth, int tScreenHeight, int tBitmapWidth, int tBitmapHeight);
    void Destroy();
    void Render(int tPosX, int tPosY);

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
    void CreateBuffers(ID3D11Device* tpDevice);
    void DestroyBuffers();
    void RenderBuffers(ID3D11DeviceContext* tpDeviceContext);

    //어느 위치에 해당 비트맵 2d rect를 출력할거니 tPosX, tPosY
    bool UpdateBuffers(ID3D11DeviceContext* tpDeviceContext, int tPosX, int tPosY);


private:
    ID3D11Buffer* mpVertexBuffer = nullptr;
    ID3D11Buffer* mpIndexBuffer = nullptr;

    int mCountVertex = 0;       //정점 데이터의 갯수
    int mCountIndex = 0;        //인덱스 데이터의 갯수


    //엔진 참조 변수
    CDXEngine* mpEngine = nullptr;



    CTextureRyu* mpTextureRyu = nullptr;


private:
    int mScreenWidth = 0;
    int mScreenHeight = 0;
    int mBitmapWidth = 0;
    int mBitmapHeight = 0;

    //갱신을 되도록 덜 하기 위한 비교 용도
    int mPrePosX = 0;
    int mPrePosY = 0;

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