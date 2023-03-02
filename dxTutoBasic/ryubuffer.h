#pragma once

#include "framework.h"

struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT3 Normal;
    XMFLOAT2 Tex;
};



//�뵵���� ���� ���� ������۸� ����� ������ ���� �ִ�
//���� �� �������� ���
struct ConstantBuffer
{
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;

    XMFLOAT4 vLightDir;
    XMFLOAT4 vLightColor;
};