#pragma once

#include "framework.h"

class CDXEngine;

class CMtlUnlit
{
public:
	CMtlUnlit()
	{

	}
	//���縦 ���´�
	CMtlUnlit(const CMtlUnlit& t) = delete;
	void operator=(const CMtlUnlit& t) = delete;

public:
	void Create(ID3D11Device* tpDevice);
	void Destroy();
	void Render(int tCountIndex, ID3D11ShaderResourceView* tpTexture);

	void SetEngine(CDXEngine* tpEngine)
	{
		mpEngine = tpEngine;
	}

private:
	void CreateShader(ID3D11Device* tpDevice);
	void DestroyShader();

	void SetShaderParameter(ID3D11DeviceContext* tpDeviceContext,
		ID3D11ShaderResourceView* tpTexture);
	void RenderShader(ID3D11DeviceContext* tpDeviceContext, int tCountIndex);

	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

private:

	ID3D11VertexShader* mpVertexShader = nullptr;
	ID3D11PixelShader* mpPixelShader = nullptr;

	ID3D11InputLayout* mpVertexLayout = nullptr;

	ID3D11SamplerState* mpSamplerStateLinear = nullptr;
	//���� ���� ����
	CDXEngine* mpEngine = nullptr;
};

