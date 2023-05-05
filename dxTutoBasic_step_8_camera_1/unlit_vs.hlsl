
//16byte 단위로 정렬하도록 정의해야함
cbuffer CBTransform : register(b0)
{
	matrix World;		//<--전역변수
	matrix View;			//<--전역변수
	matrix Projection;	//<--전역변수

	/*
	float4 lightDir;		//<--전역변수
	float4 lightColor;	//<--전역변수

	//ambient
	float4 ambientColor;

	//specular
	float4 cameraPosition;	//카메라위치
	float4 specularColor;		//정반사광 색상
	float specularPower;		//하일라이트 면적분포 결정 수치
	float3 padding;				//그냥 끼워넣음 <-- 16바이트 단위로 맞추기 위함
	*/
};

//응용프로그램 ---> 정점셰이더
struct VS_INPUT
{
	float4 position : POSITION;
	float2 texcoord: TEXCOORD0;
};

//hlsl 문법

//다음 용도로 임의의 구조체를 만듦
//vertex shader의 결과(출력), 즉, 예시에서는 pixel shader의 입력
//정점셰이더 ---> 픽셀셰이더
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texcoord:TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT tIn)
{
	VS_OUTPUT tResult = (VS_OUTPUT)0;

	//IA로부터 넘겨받은 정점의 위치 정보에 
	//월드, 뷰, 투영변환을 적용
	tResult.position = mul(tIn.position, World);	//mul 셰이더 내장 함수, 행렬과 벡터를 곱셈해준다. 열벡터 기준으로 동작한다
	tResult.position = mul(tResult.position, View);
	tResult.position = mul(tResult.position, Projection);

	//다음 셰이딩 단계로 그대로 전달
	tResult.texcoord = tIn.texcoord;

	return tResult;
}