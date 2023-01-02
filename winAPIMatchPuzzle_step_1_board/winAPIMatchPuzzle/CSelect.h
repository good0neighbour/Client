#pragma once

#include "CUnit.h"

class CSelect : public CUnit
{
public:
	CSelect();
	virtual~CSelect();

	CSelect(const CSelect& t);

	void operator=(const CSelect& t);

public:
	virtual CObjectRyu* clone() override
	{
		return new CSelect(*this);
	}
};

