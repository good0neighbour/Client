
//다음은 임의의 픽셀 데이터 하나의 처리에 관한 것이다

//SV_TARGET: TargetView에 만들어질 색상정보 데이터


//diffuse 확산, 난반사

Texture2D texDiffuse;						//텍스처 리소스 
SamplerState sampleStateLinear;		//샘플러 데이터



//상수버퍼 
cbuffer CBTransform
{
	matrix mWorld;            //월드변환 행렬		<-- 전역변수  //4byte*4*4 = 64byte
	matrix mView;              //뷰변환 행렬		<-- 전역변수		//4byte*4*4 = 64byte
	matrix mProjection;       //투영변환 행렬		<-- 전역변수	//4byte*4*4 = 64byte

	//난반사 관련 조명 정보 
	float4 mLightDir;			//빛방향벡터			<-- 전역변수	//4byte*4*4 = 64byte
	float4 mLightColor;		//빛 색상						<-- 전역변수	//4byte*4*4 = 64byte

	//주변광 관련 조명 정보
	float4 mAmbientColor;		//주변광 색상

	//정반사광 관련 조명 정보
	float4 mCameraPosition;       //카메라 위치 정보<-- 시선 벡터를 구하기 위해서 
	float4 mSpecularColor;     //정반사광 색상 
	float mSpecularPower;           //거듭제곱 수<-- 하일라이트 면적 결정 수치
	float3 padding; //그냥 끼워넣음<--상수버퍼는 16바이트 정렬 단위를 맞춰야 한다.
};


//정점셰이더-->픽셀셰이더
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