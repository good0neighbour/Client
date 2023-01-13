#include "CPathMgr.h"

SINGLETON_DEFINITION(CPathMgr)

CPathMgr::CPathMgr()
{
	OutputDebugString(L"CPathMgr::CPathMgr()\n");

	mPaths.clear();
}

CPathMgr::~CPathMgr()
{
	OutputDebugString(L"CPathMgr::~CPathMgr()\n");

	//다루는 데이터가 C++에서 제공하는 문자열 타입이므로 별도의 동적할당 메모리 해제 작업은 필요하지 않다.
	mPaths.clear();
}

/*
* '실행파일이 있는 경로와 실행파일 이름까지 문자열' 얻어오는 window api들이 제공된다.
* 
* GetCurrentDirectory
* 
*	현재 '프로세스'가 실행된 위치를 얻어오는 함수
* 
*		이를테면, c:/do/it/some.exe 를 바탕화면에 단축 아이콘으로 실행시키면 'c:/바탕화면' 이 얻어와짐
* 
*		<-- 이것은 우리가 원하는 것이 아니다.
* 
* GetModuleFileName
* 
*	실행파일의 위치와 이름을 그대로 얻어온다.
* 
*		이를테면, c:/do/it/some.exe 를 바탕화면에 단축 아이콘으로 실행시키면 c:/do/it/some.exe 이 얻어와진다
* 
*		<-- 이것이 원하는 것이다.
*/

bool CPathMgr::Create()
{
	wchar_t tStrPath[MAX_PATH] = { 0 };
	//현재 실행파일을 기준으로 디렉토리를 포함한 전체 경로를 얻는다.
	GetModuleFileName(NULL, tStrPath, MAX_PATH);

	//로그로 출력해보자.
	WCHAR tszTemp[256] = { 0 };
	wsprintf(tszTemp, L"%s\n", tStrPath);
	OutputDebugString(tszTemp);

	//C:\Users\User\Documents\GitHub\ClientBasic_Weekday\winAPIMatchPuzzle_step_4_pathmgr\bin\winAPIMatchPuzzle.exe

	//문자열을 가공하여, 실행파일 이름은 제거하고 경로만 얻도록 한다.
	int tCount = lstrlen(tStrPath);

	for (int ti = tCount; ti >= 0; --ti)
	{
		if ('/' == tStrPath[ti] || '\\' == tStrPath[ti])
		{
			//여기에 실행파일 이름을 제거한 순수한 경로만 얻는 코드를 만들자.

			//tStrPath + (ti + 1) 메모리 주소부터
			// MAX_PATH - (ti + 1)개의 문자를 지운다.
			memset(tStrPath + (ti + 1), 0, MAX_PATH - (ti + 1));

			break;
		}
	}

	//로그로 출력해보자.
	WCHAR tszTemp_0[256] = { 0 };
	wsprintf(tszTemp_0, L"%s\n", tStrPath);
	OutputDebugString(tszTemp_0);
	
	mPaths.insert(make_pair(ROOT, tStrPath));

	return true;
}

bool CPathMgr::MakePath(const string& tStrKey, const wchar_t* tpPath, const string& tStrBaseKey)
{
	//일단 기본 경로를 얻는다
	const wchar_t* tpBasePath = FindPath(tStrBaseKey);

	if (tpBasePath)
	{
		return false;
	}

	//우리가 얻을 최종 경로
	wstring tStrPath;
	//일단 기본 경로를 가정
	tStrPath = tpBasePath;	//형변환 wchar_t* ---> wstring으로 형변환
	//원하는 폴더 이름, \를 추가함
	tStrPath = tStrPath + tpPath + L'\\';

	//로그로 출력해보자.
	WCHAR tszTemp_0[256] = { 0 };
	wsprintf(tszTemp_0, L"%s\n", tStrPath.c_str());
	OutputDebugString(tszTemp_0);

	//새롭게 구성한 경로 정보를 추가한다
	mPaths.insert(make_pair(tStrKey, tStrPath));

	return true;
}

const wchar_t* CPathMgr::FindPath(const string& tStrKey)
{
	unordered_map<string, wstring>::iterator tItor;
	tItor = mPaths.find(tStrKey);

	if (tItor == mPaths.end())
	{
		//검색 실패
		return nullptr;
	}

	//검색 성공
	return tItor->second.c_str();
	//wstring에는 c 스타일 문자열 함수를 제공한다.
}