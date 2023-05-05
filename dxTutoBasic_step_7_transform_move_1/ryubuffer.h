#pragma once


#include "framework.h"

//���� ���α׷� ���ؿ��� �����Ͽ���
//�ý��� �޸𸮿� �� ����
//������ ����
struct SVertex
{
    XMFLOAT4 position;          //������ ��ġ ���� xyzw
    //XMFLOAT4 color;                 //������ ���� ���� rgba
    XMFLOAT3 normal;                //������ ���� ���� xyz
    XMFLOAT2 texcoord;          //������ �ؽ�ó ��ǥ(uv) ����
};

//������۴� 16����Ʈ ���� ���·� ���߾�߸� �Ѵ�.
//���� ���α׷� ���ؿ��� �����Ͽ���
//�ý��� �޸𸮿� �� ����
//struct CBTransform
//{
//    XMMATRIX mWorld;            //���庯ȯ ���           //4byte*4*4 = 64byte
//    XMMATRIX mView;              //�亯ȯ ���               //4byte*4*4 = 64byte
//    XMMATRIX mProjection;       //������ȯ ���           //4byte*4*4 = 64byte
//
//    //���ݻ� ���� ���� ���� 
//    XMFLOAT4 mLightDir;         //�������� ����           //4byte*4 = 16byte
//    XMFLOAT4 mLightColor;       //���� ����                 //4byte*4 = 16byte
//
//    //�ֺ��� ���� ���� ����
//    XMFLOAT4 mAmbientColor;     //�ֺ��� ���� 
//
//    //���ݻ籤 ���� ���� ���� 
//    XMFLOAT4 mCameraPosition;       //ī�޶� ��ġ ����<-- �ü� ���͸� ���ϱ� ���ؼ� 
//    XMFLOAT4 mSpecularColor;     //���ݻ籤 ���� 
//    float mSpecularPower;           //�ŵ����� ��<-- ���϶���Ʈ ���� ���� ��ġ
//};

//�뵵�� �°� �ΰ����� �з��Ͽ��� 
//  transform����
//  light����

struct CBTransform
{
    XMMATRIX mWorld;           
    XMMATRIX mView;              
    XMMATRIX mProjection;      
};

struct CBLight
{
    //���ݻ� ���� ���� ���� 
    XMFLOAT4 mLightDir;       
    XMFLOAT4 mLightColor;   

    //�ֺ��� ���� ���� ����
    XMFLOAT4 mAmbientColor;   

    //���ݻ籤 ���� ���� ���� 
    XMFLOAT4 mCameraPosition;      
    XMFLOAT4 mSpecularColor;    
    float mSpecularPower;        
    XMFLOAT3 padding;              
};
