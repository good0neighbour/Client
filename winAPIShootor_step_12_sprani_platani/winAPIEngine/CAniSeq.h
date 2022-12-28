#pragma once

#include <vector>
#include <string>
using namespace std;

class CTexture;

//������ ����
//������ <-- ���� �̸� �����Ͽ� �����ص� Ÿ��type
enum ANI_PO //play option
{
    LOOP = 0,   //�ݺ��ؼ� �÷��� �ɼ�
    ONCE        //�� ���� �÷��� �ɼ�
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

    //��������Ʈ �ִϸ��̼� �׽�Ʈ
    //CTexture* mTexs[2] = { nullptr, nullptr };
    vector<CTexture*> mTexs; //���� �迭

    float mTimeInterval = 0.0f; //��������Ʈ ������ �ð� ����

    int mTotalFrameCount = 0;   //��������Ʈ ������ �� ����

    int mCurFrameIndex = 0; //���� ��������Ʈ ������ ��ȣ
    float mAniTime = 0.0f;  //�ִϸ��̼� ���� �ð�<-- ��ŸŸ���� ������ ����

    //�ִϽ����� �÷��� �ɼ� ����
    ANI_PO mPlayOption = ANI_PO::LOOP;
};

