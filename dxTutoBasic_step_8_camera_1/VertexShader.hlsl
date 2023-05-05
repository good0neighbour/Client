
//16byte ������ �����ϵ��� �����ؾ���
cbuffer CBTransform : register(b0)
{
	matrix World;		//<--��������
	matrix View;			//<--��������
	matrix Projection;	//<--��������
};

cbuffer CBLight : register(b1)
{
	float4 lightDir;		//<--��������
	float4 lightColor;	//<--��������

	//ambient
	float4 ambientColor;

	//specular
	float4 cameraPosition;	//ī�޶���ġ
	float4 specularColor;		//���ݻ籤 ����
	float specularPower;		//���϶���Ʈ �������� ���� ��ġ
	float3 padding;				//�׳� �������� <-- 16����Ʈ ������ ���߱� ����
};

//�������α׷� ---> �������̴�
struct VS_INPUT
{
	float4 position : POSITION;
	float3 normal: NORMAL;
	float2 texcoord: TEXCOORD0;
};

//hlsl ����

//���� �뵵�� ������ ����ü�� ����
//vertex shader�� ���(���), ��, ���ÿ����� pixel shader�� �Է�
//�������̴� ---> �ȼ����̴�
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normal: NORMAL;//����������
	float2 texcoord:TEXCOORD0;

	//specular
	//�ü�����
	float3 viewDir:TEXCOORD1;	// <--float 3��¥������ ����. �ȼ� ���̴� ������ ó���� �� ���̹Ƿ� TEXCOORD �ø�ƽ�� �־����� ����
};

//vertex shader: vertex �ϳ��� �̷��� �����Ѵ�

//float4 pos�� �������α׷����� �Ѿ���� ����������
//POSITION�� �ø�ƽ
//������� ��ġ�� xyzw
//float4 main( float4 pos : POSITION, float4 color : COLOR) : SV_POSITION	//System Value_Position �ȼ����ؿ��� 2������ġ������_ XYZ
VS_OUTPUT main(VS_INPUT tIn)
{
	VS_OUTPUT tResult = (VS_OUTPUT)0;

	//����󿡼��� ��ġ ������
	float4 worldPosition;

	//IA�κ��� �Ѱܹ��� ������ ��ġ ������ 
	//����, ��, ������ȯ�� ����
	tResult.position = mul(tIn.position, World);	//mul ���̴� ���� �Լ�, ��İ� ���͸� �������ش�. ������ �������� �����Ѵ�

		//���庯ȯ���� ������ ������ ��ġ������
		worldPosition = tResult.position;

	tResult.position = mul(tResult.position, View);
	tResult.position = mul(tResult.position, Projection);

	tResult.normal = mul(float4(tIn.normal, 0), World).xyz;
	//���� ���̵� �ܰ�� �״�� ����
	tResult.texcoord = tIn.texcoord;

	//�ü����ͱ��ϱ�
		//������ ũ�⿡ ������ ������ ���� = �������� - ��������
		//�ü����� = ������ ��ġ������ - ī�޶���ġ������
		
		//����� ���Ǹ� ���� �����´� 
		//tResult.viewDir = worldPosition.xyz - cameraPosition.xyz;
		tResult.viewDir = cameraPosition.xyz - worldPosition.xyz;	//<--������ �ü�����
		tResult.viewDir = normalize(tResult.viewDir);		//����ȭ ����

	return tResult;
}