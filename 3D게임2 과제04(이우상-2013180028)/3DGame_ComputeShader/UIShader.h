#pragma once
#include "TexturedShader.h"
#include "UI.h"

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

	bool Initialize(int key);
	void DeleteUI(int key);
	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseObjects();
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, int key);

	CUI* GetFirstPersonUI()	const { return m_pGameUI; }
	void SetFirstPersonUI(CUI* pGameUI) { m_pGameUI = pGameUI; }
private:
	int										m_nObjects = 0;

	CTexturedRectMesh*		m_pUIMesh{ nullptr };

	const static int m_UICount = 5;
	CMaterial*						m_pGameUIMaterial{ nullptr };

	CUI*									m_pGameUI{ nullptr };

	CTexture*							m_pFirstPersonUITexture{ nullptr };
	CTexture*							m_pGameStartUITexture{ nullptr };
	CTexture*							m_pGameMenuUITexture{ nullptr };
	CTexture*							m_pGameHelpUITexture{ nullptr };
	CTexture*							m_pGameOverUITexture{ nullptr };

	list<CGameObject*>		m_UIList;
	// map<Å°, º§·ù>
	map<int, CUI*> m_UIMap;
};

