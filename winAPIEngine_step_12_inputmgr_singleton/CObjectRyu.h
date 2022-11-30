#pragma once


#include "SVector2D.h"

#include "CRef.h"

class CObjectRyu: public CRef
{
public:
	CObjectRyu();
	virtual ~CObjectRyu();

	CObjectRyu(const CObjectRyu& t);
	void operator=(const CObjectRyu& t);

	inline void SetPosition(SVector2D tPosition)
	{
		mPosition = tPosition;
	}

	inline SVector2D GetPosition()
	{
		return mPosition;
	}

//public:
//private:
protected:
	//위치 기능
	SVector2D mPosition;
};

