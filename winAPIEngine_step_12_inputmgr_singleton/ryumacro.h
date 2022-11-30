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