#pragma once

#include <functional>
using namespace std;

class CUnit;

//Ÿ�̸� Ŭ����
//����� ���� �ּ������� �� �ֵ��� �ϰڴ�.

// void �Լ��̸�()

class CRyuTimer
{
public:
	void SetTimer(float tTimeInterval, std::function<void(CUnit&)> tFunction, CUnit* tpUnit);

	void Update(float tDeltaTime);

private:
	//ȣ���� �Լ��� �����ѵ� �������
	std::function<void(CUnit&)> mFunction;

	float mTimeTick = 0.0f;			//deltaTime ������ ����
	float mTimeInterval = 0.0f;		//���� �ð� ����

	CUnit* mpUnit = nullptr;			//��ü�� ����

};

