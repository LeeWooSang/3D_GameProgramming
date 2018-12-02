#pragma once
#include "GameObject.h"

class CBillboardObject : public CGameObject
{
public:
	CBillboardObject();
	virtual ~CBillboardObject();

	virtual void Animate(float fTimeElapsed);

	float m_fRotationAngle = 0.0f;
	float m_fRotationDelta = 1.0f;
};

