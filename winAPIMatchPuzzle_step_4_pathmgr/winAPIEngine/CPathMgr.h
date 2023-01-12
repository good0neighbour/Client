#pragma once

#include <windows.h>

#include "ryumacro.h"

#include <string>

#include <unordered_map>	//�ؽ� �ڷᱸ���� STL�����̳ʷ� ���� ��
using namespace std;
/*
* ��� ������ Path Manager: �ܺ����� �ڿ����� ������ '���'�� ������ ��ü
* 
* i) ������ ��ü <-- Singleton Pattern
* ii) unordered_map <-- ��� �����͵��� ��Ƶ� �ڷᱸ��
*/

class CPathMgr
{
private:
	//�ؽ����̺�, O(1)�˻��ӵ��� ������. 'Ű/��' ���� �����͸� �ٷ��
	//Ű: ���ڿ� string
	//��: �����ڵ� ���ڿ� Ÿ�� wstring <--������ �ü������ �ٷ�� �����ڵ� ���ڿ��� �ٷ�� �ϱ� ������ �� Ÿ������ �����ߴ�.
	unordered_map<string, wstring> mPaths;

public:
	//���������� �ִ� ���(root ���)�� �����Ѵ�
	bool Create();

	//������ ��θ� ����� �ڷᱸ���� �߰��Ѵ�
	bool MakePath(const string& tStrKey, const wchar_t* tpPath, const string& tStrBaseKey = "root_path");

private:
	//���ϰ��� C ��Ÿ�� �����ڵ� ���ڿ� (window api�� c�� �����ϹǷ� ����Ÿ���� �̷��� �����ߴ� )
	const wchar_t* FindPath(const string& tStrKey);

	SINGLETON_DECLARE(CPathMgr)
};

