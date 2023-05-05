
//전역변수
Texture2D texDiffuse : register(t0);
SamplerState samplerStateLinear : register(s0);

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

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normal: NORMAL;//법선데이터
	float2 texcoord : TEXCOORD0;

	//specular
	//시선벡터
	float3 viewDir:TEXCOORD1;	// <--float 3개짜리임을 주의. 픽셀 셰이더 수준의 처리를 할 것이므로 TEXCOORD 시맨틱을 주었음을 주의
};

//pixel shader: pixel 하나당 이렇게 동작한다
// 
//결과값은 픽셀의 색상 데이터 rgba
float4 main(VS_OUTPUT tIn) : SV_TARGET		//System Value_Target 2차원 이미지 데이터 모음 _ RGBA
{
	//vertex shading 단계로부터 전달받은 정점의 색상정보로 셰이딩했다.
	//return tIn.color;

	//ambient
	float4 tResult = 0;

	float3 reflectDir = 0;			//반사벡터
	float4 specular = 0;			//최종적인 정반사광 색상



	//조명 방향을 뒤집어서 계산하기 위해 지역변수를 선언하였다.
	float4 tLightDir;
	tLightDir = -1.0f * lightDir;	//예외: 하나짜리는 모든 자릿수와 가능하다

	//dot까지 연산한 광량의 범위는 [-1,1]
	//saturate까지 연산한 범위는 [0,1]
	//tResult = saturate(dot(lightDir, tIn.normal))* lightColor;//예외: 하나짜리는 모든 자릿수와 가능하다
	float4 diffuseColor = saturate(dot(tLightDir, tIn.normal)) * lightColor;//예외: 하나짜리는 모든 자릿수와 가능하다
	diffuseColor.a = 1.0f;		//알파값은 1로 결정

	//반사벡터를 구한다
	reflectDir = reflect(lightDir, tIn.normal);
	//specular의 광량
	specular = pow(saturate(dot(reflectDir, tIn.viewDir)), specularPower);		//거듭제곱 
	//specular 색상까지 적용
	specular = specular * specularColor;



	//텍스처를 uv좌표계를 기반으로 샘플링(필터링 처리도 하면서)하여 임의의 색상값을 하나 얻어온다
	float4 texColor = texDiffuse.Sample(samplerStateLinear, tIn.texcoord);

	//색상 곱셈 연산
	//최종색상 = 주변광 + 난반사색상*텍스처색상
	tResult = ambientColor + diffuseColor * texColor;		//ambient
	//saturate까지 연산한 범위는 [0,1]
	tResult = saturate(tResult);

	//Highlight 되므로 색상연산더하기
	tResult = saturate(tResult + specular);

	return tResult;
}