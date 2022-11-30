#pragma once

//��ũ�� ���
#define RYU_INT unsigned int

//��ũ�� �Լ�
//#define SAFE_DELETE(p) if (nullptr != p) { delete p; p = nullptr; }
#define SAFE_DELETE(p) \
if (nullptr != p)\
{\
	delete p;\
	p = nullptr;\
}