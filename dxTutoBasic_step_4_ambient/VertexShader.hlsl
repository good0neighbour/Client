//�ø�ƽ : ������ ���������λ�(shader������)���� �ش� ������ ��� ������ �ϰ� �ִ����� ���� �±�(����ǥ)�����̴�

//POSITION: 3�� �������� ������ ��ġ
//SV_POSITION: ����鿡 ������ ������ ��ġ

//SV SystemValue
//SV_POSITION ��ġ��

//������ ������ ���� ������ �ϳ��� ó���� ���� ���̴�

//������� 
cbuffer CBTransform
{
	matrix mWorld;            //���庯ȯ ���		<-- ��������  //4byte*4*4 = 64byte
	matrix mView;              //�亯ȯ ���		<-- ��������		//4byte*4*4 = 64byte
	matrix mProjection;       //������ȯ ���		<-- ��������	//4byte*4*4 = 64byte

	float4 mLightDir;			//�����⺤��			<-- ��������	//4byte*4*4 = 64byte
	float4 mLightColor;		//�� ����						<-- ��������	//4byte*4*4 = 64byte
};

//�������α׷�--> �������̴�
struct VS_INPUT
{
	float4 position:POSITION;
	float3 normal:NORMAL;
	float2 texcoord:TEXCOORD;
};

//�������̴�-->�ȼ����̴�
struct VS_OUTPUT
{
	float4 position:SV_POSITION;
	//float4 color:COLOR;
	float3 normal:NORMAL;
	float2 texcoord:TEXCOORD;
};

//VS_OUTPUT main( float4 pos : POSITION, float4 color:COLOR ) 				
VS_OUTPUT main(VS_INPUT tIn)
{
	VS_OUTPUT tResult = (VS_OUTPUT)0;

	//mul ���̴����� �����ϴ� ���̴��Լ�. ��İ� ������ ����
	//(hlsl������ �����͸� �������� �Ѵ�)
	tResult.position = mul(tIn.position, mWorld);						//���庯ȯ ����
	tResult.position = mul(tResult.position, mView);	//�亯ȯ ����
	tResult.position = mul(tResult.position, mProjection);	//������ȯ ����

	//�������Ϳ� ���庯ȯ ��� ����
	// <-- ������ǥ�� ���� ���������� ���� ������ǥ�� ���� ���������� ������ ��ȯ
	tResult.normal = mul(float4(tIn.normal, 0), mWorld).xyz;


	//���� �����ʹ� �׳� ������ �״�� ���� �ܰ�� �ѱ� 
	//tResult.color = color;

	//�׳� ������ �״�� ���� �ܰ�� �ѱ� 
	//tResult.position = pos;
	//tResult.color = color;

	//�ؽ�ó ��ǥ ������ �׳� ���� �ܰ�� �ѱ�
	tResult.texcoord = tIn.texcoord;

	return tResult;
}