#pragma once

#include <functional>
using namespace std;

class CUnit;

//타이머 클래스
//기능은 가장 최소한으로 만 넣도록 하겠다.

// void 함수이름()

class CRyuTimer
{
public:
	void SetTimer(float tTimeInterval, std::function<void(CUnit&)> tFunction, CUnit* tpUnit);

	void Update(float tDeltaTime);

private:
	//호출할 함수를 기억시켜둘 멤버변수
	std::function<void(CUnit&)> mFunction;

	float mTimeTick = 0.0f;			//deltaTime 누적용 변수
	float mTimeInterval = 0.0f;		//일정 시간 간격

	CUnit* mpUnit = nullptr;			//객체의 참조

};

