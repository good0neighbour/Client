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

//class CInputMgr
//{
//private:
//	static CInputMgr* mpInstance;// = nullptr;
//
////public:
//private:
//	CInputMgr();
//	~CInputMgr();
//
//public:
//	static CInputMgr* GetInstance();
//	static void ReleaseInstance();
//};
#include <windows.h>
#include "ryumacro.h"
#include <string>
#include <unordered_map> //�ؽ� �ڷᱸ���� �����̳ʷ� ���� ��

#include <typeinfo>//typeid������ ����ϱ� ����
using namespace std;

struct SKeyInfo
{
	string mStrName = "";	//�߻�ȭ�� Ű�Է� �̸�
	DWORD mKeyInput = 0;	//���� �Է�

	bool mIsDown = false;	//ó�� ���ȴ��� ����
	bool mIsPress = false;	//������ �ִ� ������ ����
	bool mIsUp = false;		//�������� ����
};

//��ũ���Լ��� �̿��Ͽ� �ݺ��Ǵ� �ڵ带 �����ϰ� �ۼ���
class CInputMgr
{
	SINGLETON_DECLARE(CInputMgr)

private:
	//�ؽ����̺�, O(1)�˻��ӵ��� ������. 'Ű/��' ���� �����͸� �ٷ��
	unordered_map<string, SKeyInfo*> mMapKeys; //�Է��������� ��Ƶ� �ڷᱸ��

public:
	void Update();	//�Է��� ����(down, press, up) ����(����)

	SKeyInfo* FindKey(const string& tStrKey) const;	//������ �Է����� �˻�, �˻�Ű�� ���ڿ� Ÿ��

	//������ �Է¿� ���� ���¿���
	bool KeyDown(const string& tStrKey) const;
	bool KeyPress(const string& tStrKey) const;
	bool KeyUp(const string& tStrKey) const;

	//�Է����� ��� �Լ�

	//char, int �� Ÿ�Կ� ���� ��� �۵���Ű�� ���� '���ø� ������' �̿�
	//���ø��̹Ƿ� '���'�� ����
	template<typename T>
	bool AddKey(const string& tInputName, const T& tData)
	{
		SKeyInfo* tpInfo = nullptr;
		tpInfo = new SKeyInfo();

		//typeid������: ������ Ÿ�Կ� ���� ������ ��´�
		//Ÿ���� �̸��� ����
		const char* tpType = typeid(T).name();

		//strcmp ���ڿ� �� �Լ�
		if (0 == strcmp("char", tpType) || 0 == strcmp("int", tpType))
		{
			tpInfo->mKeyInput = (DWORD)tData;	//���� �Է��� �����صд�
		}
		else
		{
			return false;
		}

		tpInfo->mStrName = tInputName;	//�߻�ȭ�� �Է� �̸� ����

		//���� ����� �Է� �����͸� �ڷᱸ���� ��´�
		mMapKeys.insert(make_pair(tpInfo->mStrName, tpInfo));

		return true;
	}
};