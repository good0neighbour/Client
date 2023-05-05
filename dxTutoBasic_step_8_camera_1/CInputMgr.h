#pragma once
//<-- �ѹ��� ���������ּ���. ��Ŭ��� ������

/*
	OOP Object Oriented Programming ��ü���� ���α׷���
	: Ŭ������ ����, ��ü�� ���踦 �̿��Ͽ� ���α׷� ������ �ۼ��ϴ� ��

	Design Pattern: OOP�� �������� 

	Singleton DP ����: ��ü�� �ִ���������� 1���� �����ϴ� ���� 
	( �ΰ�������, �������� ������ ���� �뵵�ε� �� ���� �ִ�. )


	i)�ش�Ŭ����Ÿ���� ������ ������ static���� �����
	ii)�����ڴ� public�� �ƴϴ�
	iii)GetInstance()�Լ��� ���ǿ��� ��ü�� �ִ���������� 1���� �����ϴ� �Ǵ�������� ��ġ�Ѵ�
*/

//step_1

#include <windows.h>

#include "ryumacro.h"
//cpp���� �����ϴ� ���ڿ� Ÿ�� string�� ����ϱ� ���� ����
#include <string>
//�ؽ� �ڷᱸ���� STL�� �����̳ʷ� ������� ���̴�.
#include <unordered_map>
#include <vector>
//typeid���� ���� ���� 
#include <typeinfo>

using namespace std;


//'Ű�����Է� ����'�� ǥ���ϱ� ���� ����ü ����

//��(���� ������)
struct SKeyInfo
{
	//Ű �� �� �� �����ͷ� �ٷ���
	string mStrName;		//Ű ( �˻��� Ű )		�߻�ȭ�� �̸�
	//DWORD mKeyInput;		//	���� Ű�����Է� ������	
	//<-- '���� Ű' ������ �����ϱ� ���� N���� ���� Ű�����Է� �����͸� �ٷ絵�� ��������
	//<-- �����߿� ������ ������ �������̹Ƿ� �����迭�� ������� 
	vector<DWORD> mKeyInputs;


	//Ű������ �Է� ���µ�
	bool mIsDown	= false;
	bool mIsPress	= false;
	bool mIsUp		= false;

	SKeyInfo() :mIsDown(false), mIsPress(false), mIsUp(false) 
	{
		mKeyInputs.clear();
	};
};


class CInputMgr
{
	SINGLETON_DECLARE(CInputMgr)


private:
	//Ű�����Է� �����͵� �ڷᱸ��
	unordered_map<string, SKeyInfo*> mMapKeyInfos;
	//Ű�����Է� �����͵� �ڷᱸ�� ����� ����� ����
	SKeyInfo* mpTempKeyInfo = nullptr;

public:
	void Create();
	void Destroy();

	void Update();

	//������ Ű�����Է� �˻�
	SKeyInfo* FindKey(const string& tStrKey) const;

	//������ Ű�����Է��� ���� ���� ���ϱ�
	bool KeyDown(const string& tStrKey) const;
	bool KeyPress(const string& tStrKey) const;
	bool KeyUp(const string& tStrKey) const;

	//�������� ���ø� ����, N���� ���ڸ� ���� �� �ִ�
	// 
	//base case
	template<typename T>
	bool AddKey(const T& tData)
	{
		const char* tpTypeName = typeid(T).name();

		if (strcmp(tpTypeName, "char") == 0 ||
			strcmp(tpTypeName, "int") == 0)
		{
			mpTempKeyInfo->mKeyInputs.push_back((DWORD)tData);
		}

		return true;
	}

	//recursive case 		
	template<typename T,  typename... Types>
	bool AddKey(const T& tData, const Types&... arg)
	{
		if (!mpTempKeyInfo)
		{
			mpTempKeyInfo = new SKeyInfo();
		}
		
		const char* tpTypeName = typeid(T).name();
		
		if (strcmp(tpTypeName, "char") == 0 ||
			strcmp(tpTypeName, "int") == 0)
		{
			//mpTempKeyInfo->mKeyInput = (DWORD)tData;
			mpTempKeyInfo->mKeyInputs.push_back((DWORD)tData);
		}
		else
		{			
			mpTempKeyInfo->mStrName = tData;//tInputName;

			mMapKeyInfos.insert(make_pair(mpTempKeyInfo->mStrName, mpTempKeyInfo));
		}

		//���ȣ��
		AddKey(arg...);



		if (mpTempKeyInfo)
		{
			mpTempKeyInfo = nullptr;
		}


		return true;
	}
};

