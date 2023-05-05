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

cbuffer CBTransform: register(b0)
{
	matrix mWorld;
	matrix mView;
	matrix mProjection;
};

//응용프로그램--> 정점셰이더
struct VS_INPUT
{
	float4 position:POSITION;	
	float2 texcoord:TEXCOORD0;
};

//정점셰이더-->픽셀셰이더
struct VS_OUTPUT
{
	float4 position:SV_POSITION;	
	float2 texcoord:TEXCOORD0;
};

//VS_OUTPUT main( float4 pos : POSITION, float4 color:COLOR ) 				
VS_OUTPUT main(VS_INPUT tIn)
{
	VS_OUTPUT tResult = (VS_OUTPUT)0;
	
	tResult.position = mul(tIn.position, mWorld);						//월드변환 적용
	tResult.position = mul(tResult.position, mView);	//뷰변환 적용
	tResult.position = mul(tResult.position, mProjection);	//투영변환 적용
	
	tResult.texcoord = tIn.texcoord;


	return tResult;
}