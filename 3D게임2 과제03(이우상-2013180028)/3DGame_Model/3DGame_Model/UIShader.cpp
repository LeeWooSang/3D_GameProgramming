#include "stdafx.h"
#include "UIShader.h"

CUIShader::CUIShader()
{
}

CUIShader::~CUIShader()
{
}

void CUIShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	CShader::CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
}

D3D12_DEPTH_STENCIL_DESC CUIShader::CreateDepthStencilState()
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

D3D12_RASTERIZER_DESC CUIShader::CreateRasterizerState()
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

D3D12_BLEND_DESC CUIShader::CreateBlendState()
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

D3D12_SHADER_BYTECODE CUIShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSFirstPersonUI", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CUIShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSFirstPersonUI", "ps_5_1", ppd3dShaderBlob));
}

void CUIShader::ReleaseShaderVariables()
{
	CTexturedShader::ReleaseShaderVariables();
}

void CUIShader::ReleaseUploadBuffers()
{
	m_pFirstPersonUI->ReleaseUploadBuffers();
}

bool CUIShader::Initialize()
{
	if (!m_pFirstPersonUI)
	{
		// 1인칭 UI를 객체를 생성해준다
		m_pFirstPersonUI = new CFirstPersonUI;
		// UI 메쉬를 설정해줌
		m_pFirstPersonUI->SetMesh(m_pFirstPersonUIMesh);
		// 재질을 설정해줌
		m_pFirstPersonUI->SetMaterial(m_pFirstPersonUIMaterial);
		cout << "1인칭 UI 생성" << endl;
		return true;
	}
	return false;
}

void CUIShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{

	// 1인칭 UI 메쉬
	m_pFirstPersonUIMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 20.f, 20.f, 0.0f, 0.0f, 0.0f, 0.0f);
	// 1인칭 UI 텍스처 생성
	m_pFirstPersonUITexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pFirstPersonUITexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/FirstPersonMode.dds", 0);
	// 1인칭 UI 재질 생성
	m_pFirstPersonUIMaterial = new CMaterial;
	m_pFirstPersonUIMaterial->SetTexture(m_pFirstPersonUITexture);

	m_nObjects = 1;
	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, 1);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pFirstPersonUITexture, 3, false);
}

void CUIShader::ReleaseObjects()
{
	m_pFirstPersonUI->DeleteMesh();
	DeleteUI();
}

void CUIShader::DeleteUI()
{
	if (m_pFirstPersonUI)
	{
		delete m_pFirstPersonUI;
		m_pFirstPersonUI = nullptr;
		cout << "1인칭 UI 삭제" << endl;
	}
}
void CUIShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	if (m_pFirstPersonUI)
	{
		CTexturedShader::Render(pd3dCommandList, pCamera);
		m_pFirstPersonUI->Render(pd3dCommandList, pCamera);
	}
}
