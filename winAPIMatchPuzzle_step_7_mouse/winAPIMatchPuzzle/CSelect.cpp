#include "CSelect.h"

CSelect::CSelect()
{

}

CSelect::~CSelect()
{

}

CSelect::CSelect(const CSelect& t)
	:CUnit(t)
{

}

void CSelect::operator=(const CSelect& t)
{
	CUnit::operator=(t);
}