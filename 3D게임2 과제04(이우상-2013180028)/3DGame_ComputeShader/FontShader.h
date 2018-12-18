#pragma once
#include "TexturedShader.h"
#include "Font.h"

class CFontShader : public CTexturedShader
{
public:
	CFontShader();
	virtual ~CFontShader();

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
	void SetNumber(int num) { m_num = num; }
private:
	CTexturedRectMesh*		m_pFontMesh{ nullptr };

	const static int					m_FontCount = 10;
	CMaterial*						m_pFontMaterial{ nullptr };

	CFont*								m_pFont{ nullptr };

	CTexture*							m_pNumber0Texture{ nullptr };
	CTexture*							m_pNumber1Texture{ nullptr };
	CTexture*							m_pNumber2Texture{ nullptr };
	CTexture*							m_pNumber3Texture{ nullptr };
	CTexture*							m_pNumber4Texture{ nullptr };
	CTexture*							m_pNumber5Texture{ nullptr };
	CTexture*							m_pNumber6Texture{ nullptr };
	CTexture*							m_pNumber7Texture{ nullptr };
	CTexture*							m_pNumber8Texture{ nullptr };
	CTexture*							m_pNumber9Texture{ nullptr };

	// map<Å°, º§·ù>
	map<int, CFont*>			m_FontMap;
	int m_num = 0;
};

