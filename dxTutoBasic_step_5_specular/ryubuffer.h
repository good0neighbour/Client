#pragma once


#include "framework.h"

//응용 프로그램 수준에서 정의하였다
//시스템 메모리에 둘 정의
//정점의 정의
struct SVertex
{
    XMFLOAT4 position;          //정점의 위치 정보 xyzw
    //XMFLOAT4 color;                 //정점의 색상 정보 rgba
    XMFLOAT3 normal;                //정점의 법선 벡터 xyz
    XMFLOAT2 texcoord;          //정점의 텍스처 좌표(uv) 정보
};

//상수버퍼는 16바이트 정렬 형태로 맞추어야만 한다.
//응용 프로그램 수준에서 정의하였다
//시스템 메모리에 둘 정의
struct CBTransform
{
    XMMATRIX mWorld;            //월드변환 행렬           //4byte*4*4 = 64byte
    XMMATRIX mView;              //뷰변환 행렬               //4byte*4*4 = 64byte
    XMMATRIX mProjection;       //투영변환 행렬           //4byte*4*4 = 64byte

    //난반사 관련 조명 정보 
    XMFLOAT4 mLightDir;         //빛벡터의 방향           //4byte*4 = 16byte
    XMFLOAT4 mLightColor;       //빛의 색상                 //4byte*4 = 16byte

    //주변광 관련 조명 정보
    XMFLOAT4 mAmbientColor;     //주변광 색상 

    //정반사광 관련 조명 정보 
    XMFLOAT4 mCameraPosition;       //카메라 위치 정보<-- 시선 벡터를 구하기 위해서 
    XMFLOAT4 mSpecularColor;     //정반사광 색상 
    float mSpecularPower;           //거듭제곱 수<-- 하일라이트 면적 결정 수치
};
