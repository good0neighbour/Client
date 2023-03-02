#pragma once

#include "framework.h"
#include <vector>

#include <fstream>      //<--������ �帣�� ������ �������� �ٷ�� ����
#include <strstream>    //

using namespace std;

//Ŭ���� ���漱��
class CDxEngine;
class CTextureRyu;

//������ '��ġ/UV/normal'�� �ѽ����� �ٷ�� ���� ����ü
struct TEST
{
    int mFace;      //������ ��ġ ���� �ε���
    int mUV;        //������ uv ���� �ε���
    int mNormal;    //������ normal ���� �ε���
};

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
    vector<XMFLOAT3> mNormals;      //'���������� �������' ���� ������ �����͵��� ��Ƶ� �����迭
    vector<XMFLOAT2> mUVs;              //'���������� �������' UV ���� �� �����͵��� ��Ƶ� �����迭
    vector<XMFLOAT3> mVertices;         //'���������� �������' ���� ��ġ ���� �� �����͵��� ��Ƶ� �����迭

    vector<int> mIndexVertices;         //���Ͽ� ��ϵ� ������ġ�� �ε��� ���� �� �����͵��� ��Ƶ� �����迭
    vector<int> mIndexUVs;              //���Ͽ� ��ϵ� UV ������  �ε��� ������ �����͵��� ��Ƶ� �����迭
    vector<int> mIndexNormals;          //���Ͽ� ��ϵ� ���� ���� �ε��� ������ �����͵��� ��Ƶ� �����迭

    //�簡���� �߰��ܰ迡 �ʿ��� �ڷᱸ�����̴�
    vector<TEST> mRYUIndexs;    //���Ͽ� ��ϵ� 'vertexIndex/uvIndex/normalIndex'�� �� ������ �ٷ� �ڷᱸ��
    vector<XMFLOAT3> mTT;       //���Ͽ� ��ϵ� '���� ����'�� �����͵��� ��Ƶ� �����迭
    vector<XMFLOAT2> mAAAA;     //���Ͽ� ��ϵ� 'UV�ؽ�����ǥ ����'�� �����͵��� ��Ƶ� �����迭
    vector<XMFLOAT3> mNN;       //���Ͽ� ��ϵ� '���� ����'�� �����͵��� ��Ƶ� �����迭

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

        //��������/uv����/normal������ ���յ� �ϳ��� �����ε��� ������ ����
        int tRealIndex = 0;

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

                    //�¿��̹Ƿ� x��
                    //�¿� ������ ���� x���� �����´�.( -1�� ���Ѵ� )
                    tNormal.x = tNormal.x * (-1.0f);

                    //���Ͽ��� ������ �����͸� �״�� ������
                    mNN.push_back(tNormal);
                }
                //�ؽ��� ��ǥ
                if ('t' == tLine[0])
                {
                    XMFLOAT2 tUV;
                    tStr >> tJunk >> tJunk >> tUV.x >> tUV.y;
                    //���Ͽ��� ������ �����͸� �״�� ������
                    mAAAA.push_back(tUV);
                }
            }
            //����
            if ('v' == tLine[0])
            {
                if ('n' != tLine[1] && 't' != tLine[1])
                {
                    XMFLOAT3 tPos;
                    tStr >> tJunk >> tPos.x >> tPos.y >> tPos.z;

                    //�¿��̹Ƿ� x��
                    //�¿� ������ ���� x���� �����´�.( -1�� ���Ѵ� )
                    tPos.x = tPos.x * (-1.0f);

                    //���Ͽ��� ������ �����͸� �״�� ������
                    mTT.push_back(tPos);
                }
            }
            //���� �ε���
            if ('f' == tLine[0])
            {
                /*int tIndexVertex[3];
                int tIndexUV[3];
                int tIndexNormal[3];*/

                /*tStr >> tJunk >>
                    tIndexVertex[0] >> tJunk >> tIndexUV[0] >> tJunk >> tIndexNormal[0] >>
                    tIndexVertex[1] >> tJunk >> tIndexUV[1] >> tJunk >> tIndexNormal[1] >>
                    tIndexVertex[2] >> tJunk >> tIndexUV[2] >> tJunk >> tIndexNormal[2];*/

                /*mIndexVertices.push_back(tIndexVertex[0] - 1);
                mIndexVertices.push_back(tIndexVertex[1] - 1);
                mIndexVertices.push_back(tIndexVertex[2] - 1);

                mIndexUVs.push_back(tIndexVertex[0] - 1);
                mIndexUVs.push_back(tIndexVertex[1] - 1);
                mIndexUVs.push_back(tIndexVertex[2] - 1);
                
                mIndexNormals.push_back(tIndexVertex[0] - 1);
                mIndexNormals.push_back(tIndexVertex[1] - 1);
                mIndexNormals.push_back(tIndexVertex[2] - 1);*/

                TEST tTest[3];

                /*tStr >> tJunk >>
                    tTest[0].mFace >> tJunk >> tTest[0].mUV >> tJunk >> tTest[0].mNormal >>
                    tTest[1].mFace >> tJunk >> tTest[1].mUV >> tJunk >> tTest[1].mNormal >>
                    tTest[2].mFace >> tJunk >> tTest[2].mUV >> tJunk >> tTest[2].mNormal;*/

                tStr >> tJunk >>
                    tTest[0].mFace >> tJunk >> tTest[0].mUV >> tJunk >> tTest[0].mNormal >>
                    tTest[2].mFace >> tJunk >> tTest[2].mUV >> tJunk >> tTest[2].mNormal >>
                    tTest[1].mFace >> tJunk >> tTest[1].mUV >> tJunk >> tTest[1].mNormal;

                /*
                �¿츦 �����������Ƿ�
                ������ ���� ������ �ٲ��߸� �Ѵ�.
                
                : �ε��� ������ 0 1 2 ---> 0 2 1

                0 1

                2

                -------------------

                1 0

                2
                */

                //�߰� �簡��

                //�̹� ����� �ε��� �������� ����
                bool tIsBe = false;

                //ù ��° �����Ϳ� ���ؼ� ����
                for (int ti = 0; ti < mRYUIndexs.size(); ti++)
                {
                    if (tTest[0].mFace == mRYUIndexs[ti].mFace &&
                        tTest[0].mUV == mRYUIndexs[ti].mUV &&
                        tTest[0].mNormal == mRYUIndexs[ti].mNormal)
                    {
                        mIndexVertices.push_back(ti);
                        mIndexUVs.push_back(ti);
                        mIndexNormals.push_back(ti);

                        tIsBe = true;
                        break;
                    }
                }

                //�̹� ����� �ε��� ������ ���ٸ�(���ο� �����Ͷ��), �߰��Ѵ�
                if (!tIsBe)
                {
                    mRYUIndexs.push_back(tTest[0]);

                    mIndexVertices.push_back(tRealIndex);
                    mIndexUVs.push_back(tRealIndex);
                    mIndexNormals.push_back(tRealIndex);

                    tRealIndex++;   //������ �ε��� ���� ����
                }

                tIsBe = false;

                //�� ��° �����Ϳ� ���ؼ� ����
                for (int ti = 0; ti < mRYUIndexs.size(); ti++)
                {
                    if (tTest[1].mFace == mRYUIndexs[ti].mFace &&
                        tTest[1].mUV == mRYUIndexs[ti].mUV &&
                        tTest[1].mNormal == mRYUIndexs[ti].mNormal)
                    {
                        mIndexVertices.push_back(ti);
                        mIndexUVs.push_back(ti);
                        mIndexNormals.push_back(ti);

                        tIsBe = true;
                        break;
                    }
                }

                //�̹� ����� �ε��� ������ ���ٸ�(���ο� �����Ͷ��), �߰��Ѵ�
                if (!tIsBe)
                {
                    mRYUIndexs.push_back(tTest[1]);

                    mIndexVertices.push_back(tRealIndex);
                    mIndexUVs.push_back(tRealIndex);
                    mIndexNormals.push_back(tRealIndex);

                    tRealIndex++;   //������ �ε��� ���� ����
                }

                tIsBe = false;

                //�� ��° �����Ϳ� ���ؼ� ����
                for (int ti = 0; ti < mRYUIndexs.size(); ti++)
                {
                    if (tTest[2].mFace == mRYUIndexs[ti].mFace &&
                        tTest[2].mUV == mRYUIndexs[ti].mUV &&
                        tTest[2].mNormal == mRYUIndexs[ti].mNormal)
                    {
                        mIndexVertices.push_back(ti);
                        mIndexUVs.push_back(ti);
                        mIndexNormals.push_back(ti);

                        tIsBe = true;
                        break;
                    }
                }

                //�̹� ����� �ε��� ������ ���ٸ�(���ο� �����Ͷ��), �߰��Ѵ�
                if (!tIsBe)
                {
                    mRYUIndexs.push_back(tTest[2]);

                    mIndexVertices.push_back(tRealIndex);
                    mIndexUVs.push_back(tRealIndex);
                    mIndexNormals.push_back(tRealIndex);

                    tRealIndex++;   //������ �ε��� ���� ����
                }
            }
        }//while

        for (int ti = 0; ti < mRYUIndexs.size(); ++ti)
        {
            //�� ������ ������ �ε����� �̾Ƴ���
            int tIndexFace = mRYUIndexs[ti].mFace - 1;
            int tIndexUV = mRYUIndexs[ti].mUV - 1;
            int tIndexNormal = mRYUIndexs[ti].mNormal - 1;

            //�̰��� �ٷ�
            //���� �������� ������ �����͵��̴�
            mVertices.push_back(mTT[tIndexFace]);
            mUVs.push_back(mAAAA[tIndexUV]);
            mNormals.push_back(mNN[tIndexNormal]);
        }

        return true;
    }
};

