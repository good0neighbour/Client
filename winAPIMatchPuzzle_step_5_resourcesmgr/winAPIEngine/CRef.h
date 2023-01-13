#pragma once

//reference count 기능을 가지는 클래스
/*
	reference count

	동적할당한 메모리를 
	참조하는 횟수를 기억하는 변수를 두고

	참조횟수가 0보다 크면
	참조횟수만 감소하고
	참조횟수가 0이면
	실제로 동적할당한 메모리를 삭제하는 기법
*/

class CRef
{
public:
	CRef();
	virtual ~CRef();

//public:
private:
	int mRef = 0;		//참조횟수 카운트 변수

public:
	inline void AddRef()
	{
		//참조 카운트를 하나 증가한다
		++mRef;
	}

	inline int Release()
	{
		//참조 카운트를 하나 감소한다
		--mRef;

		//최종적으로 참조카운트가 0이면 
		//더이상 참조하지 않는 동적할당메모리이므로
		//삭제한다
		if (0 == mRef)
		{
			delete this;
			return 0;
		}

		return mRef;
	}
};

