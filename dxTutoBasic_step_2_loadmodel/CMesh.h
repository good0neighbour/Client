#pragma once

#include "framework.h"
#include <vector>

#include <fstream>      //<--������ �帣�� ������ �������� �ٷ�� ����
#include <strstream>    //

using namespace std;

//Ŭ���� ���漱��
class CDxEngine;
class CTextureRyu;


class CMesh
{
private:
    ID3D11Buffer* mpVertexBuffer = nullptr;    
    ID3D11Buffer* mpIndexBuffer = nullptr;

    int mCountVertex = 0;       //���� �������� ����
    int mCountIndex= 0;       //�ε��� �������� ����

    CDxEngine* mpEngine = nullptr;

    CTextureRyu* mpTextureRyu = nullptr;

public:
    CMesh()
    {
        mpVertexBuffer = nullptr;
        mpIndexBuffer = nullptr;
        mCountVertex = 0; 
        mCountIndex = 0;
        mpEngine = nullptr;
    };
    ~CMesh() {};
    //����� ���´� 
    CMesh(const CMesh& t) = delete;
    void operator=(const CMesh& t) = delete;

    //�ܺο��� ȣ��� �Լ�
public:
    void Create(ID3D11Device* tpDevice, LPCWSTR tFileName);
    void Destroy();
    void Render();

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



private:
    vector<XMFLOAT3> mNormals;      //���Ͽ� ��ϵ� ���� ���� �� �����͵��� ��Ƶ� �����迭
    vector<XMFLOAT2> mUVs;              //���Ͽ� ��ϵ� UV ���� �� �����͵��� ��Ƶ� �����迭
    vector<XMFLOAT3> mVertices;         //���Ͽ� ��ϵ� ������ġ ���� �� �����͵��� ��Ƶ� �����迭

    vector<int> mIndexVertices;         //���Ͽ� ��ϵ� ������ġ�� �ε��� ���� �� �����͵��� ��Ƶ� �����迭
    vector<int> mIndexUVs;              //���Ͽ� ��ϵ� UV ������  �ε��� ������ �����͵��� ��Ƶ� �����迭
    vector<int> mIndexNormals;          //���Ͽ� ��ϵ� ���� ���� �ε��� ������ �����͵��� ��Ƶ� �����迭

private:
    bool LoadObjFile(string tFileName)
    {
        ifstream tFile(tFileName);

        if (!tFile.is_open())
        {
            //������ ������ ���� ���
            return false;
        }

        //������ ���� ���

        //������ ���ٱ��� �ݺ�
        while (!tFile.eof())
        {
            //�� �پ� ������ �����´�.
            char tLine[256] = { 0 };
            tFile.getline(tLine, 256);

            //strstream���� �����ϴ� �Ḻ̌���� �̿��ϱ� ����
            strstream tStr;
            tStr << tLine;

            //���ʿ��� ���ڿ� ó����
            char tJunk = 0;

            if ('v' == tLine[0])
            {
                //���� ����
                if ('n' == tLine[1])
                {
                    XMFLOAT3 tNormal;
                    tStr >> tJunk >> tJunk >> tNormal.x >> tNormal.y >> tNormal.z;
                    //���Ͽ��� ������ �����͸� �״�� ������
                    mNormals.push_back(tNormal);
                }
                //�ؽ��� ��ǥ
                if ('t' == tLine[0])
                {
                    XMFLOAT3 tUV;
                    tStr >> tJunk >> tJunk >> tUV.x >> tUV.y;
                    //���Ͽ��� ������ �����͸� �״�� ������
                    mNormals.push_back(tUV);
                }
            }
            //����
            if ('v' == tLine[0])
            {
                if ('n' != tLine[1] && 't' != tLine[1])
                {
                    XMFLOAT3 tPos;
                    tStr >> tJunk >> tPos.x >> tPos.y >> tPos.z;
                    //���Ͽ��� ������ �����͸� �״�� ������
                    mVertices.push_back(tPos);
                }
            }
            //���� �ε���
            if ('f' == tLine[0])
            {
                int tIndexVertex[3];
                int tIndexUV[3];
                int tIndexNormal[3];

                tStr >> tJunk >>
                    tIndexVertex[0] >> tJunk >> tIndexUV[0] >> tJunk >> tIndexNormal[0] >>
                    tIndexVertex[1] >> tJunk >> tIndexUV[1] >> tJunk >> tIndexNormal[1] >>
                    tIndexVertex[2] >> tJunk >> tIndexUV[2] >> tJunk >> tIndexNormal[2];

                mIndexVertices.push_back(tIndexVertex[0] - 1);
                mIndexVertices.push_back(tIndexVertex[1] - 1);
                mIndexVertices.push_back(tIndexVertex[2] - 1);

                mIndexUVs.push_back(tIndexVertex[0] - 1);
                mIndexUVs.push_back(tIndexVertex[1] - 1);
                mIndexUVs.push_back(tIndexVertex[2] - 1);
                
                mIndexNormals.push_back(tIndexVertex[0] - 1);
                mIndexNormals.push_back(tIndexVertex[1] - 1);
                mIndexNormals.push_back(tIndexVertex[2] - 1);
            }
        }

        return true;
    }
};

