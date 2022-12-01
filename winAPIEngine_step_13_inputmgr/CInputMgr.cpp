#include "CInputMgr.h"

//CInputMgr* CInputMgr::mpInstance = nullptr;
//
//CInputMgr* CInputMgr::GetInstance()
//{
//	if (nullptr == mpInstance)
//	{
//		mpInstance = new CInputMgr();
//	}
//
//	return mpInstance;
//}
//
//void CInputMgr::ReleaseInstance()
//{
//	if (nullptr != mpInstance)
//	{
//		delete mpInstance;
//		mpInstance = nullptr;
//	}
//}

SINGLETON_DEFINITION(CInputMgr)

CInputMgr::CInputMgr()
{
	mMapKeys.clear();
}
CInputMgr::~CInputMgr()
{
	unordered_map<string, SKeyInfo*>::iterator tItor;
	for (tItor = mMapKeys.begin(); tItor != mMapKeys.end(); ++tItor)
	{
		/*if (nullptr != tItor->second)
		{
			delete tItor->second;
			tItor->second = nullptr;
		}*/
		SAFE_DELETE(tItor->second)
	}

	mMapKeys.clear();
}

//�Է��� ����(down, press, up) ����(����)
void CInputMgr::Update()
{
	unordered_map<string, SKeyInfo*>::iterator tItor;
	//��ϵ� �Էµ����� �鿡 ���� ��ȸ
	for (tItor = mMapKeys.begin(); tItor != mMapKeys.end(); ++tItor)
	{
		//������ �Է¿� ���� ������ �ִ��� ����
		int tPushCount = 0;
		if (GetAsyncKeyState(tItor->second->mKeyInput) & 0x8000)
		{
			++tPushCount;	//������ �ִٸ� �ش��Է¿� ���� ī��Ʈ �ϳ� ����
		}

		//�ش� �Է¿� ���� ������ �ִ�
		if (tPushCount == 1)
		{
			//down�� �ƴϰ� press�� �ƴϴ� ----> ó�� ���� ���̴�.
			if (!tItor->second->mIsDown && !tItor->second->mIsPress)
			{

			}
			//down�̰� press�� �ƴϴ� ----> ������ �ִ� ���̴�.
			else if (!tItor->second->mIsDown && !tItor->second->mIsPress)
			{

			}
		}
		//�ش� �Է¿� ���� ������ ����
		else
		{
			//down �̰ų� press�� ----> ������ �ִٰ� �������� ���̴�
			if (tItor->second->mIsDown || tItor->second->mIsPress)
			{

			}
			//up�̴� ----> �ƹ� �Էµ� ���� ���´�.
			else if (!tItor->second->mIsUp)
			{

			}
		}
	}
}

SKeyInfo* CInputMgr::FindKey(const string& tStrKey) const
{
	//�ؽ��� �˻��� Ưȭ�� �ڷᱸ����. O(1)
	//find�Լ��� const_iterator�� �����Ѵ�
	unordered_map<string, SKeyInfo*>::const_iterator tItor = mMapKeys.find(tStrKey);

	if (tItor == mMapKeys.end())
	{
		//�˻� ����
		return nullptr;
	}
	//�˻�����
	return tItor->second;
}

//������ �Է¿� down���θ� �˾ƿ´�
bool CInputMgr::KeyDown(const string& tStrKey) const
{
	SKeyInfo* tpInfo = FindKey(tStrKey);

	if (nullptr == tpInfo)
	{
		return false;
	}

	return tpInfo->mIsDown;
}

//������ �Է¿� press���θ� �˾ƿ´�
bool CInputMgr::KeyPress(const string& tStrKey) const
{
	SKeyInfo* tpInfo = FindKey(tStrKey);

	if (nullptr == tpInfo)
	{
		return false;
	}

	return tpInfo->mIsPress;
}

//������ �Է¿� up���θ� �˾ƿ´�
bool CInputMgr::KeyUp(const string& tStrKey) const
{
	SKeyInfo* tpInfo = FindKey(tStrKey);

	if (nullptr == tpInfo)
	{
		return false;
	}

	return tpInfo->mIsUp;
}