#include "stdafx.h"
#include "ParticleShader.h"

CParticleShader::CParticleShader()
{
}

CParticleShader::~CParticleShader()
{
}

void CParticleShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	CShader::CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
}

D3D12_RASTERIZER_DESC CParticleShader::CreateRasterizerState()
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

D3D12_BLEND_DESC CParticleShader::CreateBlendState()
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

D3D12_SHADER_BYTECODE CParticleShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSFireParticle", "ps_5_1", ppd3dShaderBlob));
}

void CParticleShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes * m_nObjects, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
	m_pd3dcbGameObjects->Map(0, NULL, (void **)&m_pcbMappedGameObjects);

	// 텍스처 애니메이션
	//ncbElementBytes = ((sizeof(CB_TEXTURE_ANIMATION) + 255) & ~255); //256의 배수
	//m_pTextureAnimation = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes * m_nObjects, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
	//m_pTextureAnimation->Map(0, NULL, (void**)&m_pMappedTextureAnimation);
}

void CParticleShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	int i = 0;
	for (auto iter = m_FireParticleList.begin(); iter != m_FireParticleList.end(); ++iter)
	{
		// ★★★★★
		(*iter)->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
		CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedGameObjects + (i * ncbElementBytes));
		XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&(*iter)->m_xmf4x4World)));

		//(*iter)->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
		//CB_TEXTURE_ANIMATION* pMappedTextureAnimation = (CB_TEXTURE_ANIMATION *)((UINT8 *)m_pMappedTextureAnimation + (i * ncbElementBytes));
		//XMStoreFloat4x4(&pMappedTextureAnimation->m_elapsedTime, XMMatrixTranspose(XMLoadFloat4x4(&(*iter)->m_xmf4x4World)));
		++i;
	}


}

void CParticleShader::ReleaseShaderVariables()
{
	if (m_pd3dcbGameObjects)
	{
		m_pd3dcbGameObjects->Unmap(0, NULL);
		m_pd3dcbGameObjects->Release();
	}

	CTexturedShader::ReleaseShaderVariables();
}

void CParticleShader::ReleaseUploadBuffers()
{
	for (auto iter = m_FireParticleList.begin(); iter != m_FireParticleList.end(); ++iter)
		(*iter)->ReleaseUploadBuffers();
	
}

bool CParticleShader::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
			// 총알 발사 키
		case VK_CONTROL:
		{
			if (m_pBullet)
			{
				CGameObject* pParticle = new CParticle;
				pParticle->SetMesh(m_pFireParticleMesh);
				pParticle->SetMaterial(m_pFireParticleMaterial);
				pParticle->SetLook(m_pFramePlayer->GetLookVector());
				//// 또한 플레이어의 Up벡터, Right벡터도 똑같이 설정해주어야 플레이어가 회전했을 때,
				//// 총알 모양도 회전이 된 모양으로 바뀐다.
				pParticle->SetUp(m_pFramePlayer->GetUpVector());
				pParticle->SetRight(m_pFramePlayer->GetRightVector());
				//// 총알의 생성위치는 플레이어의 위치로 설정
				pParticle->SetPosition(m_pBullet->GetPosition().x, m_pBullet->GetPosition().y, m_pBullet->GetPosition().z);
				// 총알이 나아가는 방향은 총알이 바라보는 방향으로 준다.
				pParticle->SetMovingDirection(pParticle->GetLook());
				m_FireParticleList.push_back(pParticle);
				cout << "총알 파티클 생성 됨" << endl;
			}
			break;
		}
		default:
			break;
		}
		break;
	default:
		break;
	}
	return(false);
}

void CParticleShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	// 불꽃 파티클 메쉬 생성
	// 총알의 모델좌표계에서 -5만큼 뒤에 위치하게 생성
	// 총알의 두깨가 5이므로, 불꽃은 맨 뒤에 있어야한다.
	m_pFireParticleMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 2.5f, 2.5f, 0.0f, 0.0f, 0.0f, -5.0f);
	// 불꽃 파티클 텍스처 생성
	m_pFireParticleTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pFireParticleTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/FireParticle.dds", 0);
	// 불꽃 파티클 재질 생성
	m_pFireParticleMaterial = new CMaterial;
	m_pFireParticleMaterial->SetTexture(m_pFireParticleTexture);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);
	m_nObjects = 100;
	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, 1);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pFireParticleTexture, 17, false);

}
void CParticleShader::ReleaseObjects()
{
	for (auto iter = m_FireParticleList.begin(); iter != m_FireParticleList.end(); )
	{
		(*iter)->DeleteMesh();
		delete (*iter);
		iter = m_FireParticleList.erase(iter);
	}
	m_FireParticleList.clear();
}

void CParticleShader::AnimateObjects(float fTimeElapsed)
{
	double distance = 0.f;

	// 플레이어의 총알 리스트를 루프를 통해 순회하면서, 애니메이트 시켜준다.
	for (auto iter = m_FireParticleList.begin(); iter != m_FireParticleList.end();)
	{
		// 플레이어 위치와 총알의 위치 거리를 계산하는 공식이다. 
		distance = sqrt((pow(((*iter)->GetPosition().x - m_pFramePlayer->GetPosition().x), 2.0)
			+ pow(((*iter)->GetPosition().y - m_pFramePlayer->GetPosition().y), 2.0)
			+ pow(((*iter)->GetPosition().z - m_pFramePlayer->GetPosition().z), 2.0)));

		// 플레이어와 총알의 거리가 250m보다 커지면, 총알의 유효사거리를 벗어난거므로
			// 총알을 계속 그리지 않고, 지워주어야 프레임레이트를 올릴 수 있다.
		if (distance >= MaxBulletDistance)
		{
			cout << "플레이어 파티클 거리벗어남 삭제" << endl;
			delete (*iter);
			iter = m_FireParticleList.erase(iter);
		}
		else
		{
			((CParticle*)(*iter))->Animate(fTimeElapsed);
			++iter;
		}
	}
}


void CParticleShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CTexturedShader::Render(pd3dCommandList, pCamera);
	UpdateShaderVariables(pd3dCommandList);

	for (auto iter = m_FireParticleList.begin(); iter != m_FireParticleList.end(); ++iter)
	{
		((*iter))->Render(pd3dCommandList, pCamera);
	}
}
