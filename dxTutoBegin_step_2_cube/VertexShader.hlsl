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

struct VS_OUTPUT
{
	float4 position:SV_POSITION;
	float4 color:COLOR;
};

VS_OUTPUT main( float4 pos : POSITION, float4 color:COLOR )
{
	VS_OUTPUT tResult = (VS_OUTPUT)0;

	//mul ���̴����� �����ϴ� ���̴��Լ�. ��İ� ������ ����
	//(hlsl������ �����͸� �������� �Ѵ�)
	tResult.position = mul(pos, mWorld);					//���庯ȯ ����
	tResult.position = mul(tResult.position, mView);		//�亯ȯ ����
	tResult.position = mul(tResult.position, mProjection);	//������ȯ ����

	//���� �����ʹ� �׳� ������ �״�� ���� �ܰ�� �ѱ�
	tResult.color = color;

	//�׳� ������ �״�� ���� �ܰ�� �ѱ�
	/*tResult.position = pos;
	tResult.color = color;*/

	return tResult;
}