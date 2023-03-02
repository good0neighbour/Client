
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);


cbuffer ConstantBuffer : register(b0)   
{
    matrix World;
    matrix View;
    matrix Projection;

    float4 vLightDir;
    float4 vLightColor;
}

struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Norm : NORMAL;
    float2 Tex : TEXCOORD0;
};

//light
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Norm : NORMAL;
    float2 Tex : TEXCOORD0;
};

PS_INPUT main(VS_INPUT i)
{
    PS_INPUT o = (PS_INPUT)0;

    o.Pos = mul(i.Pos, World);
    o.Pos = mul(o.Pos, View);
    o.Pos = mul(o.Pos, Projection);
    
    o.Norm = mul(float4(i.Norm, 1), World).xyz;
    o.Tex = i.Tex;

    return o;
}





