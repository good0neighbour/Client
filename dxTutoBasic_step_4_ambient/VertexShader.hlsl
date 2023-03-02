//시맨틱 : 랜더링 파이프라인상(shader측에서)에서 해당 변수가 어떠한 역할을 하고 있는지에 대한 태그(꼬리표)역할이다

//POSITION: 3디 공간상의 정점의 위치
//SV_POSITION: 근평면에 투영된 정점의 위치

//SV SystemValue
//SV_POSITION 위치값

//다음은 임의의 정점 데이터 하나의 처리에 관한 것이다

//상수버퍼 
cbuffer CBTransform
{
	matrix mWorld;            //월드변환 행렬		<-- 전역변수  //4byte*4*4 = 64byte
	matrix mView;              //뷰변환 행렬		<-- 전역변수		//4byte*4*4 = 64byte
	matrix mProjection;       //투영변환 행렬		<-- 전역변수	//4byte*4*4 = 64byte

	float4 mLightDir;			//빛방향벡터			<-- 전역변수	//4byte*4*4 = 64byte
	float4 mLightColor;		//빛 색상						<-- 전역변수	//4byte*4*4 = 64byte
};

//응용프로그램--> 정점셰이더
struct VS_INPUT
{
	float4 position:POSITION;
	float3 normal:NORMAL;
	float2 texcoord:TEXCOORD;
};

//정점셰이더-->픽셀셰이더
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

	//mul 셰이더에서 제공하는 셰이더함수. 행렬과 벡터의 곱셈
	//(hlsl에서는 열벡터를 기준으로 한다)
	tResult.position = mul(tIn.position, mWorld);						//월드변환 적용
	tResult.position = mul(tResult.position, mView);	//뷰변환 적용
	tResult.position = mul(tResult.position, mProjection);	//투영변환 적용

	//법선벡터에 월드변환 행렬 적용
	// <-- 로컬좌표계 상의 법선벡터의 값을 월드좌표계 상의 법선벡터의 값으로 변환
	tResult.normal = mul(float4(tIn.normal, 0), mWorld).xyz;


	//색상 데이터는 그냥 데이터 그대로 다음 단계로 넘김 
	//tResult.color = color;

	//그냥 데이터 그대로 다음 단계로 넘김 
	//tResult.position = pos;
	//tResult.color = color;

	//텍스처 좌표 정보는 그냥 다음 단계로 넘김
	tResult.texcoord = tIn.texcoord;

	return tResult;
}