#pragma once
#include "GameObject.h"

struct CB_TEXTURE_ANIMATION
{
	float	m_elapsedTime = 0.f;
};

class CParticle : public CGameObject
{
public:
	CParticle();
	virtual ~CParticle();

	virtual void Move(XMFLOAT3& vDirection, float fSpeed);
	void Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle);

	virtual void Animate(float fElapsedTime);
	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
};

