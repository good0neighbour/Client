#pragma once

#include "ryumacro.h"

class CExplosion;

class CEfxMgr
{
public:
	CExplosion* mpExplosion = nullptr;

	SINGLETON_DECLARE(CEfxMgr)
};

