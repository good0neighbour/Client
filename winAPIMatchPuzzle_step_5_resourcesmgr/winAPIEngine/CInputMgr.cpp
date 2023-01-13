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

//입력의 상태(down, press, up) 결정(갱신)
void CInputMgr::Update()
{
	unordered_map<string, SKeyInfo*>::iterator tItor;
	//등록된 입력데이터 들에 대해 순회
	for (tItor = mMapKeys.begin(); tItor != mMapKeys.end(); ++tItor)
	{
		//임의의 입력에 대해 눌림이 있는지 검토
		int tPushCount = 0;
		for (int ti = 0;ti<tItor->second->mKeyInputs.size();++ti)//실제 입력 갯수만큼
		{
			if (GetAsyncKeyState(tItor->second->mKeyInputs[ti]) & 0x8000)
			{
				++tPushCount;		//눌림이 있다면 해당입력에 대해 카운트 하나 증가
			}
		}

		//해당 입력에 대해 눌림이 있다
		//if (tPushCount == 1)
		//키입력이 여러개임을 고려, 키조합의 키갯수만큼, 즉 키조합 '입력'에 대해 
		if (tPushCount == tItor->second->mKeyInputs.size())
		{
			//down도 아니고 press도 아니다 ----> 처음 눌린 것이다.
			if (!tItor->second->mIsDown && !tItor->second->mIsPress)
			{
				tItor->second->mIsDown = true;	//<--
				tItor->second->mIsPress = false;
				tItor->second->mIsUp = false;
			}
			//down이고 press는 아니다 ----> 눌리고 있는 중이다.
			else if (tItor->second->mIsDown && !tItor->second->mIsPress)
			{
				tItor->second->mIsDown = false;	
				tItor->second->mIsPress = true;	//<--
				tItor->second->mIsUp = false;
			}
		}
		//해당 입력에 대해 눌림이 없다
		else
		{
			//down 이거나 press이다 ----> 눌림이 있다가 떼어지는 것이다
			if (tItor->second->mIsDown || tItor->second->mIsPress)
			{
				tItor->second->mIsDown = false;
				tItor->second->mIsPress = false;	
				tItor->second->mIsUp = true;	//<--
			}
			//up 이다 ----> 아무 입력도 없는 상태이다.
			else if (tItor->second->mIsUp)
			{
				tItor->second->mIsDown = false;
				tItor->second->mIsPress = false;
				tItor->second->mIsUp = false;
			}
		}

	}
}

SKeyInfo* CInputMgr::FindKey(const string& tStrKey) const
{
	//해쉬는 검색에 특화된 자료구조다. O(1)
	//find함수는 const_iterator를 리턴한다
	unordered_map<string, SKeyInfo*>::const_iterator tItor = mMapKeys.find(tStrKey);

	if (tItor == mMapKeys.end())
	{
		//검색 실패
		return nullptr;
	}
	//검색성공
	return tItor->second;
}


//임의의 입력에 down여부를 알아온다
bool CInputMgr::KeyDown(const string& tStrKey) const
{
	SKeyInfo* tpInfo = FindKey(tStrKey);
	if (nullptr == tpInfo)
	{
		return false;
	}

	return tpInfo->mIsDown;
}
//임의의 입력에 press여부를 알아온다
bool CInputMgr::KeyPress(const string& tStrKey) const
{
	SKeyInfo* tpInfo = FindKey(tStrKey);
	if (nullptr == tpInfo)
	{
		return false;
	}

	return tpInfo->mIsPress;
}
//임의의 입력에 up여부를 알아온다
bool CInputMgr::KeyUp(const string& tStrKey) const
{
	SKeyInfo* tpInfo = FindKey(tStrKey);
	if (nullptr == tpInfo)
	{
		return false;
	}

	return tpInfo->mIsUp;
}