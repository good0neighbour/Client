#include "CObjectRyu.h"


CObjectRyu::CObjectRyu()
	:mPosition()
{

}
CObjectRyu::~CObjectRyu()
{
}
CObjectRyu::CObjectRyu(const CObjectRyu& t)
{
	mPosition = t.mPosition;
}
void CObjectRyu::operator=(const CObjectRyu& t)
{
	mPosition = t.mPosition;
}