#include "stdafx.h"
#include "Billboard_Tree.h"

CBillboard_Tree::CBillboard_Tree()
{
}

CBillboard_Tree::~CBillboard_Tree()
{
}

void CBillboard_Tree::Animate(float fTimeElapsed, CCamera* pCamera)
{
	XMFLOAT3 xmf3CameraPosition = pCamera->GetPosition();
	SetLookAt(xmf3CameraPosition);
}

void CBillboard_Tree::SetLookAt(XMFLOAT3& xmf3Target)
{
	XMFLOAT3 xmf3Position(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
	
	XMFLOAT3 xmf3Up(0.0f, 1.0f, 0.0);
	XMFLOAT3 xmf3Look = Vector3::SubtractNormalize(xmf3Target, xmf3Position, true);
	XMFLOAT3 xmf3Right = Vector3::CrossProduct(xmf3Up, xmf3Look, true);

	m_xmf4x4World._11 = xmf3Right.x;
	m_xmf4x4World._12 = xmf3Right.y;
	m_xmf4x4World._13 = xmf3Right.z;

	m_xmf4x4World._21 = xmf3Up.x;
	m_xmf4x4World._22 = xmf3Up.y;
	m_xmf4x4World._23 = xmf3Up.z;

	m_xmf4x4World._31 = xmf3Look.x;
	m_xmf4x4World._32 = xmf3Look.y;
	m_xmf4x4World._33 = xmf3Look.z;
}