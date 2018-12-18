#include "Stdafx.h"
#include "UIManager.h"
#include "Scene.h"

extern int g_GameState;

CUIManager::CUIManager()
{
}

CUIManager::~CUIManager()
{
}

void CUIManager::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_pUIShader = new CUIShader;
	m_pUIShader->CreateShader(pd3dDevice, pd3dCommandList, m_pScene->GetGraphicsRootSignature());
	m_pUIShader->BuildObjects(pd3dDevice, pd3dCommandList);
	m_pUIShader->Initialize(GAMESTART);

	m_pFontShader = new CFontShader;
	m_pFontShader->CreateShader(pd3dDevice, pd3dCommandList, m_pScene->GetGraphicsRootSignature());
	m_pFontShader->BuildObjects(pd3dDevice, pd3dCommandList);
	m_pFontShader->Initialize(ZERO);
	m_pScene->SetFontShader(m_pFontShader);

	m_pTenthFontShader = new CTenthFontShader;
	m_pTenthFontShader->CreateShader(pd3dDevice, pd3dCommandList, m_pScene->GetGraphicsRootSignature());
	m_pTenthFontShader->BuildObjects(pd3dDevice, pd3dCommandList);
	m_pTenthFontShader->Initialize(ZERO);
	m_pScene->SetTenthFontShader(m_pTenthFontShader);

	m_pMissileUIShader = new CMissileUIShader;
	m_pMissileUIShader->CreateShader(pd3dDevice, pd3dCommandList, m_pScene->GetGraphicsRootSignature());
	m_pMissileUIShader->BuildObjects(pd3dDevice, pd3dCommandList);
	m_pMissileUIShader->Initialize(0);
}

void CUIManager::ReleaseObjects()
{
	if (m_pUIShader)
	{
		m_pUIShader->ReleaseUploadBuffers();
		m_pUIShader->ReleaseShaderVariables();
		m_pUIShader->ReleaseObjects();
		delete m_pUIShader;
	}

	if (m_pFontShader)
	{
		m_pFontShader->ReleaseUploadBuffers();
		m_pFontShader->ReleaseShaderVariables();
		m_pFontShader->ReleaseObjects();
		delete m_pFontShader;
	}

	if (m_pTenthFontShader)
	{
		m_pTenthFontShader->ReleaseUploadBuffers();
		m_pTenthFontShader->ReleaseShaderVariables();
		m_pTenthFontShader->ReleaseObjects();
		delete m_pTenthFontShader;
	}

	if (m_pMissileUIShader)
	{
		m_pMissileUIShader->ReleaseUploadBuffers();
		m_pMissileUIShader->ReleaseShaderVariables();
		m_pMissileUIShader->ReleaseObjects();
		delete m_pMissileUIShader;
	}
}

void CUIManager::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
	if (m_pFontShader)
		m_pFontShader->Render(pd3dCommandList, NULL, ZERO);

	if (m_pTenthFontShader)
		m_pTenthFontShader->Render(pd3dCommandList, NULL, ZERO);

	if (m_pMissileUIShader)
		m_pMissileUIShader->Render(pd3dCommandList, NULL, 0);

	if (m_pUIShader)
	{
		if (m_CameraMode == FIRST_PERSON_CAMERA)
			m_pUIShader->Render(pd3dCommandList, NULL, FIRST_PERSON);
		else
			m_pUIShader->Render(pd3dCommandList, NULL, g_GameState);
	}

}
