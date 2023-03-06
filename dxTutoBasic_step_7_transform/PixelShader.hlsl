
//다음은 임의의 픽셀 데이터 하나의 처리에 관한 것이다

//SV_TARGET: TargetView에 만들어질 색상정보 데이터


//diffuse 확산, 난반사

Texture2D texDiffuse:register(t0);						//텍스처 리소스 
SamplerState sampleStateLinear:register(s0);		//샘플러 데이터



//상수버퍼 
//cbuffer CBTransform
//{
//	matrix mWorld;            //월드변환 행렬		<-- 전역변수  //4byte*4*4 = 64byte
//	matrix mView;              //뷰변환 행렬		<-- 전역변수		//4byte*4*4 = 64byte
//	matrix mProjection;       //투영변환 행렬		<-- 전역변수	//4byte*4*4 = 64byte
//
//	//난반사 관련 조명 정보 
//	float4 mLightDir;			//빛방향벡터			<-- 전역변수	//4byte*4*4 = 64byte
//	float4 mLightColor;		//빛 색상						<-- 전역변수	//4byte*4*4 = 64byte
//
//	//주변광 관련 조명 정보
//	float4 mAmbientColor;		//주변광 색상
//
//	//정반사광 관련 조명 정보
//	float4 mCameraPosition;       //카메라 위치 정보<-- 시선 벡터를 구하기 위해서 
//	float4 mSpecularColor;     //정반사광 색상 
//	float mSpecularPower;           //거듭제곱 수<-- 하일라이트 면적 결정 수치
//	float3 padding; //그냥 끼워넣음<--상수버퍼는 16바이트 정렬 단위를 맞춰야 한다.
//};


//cbuffer CBTransform: register(b0)
//{
//	matrix mWorld;
//	matrix mView;
//	matrix mProjection;
//};

cbuffer CBLight: register(b1)
{
	float4 mLightDir;
	float4 mLightColor;

	float4 mAmbientColor;

	float4 mCameraPosition;
	float4 mSpecularColor;
	float mSpecularPower;
	float3 padding;
};


//정점셰이더-->픽셀셰이더
struct VS_OUTPUT
{
	float4 position:SV_POSITION;
	//float4 color:COLOR;
	float3 normal:NORMAL;
	float2 texcoord:TEXCOORD0;

	//시선벡터 
	float3 viewDir:TEXCOORD1;
};


float4 main(VS_OUTPUT tIn) : SV_TARGET
{
	//-색상연산 룰-
	//색상연산은 같은 자릿수에서만 가능하다.( 자릿수가 동일해야 색상연산이 가능하다.)
	//tResult = float4(0, 0, 0, 1) + float2(1, 0);
	//-->단, 색상연산에서 한자릿수는 모든 자릿수와 연산이 가능하다

	//픽셀 셰이더에서 색상연산
	//단, 색상연산에서 한자릿수는 모든 자릿수와 연산이 가능하다
	//float4 tResult = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 tResult = 0.0f;

	//확산광(난반사광)
	float4 tDiffuse = 0.0f;

	float4 tLightDir;		//빛 벡터( 뒤집어서 준비한다 )
	tLightDir = -1.0f * mLightDir;

	//램버트 조명모델: 빛벡터 dot 법선벡터
	//saturate함수를 이용하여  [-1,1] ---> [0,1] 사이의 스칼라값으로 도출
	//스칼라(한자릿수)*빛색상(4자릿수) = 음영이 적용된 색상(4자릿수)
	tDiffuse = saturate(dot(tLightDir, tIn.normal))* mLightColor;	

	//diffuse를 구하기 위해서는 빛반사를 뒤집어 계산했지만
	//specular를 구하기 위해서는 빛반사를 뒤집으면 안된다.


	//정반사광 색상
	float4 tSpecularColor = 0.0f;

	float3 reflectDir = 0.0f;		//반사 벡터 
	//반사벡터를 구한다
	reflectDir = reflect(-1.0f*tLightDir, tIn.normal);	//빛벡터는 다시 한번 뒤집어 원래대로 만듦

	//[-1, 1] --> [0,1]
	//거듭제곱 pow(x, y)
	tSpecularColor = pow(saturate(dot(reflectDir, tIn.viewDir)), mSpecularPower);

	//응용프로그램에서 입력받은 색상값도 적용( 색상 곱셈 연산 )
	tSpecularColor = tSpecularColor * mSpecularColor;


	

	//앞 단계에서 받은 색상 정보를 그대로 넘겼다.
	//return tIn.color;

	float4 tTexColor = 0.0f;
	//텍스처 좌표(uv좌표) 정보를 기반으로 , 샘플링 데이터를 참고하여 텍셀을 샘플링하여 최종색상 결정
	tTexColor = texDiffuse.Sample(sampleStateLinear, tIn.texcoord);

	////색상 곱셈 연산<-- 각각의 자릿수끼리 곱한다
	////최종색상 = 난반사광*텍스처색상
	//tResult = tDiffuse * tTexColor;
	//tResult.a = 1.0f;		//알파값은 1로 지정


	//색상 곱셈 연산<-- 각각의 자릿수끼리 곱한다
	//색상 덧셈 연산<-- 각각의 자릿수끼리 더한다
	//최종색상 = 난반사광*텍스처색상 + 주변광
	tResult = tDiffuse * tTexColor + mAmbientColor;		
	//정반사광 까지 적용
	//최종색상 = (난반사광*텍스처색상 + 주변광) + 정반사광
	tResult = tResult + tSpecularColor;

	//saturate [0,1] 사이의 값으로 만든다.
	tResult = saturate(tResult);

	tResult.a = 1.0f;		//알파값은 1로 지정

	return tResult;
}