#pragma once
#include "TexturedShader.h"
#include "UI.h"

class CMissileUIShader : public CTexturedShader
{
public:
	CMissileUIShader();
	virtual ~CMissileUIShader();

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

private:
	int										m_nObjects = 0;

	CTexturedRectMesh*		m_pUIMesh{ nullptr };

	const static int m_UICount = 1;
	CMaterial*						m_pMissileUIMaterial{ nullptr };

	CUI*									m_pMissileUI{ nullptr };

	CTexture*							m_pMissileUITexture{ nullptr };

	// map<Å°, º§·ù>
	map<int, CUI*> m_UIMap;
};

