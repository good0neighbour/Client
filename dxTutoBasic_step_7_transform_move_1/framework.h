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



//#include "CDxEngine.h"

//directX11버전은 window sdk에 이제 포함되어 있으므로( 따로 설치하지 않고 )
//
//헤더파일을 포함하고 
//라이브러리 파일을 링크함으로 해서 
//바로 사용가능하다.

//dx11 header
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

//dx11
#pragma comment(lib, "d3d11.lib")			//for direct3d
#pragma comment(lib, "d3dcompiler.lib")			//for shader

#pragma comment(lib, "dxguid.lib")			//for texture dds사용을 위해 

using namespace std;
//for DirectX namespace
using namespace DirectX;
