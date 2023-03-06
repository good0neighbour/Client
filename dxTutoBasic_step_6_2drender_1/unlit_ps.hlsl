
//������ ������ �ȼ� ������ �ϳ��� ó���� ���� ���̴�

//SV_TARGET: TargetView�� ������� �������� ������


//diffuse Ȯ��, ���ݻ�

Texture2D texDiffuse;						//�ؽ�ó ���ҽ� 
SamplerState sampleStateLinear;		//���÷� ������



//������� 
cbuffer CBTransform
{
	matrix mWorld;            //���庯ȯ ���		<-- ��������  //4byte*4*4 = 64byte
	matrix mView;              //�亯ȯ ���		<-- ��������		//4byte*4*4 = 64byte
	matrix mProjection;       //������ȯ ���		<-- ��������	//4byte*4*4 = 64byte

	//���ݻ� ���� ���� ���� 
	float4 mLightDir;			//�����⺤��			<-- ��������	//4byte*4*4 = 64byte
	float4 mLightColor;		//�� ����						<-- ��������	//4byte*4*4 = 64byte

	//�ֺ��� ���� ���� ����
	float4 mAmbientColor;		//�ֺ��� ����

	//���ݻ籤 ���� ���� ����
	float4 mCameraPosition;       //ī�޶� ��ġ ����<-- �ü� ���͸� ���ϱ� ���ؼ� 
	float4 mSpecularColor;     //���ݻ籤 ���� 
	float mSpecularPower;           //�ŵ����� ��<-- ���϶���Ʈ ���� ���� ��ġ
	float3 padding; //�׳� ��������<--������۴� 16����Ʈ ���� ������ ����� �Ѵ�.
};


//�������̴�-->�ȼ����̴�
struct VS_OUTPUT
{
	float4 position:SV_POSITION;
	float2 texcoord:TEXCOORD0;
};


float4 main(VS_OUTPUT tIn) : SV_TARGET
{
	float4 tResult = 0.0f;

	tResult = texDiffuse.Sample(sampleStateLinear, tIn.texcoord);
	tResult.a = 1.0f;

	return tResult;
}