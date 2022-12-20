#pragma once

//#include <math.h>
#include <cmath>
#include <cfloat>
using namespace std;

struct SVector2D
{
	//연속적인 2차원 공간을 가정
	float mX = 0.0f;
	float mY = 0.0f;

	//기본 생성자
	SVector2D()
		:mX(0.0f), mY(0.0f)
	{
	}

	//매개변수 있는 생성자
	SVector2D(float tX, float tY)
		:mX(tX), mY(tY)
	{
	}

	//복사 생성자
	SVector2D(const SVector2D& t)		
	{
		mX = t.mX;
		mY = t.mY;
	}

	//연산자 오버로딩operator overload

	//복사대입연산자, 연쇄 대입은 고려하지 않겠다.
	/*void operator=(const SVector2D& t)
	{
		mX = t.mX;
		mY = t.mY;
	}*/

	//벡터끼리의 덧셈
	//SVector2D operator+(const SVector2D& t)
	//{
	//	SVector2D tResult;		
	//	//지역객체를 생성하였다. 왜냐하면 +라는 이항연산의 결과로 새로운 값이 만들어지기 때문이다.

	//	tResult.mX = this->mX + t.mX;
	//	tResult.mY = this->mY + t.mY;

	//	//지역객체를 리턴함으로써 '복사'되어 콜한 측에 전달된다.
	//	return tResult;
	//}

	////벡터끼리의 뺄셈
	//SVector2D operator-(const SVector2D& t)
	//{
	//	SVector2D tResult;
	//	//지역객체를 생성하였다. 왜냐하면 -라는 이항연산의 결과로 새로운 값이 만들어지기 때문이다.

	//	tResult.mX = this->mX - t.mX;
	//	tResult.mY = this->mY - t.mY;

	//	//지역객체를 리턴함으로써 '복사'되어 콜한 측에 전달된다.
	//	return tResult;
	//}

	//SVector2D operator*(const float tScalar)
	//{
	//	SVector2D tResult;
	//	//지역객체를 생성하였다. 왜냐하면 *라는 이항연산의 결과로 새로운 값이 만들어지기 때문이다.

	//	tResult.mX = this->mX* tScalar;
	//	tResult.mY = this->mY * tScalar;

	//	//지역객체를 리턴함으로써 '복사'되어 콜한 측에 전달된다.
	//	return tResult;
	//}

	//벡터끼리의 덧셈
	//const 리턴값이 read only		//매개변수는 read only			
	const SVector2D operator+(const SVector2D& t)  const	//<-- 멤버변수들 read only
	{
		return SVector2D( mX + t.mX, mY+ t.mY);
	}

	//벡터끼리의 뺄셈
	const SVector2D operator-(const SVector2D& t)  const	//<-- 멤버변수들 read only
	{
		return SVector2D(mX - t.mX, mY - t.mY);
	}

	//벡터의 스칼라곱 
	const SVector2D operator*(float tScalar)  const	
	{
		return SVector2D(mX *tScalar, mY *tScalar);
	}

	const SVector2D operator/(float tScalar)  const
	{
		return SVector2D(mX / tScalar, mY / tScalar);
	}

	//반대방향의 벡터
	const SVector2D operator-() const
	{
		return SVector2D(-mX, -mY);
	}

	SVector2D& operator+=(const SVector2D& t)
	{
		return *this = *this + t;
	}

	SVector2D& operator-=(const SVector2D& t)
	{
		return *this = *this - t;
	}

	SVector2D& operator*=(float tScalar)
	{
		return *this = *this * tScalar;
	}

	SVector2D& operator/=(float tScalar)
	{
		return *this = *this / tScalar;
	}

	//내적 dot product <----결과는 스칼라
	float Dot(const SVector2D& t) const
	{
		return mX * t.mX + mY * t.mY;
	}

	//외적 const product <----결과는 벡터
	//여기서는 결과 벡터의 z성분의 크기만 구하겠다
	float Determinent(const SVector2D& t) const
	{
		return mX * t.mY - mY * t.mX;
	}

	float Length() const
	{
		return sqrt(Dot(*this));
	}

	float Distance(const SVector2D& t) const
	{
		return (*this - t).Length();
	}

	SVector2D Normalize() const
	{
		float tLength = Length();

		//<--엡실론: 무한히 극히 작은 수
		//컴퓨터의 실수 계산은 반드시 오차를 발생시킨다. 그러므로 tLength == 0.0f 이런 표현은 위험하다.
		if (tLength < FLT_EPSILON)
		{
			//영벡터를 리턴
			return SVector2D::ZERO;	//<--수정 필요
		}

		return *this / tLength;	//크기분의 1을 스칼라 곱
	}

	bool IsZero() const
	{
		return *this == SVector2D::ZERO;
	}

	//비교 연산자
	bool operator==(const SVector2D& t) const
	{
		return mX == t.mX && mY == t.mY;
	}

	bool operator!=(const SVector2D& t) const
	{
		return !(*this == t);
	}

	//'mX와 t.mX가 같고 mY와 t.mY가 같은 경우에
	//'mX가 t.mX보다 크고 mY가 t.mY보다 큰 경우에'가 아닌 경우
	//		(드모르간의 법칙으로 반대의 경우를 표현한 코드)
	bool operator<(const SVector2D& t) const
	{
		if (mX != t.mX)
		{
			return mX < t.mX;
		}
		//or
		if (mY != t.mY)
		{
			return mY < t.mY;
		}

		return false;
	}

	static const SVector2D ZERO;
};

/*
	
컴퓨터의 기본구조

	CPU
	RAM

	DISK

((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

엔디언: 2바이트 이상에 데이터를 바이트를 어떤 순서로 메모리상에 배치하느냐?

리틀 엔디언:x86계열의 CPU에서는 리틀엔디언으로 처리한다

		2바이트 이상의 데이터를 하위바이트부터 배치한다.

빅 엔디언

		2바이트 이상의 데이터를 상위바이트부터 배치한다.
		\

		I)벡터의 개념
		벡터vector: 크기와 방향을 아우르는 개념
			vs 스칼라scalar :크기만을 말한다.

			예)		속력speed 100km/h			시속 100km				<--- 크기
					속도velocity 			동쪽방향으로 시속100km							<--- 크기에 더불어 방향도 나타낸다.

		II)벡터의 표기법

			수벡터 NUMERIC VECTOR 대수적으로 벡터를 표기하는 방법
				소괄호 안에 쉼표로 구분하여 실수의 순서쌍을 써준다.

			예)	(1,0)

			기하벡터 GEOMETRY VECTOR 기하적으로 벡터를 표기하는 방법
				화살표로 표기한다.
				화살표가 지향하는 것이 방향이고
				화살표의 길이가 크기이다.

			예)	---->

		III)벡터의 연산

			벡터끼리의 덧셈(뺄셈)

				각각의 구성성분끼리 더한다(뺀다)

				(1,0) + (0,1) = (1+0, 0+1) = (1,1)
				(1,0) - (1,0) = (1-1, 0-0) = (0,0)

			벡터의 스칼라곱

				스칼라를 벡터의 각각의 구성성분에 곱한다

				(1,0)*2 = (1*2, 0*2) = (2,0)

			벡터끼리의 곱셈
				내적
				외적

			크기
			정규화: 벡터의 크기를 1로 만드는 연산

		IV)기타

			단위벡터: 크기가 1인 벡터
			법선벡터: 평면에 수직인 단위벡터

	좌표공간 상에서 위치와 벡터는 연산이 통합되어 있다.
*/