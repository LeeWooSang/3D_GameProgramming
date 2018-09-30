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
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256�� ���
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
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;

	float fxPitch = 12.0f * 3.5f;
	float fyPitch = 12.0f * 3.5f;
	float fzPitch = 12.0f * 3.5f;

	float fTerrainWidth = pTerrain->GetWidth();
	float fTerrainLength = pTerrain->GetLength();

	int xObjects = int(fTerrainWidth / fxPitch);
	int yObjects = 2;
	int zObjects = int(fTerrainLength / fzPitch);
	//m_nObjects = 100;
	m_nObjects = xObjects * zObjects;

	// �ؽ�ó ����
	const UINT TextureCount = 2;

	//CTexture* pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	CTexture* pTexture = new CTexture(TextureCount, RESOURCE_TEXTURE2D, 0);

	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/tree01S.dds", 0);
	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/tree02S.dds", 1);
	//pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Lava(Diffuse).dds", 0);
	//	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Lava(Emissive).dds", 0);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, 2);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTexture, 3, false);
	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTexture, 3, true);

#ifdef _WITH_BATCH_MATERIAL
	m_pMaterial = new CMaterial();
	m_pMaterial->SetTexture(pTexture);
#else
	CMaterial *pCubeMaterial = new CMaterial();
	pCubeMaterial->SetTexture(pTexture);
#endif
	// ������ ���� �޽� ����
	float Width = 35.f;
	float Height = 70.f;

	m_pTexturedRectMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, Width, Height, 0.f, 0.f, 0.f, 0.f);
	m_pBillboardTree = new CBillboardTree[m_nObjects];
	cout << "������ ���� �޽� ����" << endl;

	default_random_engine dre;
	uniform_real_distribution<double> urd_x(0.f, fTerrainWidth - 100);
	uniform_real_distribution<double> urd_z(0.f, fTerrainLength - 100);
	double x = 0, z = 0;
	for (int i = 0; i < m_nObjects; ++i)
	{
		x = urd_x(dre);
		z = urd_z(dre);
		((CBillboardTree*)m_pBillboardTree)[i].SetMesh(m_pTexturedRectMesh);
		m_pBillboardTree[i].SetPosition(x, pTerrain->GetHeight(x, z) + Height/2.f, z);
		m_pBillboardTree[i].SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));

		//cout << m_pBillboardTree[i].GetPosition().x << ", "
		//	<< m_pBillboardTree[i].GetPosition().y << ", "
		//	<< m_pBillboardTree[i].GetPosition().z << endl;
	}
	cout << "������Ʈ(����) ������ ���� : " << m_nObjects << "�� �����Ϸ�" << endl;
}

void CObjectsShader::ReleaseObjects()
{
	if (m_pBillboardTree)
		delete[] m_pBillboardTree;
	if (m_pTexturedRectMesh)
		delete m_pTexturedRectMesh;


#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) 
		delete m_pMaterial;
#endif
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

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) 
		m_pMaterial->ReleaseUploadBuffers();
#endif
}

void CObjectsShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CTexturedShader::Render(pd3dCommandList, pCamera);

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) 
		m_pMaterial->UpdateShaderVariables(pd3dCommandList);
#endif

	for (int i = 0; i < m_nObjects; ++i)
	{
		if (m_pBillboardTree != nullptr)
			m_pBillboardTree[i].Render(pd3dCommandList, pCamera);
	}
}
