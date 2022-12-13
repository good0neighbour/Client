#include "CActor.h"

CActor::CActor()
{

}
CActor::~CActor()
{

}

CActor::CActor(const CActor& t)
	:CUnit(t)
{

}
void CActor::operator=(const CActor& t)
{
	CUnit::operator=(t);
}