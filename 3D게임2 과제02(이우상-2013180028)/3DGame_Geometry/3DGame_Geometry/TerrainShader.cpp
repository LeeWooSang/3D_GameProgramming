#include "stdafx.h"
#include "TerrainShader.h"

extern int g_FillMode;
extern bool g_OnGeometry;

CTerrainShader::CTerrainShader()
{
}

CTerrainShader::~CTerrainShader()
{
}

void CTerrainShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	// 기하셰이더를 사용해서 터레인을 만든 것과, 
	// 기하셰이더를 사용하지 않고 만든 터레인
	// 두개를 하기위해 파이프라인 스테이트를 두개 만든다.
	//m_nPipelineStates = 1;
	m_nPipelineStates = 2;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	ID3DBlob* pd3dVertexShaderBlob = NULL, *pd3dPixelShaderBlob = NULL;

	ID3DBlob* pd3dGeometryShaderBlob = nullptr;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc[2];

	for (int i = 0; i < m_nPipelineStates; ++i)
	{
		// Zeromemory 기능???
		::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC) * 2);

		d3dPipelineStateDesc[i].pRootSignature = pd3dGraphicsRootSignature;
		d3dPipelineStateDesc[i].VS = CreateVertexShader(&pd3dVertexShaderBlob);
		if (i == 0)
			d3dPipelineStateDesc[i].GS = CreateGeometryShader(&pd3dGeometryShaderBlob);
		d3dPipelineStateDesc[i].PS = CreatePixelShader(&pd3dPixelShaderBlob);
		d3dPipelineStateDesc[i].RasterizerState = CreateRasterizerState();
		d3dPipelineStateDesc[i].BlendState = CreateBlendState();
		d3dPipelineStateDesc[i].DepthStencilState = CreateDepthStencilState();
		d3dPipelineStateDesc[i].InputLayout = CreateInputLayout();
		d3dPipelineStateDesc[i].SampleMask = UINT_MAX;
		d3dPipelineStateDesc[i].PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		d3dPipelineStateDesc[i].NumRenderTargets = 1;
		d3dPipelineStateDesc[i].RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		d3dPipelineStateDesc[i].DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		d3dPipelineStateDesc[i].SampleDesc.Count = 1;
		d3dPipelineStateDesc[i].Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc[i], __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[i]);
	}

	if (pd3dVertexShaderBlob)
		pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob)
		pd3dPixelShaderBlob->Release();
	if (pd3dGeometryShaderBlob)
		pd3dGeometryShaderBlob->Release();
	for (int i = 0; i < m_nPipelineStates; ++i)
	{
		if (d3dPipelineStateDesc[i].InputLayout.pInputElementDescs)
			delete[] d3dPipelineStateDesc[i].InputLayout.pInputElementDescs;
	}
}

D3D12_RASTERIZER_DESC  CTerrainShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));

	if (g_FillMode == SOLID)
		d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	else if (g_FillMode == WIRE)
		d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;

	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
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

D3D12_INPUT_LAYOUT_DESC CTerrainShader::CreateInputLayout()
{
	// 셰이더 코드로 넘겨줄 흙 uv 좌표 개수가 1개가 추가 되었으므로, InputLayout 개수도 한개 증가한다.
	UINT nInputElementDescs = 5;

	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	// 터레인 지형의 위치 정보
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	// 터레인의 색깔 정보
	pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	// 기존 지형의 텍스처 uv
	pd3dInputElementDescs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	// 기존 지형의 디테일 텍스처 uv
	pd3dInputElementDescs[3] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	// 흙 디테일 텍스처 uv 추가
	pd3dInputElementDescs[4] = { "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CTerrainShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VS_Terrain", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTerrainShader::CreateGeometryShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "GS_Terrain", "gs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTerrainShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PS_GSTerrain", "ps_5_1", ppd3dShaderBlob));
}

void CTerrainShader::OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList)
{
	if (m_ppd3dPipelineStates)
	{
		if(g_OnGeometry == USE)
			pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);
		else
			pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[1]);
	}


	if (m_pd3dCbvSrvDescriptorHeap)
		pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);

	UpdateShaderVariables(pd3dCommandList);
}