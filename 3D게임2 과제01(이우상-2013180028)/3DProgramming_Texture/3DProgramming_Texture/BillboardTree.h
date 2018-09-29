#pragma once

#include "GameObject.h"
#include "Camera.h"

class CBillboardTree : public CGameObject
{
public:
	CBillboardTree();
	virtual ~CBillboardTree();

	void SetMesh(CMesh *pMesh);

	void SetLookAt(XMFLOAT3&);

	virtual void Render(ID3D12GraphicsCommandList*, CCamera* pCamera = NULL);
	void Animate(float, CCamera*);
};

