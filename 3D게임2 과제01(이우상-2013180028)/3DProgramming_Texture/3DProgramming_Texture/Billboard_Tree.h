#pragma once

#include "GameObject.h"
#include "Camera.h"

class CBillboard_Tree : public CGameObject
{
public:
	CBillboard_Tree();
	virtual ~CBillboard_Tree();

	void Animate(float, CCamera*);
	void SetLookAt(XMFLOAT3&);
};

