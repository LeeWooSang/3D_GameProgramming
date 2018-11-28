#pragma once
#include "GameObject.h"

class CBullet : public CGameObject
{
public:
	CBullet();
	virtual ~CBullet();

	virtual void Move(XMFLOAT3& vDirection, float fSpeed);
	void Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle);

	virtual void Animate(float fElapsedTime);
	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

	void SetCollision(bool Collision) { m_Collision = Collision; }
	bool GetCollision()	const { return m_Collision; }

	void SetID(int id) { m_ID = id; }
	int GetID()	const { return m_ID; }

private:
	bool m_Collision = false;
	int m_ID;
};

