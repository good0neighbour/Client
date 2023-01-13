#include "CResourcesMgr.h"

#include "CTexture.h"
#include "CAPIEngine.h"

#include "CPathMgr.h"

SINGLETON_DEFINITION(CResourcesMgr)

CResourcesMgr::CResourcesMgr()
{
	OutputDebugString(L"CResourcesMgr::CResourcesMgr()\n");
}

CResourcesMgr::~CResourcesMgr()
{
	OutputDebugString(L"CResourcesMgr::~CResourcesMgr()\n");
}

void CResourcesMgr::Create(CAPIEngine* tpEngine)
{
	mpEngine = tpEngine;
}

CTexture* CResourcesMgr::LoadTexture(const string& tStrKey, const wchar_t* tpFileName, const string& tStrPathKey)
{
	CTexture* tpTex = FindTexture(tStrKey);
	//�̹� �ε�Ǿ� ��ϵǾ��ִ� �ڿ��̶�� �װ��� ����
	if (tpTex)
	{
		return tpTex;
	}

	//���� �ε��Ͽ� ��ϵǾ����� ���� �ڿ��̶��
	//�����Ͽ� ���

	//��θ� ����
	const wchar_t* tszTemp = nullptr;
	tszTemp = CPathMgr::GetInstance()->FindPath(tStrPathKey);

	//�ش� �ڿ������̸��� ������ ��ü ��� = ��� + ���� �̸�
	//<-- ��δ� CPathMgr��ü���Լ� �����Ͽ� �����
	wstring tString = tszTemp;
	tString = tString + tpFileName;

	tpTex = new CTexture();
	tpTex->LoadTexture(mpEngine->GetHInstance(), mpEngine->GetHDC(), tString.c_str());

	//���� ������ �����͸� �ε��ϴµ� �����ϸ� �� ó���� �ʿ��ϴ�.

	//�����͸� ����صд�.
	mTexs.insert(make_pair(tStrKey, tpTex));

	return tpTex;
}

CTexture* CResourcesMgr::FindTexture(const string& tStrKey)
{
	unordered_map<string, CTexture*>::iterator tItor;

	tItor = mTexs.find(tStrKey);

	if (tItor == mTexs.end())
	{
		//�˻� ����
		return nullptr;
	}

	//�˻� ����
	return tItor->second;
}