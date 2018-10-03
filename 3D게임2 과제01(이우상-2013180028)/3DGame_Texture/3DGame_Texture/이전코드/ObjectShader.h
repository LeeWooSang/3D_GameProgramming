#pragma once
#include "TexturedShader.h"
#include "TexturedRectMesh.h"

class CObjectsShader : public CTexturedShader
{
public:
	CObjectsShader();
	virtual ~CObjectsShader();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void AnimateObjects(float fTimeElapsed, CCamera* pCamera);
	virtual void ReleaseObjects();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBuffers();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

protected:
	CGameObject**	m_ppObjects = 0;
	int							m_nObjects = 0;

	ID3D12Resource*					m_pd3dcbGameObjects = NULL;
	CB_GAMEOBJECT_INFO*		m_pcbMappedGameObjects = NULL;

	CTexturedRectMesh*	m_pTexturedRectMesh = nullptr;
	CGameObject*				m_pBillboardTree = nullptr;

	list<CMaterial*>			m_list_pMaterial;
//#ifdef _WITH_BATCH_MATERIAL
//	CMaterial*	m_pMaterial = NULL;
//#endif
};

