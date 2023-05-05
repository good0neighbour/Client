
//������ ������ �ȼ� ������ �ϳ��� ó���� ���� ���̴�

//SV_TARGET: TargetView�� ������� �������� ������


//diffuse Ȯ��, ���ݻ�

Texture2D texDiffuse:register(t0);						//�ؽ�ó ���ҽ� 
SamplerState sampleStateLinear:register(s0);		//���÷� ������



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


//cbuffer CBTransform: register(b0)
//{
//	matrix mWorld;
//	matrix mView;
//	matrix mProjection;
//};

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

	//diffuse�� ���ϱ� ���ؼ��� ���ݻ縦 ������ ���������
	//specular�� ���ϱ� ���ؼ��� ���ݻ縦 �������� �ȵȴ�.


	//���ݻ籤 ����
	float4 tSpecularColor = 0.0f;

	float3 reflectDir = 0.0f;		//�ݻ� ���� 
	//�ݻ纤�͸� ���Ѵ�
	reflectDir = reflect(-1.0f*tLightDir, tIn.normal);	//�����ʹ� �ٽ� �ѹ� ������ ������� ����

	//[-1, 1] --> [0,1]
	//�ŵ����� pow(x, y)
	tSpecularColor = pow(saturate(dot(reflectDir, tIn.viewDir)), mSpecularPower);

	//�������α׷����� �Է¹��� ���󰪵� ����( ���� ���� ���� )
	tSpecularColor = tSpecularColor * mSpecularColor;


	

	//�� �ܰ迡�� ���� ���� ������ �״�� �Ѱ��.
	//return tIn.color;

	float4 tTexColor = 0.0f;
	//�ؽ�ó ��ǥ(uv��ǥ) ������ ������� , ���ø� �����͸� �����Ͽ� �ؼ��� ���ø��Ͽ� �������� ����
	tTexColor = texDiffuse.Sample(sampleStateLinear, tIn.texcoord);

	////���� ���� ����<-- ������ �ڸ������� ���Ѵ�
	////�������� = ���ݻ籤*�ؽ�ó����
	//tResult = tDiffuse * tTexColor;
	//tResult.a = 1.0f;		//���İ��� 1�� ����


	//���� ���� ����<-- ������ �ڸ������� ���Ѵ�
	//���� ���� ����<-- ������ �ڸ������� ���Ѵ�
	//�������� = ���ݻ籤*�ؽ�ó���� + �ֺ���
	tResult = tDiffuse * tTexColor + mAmbientColor;		
	//���ݻ籤 ���� ����
	//�������� = (���ݻ籤*�ؽ�ó���� + �ֺ���) + ���ݻ籤
	tResult = tResult + tSpecularColor;

	//saturate [0,1] ������ ������ �����.
	tResult = saturate(tResult);

	tResult.a = 1.0f;		//���İ��� 1�� ����

	return tResult;
}