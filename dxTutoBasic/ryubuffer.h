#pragma once

#include "framework.h"

struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT3 Normal;
    XMFLOAT2 Tex;
};



//용도별로 여러 개의 상수버퍼를 만들어 적용할 수도 있다
//월드 뷰 프로젝션 행렬
struct ConstantBuffer
{
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;

    XMFLOAT4 vLightDir;
    XMFLOAT4 vLightColor;
};