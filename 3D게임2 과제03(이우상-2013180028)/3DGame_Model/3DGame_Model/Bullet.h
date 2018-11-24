#pragma once
#include "GameObject.h"

class CBullet : public CGameObject
{
public:
	CBullet();
	~CBullet();

	CMesh* GetMesh()	const { return m_pMesh; }

	virtual void Move(XMFLOAT3& vDirection, float fSpeed);
	void Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle);

	virtual void Animate(float fElapsedTime);
	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
};

