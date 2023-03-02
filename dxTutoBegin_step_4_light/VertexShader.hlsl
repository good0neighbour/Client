//�ø�ƽ : ������ ���������λ�(shader������)���� �ش� ������ ��� ������ �ϰ� �ִ����� ���� �±�(����ǥ)�����̴�

//POSITION: 3�� �������� ������ ��ġ
//SV_POSITION: ����鿡 ������ ������ ��ġ

//SV SystemValue
//SV_POSITION ��ġ��

//������ ������ ���� ������ �ϳ��� ó���� ���� ���̴�

//�������
cbuffer CBTransform
{
	matrix mWorld;		//���庯ȯ ���	<-- ��������
	matrix mView;		//�亯ȯ ���	<-- ��������
	matrix mProjection;	//������ȯ ���	<-- ��������
};

//�������α׷�--> �������̴�
struct VS_INPUT
{
	float4 position:POSITION;
	float3 normal:NORMAL;
};

//�������̴�-->�ȼ����̴�
struct VS_OUTPUT
{
	float4 position:SV_POSITION;
	//float4 color:COLOR;
	float3 normal:NORMAL;
};

//VS_OUTPUT main( float4 pos : POSITION, float4 color:COLOR )
VS_OUTPUT main( float4 tIn : POSITION, float3 normal : NORMAL )
{
	VS_OUTPUT tResult = (VS_OUTPUT)0;

	//mul ���̴����� �����ϴ� ���̴��Լ�. ��İ� ������ ����
	//(hlsl������ �����͸� �������� �Ѵ�)
	tResult.position = mul(tIn.position, mWorld);					//���庯ȯ ����
	tResult.position = mul(tResult.position, mView);		//�亯ȯ ����
	tResult.position = mul(tResult.position, mProjection);	//������ȯ ����

	//�������Ϳ� ���庯ȯ ��� ����
	tResult.normal = mul(float4(tIn.normal, 0), World).xyz;

	//���� �����ʹ� �׳� ������ �״�� ���� �ܰ�� �ѱ�
	//tResult.color = color;

	//�׳� ������ �״�� ���� �ܰ�� �ѱ�
	/*tResult.position = pos;
	tResult.color = color;*/

	return tResult;
}