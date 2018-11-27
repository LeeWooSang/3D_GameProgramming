#pragma once
#include "GameObject.h"

struct CB_TEXTURE_ANIMATION
{
	XMFLOAT4X4	m_xmf4x4World;
	UINT					m_elapsedTime = 0;
	int						m_frameSheet = 1;
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

	float elapsedTime = 0.f;
	//int elapsedTime = 0;
	int AnimationFrame = 1;
};

