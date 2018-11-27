#pragma once
#include "GameObject.h"

class CExplosionParticle : public CGameObject
{
public:
	CExplosionParticle();
	virtual ~CExplosionParticle();

	void Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle);

	virtual void Animate(float fElapsedTime);
	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

	float GetExplosionTime()	const { return m_ExplosionTime; }
	bool GetOnExplosion()		const { return m_OnExplosion; }

	float elapsedTime = 0.f;
	int AnimationFrame = 1;
private:
	float m_ExplosionTime = 0.f;
	bool m_OnExplosion = false;
};

