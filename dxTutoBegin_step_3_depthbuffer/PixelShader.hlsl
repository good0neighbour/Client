
//������ ������ �ȼ� ������ �ϳ��� ó���� ���� ���̴�


//SV_TARGET: TargetView�� ������� �������� ������

struct VS_OUTPUT
{
	float4 position:SV_POSITION;
	float4 color:COLOR;
};

float4 main(VS_OUTPUT tIn) : SV_TARGET
{
	//�� �ܰ迡�� ���� ���� ������ �״�� �Ѱ��.
	return tIn.color;
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
}