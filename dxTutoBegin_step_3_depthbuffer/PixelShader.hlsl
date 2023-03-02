
//다음은 임의의 픽셀 데이터 하나의 처리에 관한 것이다


//SV_TARGET: TargetView에 만들어질 색상정보 데이터

struct VS_OUTPUT
{
	float4 position:SV_POSITION;
	float4 color:COLOR;
};

float4 main(VS_OUTPUT tIn) : SV_TARGET
{
	//앞 단계에서 받은 색상 정보를 그대로 넘겼다.
	return tIn.color;
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
}