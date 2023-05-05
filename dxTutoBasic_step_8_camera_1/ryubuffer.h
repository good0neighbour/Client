#pragma once

#include "framework.h"


//�̰��� ���� �� ��Ÿ���� ����ü�� �����̴�
//float ������ 3���� ������ ����(���� ����, ��ġ)�̴�.
// �� ���Ǵ� '�ý��� �޸�'�� ���� �������� '������ ����'�ϴ� ���̴�
struct SimpleVertex
{
    //XMFLOAT3 position;
    XMFLOAT4 position;      //������ ��ġ ���� xyzw
    //XMFLOAT4 color;             //������ ���� ���� rgba
    XMFLOAT3 normal;        //������ �������� xyz
    XMFLOAT2 texcoord;      //������ �ؽ�ó ��ǥ(uv) ����
};

//������۴� 16����Ʈ�� ����� ������ ���� �ȳ��� ��ٷο� ��!!!!!!!!
//https://vsts2010.tistory.com/search/simd
//  �� ���Ǵ� �ý��� �޸�(�������α׷� ����)�� ���� �������� ������ �����ϴ� ���̴�.
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

//�ǹ̿� �°� ������� ����ü�� �����Ͽ���.
//���⼭�� transform�� ���� ��, light�� ���� ������ �и��Ͽ���.
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


//���庯ȯ:     ������ǥ�� ������ ������ ��ġ�� ������ǥ�� ���ؿ� ������ ��ġ�� ��ȯ�Ѵ�
//�亯ȯ:      ������ǥ�� ������ ������ ��ġ�� ����ǥ�� ���ؿ� ������ ��ġ�� ��ȯ�Ѵ�
//���� ��ȯ:    ����ǥ�� ������ ������ ��ġ�� ���Ժ� ���ؿ� ������ ��ġ�� ��ȯ�ϰ� ����鿡 �����ϴ� ��ȯ�̴�.