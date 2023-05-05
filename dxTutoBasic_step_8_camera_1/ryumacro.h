#pragma once

//매크로 상수
#define RYU_NUMBER 777

//매크로 함수
//매크로 함수는 한줄로 작성
//장점: 
//		호출부의 가독성이 좋아진다
//		코드 작성의 효율성이 좋아진다
//단점: 
//		디버깅이 불가능하다

//#define SAFE_DELETE(p) if (nullptr != p) { delete p; p = nullptr; }

//여러줄 처럼 보이게 작성하려면 \로 연결하자 
#define SAFE_DELETE(p) \
		if (nullptr != p) \
		{ \
			delete p; \
			p = nullptr; \
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
	}\
}


#define ROOT_PATH "root_path"
#define RESOURCES "resources_path"


#define FIND_TEX(p) CResourcesMgr::GetInstance()->FindTexture(p) 