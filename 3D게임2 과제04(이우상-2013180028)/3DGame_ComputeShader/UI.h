#pragma once
#include "GameObject.h"

class CUI : public CGameObject
{
public:
	CUI();
	virtual ~CUI();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera = NULL);
};

