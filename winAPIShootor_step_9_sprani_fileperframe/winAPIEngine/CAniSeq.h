#pragma once

#include <vector>
#include <string>
using namespace std;

class CTexture;

class CAniSeq
{
public:
    CAniSeq();
    ~CAniSeq();

    CAniSeq(const CAniSeq& t);

    void operator=(const CAniSeq& t);

    //�ִϸ��̼� ������ ����
    void Update(float tDeltaTime);

private:
    //�ִϸ��̼� �������� ������ �ĺ���
    string mId = "";

    //��������Ʈ �ִϸ��̼� �׽�Ʈ
    //CTexture* mTexs[2] = { nullptr, nullptr };
    vector<CTexture*> mTexs; //���� �迭

    float mTimeInterval = 0.0f; //��������Ʈ ������ �ð� ����

    int mTotalFrameCount = 0;   //��������Ʈ ������ �� ����

    int mCurFrameIndex = 0; //���� ��������Ʈ ������ ��ȣ
    float mAniTime = 0.0f;  //�ִϸ��̼� ���� �ð�<-- ��ŸŸ���� ������ ����
};

