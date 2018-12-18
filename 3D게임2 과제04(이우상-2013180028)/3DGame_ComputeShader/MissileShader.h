#pragma once
#include "TexturedShader.h"
#include "Missile.h"
#include "FramePlayer.h"

#include "FireParticleShader.h"
#include "FireParticle.h"

#include "ExplosionParticle.h"
#include "ExplosionParticleShader.h"

#include "FontShader.h"
#include "TenthFontShader.h"

class CMissileShader : public CTexturedShader
{
public:
	CMissileShader();
	virtual ~CMissileShader();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	virtual void ReleaseUploadBuffers();

	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	virtual void ReleaseObjects();

	CFramePlayer* GetFramePlayer() const { return m_pFramePlayer; }
	void SetFramePlayer(CFramePlayer* p) { m_pFramePlayer = p; }

	list<CGameObject*>& GetMissileList() { return m_MissileList; }

	void SetParticleShader(CFireParticleShader* pFireParticleShader) { m_pFireParticleShader = pFireParticleShader; }
	void SetExplosionParticleShader(CExplosionParticleShader* pExplosionParticleShader) { m_pExplosionParticleShader = pExplosionParticleShader; }

	void SetFontShader(CFontShader* pFontShader) { m_pFontShader = pFontShader; }
	void SetTenthFontShader(CTenthFontShader* pTenthFontShader) { m_pTenthFontShader = pTenthFontShader; }

private:
	int											m_nObjects = 0;

	ID3D12Resource*					m_pd3dcbGameObjects = NULL;
	CB_GAMEOBJECT_INFO*		m_pcbMappedGameObjects = NULL;

	CCubeMeshTextured*		m_pMissileTexturedMesh{ nullptr };
	CTexture*							m_pMissileTexture{ nullptr };
	CMaterial*						m_pMissileMaterial{ nullptr };
	CFramePlayer*					m_pFramePlayer{ nullptr };
	list<CGameObject*>		m_MissileList;

	const double					MaxMissileDistance = 250.f;
	static int							m_MissileCount;
	CFireParticleShader*			m_pFireParticleShader{ nullptr };
	CFireParticle*					m_pFireParticle{ nullptr };

	CExplosionParticleShader*			m_pExplosionParticleShader{ nullptr };
	CExplosionParticle*						m_pExplosionParticle{ nullptr };

	CFontShader*								m_pFontShader{ nullptr };
	CTenthFontShader*						m_pTenthFontShader{ nullptr };
};

