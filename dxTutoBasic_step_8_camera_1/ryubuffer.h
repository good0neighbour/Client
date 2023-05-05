#pragma once

#include "framework.h"


//이것은 정점 을 나타내는 구조체의 정의이다
//float 데이터 3개로 구성된 벡터(이자 정점, 위치)이다.
// 이 정의는 '시스템 메모리'에 만들 데이터의 '형식을 정의'하는 것이다
struct SimpleVertex
{
    //XMFLOAT3 position;
    XMFLOAT4 position;      //정점의 위치 정보 xyzw
    //XMFLOAT4 color;             //정점의 색상 정보 rgba
    XMFLOAT3 normal;        //정점의 법선벡터 xyz
    XMFLOAT2 texcoord;      //정점의 텍스처 좌표(uv) 정보
};

//상수버퍼는 16바이트의 배수로 만들어야 버그 안난다 까다로운 것!!!!!!!!
//https://vsts2010.tistory.com/search/simd
//  이 정의는 시스템 메모리(응용프로그램 수준)에 만들 데이터의 형식을 정의하는 것이다.
//struct CBTransform
//{
//    //transform
//    XMMATRIX mWorld;
//    XMMATRIX mView;
//    XMMATRIX mProjection;
//
//
//    //light
//    XMFLOAT4 mLightDir;
//    XMFLOAT4 mLightColor;
//
//    //ambient light
//    XMFLOAT4 mAmbientColor;
//
//    //specular
//    XMFLOAT4 mCameraPosition;
//    XMFLOAT4 mSpecularColor;
//    float mSpecularPower;
//};

//의미에 맞게 상수버퍼 구조체를 수정하였다.
//여기서는 transform에 관한 것, light에 관한 것으로 분리하였다.
struct CBTransform
{
    //transform
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;
};

struct CBLight
{
    //light
    XMFLOAT4 mLightDir;
    XMFLOAT4 mLightColor;
    XMFLOAT4 mAmbientColor;

    //specular
    XMFLOAT4 mCameraPosition;
    XMFLOAT4 mSpecularColor;
    float mSpecularPower;
    XMFLOAT3 padding;
};


//월드변환:     로컬좌표계 기준의 정점의 위치를 월드좌표계 기준에 정점의 위치로 변환한다
//뷰변환:      월드좌표계 기준의 정점의 위치를 뷰좌표계 기준에 정점의 위치로 변환한다
//투영 변환:    뷰좌표게 기준의 정점의 위치를 정규뷰 기준에 정점의 위치로 변환하고 근평면에 투영하는 변환이다.