#pragma once

#include "CUnit.h"

class CPiece: public CUnit
{
public:
	CPiece();
	virtual~CPiece();

	CPiece(const CPiece& t);

	void operator=(const CPiece& t);

public:
	virtual CObjectRyu* clone() override
	{
		return new CPiece(*this);
	}
};

