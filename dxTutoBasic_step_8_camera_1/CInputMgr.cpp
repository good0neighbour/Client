#include "CInputMgr.h"

#include <windows.h>


SINGLETON_DEFINITION(CInputMgr)

CInputMgr::CInputMgr()
{
	OutputDebugString(L"CInputMgr::CInputMgr\n");

	mMapKeyInfos.clear();
	mpTempKeyInfo = nullptr;
}
CInputMgr::~CInputMgr()
{
	OutputDebugString(L"CInputMgr::~CInputMgr\n");

	//�ڷᱸ���� �����͸� ����
	unordered_map < string, SKeyInfo*>::iterator tItor;
	for (tItor = mMapKeyInfos.begin(); tItor != mMapKeyInfos.end(); ++tItor)
	{
		if (tItor->second)
		{
			delete tItor->second;
			tItor->second = nullptr;
		}
	}
	mMapKeyInfos.clear();

	mpTempKeyInfo = nullptr;
}



void CInputMgr::Create()
{

}
void CInputMgr::Destroy()
{

}

void CInputMgr::Update()
{
	//��ϵ� Ű:�� ���� �����͸� ��ȸ
	unordered_map < string, SKeyInfo*>::iterator tItor;
	for (tItor = mMapKeyInfos.begin(); tItor != mMapKeyInfos.end(); ++tItor)
	{
		int tPushCount = 0;
		//����Ű( N���� Ű�����Է� )�� ��� 
		for (int ti = 0; ti < tItor->second->mKeyInputs.size(); ++ti)
		{
			//������ �ش� Ű�Է¿� ���� ���� ���´� 
			if (GetAsyncKeyState(tItor->second->mKeyInputs[ti]) & 0x8000)
			{
				++tPushCount;
			}
		}


		//if (1 == tPushCount)//������ �ش� Ű�Է¿� ���� ���� ���� ���´� 
		//����Ű(N���� Ű�����Է�)�� ���������� ���Դٸ� 
		if( tItor->second->mKeyInputs.size() == tPushCount )
		{
			//��ϵ� ���µ��� �Բ� �����Ͽ� �����غ���
			//down�� �ƴϰ� �׸��� press�� �ƴ϶��, ó�� ���� ���̴�
			if (!tItor->second->mIsDown && !tItor->second->mIsPress)
			{
				//ó�� ���� ���´�
				tItor->second->mIsDown = true;
			}//��ϵ� ���°� down�̰� press�� �ƴ϶��, ������ �ִ� ���̴�
			else if (tItor->second->mIsDown && !tItor->second->mIsPress)
			{
				//������ �ִ� ���̴�
				tItor->second->mIsDown = false;
				tItor->second->mIsPress = true;
			}
		}
		else//���� ���� ���°� �ƴϴ� 
		{
			//���� ��Ͽ� down�̰ų� �Ǵ� press���µ�, ���� ������ �ƴ϶��, 
			//�������� ���̴� 
			if (tItor->second->mIsDown || tItor->second->mIsPress)
			{
				//�������� ���̴�
				tItor->second->mIsDown = false;
				tItor->second->mIsPress = false;
				tItor->second->mIsUp = true;
			}//���� ��Ͽ� down�� �ƴϰ� press�� �ƴϰ�, ���� ������ �ƴ϶�� �ƹ� �Էµ� ���� ���̴�
			else if (tItor->second->mIsUp)
			{
				//���� �ƹ� �Էµ� ���� ���̴�
				tItor->second->mIsUp = false;
			}
		}

	}
}

SKeyInfo* CInputMgr::FindKey(const string& tStrKey) const
{
	//���ڿ� Ÿ���� Ű�� �˻��� Ű�� ��� �˻�, �ؽ����̺��̹Ƿ� �˻��ӵ��� O(1)
	unordered_map<string, SKeyInfo*>::const_iterator tItor = mMapKeyInfos.find(tStrKey);

	if (tItor == mMapKeyInfos.end() )
	{
		return nullptr;
	}

	return tItor->second;
}

bool CInputMgr::KeyDown(const string& tStrKey) const
{
	//������ �ش�Ű�����Է��� �˻��Ͽ� ã�´�
	SKeyInfo* tpInfo = FindKey(tStrKey);

	if (!tpInfo)
	{
		return false;
	}

	return tpInfo->mIsDown;	//�ش� Ű�����Է��� down���¸� ����
}
bool CInputMgr::KeyPress(const string& tStrKey) const
{
	//������ �ش�Ű�����Է��� �˻��Ͽ� ã�´�
	SKeyInfo* tpInfo = FindKey(tStrKey);

	if (!tpInfo)
	{
		return false;
	}

	return tpInfo->mIsPress;	//�ش� Ű�����Է��� press���¸� ����
}
bool CInputMgr::KeyUp(const string& tStrKey) const
{
	//������ �ش�Ű�����Է��� �˻��Ͽ� ã�´�
	SKeyInfo* tpInfo = FindKey(tStrKey);

	if (!tpInfo)
	{
		return false;
	}

	return tpInfo->mIsUp;	//�ش� Ű�����Է��� up���¸� ����
}