#pragma once

#include "framework.h"
#include <vector>

/*
    CBitmapRect는 기본적으로   CBitmapRect와 같다.

    이것은 '사각형'에 텍스처를 매핑하여
    2D를 모사 하는 것이다.

    기본적으로는  CBitmapRect와 같고
    거기에 Dynamic Buffer를 이용하여 
    vertex buffer의 데이터를 
    Map/Unmap을 이용하여 갱신하도록 하겠다


*/

using namespace std;

//클래스 전방선언
class CDxEngine;
class CTextureRyu;

struct SBitmapVertex
{
    XMFLOAT4 position;      //정점의 위치정보  
    //test 셰이더와 일단 맞추기 위해 땜빵
    //XMFLOAT3 normal;
    XMFLOAT2 texcoord;      //정점의  uv좌표 정보
};


class CBitmapRect
{
private:
    ID3D11Buffer* mpVertexBuffer = nullptr;    
    ID3D11Buffer* mpIndexBuffer = nullptr;

    int mCountVertex = 0;       //정점 데이터의 갯수
    int mCountIndex= 0;       //인덱스 데이터의 갯수

    CDxEngine* mpEngine = nullptr;

    CTextureRyu* mpTextureRyu = nullptr;

    //2d render를 위해 필요한 부가적인 데이터
private:
    //화면의 너비 높이, 이미지의 너비,높이
    int mScreenWidth = 0;
    int mScreenHeight = 0;
    int mBitmapWidth = 0;
    int mBitmapHeight = 0;

    //갱신을 되도록 덜하기 위한 구조를 만들기 위한 변수 
    int mPrePosX = 0;
    int mPrePosY = 0;


public:
    CBitmapRect()
    {
        mpVertexBuffer = nullptr;
        mpIndexBuffer = nullptr;
        mCountVertex = 0; 
        mCountIndex = 0;
        mpEngine = nullptr;
    };
    ~CBitmapRect() {};
    //복사는 막는다 
    CBitmapRect(const CBitmapRect& t) = delete;
    void operator=(const CBitmapRect& t) = delete;
    //외부에서 호출될 함수
public:
    void Create(ID3D11Device* tpDevice, LPCWSTR tFileName,int tScreenWidth, int tScreenHeight, int tBitmapWidth, int tBitmapHeight);
    void Destroy();
    void Render(int tPosX, int tPosY);

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

    //이 함수는 동적 정점 버퍼를 이용하여 
    //매 프레임마다( 또는 프레임 당 N번 )데이터를 직접 변경하는 함수이다.
    void UpdateBuffers(ID3D11DeviceContext* tpDeviceContext, int tPosX, int tPosY);
};

