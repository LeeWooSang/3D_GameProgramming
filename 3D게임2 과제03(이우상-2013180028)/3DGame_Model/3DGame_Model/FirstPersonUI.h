#pragma once
#include "GameObject.h"

class CFirstPersonUI : public CGameObject
{
public:
	CFirstPersonUI();
	virtual ~CFirstPersonUI();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera = NULL);
};

