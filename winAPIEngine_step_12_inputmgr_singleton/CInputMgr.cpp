#include "CInputMgr.h"

CInputMgr* CInputMgr::mpInstance = nullptr;

CInputMgr::CInputMgr()
{

}
CInputMgr::~CInputMgr()
{

}

CInputMgr* CInputMgr::GetInstance()
{
	if (nullptr == mpInstance)
	{
		mpInstance = new CInputMgr();
	}

	return mpInstance;
}

void CInputMgr::ReleaseInstance()
{
	if (nullptr != mpInstance)
	{
		delete mpInstance;
		mpInstance = nullptr;
	}
}