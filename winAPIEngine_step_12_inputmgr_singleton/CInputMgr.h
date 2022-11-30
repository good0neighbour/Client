#pragma once

/*
*	구조화 프로그래밍 structured programming
*	:함수와 구조체 등의 구조를 만들어 프로그램 구조를 작성하는 방법론
* 
*	객체지향 프로그래밍 Object Oriented Programming
*	:클래스 간 관계, 객체 간 관계를 가지고 프로그램 구조를 작성하는 방법론
* 
*	일반화 프로그래밍 General Programming
*	:타입을 매개변수화하여 다루는 기법
* 
* Design Pattern
*	:OOP에서의 모범답안집
* 
* Singleton Pattern
* :	객체의 최대 생성갯수를 1개로 제한하는 패턴
* 
* 싱글톤 패턴을 만드는 방법
* i) 해당 클래스 타입의 멤버변수 mpInstance를 static으로 선언한다.
* ii) 생성자는 public이 아니다.
* iii) GetInstance()함수의 정의에는 객체의 최대 생성갯수를 1개로 제한하는 판단제어구조를 위치시킨다
*/
class CInputMgr
{
private:
	static CInputMgr* mpInstance;// = nullptr;

//public:
private:
	CInputMgr();
	~CInputMgr();

public:
	static CInputMgr* GetInstance();
	static void ReleaseInstance();
};

