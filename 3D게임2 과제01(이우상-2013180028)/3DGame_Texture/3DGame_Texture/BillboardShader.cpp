#include "stdafx.h"
#include "BillboardShader.h"
#include "HeightMapTerrain.h"
#include "BillboardObject.h"
#include "Material.h"
#include "DDSTextureLoader12.h"

CBillboardShader::CBillboardShader()
{
}

CBillboardShader::~CBillboardShader()
{
}

D3D12_RASTERIZER_DESC CBillboardShader::CreateRasterizerState()
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

void CBillboardShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;

	float fTerrainWidth = pTerrain->GetWidth();
	float fTerrainLength = pTerrain->GetLength();

	int xObjects = 35;
	int zObjects = 50;
	//m_nObjects = (xObjects * zObjects);
	m_nObjects = 12000;

	const int Grass_Texture_Count = 2;
	const int Flower_Texture_Count = 2;
	const int Tree_Texture_Count = 5;
	const int Sand_Texture_Count = 0;
	int Total_Texutre_count = Grass_Texture_Count + Flower_Texture_Count + Tree_Texture_Count + Sand_Texture_Count;

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
	CTexturedRectMesh* pGrassMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 10.0f, GrassHeight, 0.0f, 0.0f, 0.0f, 0.0f);
	CTexturedRectMesh* pFlowerMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 20.0f, FlowerHeight, 0.0f, 0.0f, 0.0f, 0.0f);
	CTexturedRectMesh* pTreeMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 50.0f, TreeHeight, 0.0f, 0.0f, 0.0f, 0.0f);

	m_ppObjects = new CGameObject*[m_nObjects];

	CBillboardObject *pBillboardObject = NULL;

	float fxPitch = 0.25f;
	float fzPitch = 0.25f;

	CMaterial *pMaterial = NULL;
	CMesh *pMesh = NULL;

	float xPosition = 950.0f;
	default_random_engine dre;

	uniform_real_distribution<double> urd_x(0.f, fTerrainWidth - 100);
	uniform_real_distribution<double> urd_z(0.f, fTerrainLength - 100);

	// 텍스처 종류
	uniform_int_distribution<int> uid_TextureType(0, 2);
	uniform_int_distribution<int> uid_Material(0, 1);
	uniform_int_distribution<int> uid_TreeMaterial(0, Tree_Texture_Count - 1);
	double x = 0, z = 0;
	for (int i = 0; i < m_nObjects; ++i)
	{
		x = urd_x(dre);
		z = urd_z(dre);
		while (pTerrain->GetHeight(x, z) < Terrain_Height)
		{
			x = urd_x(dre);
			z = urd_z(dre);
		}
		pBillboardObject = new CBillboardObject;

		// 텍스처, 재질 랜덤 지정
		if (uid_TextureType(dre) == GRASS)
		{
			pBillboardObject->SetMesh(0, pGrassMesh);
			pBillboardObject->SetMaterial(ppFlowerMaterials[uid_Material(dre)]);
			pBillboardObject->SetPosition(x, pTerrain->GetHeight(x, z) + GrassHeight / 2.f, z);
		}
		else if (uid_TextureType(dre) == FLOWER)
		{
			pBillboardObject->SetMesh(0, pFlowerMesh);
			pBillboardObject->SetMaterial(ppGrassMaterials[uid_Material(dre)]);
			pBillboardObject->SetPosition(x, pTerrain->GetHeight(x, z) + FlowerHeight / 2.f, z);
		}
		else if (uid_TextureType(dre) == TREE)
		{
			pBillboardObject->SetMesh(0, pTreeMesh);
			// 나무는 3개
			pBillboardObject->SetMaterial(ppTreeMaterials[uid_TreeMaterial(dre)]);
			pBillboardObject->SetPosition(x, pTerrain->GetHeight(x, z) + TreeHeight / 2.7f, z);
		}

		pBillboardObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
		m_ppObjects[i] = pBillboardObject;
	}

	cout << "오브젝트 빌보드 개수 : " << m_nObjects << "개 생성완료" << endl;
}

void CBillboardShader::ReleaseUploadBuffers()
{
	CObjectsShader::ReleaseUploadBuffers();
}

void CBillboardShader::ReleaseObjects()
{
	CObjectsShader::ReleaseObjects();
}

void CBillboardShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	XMFLOAT3 xmf3CameraPosition = pCamera->GetPosition();
	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
			m_ppObjects[j]->SetLookAt(xmf3CameraPosition, XMFLOAT3(0.0f, 1.0f, 0.0f));
	}

	CObjectsShader::Render(pd3dCommandList, pCamera);
}