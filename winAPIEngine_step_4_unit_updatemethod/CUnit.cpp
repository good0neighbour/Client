#include "CUnit.h"

#include "CAPIEngine.h"

CUnit::CUnit()
	:mX(0.0f), mY(0.0f), mRadius(0.0f)
{

}
CUnit::~CUnit()
{

}

CUnit::CUnit(const CUnit& t)
{
	mX = t.mX;
	mY = t.mY;
	mRadius = t.mX;
}
void CUnit::operator=(const CUnit& t)
{
	mX = t.mX;
	mY = t.mY;
	mRadius = t.mX;
}
//CAPIEngine과의 관계를 매개변수로 표현했다.
//		<---CUnit클래스가 CAPIEngine클래스에 의존관계다.
void CUnit::Render(CAPIEngine* tpEngine)
{
	tpEngine->DrawCircle(mX, mY, mRadius);
}