#pragma once

//#include <math.h>
#include <cmath>
#include <cfloat>
using namespace std;

struct SVector2D
{
	//�������� 2���� ������ ����
	float mX = 0.0f;
	float mY = 0.0f;

	//�⺻ ������
	SVector2D()
		:mX(0.0f), mY(0.0f)
	{
	}

	//�Ű����� �ִ� ������
	SVector2D(float tX, float tY)
		:mX(tX), mY(tY)
	{
	}

	//���� ������
	SVector2D(const SVector2D& t)		
	{
		mX = t.mX;
		mY = t.mY;
	}

	//������ �����ε�operator overload

	//������Կ�����, ���� ������ ������� �ʰڴ�.
	/*void operator=(const SVector2D& t)
	{
		mX = t.mX;
		mY = t.mY;
	}*/

	//���ͳ����� ����
	//SVector2D operator+(const SVector2D& t)
	//{
	//	SVector2D tResult;		
	//	//������ü�� �����Ͽ���. �ֳ��ϸ� +��� ���׿����� ����� ���ο� ���� ��������� �����̴�.

	//	tResult.mX = this->mX + t.mX;
	//	tResult.mY = this->mY + t.mY;

	//	//������ü�� ���������ν� '����'�Ǿ� ���� ���� ���޵ȴ�.
	//	return tResult;
	//}

	////���ͳ����� ����
	//SVector2D operator-(const SVector2D& t)
	//{
	//	SVector2D tResult;
	//	//������ü�� �����Ͽ���. �ֳ��ϸ� -��� ���׿����� ����� ���ο� ���� ��������� �����̴�.

	//	tResult.mX = this->mX - t.mX;
	//	tResult.mY = this->mY - t.mY;

	//	//������ü�� ���������ν� '����'�Ǿ� ���� ���� ���޵ȴ�.
	//	return tResult;
	//}

	//SVector2D operator*(const float tScalar)
	//{
	//	SVector2D tResult;
	//	//������ü�� �����Ͽ���. �ֳ��ϸ� *��� ���׿����� ����� ���ο� ���� ��������� �����̴�.

	//	tResult.mX = this->mX* tScalar;
	//	tResult.mY = this->mY * tScalar;

	//	//������ü�� ���������ν� '����'�Ǿ� ���� ���� ���޵ȴ�.
	//	return tResult;
	//}

	//���ͳ����� ����
	//const ���ϰ��� read only		//�Ű������� read only			
	const SVector2D operator+(const SVector2D& t)  const	//<-- ��������� read only
	{
		return SVector2D( mX + t.mX, mY+ t.mY);
	}

	//���ͳ����� ����
	const SVector2D operator-(const SVector2D& t)  const	//<-- ��������� read only
	{
		return SVector2D(mX - t.mX, mY - t.mY);
	}

	//������ ��Į��� 
	const SVector2D operator*(float tScalar)  const	
	{
		return SVector2D(mX *tScalar, mY *tScalar);
	}

	const SVector2D operator/(float tScalar)  const
	{
		return SVector2D(mX / tScalar, mY / tScalar);
	}

	//�ݴ������ ����
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

	//���� dot product <----����� ��Į��
	float Dot(const SVector2D& t) const
	{
		return mX * t.mX + mY * t.mY;
	}

	//���� const product <----����� ����
	//���⼭�� ��� ������ z������ ũ�⸸ ���ϰڴ�
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

		//<--���Ƿ�: ������ ���� ���� ��
		//��ǻ���� �Ǽ� ����� �ݵ�� ������ �߻���Ų��. �׷��Ƿ� tLength == 0.0f �̷� ǥ���� �����ϴ�.
		if (tLength < FLT_EPSILON)
		{
			//�����͸� ����
			return SVector2D::ZERO;	//<--���� �ʿ�
		}

		return *this / tLength;	//ũ����� 1�� ��Į�� ��
	}

	bool IsZero() const
	{
		return *this == SVector2D::ZERO;
	}

	//�� ������
	bool operator==(const SVector2D& t) const
	{
		return mX == t.mX && mY == t.mY;
	}

	bool operator!=(const SVector2D& t) const
	{
		return !(*this == t);
	}

	//'mX�� t.mX�� ���� mY�� t.mY�� ���� ��쿡
	//'mX�� t.mX���� ũ�� mY�� t.mY���� ū ��쿡'�� �ƴ� ���
	//		(��𸣰��� ��Ģ���� �ݴ��� ��츦 ǥ���� �ڵ�)
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
	
��ǻ���� �⺻����

	CPU
	RAM

	DISK

((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

�����: 2����Ʈ �̻� �����͸� ����Ʈ�� � ������ �޸𸮻� ��ġ�ϴ���?

��Ʋ �����:x86�迭�� CPU������ ��Ʋ��������� ó���Ѵ�

		2����Ʈ �̻��� �����͸� ��������Ʈ���� ��ġ�Ѵ�.

�� �����

		2����Ʈ �̻��� �����͸� ��������Ʈ���� ��ġ�Ѵ�.
		\

		I)������ ����
		����vector: ũ��� ������ �ƿ츣�� ����
			vs ��Į��scalar :ũ�⸸�� ���Ѵ�.

			��)		�ӷ�speed 100km/h			�ü� 100km				<--- ũ��
					�ӵ�velocity 			���ʹ������� �ü�100km							<--- ũ�⿡ ���Ҿ� ���⵵ ��Ÿ����.

		II)������ ǥ���

			������ NUMERIC VECTOR ��������� ���͸� ǥ���ϴ� ���
				�Ұ�ȣ �ȿ� ��ǥ�� �����Ͽ� �Ǽ��� �������� ���ش�.

			��)	(1,0)

			���Ϻ��� GEOMETRY VECTOR ���������� ���͸� ǥ���ϴ� ���
				ȭ��ǥ�� ǥ���Ѵ�.
				ȭ��ǥ�� �����ϴ� ���� �����̰�
				ȭ��ǥ�� ���̰� ũ���̴�.

			��)	---->

		III)������ ����

			���ͳ����� ����(����)

				������ �������г��� ���Ѵ�(����)

				(1,0) + (0,1) = (1+0, 0+1) = (1,1)
				(1,0) - (1,0) = (1-1, 0-0) = (0,0)

			������ ��Į���

				��Į�� ������ ������ �������п� ���Ѵ�

				(1,0)*2 = (1*2, 0*2) = (2,0)

			���ͳ����� ����
				����
				����

			ũ��
			����ȭ: ������ ũ�⸦ 1�� ����� ����

		IV)��Ÿ

			��������: ũ�Ⱑ 1�� ����
			��������: ��鿡 ������ ��������

	��ǥ���� �󿡼� ��ġ�� ���ʹ� ������ ���յǾ� �ִ�.
*/