
//��������
Texture2D texDiffuse : register(t0);
SamplerState samplerStateLinear : register(s0);

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

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normal: NORMAL;//����������
	float2 texcoord : TEXCOORD0;

	//specular
	//�ü�����
	float3 viewDir:TEXCOORD1;	// <--float 3��¥������ ����. �ȼ� ���̴� ������ ó���� �� ���̹Ƿ� TEXCOORD �ø�ƽ�� �־����� ����
};

//pixel shader: pixel �ϳ��� �̷��� �����Ѵ�
// 
//������� �ȼ��� ���� ������ rgba
float4 main(VS_OUTPUT tIn) : SV_TARGET		//System Value_Target 2���� �̹��� ������ ���� _ RGBA
{
	//vertex shading �ܰ�κ��� ���޹��� ������ ���������� ���̵��ߴ�.
	//return tIn.color;

	//ambient
	float4 tResult = 0;

	float3 reflectDir = 0;			//�ݻ纤��
	float4 specular = 0;			//�������� ���ݻ籤 ����



	//���� ������ ����� ����ϱ� ���� ���������� �����Ͽ���.
	float4 tLightDir;
	tLightDir = -1.0f * lightDir;	//����: �ϳ�¥���� ��� �ڸ����� �����ϴ�

	//dot���� ������ ������ ������ [-1,1]
	//saturate���� ������ ������ [0,1]
	//tResult = saturate(dot(lightDir, tIn.normal))* lightColor;//����: �ϳ�¥���� ��� �ڸ����� �����ϴ�
	float4 diffuseColor = saturate(dot(tLightDir, tIn.normal)) * lightColor;//����: �ϳ�¥���� ��� �ڸ����� �����ϴ�
	diffuseColor.a = 1.0f;		//���İ��� 1�� ����

	//�ݻ纤�͸� ���Ѵ�
	reflectDir = reflect(lightDir, tIn.normal);
	//specular�� ����
	specular = pow(saturate(dot(reflectDir, tIn.viewDir)), specularPower);		//�ŵ����� 
	//specular ������� ����
	specular = specular * specularColor;



	//�ؽ�ó�� uv��ǥ�踦 ������� ���ø�(���͸� ó���� �ϸ鼭)�Ͽ� ������ ������ �ϳ� ���´�
	float4 texColor = texDiffuse.Sample(samplerStateLinear, tIn.texcoord);

	//���� ���� ����
	//�������� = �ֺ��� + ���ݻ����*�ؽ�ó����
	tResult = ambientColor + diffuseColor * texColor;		//ambient
	//saturate���� ������ ������ [0,1]
	tResult = saturate(tResult);

	//Highlight �ǹǷ� ���󿬻���ϱ�
	tResult = saturate(tResult + specular);

	return tResult;
}