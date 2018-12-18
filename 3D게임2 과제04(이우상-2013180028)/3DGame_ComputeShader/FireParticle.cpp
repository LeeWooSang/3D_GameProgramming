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

	// �Ѿ��� ���� ��Ű�� ���ؼ��� �̵��� �̷������ ���� ȸ���� ���� �Ͼ�� �Ѵ�.
	// ����, �̵��� ���� ��Ű��, ȸ���� �ϰԵǸ�, ������ �Ͼ�� �ȴ�. ���� �Ѿ��� ���庯ȯ��Ŀ� ȸ���� ���� �����ְ�,
	// �̵��� �����־�� ������ �ȴ�.
	// �Ѿ��� ȸ���ӵ��� 0�� �ƴϸ�, ȸ���� �Ѵ�.
	if (m_fRotationSpeed != 0.0f)
		Rotate(m_xmf3RotationAxis, m_fRotationSpeed * fElapsedTime);
	// �Ѿ��� �̵��ӵ��� 0�� �ƴϸ� �̵��� �Ѵ�.
	if (m_fMovingSpeed != 0.0f)
		Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);
	// �Ѿ��� �浹�ڽ��� ��� �ִϸ���Ʈ ���־�� �Ѿ��� ��ġ�� ���Կ����� �浹 �ڽ��� ���� ��ġ�� ���Ѵ�.
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