#include "Stdafx.h"
#include "MissileUIShader.h"

CMissileUIShader::CMissileUIShader()
{
}

CMissileUIShader::~CMissileUIShader()
{
}

void CMissileUIShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	CShader::CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
}

D3D12_DEPTH_STENCIL_DESC CMissileUIShader::CreateDepthStencilState()
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

D3D12_RASTERIZER_DESC CMissileUIShader::CreateRasterizerState()
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

D3D12_BLEND_DESC CMissileUIShader::CreateBlendState()
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

D3D12_SHADER_BYTECODE CMissileUIShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSMissileUI", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CMissileUIShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSUI", "ps_5_1", ppd3dShaderBlob));
}

void CMissileUIShader::ReleaseShaderVariables()
{
	CTexturedShader::ReleaseShaderVariables();
}

void CMissileUIShader::ReleaseUploadBuffers()
{
	m_pMissileUI->ReleaseUploadBuffers();
}

bool CMissileUIShader::Initialize(int key)
{
	m_pMissileUI = new CUI;
	m_pMissileUI->SetMesh(m_pUIMesh);
	m_pMissileUI->SetMaterial(&m_pMissileUIMaterial[key]);
	m_UIMap.emplace(key, m_pMissileUI);

	return true;
}

void CMissileUIShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	if (!m_pUIMesh)
		m_pUIMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 20.f, 20.f, 0.0f, 0.0f, 0.0f, 0.0f);

	m_pMissileUITexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pMissileUITexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"../Textures/GameUI/Missile.dds", 0);

	// MissileUI 재질 1개 생성
	m_pMissileUIMaterial = new CMaterial[m_UICount];
	m_pMissileUIMaterial[0].SetTexture(m_pMissileUITexture);

	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_UICount, m_UICount);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pMissileUITexture, 3, false);
}

void CMissileUIShader::ReleaseObjects()
{
	m_pUIMesh->Release();
	DeleteUI(-1);
}

void CMissileUIShader::DeleteUI(int key)
{
	if (key != -1)
	{
		auto iter = m_UIMap.find(key);
		if (iter != m_UIMap.end())
		{
			delete (*iter).second;
			iter = m_UIMap.erase(iter);
			cout << "UI 삭제" << endl;
		}
	}
	else
	{
		for (auto iter = m_UIMap.begin(); iter != m_UIMap.end();)
		{
			delete (*iter).second;
			iter = m_UIMap.erase(iter);
		}
		m_UIMap.clear();
	}
}
void CMissileUIShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, int key)
{
	auto iter = m_UIMap.find(key);
	if (iter != m_UIMap.end())
	{
		CTexturedShader::Render(pd3dCommandList, pCamera);
		(*iter).second->Render(pd3dCommandList, pCamera);
	}
}
