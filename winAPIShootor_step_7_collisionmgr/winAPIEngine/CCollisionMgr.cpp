#include "CCollisionMgr.h"

#include <Windows.h>

SINGLETON_DEFINITION(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
	OutputDebugString(L"CCollisionMgr::CCollisionMgr()\n");
}

CCollisionMgr::~CCollisionMgr()
{
	OutputDebugString(L"CCollisionMgr::~CCollisionMgr()\n");
}