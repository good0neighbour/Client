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

cbuffer CBTransform: register(b0)
{
	matrix mWorld;
	matrix mView;
	matrix mProjection;
};

//�������α׷�--> �������̴�
struct VS_INPUT
{
	float4 position:POSITION;	
	float2 texcoord:TEXCOORD0;
};

//�������̴�-->�ȼ����̴�
struct VS_OUTPUT
{
	float4 position:SV_POSITION;	
	float2 texcoord:TEXCOORD0;
};

//VS_OUTPUT main( float4 pos : POSITION, float4 color:COLOR ) 				
VS_OUTPUT main(VS_INPUT tIn)
{
	VS_OUTPUT tResult = (VS_OUTPUT)0;
	
	tResult.position = mul(tIn.position, mWorld);						//���庯ȯ ����
	tResult.position = mul(tResult.position, mView);	//�亯ȯ ����
	tResult.position = mul(tResult.position, mProjection);	//������ȯ ����
	
	tResult.texcoord = tIn.texcoord;


	return tResult;
}