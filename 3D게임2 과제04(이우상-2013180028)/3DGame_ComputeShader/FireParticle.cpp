#include "Stdafx.h"
#include "FireParticle.h"
#include "FireParticleShader.h"

CFireParticle::CFireParticle()
{
	m_xmf3RotationAxis = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_xmf3MovingDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_fRotationSpeed = 900.f;
	m_fMovingSpeed = 120.f;
	//m_fMovingSpeed = 10.f;
	SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 1.5f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

CFireParticle::~CFireParticle()
{
}

void CFireParticle::Move(XMFLOAT3& vDirection, float fSpeed)
{
	SetPosition(m_xmf4x4World._41 + vDirection.x * fSpeed, m_xmf4x4World._42 + vDirection.y * fSpeed, m_xmf4x4World._43 + vDirection.z * fSpeed);
}

void CFireParticle::Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationAxis(xmf3RotationAxis, fAngle);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CFireParticle::Animate(float fElapsedTime)
{
	elapsedTime += fElapsedTime * 5;

	// 총알을 자전 시키기 위해서는 이동이 이루어지기 전에 회전이 먼저 일어나야 한다.
	// 만약, 이동을 먼저 시키고, 회전을 하게되면, 공전이 일어나게 된다. 따라서 총알의 월드변환행렬에 회전을 먼저 시켜주고,
	// 이동을 시켜주어야 자전이 된다.
	// 총알의 회전속도가 0이 아니면, 회전을 한다.
	if (m_fRotationSpeed != 0.0f)
		Rotate(m_xmf3RotationAxis, m_fRotationSpeed * fElapsedTime);
	// 총알의 이동속도가 0이 아니면 이동을 한다.
	if (m_fMovingSpeed != 0.0f)
		Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);
	// 총알의 충돌박스를 계속 애니메이트 해주어야 총알의 위치가 변함에따라 충돌 박스도 같이 위치가 변한다.
	//m_xmOOBBTransformed.Transform(m_xmOOBB, XMLoadFloat4x4(&m_xmf4x4World));
	//XMStoreFloat4(&m_xmOOBBTransformed.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOOBBTransformed.Orientation)));
}

void CFireParticle::OnPrepareRender()
{
}

void CFireParticle::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
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

	//pd3dCommandList->SetGraphicsRootDescriptorTable(2, m_d3dCbvGPUDescriptorHandle);
	pd3dCommandList->SetGraphicsRootDescriptorTable(18, m_d3dCbvGPUDescriptorHandle);

	if (m_pMesh)
	{
		m_pMesh->Render(pd3dCommandList);
	}
}