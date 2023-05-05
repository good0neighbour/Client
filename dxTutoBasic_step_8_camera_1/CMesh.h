#pragma once

#include "framework.h"
#include <vector>
#include <string>

#include <fstream>      //<--������ �ٷ�� ����
#include <strstream>    //<-- �Ľ�(parsing ������ ��ū�� ������� ������ҵ��� �и��س��� ��)�� ���� ���� ����� �����ϹǷ� �̰��� ���
class CDXEngine;
class CTextureRyu;


//������ '��ġ/UV/normal' �� �ѽ����� �ٷ�� ���� ����ü
struct TEST
{
    int mFace;
    int mUV;
    int mNormal;
};



class CMesh
{
public:
    CMesh()
    {
        mpVertexBuffer = nullptr;
        mpIndexBuffer = nullptr;

        mCountVertex = 0;
        mCountIndex = 0;

        mpEngine = nullptr;
    }
    //����� ���´�
    CMesh(const CMesh& t) = delete;
    void operator=(const CMesh& t) = delete;


    //�ܺο��� ȣ��� �Լ���
public:
    void Create(ID3D11Device* tpDevice, LPCWSTR tFileNameModel, LPCWSTR tFileName);
    void Destroy();
    void Render();

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
    void CreateBuffers(ID3D11Device* tpDevice, LPCWSTR tFileName);
    void DestroyBuffers();
    void RenderBuffers(ID3D11DeviceContext* tpDeviceContext);


private:
    ID3D11Buffer* mpVertexBuffer = nullptr;
    ID3D11Buffer* mpIndexBuffer = nullptr;

    int mCountVertex = 0;       //���� �������� ����
    int mCountIndex = 0;        //�ε��� �������� ����


    //���� ���� ����
    CDXEngine* mpEngine = nullptr;



    CTextureRyu* mpTextureRyu = nullptr;

    //new
private:
    //--------------------------
    vector<XMFLOAT3> mVertices;         //'����������' ������� ���� ��ġ �����͸� ��Ƶ� �ڷᱸ��
    vector<XMFLOAT2> mUVs;              //'����������' ������� UV �����͸� ��Ƶ� �ڷᱸ��
    vector<XMFLOAT3> mNormals;          //'����������' ������� ���� �����͸� ��Ƶ� �ڷᱸ��

    vector<int> mIndexVertices;             //'����������' ������� ������ �ε��� �����͸� ��Ƶ� �ڷᱸ��
    vector<int> mIndexUVs;             //'����������' ������� UV�� �ε��� �����͸� ��Ƶ� �ڷᱸ��
    vector<int> mIndexNormals;             //'����������' ������� ������ �ε��� �����͸� ��Ƶ� �ڷᱸ��
    //--------------------------

    //�簡���� �߰��ܰ迡 �ʿ��� �ڷᱸ�����̴�
    vector<TEST> mRYUIndexs;            //���Ͽ� ��ϵ� 'vertexIndex/UVIndex/NormalIndex'�� �ѽ����� �ٷ� �ڷᱸ��
    vector<XMFLOAT3> mTT;               //���Ͽ� ��ϵ� '���� ����'�� �����͵��� ��Ƶ� �����迭
    vector<XMFLOAT2> mAAAA;         //���Ͽ� ��ϵ� 'UV�ؽ�ó��ǥ ����'�� �����͵��� ��Ƶ� �����迭
    vector<XMFLOAT3> mNN;           //���Ͽ� ��ϵ� '���� ����'�� �����͵��� ��Ƶ� �����迭


    //obj ���Ϸε� �Լ�
    bool  LoadFromObjFile(string tFileName)
    {
        ifstream tFile(tFileName);

        if (!tFile.is_open())
        {
            //������ ������ ����
            return false;
        }

        //������ ����

        //�ε��� ī��Ʈ �� ����
        int tIndexReal = 0;

        //������ ������ �ݺ�
        while (!tFile.eof())
        {
            //'����'�� �� ������ �����͸� ����ص� ����'����'.
            char tLine[256] = { 0 };
            tFile.getline(tLine, 256);

            //strstream Ÿ������ ���������� �ϳ� �����Ͽ���.
            //�Ľ�(parsing ������ ��ū�� ������� ������ҵ��� �и��س��� ��)�� ���� ���� ����� �����ϹǷ� �̰��� ���
            strstream tStr;
            tStr << tLine;

            //���ʿ��� ���� �ϳ��� �ɷ����� ���� ��������
            char tJunk;

            if ('v' == tLine[0])
            {
                if ('n' == tLine[1])
                {
                    XMFLOAT3 tNormal;
                    tStr >> tJunk >> tJunk >> tNormal.x >> tNormal.y >> tNormal.z;

                    //�¿��̹Ƿ� x��
                    //�¿� ���� ���� �����Ѵ� ---> -1�� ���Ѵ�
                    tNormal.x = tNormal.x * (-1.0f);

                    mNN.push_back(tNormal);
                }
                //�ؽ�ó ��ǥ
                if ('t' == tLine[1])
                {
                    XMFLOAT2 tUV;
                    tStr >> tJunk >> tJunk >> tUV.x >> tUV.y;

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
                    //�¿� ���� ���� �����Ѵ� ---> -1�� ���Ѵ�
                    tPos.x = tPos.x * (-1.0f);

                    mTT.push_back(tPos);
                }
            }

            //��
            if ('f' == tLine[0])
            {
                /*int tIndexVertex[3];
                int tIndexUV[3];
                int tIndexNormal[3];*/
                TEST tTest[3];

                /*tStr >> tJunk >>
                    tTest[0].mFace >> tJunk >> tTest[0].mUV >> tJunk >> tTest[0].mNormal >>
                    tTest[1].mFace >> tJunk >> tTest[1].mUV >> tJunk >> tTest[1].mNormal >>
                    tTest[2].mFace >> tJunk >> tTest[2].mUV >> tJunk >> tTest[2].mNormal;*/

                //�¿� ������ ����Ͽ�
                //�ε��� ������ �ٲ��ش�
                // 0 1 2 -----> 0 2 1
                tStr >> tJunk >>
                    tTest[0].mFace >> tJunk >> tTest[0].mUV >> tJunk >> tTest[0].mNormal >>
                    tTest[2].mFace >> tJunk >> tTest[2].mUV >> tJunk >> tTest[2].mNormal >>
                    tTest[1].mFace >> tJunk >> tTest[1].mUV >> tJunk >> tTest[1].mNormal;


                //�߰� �簡��. �ش� �ܰ踦 ��ġ�� ���� ��� �ε��� ������ ����� �����ȴ�

                bool tIsBe = false;

                //���� �̷�� ù��°(0��°) ������ �������� ��츦 ó��
                //�����˻�
                //������ ����� �ε��� �����Ͷ�� ���⼭ üũ
                for (int ti = 0; ti < mRYUIndexs.size(); ++ti)
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

                //�ش� �ڷᱸ���� ���ԵǾ� ���� ���� ��� ( ���ο� ���̶�� ) �߰��ؾ�¡
                if (!tIsBe)
                {
                    mRYUIndexs.push_back(tTest[0]);

                    mIndexVertices.push_back(tIndexReal);
                    mIndexUVs.push_back(tIndexReal);
                    mIndexNormals.push_back(tIndexReal);

                    tIndexReal++;
                }

                //================================================================================

                tIsBe = false;

                //���� �̷�� ù��°(1��°) ������ �������� ��츦 ó��
                //�����˻�
                //������ ����� �ε��� �����Ͷ�� ���⼭ üũ
                for (int ti = 0; ti < mRYUIndexs.size(); ++ti)
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

                //�ش� �ڷᱸ���� ���ԵǾ� ���� ���� ��� ( ���ο� ���̶�� ) �߰��ؾ�¡
                if (!tIsBe)
                {
                    mRYUIndexs.push_back(tTest[1]);

                    mIndexVertices.push_back(tIndexReal);
                    mIndexUVs.push_back(tIndexReal);
                    mIndexNormals.push_back(tIndexReal);

                    tIndexReal++;
                }

                //================================================================================

                tIsBe = false;

                //���� �̷�� ù��°(2��°) ������ �������� ��츦 ó��
                //�����˻�
                //������ ����� �ε��� �����Ͷ�� ���⼭ üũ
                for (int ti = 0; ti < mRYUIndexs.size(); ++ti)
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

                //�ش� �ڷᱸ���� ���ԵǾ� ���� ���� ��� ( ���ο� ���̶�� ) �߰��ؾ�¡
                if (!tIsBe)
                {
                    mRYUIndexs.push_back(tTest[2]);

                    mIndexVertices.push_back(tIndexReal);
                    mIndexUVs.push_back(tIndexReal);
                    mIndexNormals.push_back(tIndexReal);

                    tIndexReal++;
                }




                /*
                //mIndexVertices.push_back(tIndexVertex[0] - 1);
                //mIndexVertices.push_back(tIndexVertex[1] - 1);
                //mIndexVertices.push_back(tIndexVertex[2] - 1);

                //mIndexUVs.push_back(tIndexUV[0] - 1);
                //mIndexUVs.push_back(tIndexUV[1] - 1);
                //mIndexUVs.push_back(tIndexUV[2] - 1);

                //mIndexNormals.push_back(tIndexNormal[0] - 1);
                //mIndexNormals.push_back(tIndexNormal[1] - 1);
                //mIndexNormals.push_back(tIndexNormal[2] - 1);
                */
            }//f
        }//while



        for (int ti = 0; ti < mRYUIndexs.size(); ++ti)
        {
            //�� ������ ������ �ε����� �̾Ƴ���
            int tIndexFace          = mRYUIndexs[ti].mFace - 1;
            int tIndexUV            = mRYUIndexs[ti].mUV - 1;
            int tIndexNormal        = mRYUIndexs[ti].mNormal - 1;

            //�̰��� �ٷ�
            //���� �������� ������ �����͵��̴�
            mVertices.push_back(mTT[tIndexFace]);
            mUVs.push_back(mAAAA[tIndexUV]);
            mNormals.push_back(mNN[tIndexNormal]);
        }




        return true;
    }





    //old
    /*
private:
    vector<XMFLOAT3> mNormals;      //���Ͽ� ��ϵ� '���� ����'�� �����͵��� ��Ƶ� �����迭
    vector<XMFLOAT2> mUVs;              //���Ͽ� ��ϵ� '�ؽ�ó��ǥ ����'�� �����͵��� ��Ƶ� �����迭
    vector<XMFLOAT3> mVertices;         //���Ͽ� ��ϵ� '���� ����'�� �����͵��� ��Ƶ� �����迭

    vector<int> mIndexVertices;         //���Ͽ� ��ϵ� '���� �ε���' ������ �����͵��� ��Ƶ� �����迭
    vector<int> mIndexUVs;              //���Ͽ� ��ϵ� �ؽ�ó '��ǥ �ε���' ������ �����͵��� ��Ƶ� �����迭
    vector<int> mIndexNormals;          //���Ͽ� ��ϵ� '���� �ε���' ������ �����͵��� ��Ƶ� �����迭

    //obj ���� �ε� �Լ�
    bool  LoadFromObjFile(string tFileName)
    {
        ifstream tFile(tFileName);

        if (!tFile.is_open())
        {
            //������ ������ ����
            return false;
        }

        //������ ����

        //������ ������ �ݺ�
        while (!tFile.eof())
        {
            //'����'�� �� ������ �����͸� ����ص� ����'����'.
            char tLine[256] = { 0 };
            tFile.getline(tLine, 256);

            //strstream Ÿ������ ���������� �ϳ� �����Ͽ���.
            //�Ľ�(parsing ������ ��ū�� ������� ������ҵ��� �и��س��� ��)�� ���� ���� ����� �����ϹǷ� �̰��� ���
            strstream tStr;
            tStr << tLine;

            //���ʿ��� ���� �ϳ��� �ɷ����� ���� ��������
            char tJunk;

            if ('v' == tLine[0])
            {
                if ('n' == tLine[1])
                {
                    XMFLOAT3 tNormal;
                    tStr >> tJunk >> tJunk >> tNormal.x >> tNormal.y >> tNormal.z;

                    mNormals.push_back(tNormal);
                }
                //�ؽ�ó ��ǥ
                if ('t' == tLine[1])
                {
                    XMFLOAT2 tUV;
                    tStr >> tJunk >> tJunk >> tUV.x >> tUV.y;

                    mUVs.push_back(tUV);
                }
            }

            //����
            if ('v' == tLine[0])
            {
                if ('n' != tLine[1] && 't' != tLine[1])
                {
                    XMFLOAT3 tPos;
                    tStr >> tJunk >> tPos.x >> tPos.y >> tPos.z;

                    //�¿�� ��������.
                    //tPos.x = (-1.0f) * tPos.x;

                    mVertices.push_back(tPos);
                }
            }

            //��
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

                mIndexUVs.push_back(tIndexUV[0] - 1);
                mIndexUVs.push_back(tIndexUV[1] - 1);
                mIndexUVs.push_back(tIndexUV[2] - 1);

                mIndexNormals.push_back(tIndexNormal[0] - 1);
                mIndexNormals.push_back(tIndexNormal[1] - 1);
                mIndexNormals.push_back(tIndexNormal[2] - 1);
            }
        }//while
        return true;
    }
    */
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