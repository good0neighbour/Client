//시맨틱 : 랜더링 파이프라인상(shader측에서)에서 해당 변수가 어떠한 역할을 하고 있는지에 대한 태그(꼬리표)역할이다

//POSITION: 3디 공간상의 정점의 위치
//SV_POSITION: 근평면에 투영된 정점의 위치

//SV SystemValue
//SV_POSITION 위치값

//다음은 임의의 정점 데이터 하나의 처리에 관한 것이다

//상수버퍼 
//cbuffer CBTransform
//{
//	matrix mWorld;            //월드변환 행렬		<-- 전역변수  //4byte*4*4 = 64byte
//	matrix mView;              //뷰변환 행렬		<-- 전역변수		//4byte*4*4 = 64byte
//	matrix mProjection;       //투영변환 행렬		<-- 전역변수	//4byte*4*4 = 64byte
//
//	//난반사 관련 조명 정보 
//	float4 mLightDir;			//빛방향벡터			<-- 전역변수	//4byte*4*4 = 64byte
//	float4 mLightColor;		//빛 색상						<-- 전역변수	//4byte*4*4 = 64byte
//
//	//주변광 관련 조명 정보
//	float4 mAmbientColor;		//주변광 색상
//
//	//정반사광 관련 조명 정보
//	float4 mCameraPosition;       //카메라 위치 정보<-- 시선 벡터를 구하기 위해서 
//	float4 mSpecularColor;     //정반사광 색상 
//	float mSpecularPower;           //거듭제곱 수<-- 하일라이트 면적 결정 수치
//	float3 padding; //그냥 끼워넣음<--상수버퍼는 16바이트 정렬 단위를 맞춰야 한다.
//};


//버퍼의 슬롯 번호 부여
//	register( 접두사 슬롯번호 )로 부여할 수 있다

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

//응용프로그램--> 정점셰이더
struct VS_INPUT
{
	float4 position:POSITION;
	float3 normal:NORMAL;
	float2 texcoord:TEXCOORD0;
};

//정점셰이더-->픽셀셰이더
struct VS_OUTPUT
{
	float4 position:SV_POSITION;
	//float4 color:COLOR;
	float3 normal:NORMAL;
	float2 texcoord:TEXCOORD0;

	//시선벡터 
	float3 viewDir:TEXCOORD1;
};

//VS_OUTPUT main( float4 pos : POSITION, float4 color:COLOR ) 				
VS_OUTPUT main(VS_INPUT tIn)
{
	VS_OUTPUT tResult = (VS_OUTPUT)0;

	float4 worldPosition;

	//mul 셰이더에서 제공하는 셰이더함수. 행렬과 벡터의 곱셈
	//(hlsl에서는 열벡터를 기준으로 한다)
	tResult.position = mul(tIn.position, mWorld);						//월드변환 적용

			//지역변수에 월드 상에 물체의 위치를 기억시켜둔다.
			worldPosition = tResult.position;

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



	//시선벡터를 구하자
	//임의의 크기에 임의의 방향의 벡터 = 목적지점 - 시작지점 
	//tResult.viewDir = worldPosition - mCameraPosition;
	tResult.viewDir = mCameraPosition - worldPosition;		//시선벡터는 뒤집어서 준비한다.
	tResult.viewDir = normalize(tResult.viewDir);		//크기가 1인 벡터로 정규화한다.



	return tResult;
}