#pragma once



/*
		����ȭ ���α׷��� structured programming
		: �Լ��� ����ü ���� ������ ����� ���α׷� ������ �ۼ��ϴ� �����

		��ü���� ���α׷��� Object Oriented Programming
		: Ŭ���� �� ����, ��ü �� ���� �� ������ ���α׷� ������ �ۼ��ϴ� �����

		�Ϲ�ȭ ���α׷��� General Programming
		: Ÿ���� �Ű�����ȭ�Ͽ� �ٷ�� ��� 
	
	Design Pattern
		: OOP������ ��������
	
	Singleton Pattern �� ����
	:	��ü�� �ִ� ���������� 1���� �����ϴ� ����


	�̱��� ������ ����� ���
	i) �ش� Ŭ���� Ÿ���� ������� mpInstance�� static���� �����Ѵ� 
	ii) �����ڴ� public�� �ƴϴ�.
	iii) GetInstance()�Լ��� ���ǿ��� ��ü�� �ִ� ���������� 1���� �����ϴ� �Ǵ�������� ��ġ��Ų��

*/
#include <windows.h>
#include "ryumacro.h"
#include <string>
#include <unordered_map>	//�ؽ� �ڷᱸ���� STL�����̳ʷ� ���� ��
#include <vector>	//�����迭

#include <typeinfo>//typeid ������ ����ϱ� ����
using namespace std;

struct SKeyInfo
{
	string mStrName = "";			//�߻�ȭ�� �Է� �̸�
	//DWORD mKeyInput = 0;		//���� �Է�
	vector<DWORD> mKeyInputs; //���� �Էµ�

	bool mIsDown = false;		//ó�� ���ȴ��� ����
	bool mIsPress = false;		//������ �ִ� ������ ����
	bool mIsUp = false;			//�������� ����
};



//��ũ���Լ��� �̿��Ͽ� ����ȭ�Ǿ� �ݺ��Ǵ� �ڵ带 �����ϰ� �ۼ���
class CInputMgr
{
	SINGLETON_DECLARE(CInputMgr)

private:
	//�ؽ����̺�, O(1)�˻��ӵ��� ������. 'Ű/��' ���� �����͸� �ٷ��
	unordered_map<string, SKeyInfo*> mMapKeys;	//�Է��������� ��Ƶ� �ڷᱸ��

	//���ø� �������� base case�Լ����� �� ó���� ���� Ŭ���� �������� ��ȿ�ϵ��� �������ȭ
	SKeyInfo* mpKeyInfo = nullptr;

private:
	SKeyInfo* FindKey(const string& tStrKey) const;			//������ �Է����� �˻�, �˻�Ű�� ���ڿ� Ÿ��

public:
	void Update();			//�Է��� ����(down, press, up) ����(����)	

	//������ �Է¿� ���� ���¿���
	bool KeyDown(const string& tStrKey) const;
	bool KeyPress(const string& tStrKey) const;
	bool KeyUp(const string& tStrKey) const;

	//�Է����� ��� �Լ�

	//char, int �� Ÿ�Կ� ���� ��� �۵���Ű�� ���� '���ø� ����' �̿�
	//���ø��̹Ƿ� '���'�� ����
	//template<typename T>
	//bool AddKey(const string& tInputName, const T& tData)
	//{
	//	SKeyInfo* tpInfo = nullptr;
	//	tpInfo = new SKeyInfo();


	//	//typeid������: ������ Ÿ�Կ� ���� ������ ��´�
	//	//Ÿ���� �̸��� ����
	//	const char* tpType = typeid(T).name();

	//	//strcmp C�� ���ڿ� �� �Լ�,  ������ 0����
	//	if ( 0 == strcmp("char", tpType) || 0 == strcmp("int", tpType) )
	//	{
	//		tpInfo->mKeyInput = (DWORD)tData;				//���� �Է� �� �����صд�
	//	}
	//	else
	//	{
	//		return false;
	//	}

	//	tpInfo->mStrName = tInputName;				//�߻�ȭ�� �Է� �̸� ����

	//	//���� ����� �Է� �����͸� �ڷᱸ���� ��´�
	//	mMapKeys.insert( make_pair( tpInfo->mStrName, tpInfo ) );


	//	return true;
	//}




	//���ø� �������� �Լ��� base case �Լ�
	template<typename T>
	bool AddKey(const T& tData)
	{		
		const char* tpType = typeid(T).name();

		if (0 == strcmp("char", tpType) || 0 == strcmp("int", tpType))
		{
			mpKeyInfo->mKeyInputs.push_back((DWORD)tData);				//���� �Է� �� �����صд�
		}

		return true;
	}

	//���ø� ��������
	//recursive case
	template<typename T, typename... Types>
	bool AddKey(const T& tData, const Types&... arg)
	{	
		//���ȣ��ǹǷ� �ѹ��� ������.
		if (nullptr == mpKeyInfo)
		{
			mpKeyInfo = new SKeyInfo();
		}

		const char* tpType = typeid(T).name();
		if (0 == strcmp("char", tpType) || 0 == strcmp("int", tpType))
		{			
			mpKeyInfo->mKeyInputs.push_back((DWORD)tData);
		}
		else//���ڿ��̸�
		{
			mpKeyInfo->mStrName = tData;//tInputName;				//�߻�ȭ�� �Է� �̸� ����

			mMapKeys.insert(make_pair(mpKeyInfo->mStrName, mpKeyInfo));
		}		

		//���⼭ ���ȣ���� �Ͼ��.
		AddKey(arg...);

		if (nullptr != mpKeyInfo)
		{
			mpKeyInfo = nullptr;
		}

		return true;
	}
	
};

