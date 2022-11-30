#pragma once

/*
*	����ȭ ���α׷��� structured programming
*	:�Լ��� ����ü ���� ������ ����� ���α׷� ������ �ۼ��ϴ� �����
* 
*	��ü���� ���α׷��� Object Oriented Programming
*	:Ŭ���� �� ����, ��ü �� ���踦 ������ ���α׷� ������ �ۼ��ϴ� �����
* 
*	�Ϲ�ȭ ���α׷��� General Programming
*	:Ÿ���� �Ű�����ȭ�Ͽ� �ٷ�� ���
* 
* Design Pattern
*	:OOP������ ��������
* 
* Singleton Pattern
* :	��ü�� �ִ� ���������� 1���� �����ϴ� ����
* 
* �̱��� ������ ����� ���
* i) �ش� Ŭ���� Ÿ���� ������� mpInstance�� static���� �����Ѵ�.
* ii) �����ڴ� public�� �ƴϴ�.
* iii) GetInstance()�Լ��� ���ǿ��� ��ü�� �ִ� ���������� 1���� �����ϴ� �Ǵ�������� ��ġ��Ų��
*/
class CInputMgr
{
private:
	static CInputMgr* mpInstance;// = nullptr;

//public:
private:
	CInputMgr();
	~CInputMgr();

public:
	static CInputMgr* GetInstance();
	static void ReleaseInstance();
};

