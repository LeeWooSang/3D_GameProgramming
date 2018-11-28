#include "stdafx.h"
#include "ExplosionParticleShader.h"
#include "ExplosionParticle.h"

CExplosionParticleShader::CExplosionParticleShader()
{
}

CExplosionParticleShader::~CExplosionParticleShader()
{
}

void CExplosionParticleShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	CShader::CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
}

D3D12_RASTERIZER_DESC CExplosionParticleShader::CreateRasterizerState()
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

D3D12_BLEND_DESC CExplosionParticleShader::CreateBlendState()
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

D3D12_SHADER_BYTECODE CExplosionParticleShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSFireParticle", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CExplosionParticleShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSFireParticle", "ps_5_1", ppd3dShaderBlob));
}

void CExplosionParticleShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	// 텍스처 애니메이션
	UINT ncbElementBytes = ((sizeof(CB_TEXTURE_ANIMATION) + 255) & ~255); //256의 배수
	m_pTextureAnimation = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes * m_nObjects, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
	m_pTextureAnimation->Map(0, NULL, (void**)&m_pMappedTextureAnimation);
}

void CExplosionParticleShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	//UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);
	UINT ncbElementBytes = ((sizeof(CB_TEXTURE_ANIMATION) + 255) & ~255); //256의 배수
	int i = 0;
	for (auto iter = m_ExplosionParticleList.begin(); iter != m_ExplosionParticleList.end(); ++iter)
	{
		// ★★★★★
		(*iter)->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
		CB_TEXTURE_ANIMATION* pMappedTextureAnimation = (CB_TEXTURE_ANIMATION *)((UINT8 *)m_pMappedTextureAnimation + (i * ncbElementBytes));
		XMStoreFloat4x4(&pMappedTextureAnimation->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&(*iter)->m_xmf4x4World)));
		pMappedTextureAnimation->m_elapsedTime = ((CParticle*)(*iter))->elapsedTime;
		pMappedTextureAnimation->m_frameSheet = ((CParticle*)(*iter))->AnimationFrame;
		++i;
	}


}

void CExplosionParticleShader::ReleaseShaderVariables()
{
	if (m_pd3dcbGameObjects)
	{
		m_pd3dcbGameObjects->Unmap(0, NULL);
		m_pd3dcbGameObjects->Release();
	}

	if (m_pTextureAnimation)
	{
		m_pTextureAnimation->Unmap(0, NULL);
		m_pTextureAnimation->Release();
	}

	CTexturedShader::ReleaseShaderVariables();
}

void CExplosionParticleShader::ReleaseUploadBuffers()
{
	for (auto iter = m_ExplosionParticleList.begin(); iter != m_ExplosionParticleList.end(); ++iter)
		(*iter)->ReleaseUploadBuffers();

}

void CExplosionParticleShader::Initialize(XMFLOAT3 position)
{
	cout << "총알 터지는 파티클 생성" << endl;
	CGameObject* pExplosionParticle = new CExplosionParticle;
	pExplosionParticle->SetPosition(position);
	pExplosionParticle->SetMesh(m_pExplosionParticleMesh);
	pExplosionParticle->SetMaterial(m_pExplosionParticleMaterial);
	// 리스트에 파티클 넣어줌
	m_ExplosionParticleList.push_back(pExplosionParticle);
}
void CExplosionParticleShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	// 불꽃 파티클 메쉬 생성
	m_pExplosionParticleMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 10.f, 10.f, 0.0f, 0.0f, 0.0f, 0.0f);

	// 불꽃 파티클 텍스처 생성
	m_pExplosionParticleTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pExplosionParticleTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/FireParticle.dds", 0);
	// 불꽃 파티클 재질 생성
	m_pExplosionParticleMaterial = new CMaterial;
	m_pExplosionParticleMaterial->SetTexture(m_pExplosionParticleTexture);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);
	m_nObjects = 100;
	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, 1);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pTextureAnimation, ncbElementBytes);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pExplosionParticleTexture, 17, false);

}
void CExplosionParticleShader::ReleaseObjects()
{
	for (auto iter = m_ExplosionParticleList.begin(); iter != m_ExplosionParticleList.end(); )
	{
		(*iter)->DeleteMesh();
		delete (*iter);
		iter = m_ExplosionParticleList.erase(iter);
	}
	m_ExplosionParticleList.clear();
}

void CExplosionParticleShader::AnimateObjects(float fTimeElapsed)
{
	for (auto iter = m_ExplosionParticleList.begin(); iter != m_ExplosionParticleList.end(); )
	{
		if (((CExplosionParticle*)(*iter))->GetOnExplosion() == true)
		{
			delete (*iter);
			iter = m_ExplosionParticleList.erase(iter);
		}
		else
		{
			(*iter)->Animate(fTimeElapsed);
			++iter;
		}
	}
	//double distance = 0.f;

	//// 플레이어의 총알 리스트를 루프를 통해 순회하면서, 애니메이트 시켜준다.
	//for (auto iter = m_ExplosionParticleList.begin(); iter != m_ExplosionParticleList.end();)
	//{
	//	// 플레이어 위치와 총알의 위치 거리를 계산하는 공식이다. 
	//	distance = sqrt((pow(((*iter)->GetPosition().x - m_pFramePlayer->GetPosition().x), 2.0)
	//		+ pow(((*iter)->GetPosition().y - m_pFramePlayer->GetPosition().y), 2.0)
	//		+ pow(((*iter)->GetPosition().z - m_pFramePlayer->GetPosition().z), 2.0)));

	//	// 플레이어와 총알의 거리가 250m보다 커지면, 총알의 유효사거리를 벗어난거므로
	//		// 총알을 계속 그리지 않고, 지워주어야 프레임레이트를 올릴 수 있다.
	//	if (distance >= MaxBulletDistance)
	//	{
	//		cout << "총알 터지는 파티클 거리벗어남 삭제" << endl;
	//		delete (*iter);
	//		iter = m_ExplosionParticleList.erase(iter);
	//	}
	//	else
	//	{
	//		((CParticle*)(*iter))->Animate(fTimeElapsed);
	//		++iter;
	//	}
	//}
}


void CExplosionParticleShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CTexturedShader::Render(pd3dCommandList, pCamera);
	UpdateShaderVariables(pd3dCommandList);
	XMFLOAT3 xmf3CameraPosition = pCamera->GetPosition();
	for (auto iter = m_ExplosionParticleList.begin(); iter != m_ExplosionParticleList.end(); ++iter)
	{
		(*iter)->SetLookAt(xmf3CameraPosition, XMFLOAT3(0.f, 1.f, 0.f));
		((*iter))->Render(pd3dCommandList, pCamera);
	}
}
