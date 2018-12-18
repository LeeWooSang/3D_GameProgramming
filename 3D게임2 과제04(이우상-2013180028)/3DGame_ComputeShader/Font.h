#pragma once
#include "GameObject.h"

class CFont : public CGameObject
{
public:
	CFont();
	virtual ~CFont();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera = NULL);
};

