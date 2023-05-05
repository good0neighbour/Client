
//��������
Texture2D texDiffuse : register(t0);
SamplerState samplerStateLinear : register(s0);

////16byte ������ �����ϵ��� �����ؾ���
//cbuffer CBTransform
//{
//	matrix World;		//<--��������
//	matrix View;			//<--��������
//	matrix Projection;	//<--��������
//
//	float4 lightDir;		//<--��������
//	float4 lightColor;	//<--��������
//
//	//ambient
//	float4 ambientColor;
//
//	//specular
//	float4 cameraPosition;	//ī�޶���ġ
//	float4 specularColor;		//���ݻ籤 ����
//	float specularPower;		//���϶���Ʈ �������� ���� ��ġ
//	float3 padding;				//�׳� �������� <-- 16����Ʈ ������ ���߱� ����, 16����Ʈ ���Ŀ� ���� ������
//};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

//pixel shader: pixel �ϳ��� �̷��� �����Ѵ�
//������� �ȼ��� ���� ������ rgba
float4 main(VS_OUTPUT tIn) : SV_TARGET		//System Value_Target 2���� �̹��� ������ ���� _ RGBA
{
	float4 tResult = texDiffuse.Sample(samplerStateLinear, tIn.texcoord);

	return tResult;
}