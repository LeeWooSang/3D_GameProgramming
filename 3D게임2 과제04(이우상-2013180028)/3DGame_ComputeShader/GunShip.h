#pragma once
#include "FrameObject.h"

class CGunShip : public CFrameObject
{
public:
	CGunShip(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~CGunShip();

private:
	CFrameObject					*m_pMainRotorFrame = NULL;
	CFrameObject					*m_pTailRotorFrame = NULL;

public:
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4 *pxmf4x4Parent = NULL);
};
