#include "CEnemy.h"

CEnemy::CEnemy()
{
	
}
CEnemy::~CEnemy()
{

}

CEnemy::CEnemy(const CEnemy& t)
	:CUnit(t)
{
	
}
void CEnemy::operator=(const CEnemy& t)
{
	CUnit::operator=(t);
}