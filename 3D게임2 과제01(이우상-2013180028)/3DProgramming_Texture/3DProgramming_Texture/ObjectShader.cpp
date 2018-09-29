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
		//CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedGameObjects + (j * ncbElementBytes));
		//XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->m_xmf4x4World)));

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
	m_nObjects = (xObjects * yObjects * zObjects);

	CTexture *pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/tree01S.dds", 0);
	//pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Lava(Diffuse).dds", 0);
	//	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Lava(Emissive).dds", 0);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, 1);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTexture, 3, false);

#ifdef _WITH_BATCH_MATERIAL
	m_pMaterial = new CMaterial();
	m_pMaterial->SetTexture(pTexture);
#else
	CMaterial *pCubeMaterial = new CMaterial();
	pCubeMaterial->SetTexture(pTexture);
#endif
	m_pTexturedRectMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 0.f, 12.f, 12.f, 200.f, 1000.f, 1000.f);
	m_pBillboardTree = new CBillboardTree[m_nObjects];

	for (int i = 0, x = 0; x < xObjects; x++)
	{
		for (int z = 0; z < zObjects; z++)
		{
			for (int y = 0; y < yObjects; y++)
			{
				((CBillboardTree*)m_pBillboardTree)->SetMesh(m_pTexturedRectMesh);
				float xPosition = x * fxPitch;
				float zPosition = z * fzPitch;
				float fHeight = pTerrain->GetHeight(xPosition, zPosition);
				m_pBillboardTree[i++].SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
			}
		}
	}

	cout << "빌보드 나무 생성완료" << endl;
//	CCubeMeshTextured* pCubeMesh = new CCubeMeshTextured(pd3dDevice, pd3dCommandList, 12.0f, 12.0f, 12.0f);
//	
//	m_ppObjects = new CGameObject*[m_nObjects];
//
//	XMFLOAT3 xmf3RotateAxis, xmf3SurfaceNormal;
//	CRotatingObject *pRotatingObject = NULL;
//	for (int i = 0, x = 0; x < xObjects; x++)
//	{
//		for (int z = 0; z < zObjects; z++)
//		{
//			for (int y = 0; y < yObjects; y++)
//			{
//				pRotatingObject = new CRotatingObject(1);
//				pRotatingObject->SetMesh(0, pCubeMesh);
//#ifndef _WITH_BATCH_MATERIAL
//				pRotatingObject->SetMaterial(pCubeMaterial);
//#endif
//				float xPosition = x * fxPitch;
//				float zPosition = z * fzPitch;
//				float fHeight = pTerrain->GetHeight(xPosition, zPosition);
//				pRotatingObject->SetPosition(xPosition, fHeight + (y * 3.0f * fyPitch) + 6.0f, zPosition);
//				if (y == 0)
//				{
//					xmf3SurfaceNormal = pTerrain->GetNormal(xPosition, zPosition);
//					xmf3RotateAxis = Vector3::CrossProduct(XMFLOAT3(0.0f, 1.0f, 0.0f), xmf3SurfaceNormal);
//					if (Vector3::IsZero(xmf3RotateAxis)) xmf3RotateAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
//					float fAngle = acos(Vector3::DotProduct(XMFLOAT3(0.0f, 1.0f, 0.0f), xmf3SurfaceNormal));
//					pRotatingObject->Rotate(&xmf3RotateAxis, XMConvertToDegrees(fAngle));
//				}
//				pRotatingObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
//				pRotatingObject->SetRotationSpeed(36.0f * (i % 10) + 36.0f);
//				pRotatingObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
//				m_ppObjects[i++] = pRotatingObject;
//			}
//		}
//	}
}

void CObjectsShader::ReleaseObjects()
{
	if (m_pBillboardTree)
		delete[] m_pBillboardTree;
	if (m_pTexturedRectMesh)
		delete m_pTexturedRectMesh;

	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) 
			if (m_ppObjects[j]) 
				delete m_ppObjects[j];
		delete[] m_ppObjects;
	}

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) 
		delete m_pMaterial;
#endif
}

void CObjectsShader::AnimateObjects(float fTimeElapsed, CCamera* pCamera)
{
	//if (m_pBillboardTree != nullptr)
	//{
	//	for (int i = 0; i < m_nObjects; ++i)
	//	{
	//		((CBillboardTree*)m_pBillboardTree)[i].Animate(fTimeElapsed, pCamera);
	//		//cout << m_pBillboardTree[i].GetPosition().x << ", " << m_pBillboardTree[i].GetPosition().y << ", " << m_pBillboardTree[i].GetPosition().z << endl;
	//	}	
	//}

	//for (int j = 0; j < m_nObjects; j++)
	//{
	//	m_ppObjects[j]->Animate(fTimeElapsed);
	//}
}

void CObjectsShader::ReleaseUploadBuffers()
{
	if (m_pBillboardTree != nullptr)
	{
		for (int i = 0; i < m_nObjects; ++i)
		{
			m_pBillboardTree[i].ReleaseUploadBuffers();
		}
	}

	//if (m_ppObjects)
	//{
	//	for (int j = 0; j < m_nObjects; j++) 
	//		if (m_ppObjects[j]) m_ppObjects[j]->ReleaseUploadBuffers();
	//}

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
		//if (m_ppObjects[i]) 
		//	m_ppObjects[i]->Render(pd3dCommandList, pCamera);

		if (m_pBillboardTree != nullptr)
			m_pBillboardTree[i].Render(pd3dCommandList, pCamera);
	}
}
