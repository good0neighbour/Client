#include "CResourcesMgr.h"


#include "CTexture.h"
#include "CAPIEngine.h"

#include "CPathMgr.h"

SINGLETON_DEFINITION(CResourcesMgr)

CResourcesMgr::CResourcesMgr()
{
	OutputDebugString(L"CResourcesMgr::CResourcesMgr()\n");

	mTexs.clear();
	mpEngine = nullptr;
}

CResourcesMgr::~CResourcesMgr()
{
	OutputDebugString(L"CResourcesMgr::~CResourcesMgr()\n");


	unordered_map<string, CTexture*>::iterator tItor;
	for (tItor = mTexs.begin(); tItor != mTexs.end(); ++tItor)
	{
		SAFE_RELEASE(tItor->second);
	}
	mTexs.clear();

	mpEngine = nullptr;
}

void CResourcesMgr::Create(CAPIEngine* tpEngine)
{
	mpEngine = tpEngine;
}

CTexture* CResourcesMgr::LoadTexture(const string& tStrKey, const wchar_t* tpFileName, const string& tStrPathKey)
{
	CTexture* tpTex = FindTexture(tStrKey);
	//이미 로드되어 등록되어 있는 자원이라면 그것을 리턴
	if (tpTex)
	{
		return tpTex;
	}

	//아직 로드하여 등록되어 있지 않은 자원이라면 
	//로드하여 등록

	//경로를 얻자
	const wchar_t* tszTemp = nullptr;
	tszTemp = CPathMgr::GetInstance()->FindPath(tStrPathKey);

	//해당 자원파일이름을 포함한 전체 경로 = 경로 + 파일이름
	//<-- 경로 는 CPathMgr객체에게서 질의하여 얻었다 
	wstring tString = tszTemp;		
	tString = tString + tpFileName;


	tpTex = new CTexture();
	tpTex->AddRef();
	bool tIs = tpTex->LoadTexture(mpEngine->GetHInstance(), mpEngine->GetHDC(), tString.c_str());


	//만약 파일의 데이터를 로드하는데 실패하면 의  처리가 필요하다.
	if (!tIs)
	{
		SAFE_RELEASE(tpTex);

		return nullptr;
	}

	//파일의 데이터를 로드하는데 성공한 경우

	//데이터를 등록해둔다.
	mTexs.insert(make_pair(tStrKey, tpTex));

	return tpTex;
}

CTexture* CResourcesMgr::FindTexture(const string& tStrKey)
{
	unordered_map<string, CTexture*>::iterator tItor;

	tItor = mTexs.find(tStrKey);

	if (tItor == mTexs.end() )
	{
		//검색 실패
		return nullptr;
	}

	//검색 성공
	return tItor->second;
}