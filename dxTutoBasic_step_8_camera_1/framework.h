// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


//#include "CDXEngine.h"

//directX11 버전은 window sdk에 이제 포함되므로
//( 따로 설치하지 않고 )
//헤더를 포함하고
//
//관련 라이브러리를 링크해주면 간단히 설정을 끝낼 수 있다
//
//라이브러리 링크는 코드상에서 하겠다

//dx11 lib header
#include <d3d11_1.h>
#include <directxcolors.h>

#include <d3dcompiler.h>
#include <DirectXMath.h>

//dx11 lib link
#pragma comment(lib, "d3d11.lib")			//for direct3d
#pragma comment(lib, "d3dcompiler.lib")	//for shader

//texture lib link
#pragma comment(lib, "dxguid.lib")	

using namespace std;
//namespace
using namespace DirectX;		//DirectX 용 namespace



#include "CInputMgr.h"