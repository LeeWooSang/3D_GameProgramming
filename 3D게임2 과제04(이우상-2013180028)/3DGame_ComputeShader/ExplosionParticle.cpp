#include "Stdafx.h"
#include "ExplosionParticle.h"
#include "ExplosionParticleShader.h"

CExplosionParticle::CExplosionParticle()
{
	m_xmf3RotationAxis = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_fRotationSpeed = 900.f;
	SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 1.5f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

CExplosionParticle::~CExplosionParticle()
{
}

void CExplosionParticle::Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationAxis(xmf3RotationAxis, fAngle);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CExplosionParticle::Animate(float fElapsedTime)
{
	elapsedTime += fElapsedTime * 5;
	m_ExplosionTime += fElapsedTime;
	if (m_ExplosionTime > 1.f)
	{
		m_OnExplosion = true;
	}

	if (m_fRotationSpeed != 0.0f)
		Rotate(m_xmf3RotationAxis, m_fRotationSpeed * fElapsedTime);
	// 총알의 충돌박스를 계속 애니메이트 해주어야 총알의 위치가 변함에따라 충돌 박스도 같이 위치가 변한다.
	//m_xmOOBBTransformed.Transform(m_xmOOBB, XMLoadFloat4x4(&m_xmf4x4World));
	//XMStoreFloat4(&m_xmOOBBTransformed.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOOBBTransformed.Orientation)));
}

void CExplosionParticle::OnPrepareRender()
{
}

void CExplosionParticle::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	OnPrepareRender();

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

	pd3dCommandList->SetGraphicsRootDescriptorTable(18, m_d3dCbvGPUDescriptorHandle);

	if (m_pMesh)
	{
		m_pMesh->Render(pd3dCommandList);
	}
}