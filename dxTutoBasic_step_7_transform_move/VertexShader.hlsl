//�ø�ƽ : ������ ���������λ�(shader������)���� �ش� ������ ��� ������ �ϰ� �ִ����� ���� �±�(����ǥ)�����̴�

//POSITION: 3�� �������� ������ ��ġ
//SV_POSITION: ����鿡 ������ ������ ��ġ

//SV SystemValue
//SV_POSITION ��ġ��

//������ ������ ���� ������ �ϳ��� ó���� ���� ���̴�

//������� 
//cbuffer CBTransform
//{
//	matrix mWorld;            //���庯ȯ ���		<-- ��������  //4byte*4*4 = 64byte
//	matrix mView;              //�亯ȯ ���		<-- ��������		//4byte*4*4 = 64byte
//	matrix mProjection;       //������ȯ ���		<-- ��������	//4byte*4*4 = 64byte
//
//	//���ݻ� ���� ���� ���� 
//	float4 mLightDir;			//�����⺤��			<-- ��������	//4byte*4*4 = 64byte
//	float4 mLightColor;		//�� ����						<-- ��������	//4byte*4*4 = 64byte
//
//	//�ֺ��� ���� ���� ����
//	float4 mAmbientColor;		//�ֺ��� ����
//
//	//���ݻ籤 ���� ���� ����
//	float4 mCameraPosition;       //ī�޶� ��ġ ����<-- �ü� ���͸� ���ϱ� ���ؼ� 
//	float4 mSpecularColor;     //���ݻ籤 ���� 
//	float mSpecularPower;           //�ŵ����� ��<-- ���϶���Ʈ ���� ���� ��ġ
//	float3 padding; //�׳� ��������<--������۴� 16����Ʈ ���� ������ ����� �Ѵ�.
//};


//������ ���� ��ȣ �ο�
//	register( ���λ� ���Թ�ȣ )�� �ο��� �� �ִ�

cbuffer CBTransform: register(b0)
{
	matrix mWorld;
	matrix mView;
	matrix mProjection;
};

cbuffer CBLight: register(b1)
{
	float4 mLightDir;
	float4 mLightColor;
	
	float4 mAmbientColor;
	
	float4 mCameraPosition;
	float4 mSpecularColor;
	float mSpecularPower;
	float3 padding;
};

//�������α׷�--> �������̴�
struct VS_INPUT
{
	float4 position:POSITION;
	float3 normal:NORMAL;
	float2 texcoord:TEXCOORD0;
};

//�������̴�-->�ȼ����̴�
struct VS_OUTPUT
{
	float4 position:SV_POSITION;
	//float4 color:COLOR;
	float3 normal:NORMAL;
	float2 texcoord:TEXCOORD0;

	//�ü����� 
	float3 viewDir:TEXCOORD1;
};

//VS_OUTPUT main( float4 pos : POSITION, float4 color:COLOR ) 				
VS_OUTPUT main(VS_INPUT tIn)
{
	VS_OUTPUT tResult = (VS_OUTPUT)0;

	float4 worldPosition;

	//mul ���̴����� �����ϴ� ���̴��Լ�. ��İ� ������ ����
	//(hlsl������ �����͸� �������� �Ѵ�)
	tResult.position = mul(tIn.position, mWorld);						//���庯ȯ ����

			//���������� ���� �� ��ü�� ��ġ�� �����ѵд�.
			worldPosition = tResult.position;

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



	//�ü����͸� ������
	//������ ũ�⿡ ������ ������ ���� = �������� - �������� 
	//tResult.viewDir = worldPosition - mCameraPosition;
	tResult.viewDir = mCameraPosition - worldPosition;		//�ü����ʹ� ����� �غ��Ѵ�.
	tResult.viewDir = normalize(tResult.viewDir);		//ũ�Ⱑ 1�� ���ͷ� ����ȭ�Ѵ�.



	return tResult;
}