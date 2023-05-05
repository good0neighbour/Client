#pragma once

#include "framework.h"
#include <vector>

/*
    CBitmapRect�� �⺻������   CBitmapRect�� ����.

    �̰��� '�簢��'�� �ؽ�ó�� �����Ͽ�
    2D�� ��� �ϴ� ���̴�.

    �⺻�����δ�  CBitmapRect�� ����
    �ű⿡ Dynamic Buffer�� �̿��Ͽ� 
    vertex buffer�� �����͸� 
    Map/Unmap�� �̿��Ͽ� �����ϵ��� �ϰڴ�


*/

using namespace std;

//Ŭ���� ���漱��
class CDxEngine;
class CTextureRyu;

struct SBitmapVertex
{
    XMFLOAT4 position;      //������ ��ġ����  
    //test ���̴��� �ϴ� ���߱� ���� ����
    //XMFLOAT3 normal;
    XMFLOAT2 texcoord;      //������  uv��ǥ ����
};


class CBitmapRect
{
private:
    ID3D11Buffer* mpVertexBuffer = nullptr;    
    ID3D11Buffer* mpIndexBuffer = nullptr;

    int mCountVertex = 0;       //���� �������� ����
    int mCountIndex= 0;       //�ε��� �������� ����

    CDxEngine* mpEngine = nullptr;

    CTextureRyu* mpTextureRyu = nullptr;

    //2d render�� ���� �ʿ��� �ΰ����� ������
private:
    //ȭ���� �ʺ� ����, �̹����� �ʺ�,����
    int mScreenWidth = 0;
    int mScreenHeight = 0;
    int mBitmapWidth = 0;
    int mBitmapHeight = 0;

    //������ �ǵ��� ���ϱ� ���� ������ ����� ���� ���� 
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
    //����� ���´� 
    CBitmapRect(const CBitmapRect& t) = delete;
    void operator=(const CBitmapRect& t) = delete;
    //�ܺο��� ȣ��� �Լ�
public:
    void Create(ID3D11Device* tpDevice, LPCWSTR tFileName,int tScreenWidth, int tScreenHeight, int tBitmapWidth, int tBitmapHeight);
    void Destroy();
    void Render(int tPosX, int tPosY);

    inline void SetEngine(CDxEngine* tpEngine)
    {
        mpEngine = tpEngine;
    }
    //�ε��� �������� ���� ���� 
    inline int GetCountIndex() const
    {
        return mCountIndex;
    }
    ID3D11ShaderResourceView* GetTexture() const;

    //���ο��� ȣ��� �Լ�
private:
    void CreateBuffers(ID3D11Device* tpDevice);
    void DestroyBuffers();
    void RenderBuffers(ID3D11DeviceContext* tpDeviceContext);

    //�� �Լ��� ���� ���� ���۸� �̿��Ͽ� 
    //�� �����Ӹ���( �Ǵ� ������ �� N�� )�����͸� ���� �����ϴ� �Լ��̴�.
    void UpdateBuffers(ID3D11DeviceContext* tpDeviceContext, int tPosX, int tPosY);
};

