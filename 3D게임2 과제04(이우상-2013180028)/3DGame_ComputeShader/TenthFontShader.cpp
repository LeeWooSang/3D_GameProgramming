#include "Stdafx.h"
#include "TenthFontShader.h"

CTenthFontShader::CTenthFontShader()
{
}

CTenthFontShader::~CTenthFontShader()
{
}

void CTenthFontShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	CShader::CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
}

D3D12_DEPTH_STENCIL_DESC CTenthFontShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	d3dDepthStencilDesc.DepthEnable = FALSE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0xff;
	d3dDepthStencilDesc.StencilWriteMask = 0xff;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_INCR;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_DECR;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	return(d3dDepthStencilDesc);
}

D3D12_RASTERIZER_DESC CTenthFontShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	//d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);
}

D3D12_BLEND_DESC CTenthFontShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

D3D12_SHADER_BYTECODE CTenthFontShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSTenthFont", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTenthFontShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSFont", "ps_5_1", ppd3dShaderBlob));
}

void CTenthFontShader::ReleaseShaderVariables()
{
	CTexturedShader::ReleaseShaderVariables();
}

void CTenthFontShader::ReleaseUploadBuffers()
{
	for(auto iter = m_FontMap.begin(); iter != m_FontMap.end(); ++iter)
		(*iter).second->ReleaseUploadBuffers();
}

bool CTenthFontShader::Initialize(int key)
{
	m_num = key;
	m_pFont = new CFont;
	m_pFont->SetMesh(m_pFontMesh);
	m_pFont->SetMaterial(&m_pFontMaterial[key]);
	m_FontMap.emplace(key, m_pFont);
	cout << key << " 폰트 생성" << endl;
	return true;
}

void CTenthFontShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	if (!m_pFontMesh)
		m_pFontMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 20.f, 20.f, 0.0f, 0.0f, 0.0f, 0.0f);

	m_pNumber0Texture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pNumber0Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"../Textures/Font/Red_0.dds", 0);

	m_pNumber1Texture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pNumber1Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"../Textures/Font/Red_1.dds", 0);

	m_pNumber2Texture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pNumber2Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"../Textures/Font/Red_2.dds", 0);

	m_pNumber3Texture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pNumber3Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"../Textures/Font/Red_3.dds", 0);

	m_pNumber4Texture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pNumber4Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"../Textures/Font/Red_4.dds", 0);

	m_pNumber5Texture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pNumber5Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"../Textures/Font/Red_5.dds", 0);

	m_pNumber6Texture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pNumber6Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"../Textures/Font/Red_6.dds", 0);

	m_pNumber7Texture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pNumber7Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"../Textures/Font/Red_7.dds", 0);

	m_pNumber8Texture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pNumber8Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"../Textures/Font/Red_8.dds", 0);

	m_pNumber9Texture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pNumber9Texture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"../Textures/Font/Red_9.dds", 0);
	
	// FONT 재질 10개 생성
	m_pFontMaterial = new CMaterial[m_FontCount];
	m_pFontMaterial[ZERO].SetTexture(m_pNumber0Texture);
	m_pFontMaterial[ONE].SetTexture(m_pNumber1Texture);
	m_pFontMaterial[TWO].SetTexture(m_pNumber2Texture);
	m_pFontMaterial[THREE].SetTexture(m_pNumber3Texture);
	m_pFontMaterial[FOUR].SetTexture(m_pNumber4Texture);
	m_pFontMaterial[FIVE].SetTexture(m_pNumber5Texture);
	m_pFontMaterial[SIX].SetTexture(m_pNumber6Texture);
	m_pFontMaterial[SEVEN].SetTexture(m_pNumber7Texture);
	m_pFontMaterial[EIGHT].SetTexture(m_pNumber8Texture);
	m_pFontMaterial[NINE].SetTexture(m_pNumber9Texture);

	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_FontCount, m_FontCount);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pNumber0Texture, 3, false);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pNumber1Texture, 3, false);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pNumber2Texture, 3, false);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pNumber3Texture, 3, false);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pNumber4Texture, 3, false);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pNumber5Texture, 3, false);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pNumber6Texture, 3, false);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pNumber7Texture, 3, false);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pNumber8Texture, 3, false);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pNumber9Texture, 3, false);
}

void CTenthFontShader::ReleaseObjects()
{
	m_pFontMesh->Release();
	DeleteUI(-1);
}

void CTenthFontShader::DeleteUI(int key)
{
	if (key != -1)
	{
		auto iter = m_FontMap.find(key);
		if (iter != m_FontMap.end())
		{
			delete (*iter).second;
			iter = m_FontMap.erase(iter);
			cout << "FONT 삭제" << endl;
		}
	}
	else
	{
		for (auto iter = m_FontMap.begin(); iter != m_FontMap.end();)
		{
			delete (*iter).second;
			iter = m_FontMap.erase(iter);
		}
		m_FontMap.clear();
	}
}
void CTenthFontShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, int key)
{
	auto iter = m_FontMap.find(m_num);
	if (iter != m_FontMap.end())
	{
		CTexturedShader::Render(pd3dCommandList, pCamera);
		(*iter).second->Render(pd3dCommandList, pCamera);
	}
}
