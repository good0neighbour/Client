
//16byte 단위로 정렬하도록 정의해야함
cbuffer CBTransform : register(b0)
{
	matrix World;		//<--전역변수
	matrix View;			//<--전역변수
	matrix Projection;	//<--전역변수
};

cbuffer CBLight : register(b1)
{
	float4 lightDir;		//<--전역변수
	float4 lightColor;	//<--전역변수

	//ambient
	float4 ambientColor;

	//specular
	float4 cameraPosition;	//카메라위치
	float4 specularColor;		//정반사광 색상
	float specularPower;		//하일라이트 면적분포 결정 수치
	float3 padding;				//그냥 끼워넣음 <-- 16바이트 단위로 맞추기 위함
};

//응용프로그램 ---> 정점셰이더
struct VS_INPUT
{
	float4 position : POSITION;
	float3 normal: NORMAL;
	float2 texcoord: TEXCOORD0;
};

//hlsl 문법

//다음 용도로 임의의 구조체를 만듦
//vertex shader의 결과(출력), 즉, 예시에서는 pixel shader의 입력
//정점셰이더 ---> 픽셀셰이더
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normal: NORMAL;//법선데이터
	float2 texcoord:TEXCOORD0;

	//specular
	//시선벡터
	float3 viewDir:TEXCOORD1;	// <--float 3개짜리임을 주의. 픽셀 셰이더 수준의 처리를 할 것이므로 TEXCOORD 시맨틱을 주었음을 주의
};

//vertex shader: vertex 하나당 이렇게 동작한다

//float4 pos는 응용프로그램에서 넘어오는 정점데이터
//POSITION은 시맨틱
//결과값은 위치값 xyzw
//float4 main( float4 pos : POSITION, float4 color : COLOR) : SV_POSITION	//System Value_Position 픽셀수준에서 2차원위치데이터_ XYZ
VS_OUTPUT main(VS_INPUT tIn)
{
	VS_OUTPUT tResult = (VS_OUTPUT)0;

	//월드상에서의 위치 데이터
	float4 worldPosition;

	//IA로부터 넘겨받은 정점의 위치 정보에 
	//월드, 뷰, 투영변환을 적용
	tResult.position = mul(tIn.position, World);	//mul 셰이더 내장 함수, 행렬과 벡터를 곱셈해준다. 열벡터 기준으로 동작한다

		//월드변환까지 적용한 정점의 위치데이터
		worldPosition = tResult.position;

	tResult.position = mul(tResult.position, View);
	tResult.position = mul(tResult.position, Projection);

	tResult.normal = mul(float4(tIn.normal, 0), World).xyz;
	//다음 셰이딩 단계로 그대로 전달
	tResult.texcoord = tIn.texcoord;

	//시선벡터구하기
		//임의의 크기에 임의의 방향의 벡터 = 목적지점 - 시작지점
		//시선벡터 = 정점의 위치데이터 - 카메라위치데이터
		
		//계산의 편의를 위해 뒤집는다 
		//tResult.viewDir = worldPosition.xyz - cameraPosition.xyz;
		tResult.viewDir = cameraPosition.xyz - worldPosition.xyz;	//<--뒤집힌 시선벡터
		tResult.viewDir = normalize(tResult.viewDir);		//정규화 수행

	return tResult;
}