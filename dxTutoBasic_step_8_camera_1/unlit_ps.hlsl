
//전역변수
Texture2D texDiffuse : register(t0);
SamplerState samplerStateLinear : register(s0);

////16byte 단위로 정렬하도록 정의해야함
//cbuffer CBTransform
//{
//	matrix World;		//<--전역변수
//	matrix View;			//<--전역변수
//	matrix Projection;	//<--전역변수
//
//	float4 lightDir;		//<--전역변수
//	float4 lightColor;	//<--전역변수
//
//	//ambient
//	float4 ambientColor;
//
//	//specular
//	float4 cameraPosition;	//카메라위치
//	float4 specularColor;		//정반사광 색상
//	float specularPower;		//하일라이트 면적분포 결정 수치
//	float3 padding;				//그냥 끼워넣음 <-- 16바이트 단위로 맞추기 위함, 16바이트 정렬용 더미 데이터
//};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

//pixel shader: pixel 하나당 이렇게 동작한다
//결과값은 픽셀의 색상 데이터 rgba
float4 main(VS_OUTPUT tIn) : SV_TARGET		//System Value_Target 2차원 이미지 데이터 모음 _ RGBA
{
	float4 tResult = texDiffuse.Sample(samplerStateLinear, tIn.texcoord);

	return tResult;
}