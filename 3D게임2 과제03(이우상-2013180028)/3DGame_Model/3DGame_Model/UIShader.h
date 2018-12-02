#pragma once
#include "TexturedShader.h"
#include "FirstPersonUI.h"

class CUIShader : public CTexturedShader
{
public:
	CUIShader();
	virtual ~CUIShader();

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void ReleaseShaderVariables();
	virtual void ReleaseUploadBuffers();

	bool Initialize();
	void DeleteUI();
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void ReleaseObjects();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

	CFirstPersonUI* GetFirstPersonUI()	const { return m_pFirstPersonUI; }
	void SetFirstPersonUI(CFirstPersonUI* pFirstPersonUI) { m_pFirstPersonUI = pFirstPersonUI; }
private:
	CTexturedRectMesh*		m_pFirstPersonUIMesh{ nullptr };
	CTexture*							m_pFirstPersonUITexture{ nullptr };
	CMaterial*						m_pFirstPersonUIMaterial{ nullptr };
	CFirstPersonUI*				m_pFirstPersonUI{ nullptr };
	
	int										m_nObjects = 0;

	list<CGameObject*>		m_FirstPersonUIList;
};

