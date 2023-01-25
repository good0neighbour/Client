#pragma once



/*
		구조화 프로그래밍 structured programming
		: 함수와 구조체 등의 구조를 만들어 프로그램 구조를 작성하는 방법론

		객체지향 프로그래밍 Object Oriented Programming
		: 클래스 간 관계, 객체 간 관계 를 가지고 프로그램 구조를 작성하는 방법론

		일반화 프로그래밍 General Programming
		: 타입을 매개변수화하여 다루는 기법 
	
	Design Pattern
		: OOP에서의 모범답안집
	
	Singleton Pattern 의 목적
	:	객체의 최대 생성갯수를 1개로 제한하는 패턴


	싱글톤 패턴을 만드는 방법
	i) 해당 클래스 타입의 멤버변수 mpInstance를 static으로 선언한다 
	ii) 생성자는 public이 아니다.
	iii) GetInstance()함수의 정의에는 객체의 최대 생성갯수를 1개로 제한하는 판단제어구조를 위치시킨다

*/
#include <windows.h>
#include "ryumacro.h"
#include <string>
#include <unordered_map>	//해쉬 자료구조를 STL컨테이너로 만든 것
#include <vector>	//가변배열

#include <typeinfo>//typeid 연산자 사용하기 위해
using namespace std;

struct SKeyInfo
{
	string mStrName = "";			//추상화된 입력 이름
	//DWORD mKeyInput = 0;		//실제 입력
	vector<DWORD> mKeyInputs; //실제 입력들

	bool mIsDown = false;		//처음 눌렸는지 여부
	bool mIsPress = false;		//눌리고 있는 중인지 여부
	bool mIsUp = false;			//떼졌는지 여부
};



//매크로함수를 이용하여 정형화되어 반복되는 코드를 간결하게 작성함
class CInputMgr
{
	SINGLETON_DECLARE(CInputMgr)

private:
	//해쉬테이블, O(1)검색속도를 가진다. '키/값' 쌍의 데이터를 다룬다
	unordered_map<string, SKeyInfo*> mMapKeys;	//입력정보들을 담아둘 자료구조

	//템플릿 가변인자 base case함수에서 의 처리를 위해 클래스 영역에서 유효하도록 멤버변수화
	SKeyInfo* mpKeyInfo = nullptr;

private:
	SKeyInfo* FindKey(const string& tStrKey) const;			//임의의 입력정보 검색, 검색키는 문자열 타입

public:
	void Update();			//입력의 상태(down, press, up) 결정(갱신)	

	//임의의 입력에 대한 상태여부
	bool KeyDown(const string& tStrKey) const;
	bool KeyPress(const string& tStrKey) const;
	bool KeyUp(const string& tStrKey) const;

	//입력정보 등록 함수

	//char, int 두 타입에 대해 모두 작동시키기 위해 '템플릿 문법' 이용
	//템플릿이므로 '헤더'에 만듦
	//template<typename T>
	//bool AddKey(const string& tInputName, const T& tData)
	//{
	//	SKeyInfo* tpInfo = nullptr;
	//	tpInfo = new SKeyInfo();


	//	//typeid연산자: 임의의 타입에 대한 정보를 얻는다
	//	//타입의 이름을 얻자
	//	const char* tpType = typeid(T).name();

	//	//strcmp C의 문자열 비교 함수,  같으면 0리턴
	//	if ( 0 == strcmp("char", tpType) || 0 == strcmp("int", tpType) )
	//	{
	//		tpInfo->mKeyInput = (DWORD)tData;				//실제 입력 을 저장해둔다
	//	}
	//	else
	//	{
	//		return false;
	//	}

	//	tpInfo->mStrName = tInputName;				//추상화된 입력 이름 설정

	//	//이제 구축된 입력 데이터를 자료구조에 담는다
	//	mMapKeys.insert( make_pair( tpInfo->mStrName, tpInfo ) );


	//	return true;
	//}




	//템플릿 가변인자 함수의 base case 함수
	template<typename T>
	bool AddKey(const T& tData)
	{		
		const char* tpType = typeid(T).name();

		if (0 == strcmp("char", tpType) || 0 == strcmp("int", tpType))
		{
			mpKeyInfo->mKeyInputs.push_back((DWORD)tData);				//실제 입력 을 저장해둔다
		}

		return true;
	}

	//템플릿 가변인자
	//recursive case
	template<typename T, typename... Types>
	bool AddKey(const T& tData, const Types&... arg)
	{	
		//재귀호출되므로 한번만 만들자.
		if (nullptr == mpKeyInfo)
		{
			mpKeyInfo = new SKeyInfo();
		}

		const char* tpType = typeid(T).name();
		if (0 == strcmp("char", tpType) || 0 == strcmp("int", tpType))
		{			
			mpKeyInfo->mKeyInputs.push_back((DWORD)tData);
		}
		else//문자열이면
		{
			mpKeyInfo->mStrName = tData;//tInputName;				//추상화된 입력 이름 설정

			mMapKeys.insert(make_pair(mpKeyInfo->mStrName, mpKeyInfo));
		}		

		//여기서 재귀호출이 일어난다.
		AddKey(arg...);

		if (nullptr != mpKeyInfo)
		{
			mpKeyInfo = nullptr;
		}

		return true;
	}
	
};

