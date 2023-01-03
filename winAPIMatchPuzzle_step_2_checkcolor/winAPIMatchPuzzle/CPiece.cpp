#include "CPiece.h"

CPiece::CPiece()
{

}

CPiece::~CPiece()
{

}

CPiece::CPiece(const CPiece& t)
	:CUnit(t)
{

}

void CPiece::operator=(const CPiece& t)
{
	CUnit::operator=(t);
}