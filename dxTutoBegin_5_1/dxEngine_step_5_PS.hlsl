
Texture2D txDiffuse : register(t0);	//texture, 0������
SamplerState samLinear : register(s0);	//���û��� 0������



cbuffer ConstantBuffer : register(b0)	//buffer, 0������
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

	//���� ���
	color			= saturate(dot((float3)vLightDir, input.Norm) * vLightColor);
	color.a			= 1;

	//���ø��� �ؽ��Ŀ� �������
	color			= txDiffuse.Sample(samLinear, input.Tex) * color;

	//�������� ����
	return color;
}

