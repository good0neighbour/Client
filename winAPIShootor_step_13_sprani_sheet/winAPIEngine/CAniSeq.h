#pragma once

#include <vector>
#include <string>
using namespace std;

class CTexture;

//������ ����
//������ <-- ���� �̸� �����Ͽ� �����ص� Ÿ��type
enum ANI_PO         //play option
{
    LOOP = 0,       //�ݺ��ؼ� �÷��� �ɼ�
    ONCE                //�ѹ��� �÷��� �ɼ� 
};

enum ANI_SO
{
    FRAME_FILE = 0, //��������Ʈ �������� ���� ������ ����
    SHEET_FILE      //��������Ʈ �����ӵ��� �ϳ��� ��������Ʈ ��Ʈ ���Ͼȿ� ����
};

class CAniSeq
{
public:
    CAniSeq();
    ~CAniSeq();

    CAniSeq(const CAniSeq& t);

    void operator=(const CAniSeq& t);

    //�ִϸ��̼� ������ ����
    void Update(float tDeltaTime);

public:
//private:    
    //�ִϸ��̼� �������� ������ �ĺ���
    string mId = "";

    //CTexture* mTexs[2] = { nullptr, nullptr };
    vector<CTexture*> mTexs;        //���� �迭

    float mTimeInterval = 0.0f;     //��������Ʈ ������ �ð� ����

    int mTotalFrameCount = 0;   //��������Ʈ ������ �� ����

    int mCurFrameIndex = 0;         //���� ��������Ʈ ������ ��ȣ
    float mAniTime = 0.0f;      //�ִϸ��̼� ���� �ð�<-- ��ŸŸ���� ������ ����

    //�ִϽ����� �÷��� �ɼ� ����
    //  �⺻�� �ݺ���� ���� ����
    ANI_PO mPlayOption = ANI_PO::LOOP;
};

