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

	//자료구조의 데이터를 삭제
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
	//등록된 키:값 쌍의 데이터를 순회
	unordered_map < string, SKeyInfo*>::iterator tItor;
	for (tItor = mMapKeyInfos.begin(); tItor != mMapKeyInfos.end(); ++tItor)
	{
		int tPushCount = 0;
		//조합키( N개의 키보드입력 )를 고려 
		for (int ti = 0; ti < tItor->second->mKeyInputs.size(); ++ti)
		{
			//임의의 해당 키입력에 대해 눌린 상태다 
			if (GetAsyncKeyState(tItor->second->mKeyInputs[ti]) & 0x8000)
			{
				++tPushCount;
			}
		}


		//if (1 == tPushCount)//임의의 해당 키입력에 대해 현재 눌린 상태다 
		//조합키(N개의 키보드입력)가 성공적으로 들어왔다면 
		if( tItor->second->mKeyInputs.size() == tPushCount )
		{
			//기록된 상태들을 함께 조합하여 점검해본다
			//down이 아니고 그리고 press도 아니라면, 처음 눌린 것이다
			if (!tItor->second->mIsDown && !tItor->second->mIsPress)
			{
				//처음 눌린 상태다
				tItor->second->mIsDown = true;
			}//기록된 상태가 down이고 press는 아니라면, 눌리고 있는 것이다
			else if (tItor->second->mIsDown && !tItor->second->mIsPress)
			{
				//눌리고 있는 중이다
				tItor->second->mIsDown = false;
				tItor->second->mIsPress = true;
			}
		}
		else//현재 눌린 상태가 아니다 
		{
			//이전 기록에 down이거나 또는 press였는데, 현재 눌림이 아니라면, 
			//떼어지는 것이다 
			if (tItor->second->mIsDown || tItor->second->mIsPress)
			{
				//떼어지는 것이다
				tItor->second->mIsDown = false;
				tItor->second->mIsPress = false;
				tItor->second->mIsUp = true;
			}//이전 기록에 down도 아니고 press도 아니고, 현재 눌림이 아니라면 아무 입력도 없는 것이다
			else if (tItor->second->mIsUp)
			{
				//이제 아무 입력도 없는 것이다
				tItor->second->mIsUp = false;
			}
		}

	}
}

SKeyInfo* CInputMgr::FindKey(const string& tStrKey) const
{
	//문자열 타입의 키를 검색용 키로 삼아 검색, 해쉬테이블이므로 검색속도는 O(1)
	unordered_map<string, SKeyInfo*>::const_iterator tItor = mMapKeyInfos.find(tStrKey);

	if (tItor == mMapKeyInfos.end() )
	{
		return nullptr;
	}

	return tItor->second;
}

bool CInputMgr::KeyDown(const string& tStrKey) const
{
	//임의의 해당키보드입력을 검색하여 찾는다
	SKeyInfo* tpInfo = FindKey(tStrKey);

	if (!tpInfo)
	{
		return false;
	}

	return tpInfo->mIsDown;	//해당 키보드입력의 down상태를 리턴
}
bool CInputMgr::KeyPress(const string& tStrKey) const
{
	//임의의 해당키보드입력을 검색하여 찾는다
	SKeyInfo* tpInfo = FindKey(tStrKey);

	if (!tpInfo)
	{
		return false;
	}

	return tpInfo->mIsPress;	//해당 키보드입력의 press상태를 리턴
}
bool CInputMgr::KeyUp(const string& tStrKey) const
{
	//임의의 해당키보드입력을 검색하여 찾는다
	SKeyInfo* tpInfo = FindKey(tStrKey);

	if (!tpInfo)
	{
		return false;
	}

	return tpInfo->mIsUp;	//해당 키보드입력의 up상태를 리턴
}