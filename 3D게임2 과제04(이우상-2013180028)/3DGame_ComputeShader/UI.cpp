#include "Stdafx.h"
#include "UI.h"
#include "UIShader.h"

CUI::CUI()
{
}

CUI::~CUI()
{
}

void CUI::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	if (m_pMaterial)
	{
		if (m_pMaterial->m_pShader)
		{
			m_pMaterial->m_pShader->Render(pd3dCommandList, pCamera);
			m_pMaterial->m_pShader->UpdateShaderVariables(pd3dCommandList);

			UpdateShaderVariables(pd3dCommandList);
		}
		if (m_pMaterial->m_pTexture)
		{
			m_pMaterial->m_pTexture->UpdateShaderVariables(pd3dCommandList);
		}
	}
	if (m_pMesh)
	{
		m_pMesh->Render(pd3dCommandList);
	}
}