#include "stdafx.h"
#include "GeometryShader.h"
#include "HeightMapTerrain.h"
#include "BillboardObject.h"
#include "Material.h"
#include "DDSTextureLoader12.h"
#include "Vertex.h"
#include "GameFramework.h"

extern bool g_OnGeometry;

CGeometryShader::CGeometryShader()
{
}

CGeometryShader::~CGeometryShader()
{
	ReleaseShaderVariables();
	ReleaseUploadBuffers();
}

void CGeometryShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dPixelShaderBlob = NULL;
	ID3DBlob *pd3dGeometeryShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);			
	d3dPipelineStateDesc.GS = CreateGeometryShader(&pd3dGeometeryShaderBlob);
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);			
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();					
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[0]);

	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();
	if (pd3dGeometeryShaderBlob) pd3dGeometeryShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;

}

void CGeometryShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{

}

void CGeometryShader::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{

}

D3D12_BLEND_DESC CGeometryShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = true;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = true;
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

D3D12_INPUT_LAYOUT_DESC CGeometryShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	// 기하 셰이더
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CGeometryShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VS_Geometry", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CGeometryShader::CreateGeometryShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "GS", "gs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CGeometryShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PS_Geometry", "ps_5_1", ppd3dShaderBlob));
}

D3D12_RASTERIZER_DESC CGeometryShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));

	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

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

void CGeometryShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;

	int TerrainWidth = pTerrain->GetWidth();
	int TerrainLength = pTerrain->GetLength();

	CTexture* pTree_Texture2DArray = new CTexture(1, RESOURCE_TEXTURE2DARRAY, 0);
	pTree_Texture2DArray->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/TreeArray.dds", 0);

	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1);

	// 텍스처배열은 루트파라미터 7번에 연결되어있음
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTree_Texture2DArray, 7, false);

	m_pMaterial = new CMaterial();
	m_pMaterial->SetTexture(pTree_Texture2DArray);
	
	//float GrassHeight = 20.f, FlowerHeight = 30.f, TreeHeight = 100.f;
	
	int nStride = sizeof(CBillboardVertex);
	m_nVertices = 1000;
	XMFLOAT3 xmf3Position = XMFLOAT3(0.f, 0.f, 0.f);
	CBillboardVertex* pBillboardVertex = new CBillboardVertex[m_nVertices];

	default_random_engine dre;
	uniform_real_distribution<double> urd_x(0.f, TerrainWidth - 100);
	uniform_real_distribution<double> urd_z(0.f, TerrainLength - 100);
	int x = 0, y = 0, z = 0;

	//for (int i = 0; i < m_nVertices; ++i)
	//{
	//	xmf3Position.x = rand() % TerrainWidth;
	//	xmf3Position.z = rand() % TerrainLength;
	//	xmf3Position.y = pTerrain->GetHeight(xmf3Position.x, xmf3Position.z, false) + 10.0f;


	//	pBillboardVertex[i].m_xmf3Position = xmf3Position;
	//	pBillboardVertex[i].m_xmf2Size = XMFLOAT2(50, 70);

	//	//cout << pBillboardVertex[i].m_xmf3Position.x << ", " << pBillboardVertex[i].m_xmf3Position.y << ", "
	//	//	<< pBillboardVertex[i].m_xmf3Position.z << endl;
	//}
	for (int i = 0; i < m_nVertices;)
	{
		x = urd_x(dre) / 8;
		z = urd_z(dre) / 8;
		if (m_PositionArray[x][z] == 1)
		{
			x = x * 8.;
			z = z * 8.;
			y = pTerrain->GetHeight(x, z);

			xmf3Position.x = x;
			xmf3Position.z = z;
			xmf3Position.y = pTerrain->GetHeight(x, z) + 10.0f;
			pBillboardVertex[i].m_xmf3Position = xmf3Position;
			pBillboardVertex[i].m_xmf2Size = XMFLOAT2(50, 70);
			++i;
		}
		else
			continue;
	}
 
	m_pd3dVertexBuffer = ::CreateBufferResource
	(
		pd3dDevice,
		pd3dCommandList,
		pBillboardVertex,
		nStride * m_nVertices,
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		&m_pd3dVertexUploadBuffer
	);

	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = nStride;
	m_d3dVertexBufferView.SizeInBytes = nStride * m_nVertices;

	cout << "기하셰이더 빌보드 오브젝트 개수 : " << m_nVertices << "개 생성완료" << endl;
	if (pBillboardVertex)
		delete[] pBillboardVertex;
}

void CGeometryShader::ReleaseShaderVariables()
{
	if (m_pd3dVertexBuffer)
		m_pd3dVertexBuffer->Release();
	m_pd3dVertexBuffer = nullptr;
}
void CGeometryShader::ReleaseUploadBuffers()
{
	if (m_pd3dVertexUploadBuffer)
		m_pd3dVertexUploadBuffer->Release();
	m_pd3dVertexUploadBuffer = nullptr;

}

//void CGeometryShader::ReleaseObjects()
//{
//	CObjectsShader::ReleaseObjects();
//}

void CGeometryShader::OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList)
{
	if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);

	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);
	UpdateShaderVariables(pd3dCommandList);
}
void CGeometryShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	OnPrepareRender(pd3dCommandList);

	if (m_pMaterial)
	{
		if (m_pMaterial->m_pShader)
		{
			m_pMaterial->m_pShader->Render(pd3dCommandList, pCamera);
			m_pMaterial->m_pShader->UpdateShaderVariables(pd3dCommandList);

			UpdateShaderVariables(pd3dCommandList);
		}
		if (m_pMaterial->m_pTexture)
		{
			m_pMaterial->m_pTexture->UpdateShaderVariables(pd3dCommandList);
		}
	}
	pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	pd3dCommandList->IASetVertexBuffers(0, 1, &m_d3dVertexBufferView);
	pd3dCommandList->DrawInstanced(m_nVertices, 1, 0, 0);
}