#pragma once

#include "framework.h"
#include <vector>
#include <string>

//#include <fstream>      //<--������ �ٷ�� ����
//#include <strstream>    //<-- �Ľ�(parsing ������ ��ū�� ������� ������ҵ��� �и��س��� ��)�� ���� ���� ����� �����ϹǷ� �̰��� ���

using namespace std;

class CDXEngine;
class CTextureRyu;


/*
        �ǽð����� �簢���� �����
        �ؽ�ó�� ������ ���̴�.


*/



//2d ��ü�� ���� ���� ����ü
struct SBitmapVertex
{
    XMFLOAT4 position;      //������ ��ġ ���� xyzw
    XMFLOAT2 texcoord;      //������ �ؽ�ó ��ǥ(uv) ����
};


//������ '��ġ/UV/normal' �� �ѽ����� �ٷ�� ���� ����ü
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
    //����� ���´�
    CBitmapRect(const CBitmapRect& t) = delete;
    void operator=(const CBitmapRect& t) = delete;


    //�ܺο��� ȣ��� �Լ���
public:
    void Create(ID3D11Device* tpDevice, LPCWSTR tFileName, int tScreenWidth, int tScreenHeight, int tBitmapWidth, int tBitmapHeight);
    void Destroy();
    void Render(int tPosX, int tPosY);

    void SetEngine(CDXEngine* tpEngine)
    {
        mpEngine = tpEngine;
    }

    //�ε��������� ���� ����
    int GetCountIndex() const
    {
        return mCountIndex;
    }

    ID3D11ShaderResourceView* GetTexture() const;


    //���ο��� ȣ��� �Լ���
private:
    void CreateBuffers(ID3D11Device* tpDevice);
    void DestroyBuffers();
    void RenderBuffers(ID3D11DeviceContext* tpDeviceContext);

    //��� ��ġ�� �ش� ��Ʈ�� 2d rect�� ����ҰŴ� tPosX, tPosY
    bool UpdateBuffers(ID3D11DeviceContext* tpDeviceContext, int tPosX, int tPosY);


private:
    ID3D11Buffer* mpVertexBuffer = nullptr;
    ID3D11Buffer* mpIndexBuffer = nullptr;

    int mCountVertex = 0;       //���� �������� ����
    int mCountIndex = 0;        //�ε��� �������� ����


    //���� ���� ����
    CDXEngine* mpEngine = nullptr;



    CTextureRyu* mpTextureRyu = nullptr;


private:
    int mScreenWidth = 0;
    int mScreenHeight = 0;
    int mBitmapWidth = 0;
    int mBitmapHeight = 0;

    //������ �ǵ��� �� �ϱ� ���� �� �뵵
    int mPrePosX = 0;
    int mPrePosY = 0;

};










/*
//�������� ����� resource�ڿ� <-- directx ���� �����ϰ� �ִ�.
//���� ������ �����̴�. �̰��� �׷��� ó�� ��ġ�� ������� ����(�޸�)�� �����ϴ� ���̴�.
ID3D11Buffer* mpVertexBuffer = nullptr;

//�ε��� ���� ( ��ī�� �ִ� )
//���� ���۰� ���� ����(������)�� ���� �����̶��
//�ε��� ������ ������ �׷��� ������ ���� �ﰢ���� �̷�� �� ���� ���� ������ ���� �����س��� ������ ���Ѵ�
//�̷��� ǥ���� ������ ���� ������ ������(���� ��� ���⼭�� ������ü)�� ������ ������ ���� �� ���� �����Ƿ�
//�����ϱ� �����̴�.
ID3D11Buffer* mpIndexBuffer = nullptr;

*/