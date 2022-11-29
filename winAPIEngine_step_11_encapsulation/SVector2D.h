#pragma once
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
		:mX(0.0f), mY(0.0f)
	{
		mX = t.mX;
		mY = t.mY;
	}

	//������ �����ε�operator overload

	//������Կ�����, ���� ������ ������� �ʰڴ�.
	void operator=(const SVector2D& t)
	{
		mX = t.mX;
		mY = t.mY;
	}

	//���ͳ����� ����
	SVector2D operator+(const SVector2D& t)
	{
		SVector2D tResult;	//������ü�� �����Ͽ���. �ֳ��ϸ� +��� ���׿����� ���� ���ο� ���� ��������� �����̴�.

		tResult.mX = this->mX + t.mX;
		tResult.mY = this->mY + t.mY;

		//������ü�� ���������ν� '����'�Ǿ� ���� ���� ���޵ȴ�.
		return tResult;
	}

	//���ͳ����� ����
	SVector2D operator-(const SVector2D& t)
	{
		SVector2D tResult;	//������ü�� �����Ͽ���. �ֳ��ϸ� -��� ���׿����� ���� ���ο� ���� ��������� �����̴�.

		tResult.mX = this->mX - t.mX;
		tResult.mY = this->mY - t.mY;

		//������ü�� ���������ν� '����'�Ǿ� ���� ���� ���޵ȴ�.
		return tResult;
	}

	SVector2D operator*(const float tScalar) 
	{
		SVector2D tResult;
		//������ü�� �����Ͽ���. �ֳ��ϸ� *��� ���׿����� ���� ���ο� ���� ��������� �����̴�.

		tResult.mX = this->mX * tScalar;
		tResult.mY = this->mY * tScalar;

		//������ü�� ���������ν� '����'�Ǿ� ���� ���� ���޵ȴ�.
		return tResult;
	}
};

