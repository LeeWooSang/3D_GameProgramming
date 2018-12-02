#include "stdafx.h"
#include "BillboardObjectsShader.h"
#include "BillboardObject.h"
#include "Terrain.h"

CBillboardObjectsShader::CBillboardObjectsShader()
{
}

CBillboardObjectsShader::~CBillboardObjectsShader()
{
}

D3D12_RASTERIZER_DESC CBillboardObjectsShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	//	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
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

D3D12_BLEND_DESC CBillboardObjectsShader::CreateBlendState()
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

D3D12_SHADER_BYTECODE CBillboardObjectsShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSBillboard", "ps_5_1", ppd3dShaderBlob));
}

void CBillboardObjectsShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256ÀÇ ¹è¼ö
	m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes * m_nObjects, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbGameObjects->Map(0, NULL, (void **)&m_pcbMappedGameObjects);
}

void CBillboardObjectsShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);
	for (int j = 0; j < m_nObjects; j++)
	{
		CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedGameObjects + (j * ncbElementBytes));
		XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->m_xmf4x4World)));
	}
}

void CBillboardObjectsShader::ReleaseShaderVariables()
{
	if (m_pd3dcbGameObjects)
	{
		m_pd3dcbGameObjects->Unmap(0, NULL);
		m_pd3dcbGameObjects->Release();
	}

	CTexturedShader::ReleaseShaderVariables();
}

void CBillboardObjectsShader::ReleaseUploadBuffers()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++)
			if (m_ppObjects[j])
				m_ppObjects[j]->ReleaseUploadBuffers();
	}
}

void CBillboardObjectsShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
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

	CTexture *ppTreeTextures[3];
	ppTreeTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTreeTextures[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Tree01.dds", 0);
	ppTreeTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTreeTextures[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Tree02.dds", 0);
	ppTreeTextures[2] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTreeTextures[2]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Tree03.dds", 0);

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

	CMaterial *ppTreeMaterials[3];
	ppTreeMaterials[0] = new CMaterial();
	ppTreeMaterials[0]->SetTexture(ppTreeTextures[0]);
	ppTreeMaterials[1] = new CMaterial();
	ppTreeMaterials[1]->SetTexture(ppTreeTextures[1]);
	ppTreeMaterials[2] = new CMaterial();
	ppTreeMaterials[2]->SetTexture(ppTreeTextures[2]);

	CTexturedRectMesh *pGrassMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 8.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	CTexturedRectMesh *pFlowerMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 8.0f, 16.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	CTexturedRectMesh *pTreeMesh01 = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 24.0f, 36.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	CTexturedRectMesh *pTreeMesh02 = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 16.0f, 46.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	CRawFormatImage *pRawFormatImage = new CRawFormatImage(L"Image/ObjectsMap.raw", 257, 257, true);

	int nGrassObjects = 0, nFlowerObjects = 0, nBlacks = 0, nOthers = 0, nTreeObjects[3] = { 0, 0, 0 };
	for (int z = 2; z <= 254; z++)
	{
		for (int x = 2; x <= 254; x++)
		{
			BYTE nPixel = pRawFormatImage->GetRawImagePixel(x, z);
			switch (nPixel)
			{
			case 102: nGrassObjects++; break;
			case 128: nGrassObjects++; break;
			case 153: nFlowerObjects++; break;
			case 179: nFlowerObjects++; break;
			case 204: nTreeObjects[0]++; break;
			case 225: nTreeObjects[1]++; break;
			case 255: nTreeObjects[2]++; break;
			case 0: nBlacks++; break;
			default: nOthers++; break;
			}
		}
	}
	m_nObjects = nGrassObjects + nFlowerObjects + nTreeObjects[0] + nTreeObjects[1] + nTreeObjects[2];

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, 7);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppGrassTextures[0], 3, true);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppGrassTextures[1], 3, true);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppFlowerTextures[0], 3, true);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppFlowerTextures[1], 3, true);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppTreeTextures[0], 3, true);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppTreeTextures[1], 3, true);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppTreeTextures[2], 3, true);

	CTerrain *pTerrain = (CTerrain*)pContext;

	int nTerrainWidth = int(pTerrain->GetWidth());
	int nTerrainLength = int(pTerrain->GetLength());

	XMFLOAT3 xmf3Scale = pTerrain->GetScale();

	m_ppObjects = new CGameObject*[m_nObjects];

	CBillboardObject* pBillboardObject = NULL;
	for (int nObjects = 0, z = 2; z <= 254; z++)
	{
		for (int x = 2; x <= 254; x++)
		{
			BYTE nPixel = pRawFormatImage->GetRawImagePixel(x, z);

			float fyOffset = 0.0f;

			CMaterial *pMaterial = NULL;
			CMesh *pMesh = NULL;

			switch (nPixel)
			{
			case 102:
				pMesh = pGrassMesh;
				pMaterial = ppGrassMaterials[0];
				fyOffset = 8.0f * 0.5f;
				break;
			case 128:
				pMesh = pGrassMesh;
				pMaterial = ppGrassMaterials[1];
				fyOffset = 6.0f * 0.5f;
				break;
			case 153:
				pMesh = pFlowerMesh;
				pMaterial = ppFlowerMaterials[0];
				fyOffset = 16.0f * 0.5f;
				break;
			case 179:
				pMesh = pFlowerMesh;
				pMaterial = ppFlowerMaterials[1];
				fyOffset = 16.0f * 0.5f;
				break;
			case 204:
				pMesh = pTreeMesh01;
				pMaterial = ppTreeMaterials[0];
				fyOffset = 33.0f * 0.5f;
				break;
			case 225:
				pMesh = pTreeMesh01;
				pMaterial = ppTreeMaterials[1];
				fyOffset = 33.0f * 0.5f;
				break;
			case 255:
				pMesh = pTreeMesh02;
				pMaterial = ppTreeMaterials[2];
				fyOffset = 40.0f * 0.5f;
				break;
			default:
				break;
			}

			if (pMesh && pMaterial)
			{
				pBillboardObject = new CBillboardObject();

				pBillboardObject->SetMesh(0, pMesh);
				pBillboardObject->SetMaterial(pMaterial);

				float xPosition = x * xmf3Scale.x;
				float zPosition = z * xmf3Scale.z;
				float fHeight = pTerrain->GetHeight(xPosition, zPosition);
				pBillboardObject->SetPosition(xPosition, fHeight + fyOffset, zPosition);
				pBillboardObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nObjects));

				m_ppObjects[nObjects++] = pBillboardObject;
			}
		}
	}
}


void CBillboardObjectsShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++)
		{
			if (m_ppObjects[j])
			{
				m_ppObjects[j]->DeleteMesh();
				delete m_ppObjects[j];
			}
		}
		delete[] m_ppObjects;
	}
}

void CBillboardObjectsShader::AnimateObjects(float fTimeElapsed)
{
	for (int j = 0; j < m_nObjects; j++)
	{
		m_ppObjects[j]->Animate(fTimeElapsed);
	}
}

void CBillboardObjectsShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CTexturedShader::Render(pd3dCommandList, pCamera);
	UpdateShaderVariables(pd3dCommandList);

	XMFLOAT3 xmf3CameraPosition = pCamera->GetPosition();
	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			m_ppObjects[j]->SetLookAt(xmf3CameraPosition, XMFLOAT3(0.0f, 1.0f, 0.0f));
			m_ppObjects[j]->Render(pd3dCommandList, pCamera);
		}
	}
}
