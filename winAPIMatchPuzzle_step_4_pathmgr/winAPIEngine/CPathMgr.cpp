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

	//�ٷ�� �����Ͱ� C++���� �����ϴ� ���ڿ� Ÿ���̹Ƿ� ������ �����Ҵ� �޸� ���� �۾��� �ʿ����� �ʴ�.
	mPaths.clear();
}

/*
* '���������� �ִ� ��ο� �������� �̸����� ���ڿ�' ������ window api���� �����ȴ�.
* 
* GetCurrentDirectory
* 
*	���� '���μ���'�� ����� ��ġ�� ������ �Լ�
* 
*		�̸��׸�, c:/do/it/some.exe �� ����ȭ�鿡 ���� ���������� �����Ű�� 'c:/����ȭ��' �� ������
* 
*		<-- �̰��� �츮�� ���ϴ� ���� �ƴϴ�.
* 
* GetModuleFileName
* 
*	���������� ��ġ�� �̸��� �״�� ���´�.
* 
*		�̸��׸�, c:/do/it/some.exe �� ����ȭ�鿡 ���� ���������� �����Ű�� c:/do/it/some.exe �� ��������
* 
*		<-- �̰��� ���ϴ� ���̴�.
*/

bool CPathMgr::Create()
{
	wchar_t tStrPath[MAX_PATH] = { 0 };
	//���� ���������� �������� ���丮�� ������ ��ü ��θ� ��´�.
	GetModuleFileName(NULL, tStrPath, MAX_PATH);

	//�α׷� ����غ���.
	WCHAR tszTemp[256] = { 0 };
	wsprintf(tszTemp, L"%s\n", tStrPath);
	OutputDebugString(tszTemp);

	//C:\Users\User\Documents\GitHub\ClientBasic_Weekday\winAPIMatchPuzzle_step_4_pathmgr\bin\winAPIMatchPuzzle.exe

	//���ڿ��� �����Ͽ�, �������� �̸��� �����ϰ� ��θ� �򵵷� �Ѵ�.
	int tCount = lstrlen(tStrPath);

	for (int ti = tCount; ti >= 0; --ti)
	{
		if ('/' == tStrPath[ti] || '\\' == tStrPath[ti])
		{
			//���⿡ �������� �̸��� ������ ������ ��θ� ��� �ڵ带 ������.

			break;
		}
	}

	return true;
}

bool CPathMgr::MakePath(const string& tStrKey, const wchar_t* tpPath, const string& tStrBaseKey)
{
	return true;
}

const wchar_t* CPathMgr::FindPath(const string& tStrKey)
{
	unordered_map<string, wstring>::iterator tItor;
	tItor = mPaths.find(tStrKey);

	if (tItor == mPaths.end())
	{
		//�˻� ����
		return nullptr;
	}

	//�˻� ����
	return tItor->second.c_str();
	//wstring���� c ��Ÿ�� ���ڿ� �Լ��� �����Ѵ�.
}