#include "CBullet.h"


CBullet::CBullet()
{

}
CBullet::~CBullet()
{

}

CBullet::CBullet(const CBullet& t)
	:CUnit(t)
{

}
void CBullet::operator=(const CBullet& t)
{
	CUnit::operator=(t);
}