#pragma once

#include "ryumacro.h"

#include <windows.h>

#include <string>
#include <unordered_map>	//해쉬 자료구조를 STL컨테이너로 만든 것
using namespace std;

class CTexture;
class CAPIEngine;

class CResourcesMgr
{
private:
	//자원(resource)에는 여러 종류가 있을 수 있다. 
	//이를테면, 이미지, 사운드, 텍스트, 영상, ... 
	//하지만 여기서는 비트맵 이미지 데이터만 가정한다.
	//
	//해쉬테이블, O(1)검색속도를 가진다. '키/값' 쌍의 데이터를 다룬다
	//키: 문자열	string
	//값: CTexture의 객체의 주소값
	unordered_map<string, CTexture*> mTexs;

	CAPIEngine* mpEngine = nullptr;


public:
	void Create(CAPIEngine* tpEngine);

	CTexture* LoadTexture(const string& tStrKey, const wchar_t* tpFileName, const string& tStrPathKey);

	CTexture* FindTexture(const string& tStrKey);



	SINGLETON_DECLARE(CResourcesMgr)
};

