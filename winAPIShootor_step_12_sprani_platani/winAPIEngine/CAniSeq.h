#pragma once

#include <vector>
#include <string>
using namespace std;

class CTexture;

//열거형 문법
//열거형 <-- 값을 미리 지정하여 나열해둔 타입type
enum ANI_PO //play option
{
    LOOP = 0,   //반복해서 플레이 옵션
    ONCE        //한 번만 플레이 옵션
};

class CAniSeq
{
public:
    CAniSeq();
    ~CAniSeq();

    CAniSeq(const CAniSeq& t);

    void operator=(const CAniSeq& t);

    //애니메이션 시퀀스 진행
    void Update(float tDeltaTime);

public:
//private:
    //애니메이션 시퀀스를 구별할 식별자
    string mId = "";

    //스프라이트 애니메이션 테스트
    //CTexture* mTexs[2] = { nullptr, nullptr };
    vector<CTexture*> mTexs; //가변 배열

    float mTimeInterval = 0.0f; //스프라이트 프레임 시간 간격

    int mTotalFrameCount = 0;   //스프라이트 프레임 총 개수

    int mCurFrameIndex = 0; //현재 스프라이트 프레임 번호
    float mAniTime = 0.0f;  //애니메이션 진행 시간<-- 델타타임을 누적할 것임

    //애니시퀀스 플레이 옵션 변수
    ANI_PO mPlayOption = ANI_PO::LOOP;
};

