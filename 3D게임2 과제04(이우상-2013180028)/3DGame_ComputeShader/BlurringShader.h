#pragma once
#include "Shader.h"

class CBlurringShader : public CShader
{
public:
	CBlurringShader();
	virtual ~CBlurringShader();

	D3D12_SHADER_BYTECODE CreateHorzBlurShader(ID3DBlob **ppd3dShaderBlob);
	D3D12_SHADER_BYTECODE CreateVertBlurShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature, UINT nRenderTargets = 1);
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void ReleaseObjects();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, UINT num = 0, ID3D12Resource* input = nullptr);

	void SetOnBlurring(bool OnBlurring) { m_OnBlurring = OnBlurring; }
	bool GetOnBlurring()	const { return m_OnBlurring; }
public:
	CTexture* m_pTexture = NULL;
	bool			m_OnBlurring = false;
};
