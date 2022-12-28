#pragma once

#include <windows.h>
#include <unordered_map>
#include <string>

#include "CAniSeq.h"


using namespace std;

//class CAniSeq;
class CAPIEngine;

class CUnit;

class CAnimator
{
public:
	CAnimator();
	~CAnimator();

	CAnimator(const CAnimator& t);

	void operator=(const CAnimator& t);

public:
	//�ִϸ��̼� �������� �߰��ϴ� �Լ�
	bool AddAniSeq(const string& tId, float tTimeInterval, int tTotalFrameCount, LPCWSTR tpFileName, ANI_PO tPlayOption = ANI_PO::LOOP);
	//���� �ִϸ��̼� �������� ���� ������ ����
	void UpdateAnimation(float tDeltaTime);
	//���� �ִϸ��̼� �������� ��� ��� ������
	void Render(float tX, float tY);


	void SetDefaultAniSeq(const string& tStrDefaultAniSeq);
	void PlayAni(const string& tStrAniSeq);
	void LateUpdate();



	void Create(CAPIEngine* tpEngine);
	void Destroy();
	inline void SetId(string tId)
	{
		mId = tId;
	}

	inline void SetOwnerObject(CUnit* tpObject)
	{
		mpObject = tpObject;
	}
	inline CUnit* GetOwnerObject()
	{
		return mpObject;
	}

public:
	// Animator �� �ĺ���
	string mId = "";

	//�����߿� ���ϴ� �ִϸ��̼� �������� �˻��Ͽ� �÷��̸� �����ϱ� ���� �ڷᱸ��
	//���� �˻��ӵ��� �߿��ϹǷ� O(1)�� �ؽ��ڷᱸ���� �̿��ϱ�� ��
	//			N���� �ִϸ��̼� �������� ����
	unordered_map<string, CAniSeq*> mAniSeqs;

	//���� �÷��̵ǰ� �ִ� �ִϸ��̼� ������ ��ü ����
	CAniSeq* mpCurAniSeq = nullptr;
	string mStrKeyCurAniSeq = "";		//���� �÷��̵ǰ� �ִ� �ִϸ��̼� �������� ���ڿ� �ĺ���

	//���� ���� ����
	CAPIEngine* mpEngine = nullptr;

	//�� �ִϸ����� �� ������ ��ü ����
	CUnit* mpObject = nullptr;

	//������ �ִϸ��̼� ���� ���ư��� ���� �ʿ���.
	//�ٷ� ���� �ִϸ��̼� �������� �ĺ���
	string mStrKeyPrevAniSeq = "";

};

