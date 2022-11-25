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
//CAPIEngine���� ���踦 �Ű������� ǥ���ߴ�.
//		<---CUnitŬ������ CAPIEngineŬ������ ���������.
void CUnit::Render(CAPIEngine* tpEngine)
{
	tpEngine->DrawCircle(mX, mY, mRadius);
}