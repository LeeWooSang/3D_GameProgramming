#pragma once
#include "Shader.h"
#include "Bullet.h"
#include "FramePlayer.h"

class CBulletShader : public CTexturedShader
{
public:
	CBulletShader();
	virtual ~CBulletShader();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	virtual void ReleaseUploadBuffers();

	bool CBulletShader::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	virtual void ReleaseObjects();

	CFramePlayer* GetFramePlayer() const { return m_pFramePlayer; }
	void SetFramePlayer(CFramePlayer* p) { m_pFramePlayer = p; }

	CGameObject* GetBullet()			const { return m_pBullet; }

private:
	int											m_nObjects = 0;

	ID3D12Resource*					m_pd3dcbGameObjects = NULL;
	CB_GAMEOBJECT_INFO*		m_pcbMappedGameObjects = NULL;

	CGameObject*					m_pBullet{ nullptr };
	CCubeMeshTextured*		m_pBulletTexturedMesh{ nullptr };
	CTexture*							m_pBulletTexture{ nullptr };
	CMaterial*						m_pBulletMaterial{ nullptr };
	CFramePlayer*					m_pFramePlayer{ nullptr };
	list<CGameObject*>		m_BulletList;

	const double MaxBulletDistance = 250.f;
};

