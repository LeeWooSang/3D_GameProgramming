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

	// 텍스처 개수
	const UINT TextureCount = 3;

	CTexture* pTexture[TextureCount];
		
	pTexture[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pTexture[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/tree01S.dds", 0);

	pTexture[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pTexture[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/tree02S.dds", 0);

	pTexture[2] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pTexture[2]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/tree03S.dds", 0);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, 1);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
	
	for (int i = 0; i < TextureCount; ++i)
		CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTexture[i], 3, false);

	CMaterial* pMaterial[TextureCount] = { nullptr };
	
	for (int i = 0; i < TextureCount; ++i)
	{
		pMaterial[i] = new CMaterial();
		pMaterial[i]->SetTexture(pTexture[i]);
		m_list_pMaterial.push_back(pMaterial[i]);
	}

	// 빌보드 나무 메쉬 생성
	float Width = 35.f;
	float Height = 70.f;

	m_pTexturedRectMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, Width, Height, 0.f, 0.f, 0.f, 0.f);
	m_pBillboardTree = new CBillboardTree[m_nObjects];
	cout << "빌보드 나무 메쉬 생성" << endl;

	default_random_engine dre;
	uniform_real_distribution<double> urd_x(0.f, fTerrainWidth - 100);
	uniform_real_distribution<double> urd_z(0.f, fTerrainLength - 100);
	uniform_int_distribution<int> uid_texture(0, 2);

	double x = 0, z = 0;
	for (int i = 0; i < m_nObjects; ++i)
	{
		x = urd_x(dre);
		z = urd_z(dre);
		((CBillboardTree*)m_pBillboardTree)[i].SetMesh(m_pTexturedRectMesh);
		// 텍스처 지정
		m_pBillboardTree[i].SetMaterial(pMaterial[uid_texture(dre)]);

		m_pBillboardTree[i].SetPosition(x, pTerrain->GetHeight(x, z) + Height/2.f, z);
		m_pBillboardTree[i].SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));

		//cout << m_pBillboardTree[i].GetPosition().x << ", "
		//	<< m_pBillboardTree[i].GetPosition().y << ", "
		//	<< m_pBillboardTree[i].GetPosition().z << endl;
	}
	cout << "오브젝트(나무) 빌보드 개수 : " << m_nObjects << "개 생성완료" << endl;
}

void CObjectsShader::ReleaseObjects()
{
	if (m_pBillboardTree)
		delete[] m_pBillboardTree;
	if (m_pTexturedRectMesh)
		delete m_pTexturedRectMesh;

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

	if (m_list_pMaterial.size() != 0)
	{
		for (auto iter = m_list_pMaterial.begin(); iter != m_list_pMaterial.end(); )
		{
			(*iter)->Release();
			delete (*iter);
			iter = m_list_pMaterial.erase(iter);
		}
		m_list_pMaterial.clear();
	}
}

void CObjectsShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CTexturedShader::Render(pd3dCommandList, pCamera);

	if (m_list_pMaterial.size() != 0)
	{
		for (auto iter = m_list_pMaterial.begin(); iter != m_list_pMaterial.end(); )
		{
			(*iter)->UpdateShaderVariables(pd3dCommandList);
		}
	}

	for (int i = 0; i < m_nObjects; ++i)
	{
		if (m_pBillboardTree != nullptr)
			m_pBillboardTree[i].Render(pd3dCommandList, pCamera);
	}
}
