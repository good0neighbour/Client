#pragma once

//매크로 상수
#define RYU_INT unsigned int

//매크로 함수
//#define SAFE_DELETE(p) if (nullptr != p) { delete p; p = nullptr; }
#define SAFE_DELETE(p) \
if (nullptr != p)\
{\
	delete p;\
	p = nullptr;\
}
#define SAFE_RELEASE(p) \
if (nullptr != p)\
{\
	p->Release();\
	p = nullptr;\
}

#define SINGLETON_DECLARE(p) private:\
	static p* mpInstance;\
private:\
	p();\
	~p();\
public:\
	static p* GetInstance();\
	static void ReleaseInstance();

#define SINGLETON_DEFINITION(p) p* p::mpInstance = nullptr;\
p* p::GetInstance()\
{\
	if (nullptr == mpInstance)\
	{\
		mpInstance = new p();\
	}\
	return mpInstance;\
}\
void p::ReleaseInstance()\
{\
	if (nullptr != mpInstance)\
	{\
		delete mpInstance;\
		mpInstance = nullptr;\
	}\
}