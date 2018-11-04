#include "stdafx.h"
#include "GeometryShader.h"
#include "HeightMapTerrain.h"
#include "BillboardObject.h"
#include "Material.h"
#include "DDSTextureLoader12.h"
#include "Vertex.h"

CGeometryShader::CGeometryShader()
{
}

CGeometryShader::~CGeometryShader()
{
}

D3D12_INPUT_LAYOUT_DESC CGeometryShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	// 기하 셰이더
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
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

void CGeometryShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
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

	float fTerrainWidth = pTerrain->GetWidth();
	float fTerrainLength = pTerrain->GetLength();

	int xObjects = 35;
	int zObjects = 50;
	//m_nObjects = (xObjects * zObjects);
	//m_nObjects = 12000;
	m_nObjects = 5000;

	const int Grass_Texture_Count = 2;
	const int Flower_Texture_Count = 2;
	const int Tree_Texture_Count = 5;
	int Total_Texutre_count = Grass_Texture_Count + Flower_Texture_Count + Tree_Texture_Count;

	CTexture *ppGrassTextures[Grass_Texture_Count];
	ppGrassTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppGrassTextures[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Grass01.dds", 0);
	ppGrassTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppGrassTextures[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Grass01.dds", 0);

	CTexture *ppFlowerTextures[Flower_Texture_Count];
	ppFlowerTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppFlowerTextures[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Flower01.dds", 0);
	ppFlowerTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppFlowerTextures[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Flower02.dds", 0);

	CTexture* ppTreeTextures[Tree_Texture_Count];
	ppTreeTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTreeTextures[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Tree01.dds", 0);
	ppTreeTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTreeTextures[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Tree02.dds", 0);
	ppTreeTextures[2] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTreeTextures[2]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Tree03.dds", 0);
	ppTreeTextures[3] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTreeTextures[3]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Tree04.dds", 0);
	ppTreeTextures[4] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTreeTextures[4]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Tree05.dds", 0);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, Total_Texutre_count);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);

	// 풀, 꽃 리소스 뷰 2개
	for (int i = 0; i < 2; ++i)
	{
		CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppGrassTextures[i], 3, true);
		CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppFlowerTextures[i], 3, true);
	}

	// 나무 리소스 뷰 5개
	for (int i = 0; i < Tree_Texture_Count; ++i)
		CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppTreeTextures[i], 3, true);

	CMaterial* ppGrassMaterials[Grass_Texture_Count]{ nullptr };
	CMaterial* ppFlowerMaterials[Flower_Texture_Count]{ nullptr };
	CMaterial* ppTreeMaterials[Tree_Texture_Count]{ nullptr };

	for (int i = 0; i < 2; ++i)
	{
		ppGrassMaterials[i] = new CMaterial();
		ppGrassMaterials[i]->SetTexture(ppGrassTextures[i]);

		ppFlowerMaterials[i] = new CMaterial();
		ppFlowerMaterials[i]->SetTexture(ppFlowerTextures[i]);
	}

	for (int i = 0; i < Tree_Texture_Count; ++i)
	{
		ppTreeMaterials[i] = new CMaterial();
		ppTreeMaterials[i]->SetTexture(ppTreeTextures[i]);
	}

	float GrassHeight = 20.f, FlowerHeight = 30.f, TreeHeight = 100.f;
	//CTexturedRectMesh* pGrassMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 10.0f, GrassHeight, 0.0f, 0.0f, 0.0f, 0.0f);
	//CTexturedRectMesh* pFlowerMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 20.0f, FlowerHeight, 0.0f, 0.0f, 0.0f, 0.0f);
	//CTexturedRectMesh* pTreeMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 50.0f, TreeHeight, 0.0f, 0.0f, 0.0f, 0.0f);

	m_ppObjects = new CGameObject*[m_nObjects];
	CBillboardObject* pBillboardObject = nullptr;

	float fxPitch = 0.25f;
	float fzPitch = 0.25f;

	CMaterial* pMaterial = NULL;
	CMesh* pMesh = NULL;

	float xPosition = 950.0f;
	default_random_engine dre;

	uniform_real_distribution<double> urd_x(0.f, fTerrainWidth - 100);
	uniform_real_distribution<double> urd_z(0.f, fTerrainLength - 100);

	// 텍스처 종류
	uniform_int_distribution<int> uid_TextureType(0, 2);
	uniform_int_distribution<int> uid_Material(0, 1);
	uniform_int_distribution<int> uid_TreeMaterial(0, Tree_Texture_Count - 1);

	int nStride = sizeof(CBillboardVertex);
	m_nVertices = m_nObjects;
	XMFLOAT3 xmf3Position;
	CBillboardVertex* pBillboardVertex = new CBillboardVertex[m_nObjects];
	//CGeometryVertexMesh* pBillboardVertex = new CGeometryVertexMesh(pd3dDevice, pd3dCommandList);
	for (int i = 0; i < m_nObjects; ++i)
	{
		xmf3Position.x = urd_x(dre);
		xmf3Position.z = urd_z(dre);
		while (pTerrain->GetHeight(xmf3Position.x, xmf3Position.z) < Flatland_Height)
		{
			xmf3Position.x = urd_x(dre);
			xmf3Position.z = urd_z(dre);
		}
		xmf3Position.y = pTerrain->GetHeight(xmf3Position.x, xmf3Position.z) + pTerrain->GetHeight(xmf3Position.x, xmf3Position.z) * 0.5;
		pBillboardVertex[i] = CBillboardVertex(xmf3Position, XMFLOAT2(100.f, 100.f));
	}

	m_pd3dVertexBuffer = ::CreateBufferResource
	(
		pd3dDevice,
		pd3dCommandList,
		pBillboardVertex,
		sizeof(UINT) * m_nVertices,
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		&m_pd3dVertexUploadBuffer
	);

	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = nStride;
	m_d3dVertexBufferView.SizeInBytes = nStride * m_nVertices;

	for (int i = 0; i < m_nObjects; ++i)
	{		
		pBillboardObject = new CBillboardObject;
		// 텍스처, 재질 랜덤 지정
		if (uid_TextureType(dre) == GRASS)
		{
			pBillboardObject->SetMaterial(ppFlowerMaterials[uid_Material(dre)]);
		}
		else if (uid_TextureType(dre) == FLOWER)
		{
			pBillboardObject->SetMaterial(ppGrassMaterials[uid_Material(dre)]);
		}
		else if (uid_TextureType(dre) == TREE)
		{
			// 나무는 3개
			pBillboardObject->SetMaterial(ppTreeMaterials[uid_TreeMaterial(dre)]);
		}
		pBillboardObject->SetPosition(pBillboardVertex[i].m_xmf3Position);
		pBillboardObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
		m_ppObjects[i] = pBillboardObject;
	}
		
	cout << "기하셰이더 빌보드 오브젝트 개수 : " << m_nObjects << "개 생성완료" << endl;
}

void CGeometryShader::ReleaseUploadBuffers()
{
	CObjectsShader::ReleaseUploadBuffers();
}

void CGeometryShader::ReleaseObjects()
{
	CObjectsShader::ReleaseObjects();
}

void CGeometryShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	//XMFLOAT3 xmf3CameraPosition = pCamera->GetPosition();
	//for (int j = 0; j < m_nObjects; j++)
	//{
	//	if (m_ppObjects[j])
	//		m_ppObjects[j]->SetLookAt(xmf3CameraPosition, XMFLOAT3(0.0f, 1.0f, 0.0f));
	//}

	//CObjectsShader::Render(pd3dCommandList, pCamera);
	for (int i = 0; i < m_nObjects; ++i)
	{
		if (m_ppObjects[i]) 
			m_ppObjects[i]->Render(pd3dCommandList, pCamera);

		pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
		pd3dCommandList->IASetVertexBuffers(0, 1, &m_d3dVertexBufferView);
		pd3dCommandList->DrawInstanced(m_nVertices, 1, 0, 0);
	}
	
}