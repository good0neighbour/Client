#pragma once
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
		:mX(0.0f), mY(0.0f)
	{
		mX = t.mX;
		mY = t.mY;
	}

	//연산자 오버로딩operator overload

	//복사대입연산자, 연쇄 대입은 고려하지 않겠다.
	void operator=(const SVector2D& t)
	{
		mX = t.mX;
		mY = t.mY;
	}

	//벡터끼리의 덧셈
	SVector2D operator+(const SVector2D& t)
	{
		SVector2D tResult;	//지역객체를 생성하였다. 왜냐하면 +라는 이항연산의 계산로 새로운 값이 만들어지기 때문이다.

		tResult.mX = this->mX + t.mX;
		tResult.mY = this->mY + t.mY;

		//지역객체를 리턴함으로써 '복사'되어 콜한 측에 전달된다.
		return tResult;
	}

	//벡터끼리의 뺄셈
	SVector2D operator-(const SVector2D& t)
	{
		SVector2D tResult;	//지역객체를 생성하였다. 왜냐하면 -라는 이항연산의 계산로 새로운 값이 만들어지기 때문이다.

		tResult.mX = this->mX - t.mX;
		tResult.mY = this->mY - t.mY;

		//지역객체를 리턴함으로써 '복사'되어 콜한 측에 전달된다.
		return tResult;
	}

	SVector2D operator*(const float tScalar) 
	{
		SVector2D tResult;
		//지역객체를 생성하였다. 왜냐하면 *라는 이항연산의 계산로 새로운 값이 만들어지기 때문이다.

		tResult.mX = this->mX * tScalar;
		tResult.mY = this->mY * tScalar;

		//지역객체를 리턴함으로써 '복사'되어 콜한 측에 전달된다.
		return tResult;
	}
};

