#pragma once

#include <windows.h>

#include "ryumacro.h"

#include <string>

#include <unordered_map>	//해쉬 자료구조를 STL컨테이너로 만든 것
using namespace std;
/*
* 경로 관리자 Path Manager: 외부파일 자원들이 위피할 '경로'를 관리할 객체
* 
* i) 관이자 객체 <-- Singleton Pattern
* ii) unordered_map <-- 경로 데이터들을 담아둘 자료구조
*/

class CPathMgr
{
private:
	//해쉬테이블, O(1)검색속도를 가진다. '키/값' 쌍의 데이터를 다룬다
	//키: 문자열 string
	//값: 유니코드 문자열 타입 wstring <--윈도우 운영체제에서 다루는 유니코드 문자열을 다뤄야 하기 때문에 이 타입으로 결정했다.
	unordered_map<string, wstring> mPaths;

public:
	//실행파일이 있는 경로(root 경로)를 구축한다
	bool Create();

	//임의의 경로를 만들어 자료구조에 추가한다
	bool MakePath(const string& tStrKey, const wchar_t* tpPath, const string& tStrBaseKey = "root_path");

private:
	//리턴값은 C 스타일 유니코드 문자열 (window api는 c를 지원하므로 리턴타입을 이렇게 결정했다 )
	const wchar_t* FindPath(const string& tStrKey);

	SINGLETON_DECLARE(CPathMgr)
};

