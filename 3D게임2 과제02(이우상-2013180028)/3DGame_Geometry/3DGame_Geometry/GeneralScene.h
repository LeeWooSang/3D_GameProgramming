#pragma once
#include "Scene.h"

class CGeneralScene : public CScene
{
public:
	CGeneralScene();
	~CGeneralScene();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseObjects();

	virtual ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	virtual ID3D12RootSignature *GetGraphicsRootSignature() { return(m_pd3dGraphicsRootSignature); }
	virtual void SetGraphicsRootSignature(ID3D12GraphicsCommandList *pd3dCommandList) { pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature); }

	virtual void AnimateObjects(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera = NULL);

	virtual void ReleaseUploadBuffers();

	virtual CHeightMapTerrain* GetTerrain() { return(m_pTerrain); }
};

