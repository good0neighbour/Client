
//16byte ������ �����ϵ��� �����ؾ���
cbuffer CBTransform : register(b0)
{
	matrix World;		//<--��������
	matrix View;			//<--��������
	matrix Projection;	//<--��������

	/*
	float4 lightDir;		//<--��������
	float4 lightColor;	//<--��������

	//ambient
	float4 ambientColor;

	//specular
	float4 cameraPosition;	//ī�޶���ġ
	float4 specularColor;		//���ݻ籤 ����
	float specularPower;		//���϶���Ʈ �������� ���� ��ġ
	float3 padding;				//�׳� �������� <-- 16����Ʈ ������ ���߱� ����
	*/
};

//�������α׷� ---> �������̴�
struct VS_INPUT
{
	float4 position : POSITION;
	float2 texcoord: TEXCOORD0;
};

//hlsl ����

//���� �뵵�� ������ ����ü�� ����
//vertex shader�� ���(���), ��, ���ÿ����� pixel shader�� �Է�
//�������̴� ---> �ȼ����̴�
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texcoord:TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT tIn)
{
	VS_OUTPUT tResult = (VS_OUTPUT)0;

	//IA�κ��� �Ѱܹ��� ������ ��ġ ������ 
	//����, ��, ������ȯ�� ����
	tResult.position = mul(tIn.position, World);	//mul ���̴� ���� �Լ�, ��İ� ���͸� �������ش�. ������ �������� �����Ѵ�
	tResult.position = mul(tResult.position, View);
	tResult.position = mul(tResult.position, Projection);

	//���� ���̵� �ܰ�� �״�� ����
	tResult.texcoord = tIn.texcoord;

	return tResult;
}