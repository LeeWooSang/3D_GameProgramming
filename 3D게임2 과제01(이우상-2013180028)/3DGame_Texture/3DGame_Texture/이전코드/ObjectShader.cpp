#include "stdafx.h"
#include "ObjectShader.h"
#include "BillboardTree.h"

CObjectsShader::CObjectsShader()
{
}

CObjectsShader::~CObjectsShader()
{
}

void CObjectsShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes * m_nObjects, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbGameObjects->Map(0, NULL, (void **)&m_pcbMappedGameObjects);
}

void CObjectsShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);
	for (int j = 0; j < m_nObjects; j++)
	{
		CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedGameObjects + (j * ncbElementBytes));
		XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_pBillboardTree[j].m_xmf4x4World)));
	}
}

void CObjectsShader::ReleaseShaderVariables()
{
	if (m_pd3dcbGameObjects)
	{
		m_pd3dcbGameObjects->Unmap(0, NULL);
		m_pd3dcbGameObjects->Release();
	}

	CTexturedShader::ReleaseShaderVariables();
}

void CObjectsShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	//CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;

	//float fxPitch = 12.0f * 3.5f;
	//float fyPitch = 12.0f * 3.5f;
	//float fzPitch = 12.0f * 3.5f;

	//float fTerrainWidth = pTerrain->GetWidth();
	//float fTerrainLength = pTerrain->GetLength();

	//int xObjects = int(fTerrainWidth / fxPitch);
	//int yObjects = 2;
	//int zObjects = int(fTerrainLength / fzPitch);
	//m_nObjects = 3;
	////m_nObjects = xObjects * zObjects;

	//// 텍스처 개수
	//const UINT TextureCount = 3;

	//CTexture* pTreeTexture[TextureCount];

	//pTreeTexture[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//pTreeTexture[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/tree01S.dds", 0);

	//pTreeTexture[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//pTreeTexture[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/tree02S.dds", 0);

	//pTreeTexture[2] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//pTreeTexture[2]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Flower01.dds", 0);

	//UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	//CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, TextureCount);
	//CreateShaderVariables(pd3dDevice, pd3dCommandList);
	//CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
	//
	//for (int i = 0; i < TextureCount; ++i)
	//	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTreeTexture[i], 3, true);

	//CMaterial* pMaterial[TextureCount];
	//for (int i = 0; i < TextureCount; ++i)
	//{
	//	pMaterial[i] = new CMaterial();
	//	pMaterial[i]->SetTexture(pTreeTexture[i]);
	//	m_list_pMaterial.push_back(pMaterial[i]);
	//}

	//// 빌보드 나무 메쉬 생성
	//float Width = 35.f;
	//float Height = 70.f;

	//m_pTexturedRectMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, Width, Height, 0.f, 0.f, 0.f, 0.f);
	//m_pBillboardTree = new CBillboardTree[m_nObjects];
	//cout << "빌보드 나무 메쉬 생성" << endl;

	//default_random_engine dre;
	//uniform_real_distribution<double> urd_x(0.f, fTerrainWidth - 100);
	//uniform_real_distribution<double> urd_z(0.f, fTerrainLength - 100);
	//uniform_int_distribution<int> uid_texture(0, 2);

	//double x = 0, z = 0;
	//for (int i = 0; i < m_nObjects; ++i)
	//{
	//	x = urd_x(dre);
	//	z = urd_z(dre);
	//	((CBillboardTree*)m_pBillboardTree)[i].SetMesh(m_pTexturedRectMesh);
	//	// 텍스처 지정
	//	//m_pBillboardTree[i].SetMaterial(pMaterial[uid_texture(dre)]);
	//	m_pBillboardTree[i].SetMaterial(pMaterial[i % TextureCount]);
	//	//m_pBillboardTree[i].SetPosition(x, pTerrain->GetHeight(x, z) + Height/2.f, z);
	//	m_pBillboardTree[i].SetPosition(1030 + i * 20, 200, 1030);
	//	m_pBillboardTree[i].SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));

	//	//cout << m_pBillboardTree[i].GetPosition().x << ", "
	//	//	<< m_pBillboardTree[i].GetPosition().y << ", "
	//	//	<< m_pBillboardTree[i].GetPosition().z << endl;
	//}
	//cout << "오브젝트(나무) 빌보드 개수 : " << m_nObjects << "개 생성완료" << endl;

	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;

	float fTerrainWidth = pTerrain->GetWidth();
	float fTerrainLength = pTerrain->GetLength();

	int xObjects = 35;
	int zObjects = 50;
	m_nObjects = (xObjects * zObjects);

	CTexture *ppGrassTextures[2];
	ppGrassTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppGrassTextures[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Grass01.dds", 0);
	ppGrassTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppGrassTextures[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Grass02.dds", 0);
	CTexture *ppFlowerTextures[2];
	ppFlowerTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppFlowerTextures[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Flower01.dds", 0);
	ppFlowerTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppFlowerTextures[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Flower02.dds", 0);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, 4);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppGrassTextures[0], 3, true);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppGrassTextures[1], 3, true);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppFlowerTextures[0], 3, true);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppFlowerTextures[1], 3, true);

	CMaterial *ppGrassMaterials[2];
	ppGrassMaterials[0] = new CMaterial();
	ppGrassMaterials[0]->SetTexture(ppGrassTextures[0]);
	ppGrassMaterials[1] = new CMaterial();
	ppGrassMaterials[1]->SetTexture(ppGrassTextures[1]);

	CMaterial *ppFlowerMaterials[2];
	ppFlowerMaterials[0] = new CMaterial();
	ppFlowerMaterials[0]->SetTexture(ppFlowerTextures[0]);
	ppFlowerMaterials[1] = new CMaterial();
	ppFlowerMaterials[1]->SetTexture(ppFlowerTextures[1]);

	CTexturedRectMesh *pGrassMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 10.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	CTexturedRectMesh *pFlowerMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 10.0f, 16.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	m_ppObjects = new CGameObject*[m_nObjects];

	CBillboardTree *pBillboardObject = NULL;

	float fxPitch = 0.25f;
	float fzPitch = 0.25f;

	CMaterial *pMaterial = NULL;
	CMesh *pMesh = NULL;

	float xPosition = 950.0f;
	for (int i = 0, x = 0; x < xObjects; x++)
	{
		xPosition += x * fxPitch;
		float zPosition = 1350.0f;
		for (int z = 0; z < zObjects; z++)
		{
			pBillboardObject = new CBillboardTree();
			//pBillboardObject->SetMesh(0, (z % 2) ? pGrassMesh : pFlowerMesh);
			pBillboardObject->SetMesh(pFlowerMesh);

			if (x % 2) pMaterial = (z % 2) ? ppGrassMaterials[0] : ppFlowerMaterials[1];
			else pMaterial = (z % 2) ? ppGrassMaterials[1] : ppFlowerMaterials[0];
			pBillboardObject->SetMaterial(pMaterial);

			zPosition += z * fzPitch;
			float fHeight = pTerrain->GetHeight(xPosition, zPosition);
			pBillboardObject->SetPosition(xPosition, fHeight + (((z % 2) ? 8.0f : 16.0f) * 0.5f), zPosition);
			pBillboardObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
			m_ppObjects[i++] = pBillboardObject;
		}
	}
}

void CObjectsShader::ReleaseObjects()
{
	if (m_pBillboardTree)
		delete[] m_pBillboardTree;
	if (m_pTexturedRectMesh)
		delete m_pTexturedRectMesh;

	//if (m_list_pMaterial.size() != 0)
	//{
	//	for (auto iter = m_list_pMaterial.begin(); iter != m_list_pMaterial.end(); )
	//	{
	//		(*iter)->Release();
	//		delete (*iter);
	//		iter = m_list_pMaterial.erase(iter);
	//	}
	//	m_list_pMaterial.clear();
	//}
}

void CObjectsShader::AnimateObjects(float fTimeElapsed, CCamera* pCamera)
{
	if (m_pBillboardTree != nullptr)
	{
		for (int i = 0; i < m_nObjects; ++i)
		{
			((CBillboardTree*)m_pBillboardTree)[i].Animate(fTimeElapsed, pCamera);
		}	
	}
}

void CObjectsShader::ReleaseUploadBuffers()
{
	if (m_pBillboardTree != nullptr)
	{
		for (int i = 0; i < m_nObjects; ++i)
			m_pBillboardTree[i].ReleaseUploadBuffers();
	}
}

void CObjectsShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CTexturedShader::Render(pd3dCommandList, pCamera);


	for (int i = 0; i < m_nObjects; ++i)
	{
		if (m_pBillboardTree != nullptr)
			((CBillboardTree*)m_pBillboardTree)[i].Render(pd3dCommandList, pCamera);
	}

}
