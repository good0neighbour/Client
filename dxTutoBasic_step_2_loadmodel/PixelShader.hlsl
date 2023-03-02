
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

	float4 mLightDir;			//�����⺤��			<-- ��������	//4byte*4*4 = 64byte
	float4 mLightColor;		//�� ����						<-- ��������	//4byte*4*4 = 64byte
};


//�������̴�-->�ȼ����̴�
struct VS_OUTPUT
{
	float4 position:SV_POSITION;
	//float4 color:COLOR;
	float3 normal:NORMAL;
	float2 texcoord:TEXCOORD;
};


float4 main(VS_OUTPUT tIn) : SV_TARGET
{
	//-���󿬻� ��-
	//���󿬻��� ���� �ڸ��������� �����ϴ�.( �ڸ����� �����ؾ� ���󿬻��� �����ϴ�.)
	//tResult = float4(0, 0, 0, 1) + float2(1, 0);
	//-->��, ���󿬻꿡�� ���ڸ����� ��� �ڸ����� ������ �����ϴ�

	//�ȼ� ���̴����� ���󿬻�
	//��, ���󿬻꿡�� ���ڸ����� ��� �ڸ����� ������ �����ϴ�
	//float4 tResult = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 tResult = 0.0f;

	//Ȯ�걤(���ݻ籤)
	float4 tDiffuse = 0.0f;

	float4 tLightDir;		//�� ����( ����� �غ��Ѵ� )
	tLightDir = -1.0f * mLightDir;

	//����Ʈ �����: ������ dot ��������
	//saturate�Լ��� �̿��Ͽ�  [-1,1] ---> [0,1] ������ ��Į������ ����
	//��Į��(���ڸ���)*������(4�ڸ���) = ������ ����� ����(4�ڸ���)
	tDiffuse = saturate(dot(tLightDir, tIn.normal))* mLightColor;	
	

	//�� �ܰ迡�� ���� ���� ������ �״�� �Ѱ��.
	//return tIn.color;

	float4 tTexColor = 0.0f;
	//�ؽ�ó ��ǥ(uv��ǥ) ������ ������� , ���ø� �����͸� �����Ͽ� �ؼ��� ���ø��Ͽ� �������� ����
	tTexColor = texDiffuse.Sample(sampleStateLinear, tIn.texcoord);

	//���� ���� ����<-- ������ �ڸ������� ���Ѵ�
	//�������� = ���ݻ籤*�ؽ�ó����
	tResult = tDiffuse * tTexColor;
	tResult.a = 1.0f;		//���İ��� 1�� ����

	return tResult;
}