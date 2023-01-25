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
	//�̹� �ε�Ǿ� ��ϵǾ� �ִ� �ڿ��̶�� �װ��� ����
	if (tpTex)
	{
		return tpTex;
	}

	//���� �ε��Ͽ� ��ϵǾ� ���� ���� �ڿ��̶�� 
	//�ε��Ͽ� ���

	//��θ� ����
	const wchar_t* tszTemp = nullptr;
	tszTemp = CPathMgr::GetInstance()->FindPath(tStrPathKey);

	//�ش� �ڿ������̸��� ������ ��ü ��� = ��� + �����̸�
	//<-- ��� �� CPathMgr��ü���Լ� �����Ͽ� ����� 
	wstring tString = tszTemp;		
	tString = tString + tpFileName;


	tpTex = new CTexture();
	tpTex->AddRef();
	bool tIs = tpTex->LoadTexture(mpEngine->GetHInstance(), mpEngine->GetHDC(), tString.c_str());


	//���� ������ �����͸� �ε��ϴµ� �����ϸ� ��  ó���� �ʿ��ϴ�.
	if (!tIs)
	{
		SAFE_RELEASE(tpTex);

		return nullptr;
	}

	//������ �����͸� �ε��ϴµ� ������ ���

	//�����͸� ����صд�.
	mTexs.insert(make_pair(tStrKey, tpTex));

	return tpTex;
}

CTexture* CResourcesMgr::FindTexture(const string& tStrKey)
{
	unordered_map<string, CTexture*>::iterator tItor;

	tItor = mTexs.find(tStrKey);

	if (tItor == mTexs.end() )
	{
		//�˻� ����
		return nullptr;
	}

	//�˻� ����
	return tItor->second;
}