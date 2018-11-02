#pragma once

#include "Shader.h"

class CHeightMapTerrain;
class CSkyBox;
class CScene
{
public:
    CScene();
    ~CScene();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseObjects();

	virtual ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	virtual ID3D12RootSignature *GetGraphicsRootSignature() { return(m_pd3dGraphicsRootSignature); }
	virtual void SetGraphicsRootSignature(ID3D12GraphicsCommandList *pd3dCommandList) { pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature); }

	bool ProcessInput(UCHAR *pKeysBuffer);
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera=NULL);

	virtual void ReleaseUploadBuffers();

	virtual CHeightMapTerrain *GetTerrain() { return(m_pTerrain); }

protected:
	CShader						**m_ppShaders = NULL;
	int							m_nShaders = 0;

	CHeightMapTerrain			*m_pTerrain = NULL;
	CSkyBox						*m_pSkyBox = NULL;

	ID3D12RootSignature			*m_pd3dGraphicsRootSignature = NULL;
};
