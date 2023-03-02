
Texture2D txDiffuse : register(t0);	//texture, 0번슬롯
SamplerState samLinear : register(s0);	//샘플상태 0번슬롯



cbuffer ConstantBuffer : register(b0)	//buffer, 0번슬롯
{
	matrix World;
	matrix View;
	matrix Projection;

	float4 vLightDir;
	float4 vLightColor;
}


//light
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD0;
};



float4 main(PS_INPUT input) : SV_Target
{
	float4 color	= 0;

	//광량 계산
	color			= saturate(dot((float3)vLightDir, input.Norm) * vLightColor);
	color.a			= 1;

	//샘플링한 텍스쳐와 색상곱셈
	color			= txDiffuse.Sample(samLinear, input.Tex) * color;

	//최종색상 리턴
	return color;
}

