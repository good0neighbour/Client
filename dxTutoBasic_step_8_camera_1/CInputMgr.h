#pragma once
//<-- 한번만 컴파일해주세요. 인클루드 방지자

/*
	OOP Object Oriented Programming 객체지향 프로그래밍
	: 클래스간 관계, 객체간 관계를 이용하여 프로그램 구조를 작성하는 법

	Design Pattern: OOP의 모범답안집 

	Singleton DP 목적: 객체의 최대생성갯수를 1개로 제한하는 패턴 
	( 부가적으로, 전역적인 데이터 관리 용도로도 쓸 수는 있다. )


	i)해당클래스타입의 포인터 변수를 static으로 만든다
	ii)생성자는 public이 아니다
	iii)GetInstance()함수의 정의에는 객체의 최대생성갯수를 1개로 제한하는 판단제어구조가 위치한다
*/

//step_1

#include <windows.h>

#include "ryumacro.h"
//cpp에서 제공하는 문자열 타입 string을 사용하기 위해 포함
#include <string>
//해쉬 자료구조가 STL의 컨테이너로 만들어진 것이다.
#include <unordered_map>
#include <vector>
//typeid예약어를 쓰기 위해 
#include <typeinfo>

using namespace std;


//'키보드입력 정보'를 표현하기 위한 구조체 정의

//값(실제 데이터)
struct SKeyInfo
{
	//키 값 쌍 의 데이터로 다루자
	string mStrName;		//키 ( 검색용 키 )		추상화된 이름
	//DWORD mKeyInput;		//	실제 키보드입력 데이터	
	//<-- '조합 키' 개념을 지원하기 위해 N개의 실제 키보드입력 데이터를 다루도록 수정하자
	//<-- 실행중에 원소의 갯수는 가변적이므로 가변배열을 사용하자 
	vector<DWORD> mKeyInputs;


	//키보드의 입력 상태들
	bool mIsDown	= false;
	bool mIsPress	= false;
	bool mIsUp		= false;

	SKeyInfo() :mIsDown(false), mIsPress(false), mIsUp(false) 
	{
		mKeyInputs.clear();
	};
};


class CInputMgr
{
	SINGLETON_DECLARE(CInputMgr)


private:
	//키보드입력 데이터들 자료구조
	unordered_map<string, SKeyInfo*> mMapKeyInfos;
	//키보드입력 데이터들 자료구조 구축시 사용할 변수
	SKeyInfo* mpTempKeyInfo = nullptr;

public:
	void Create();
	void Destroy();

	void Update();

	//임의의 키보드입력 검색
	SKeyInfo* FindKey(const string& tStrKey) const;

	//임의의 키보드입력의 상태 여부 구하기
	bool KeyDown(const string& tStrKey) const;
	bool KeyPress(const string& tStrKey) const;
	bool KeyUp(const string& tStrKey) const;

	//가변인자 템플릿 문법, N개의 인자를 받을 수 있다
	// 
	//base case
	template<typename T>
	bool AddKey(const T& tData)
	{
		const char* tpTypeName = typeid(T).name();

		if (strcmp(tpTypeName, "char") == 0 ||
			strcmp(tpTypeName, "int") == 0)
		{
			mpTempKeyInfo->mKeyInputs.push_back((DWORD)tData);
		}

		return true;
	}

	//recursive case 		
	template<typename T,  typename... Types>
	bool AddKey(const T& tData, const Types&... arg)
	{
		if (!mpTempKeyInfo)
		{
			mpTempKeyInfo = new SKeyInfo();
		}
		
		const char* tpTypeName = typeid(T).name();
		
		if (strcmp(tpTypeName, "char") == 0 ||
			strcmp(tpTypeName, "int") == 0)
		{
			//mpTempKeyInfo->mKeyInput = (DWORD)tData;
			mpTempKeyInfo->mKeyInputs.push_back((DWORD)tData);
		}
		else
		{			
			mpTempKeyInfo->mStrName = tData;//tInputName;

			mMapKeyInfos.insert(make_pair(mpTempKeyInfo->mStrName, mpTempKeyInfo));
		}

		//재귀호출
		AddKey(arg...);



		if (mpTempKeyInfo)
		{
			mpTempKeyInfo = nullptr;
		}


		return true;
	}
};

