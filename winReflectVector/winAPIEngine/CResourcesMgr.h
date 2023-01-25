#pragma once

#include "ryumacro.h"

#include <windows.h>

#include <string>
#include <unordered_map>	//�ؽ� �ڷᱸ���� STL�����̳ʷ� ���� ��
using namespace std;

class CTexture;
class CAPIEngine;

class CResourcesMgr
{
private:
	//�ڿ�(resource)���� ���� ������ ���� �� �ִ�. 
	//�̸��׸�, �̹���, ����, �ؽ�Ʈ, ����, ... 
	//������ ���⼭�� ��Ʈ�� �̹��� �����͸� �����Ѵ�.
	//
	//�ؽ����̺�, O(1)�˻��ӵ��� ������. 'Ű/��' ���� �����͸� �ٷ��
	//Ű: ���ڿ�	string
	//��: CTexture�� ��ü�� �ּҰ�
	unordered_map<string, CTexture*> mTexs;

	CAPIEngine* mpEngine = nullptr;


public:
	void Create(CAPIEngine* tpEngine);

	CTexture* LoadTexture(const string& tStrKey, const wchar_t* tpFileName, const string& tStrPathKey);

	CTexture* FindTexture(const string& tStrKey);



	SINGLETON_DECLARE(CResourcesMgr)
};

