#pragma once
#include "UIShader.h"
#include "FontShader.h"
#include "TenthFontShader.h"
#include "MissileUIShader.h"

class CScene;
class CUIManager
{
public:
	CUIManager();
	~CUIManager();

	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseObjects();
	void Render(ID3D12GraphicsCommandList *pd3dCommandList);

	void SetScene(CScene* pScene) { m_pScene = pScene; }
	void SetCameraMode(DWORD CameraMode) { m_CameraMode = CameraMode; }

	CUIShader* GetUIShader()	const { return m_pUIShader; }

private:
	CScene*										m_pScene{ nullptr };
	DWORD										m_CameraMode{ THIRD_PERSON_CAMERA };
	CUIShader*									m_pUIShader{ nullptr };
	CFontShader*								m_pFontShader{ nullptr };
	CTenthFontShader*						m_pTenthFontShader{ nullptr };
	CMissileUIShader*						m_pMissileUIShader{ nullptr };
};

