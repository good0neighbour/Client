#include "CExplosion.h"

CExplosion::CExplosion()
{

}
CExplosion::~CExplosion()
{

}

CExplosion::CExplosion(const CExplosion& t)
	:CUnit(t)
{

}
void CExplosion::operator=(const CExplosion& t)
{
	CUnit::operator=(t);
}