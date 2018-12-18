#pragma once
#include "TexturedShader.h"
#include "FireParticle.h"
#include "FramePlayer.h"

class CFireParticleShader : public CTexturedShader
{
public:
	CFireParticleShader();
	virtual ~CFireParticleShader();

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void ReleaseUploadBuffers();

	bool Initialize(CGameObject* pBullet, int id);
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void ReleaseObjects();
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

	CFramePlayer* GetFramePlayer() const { return m_pFramePlayer; }
	void SetFramePlayer(CFramePlayer* p) { m_pFramePlayer = p; }
	void SetDeleteFireParticleID(int id) { m_DeleteID = id; }
private:
	int											m_nObjects = 0;

	ID3D12Resource*					m_pTextureAnimation = NULL;
	CB_TEXTURE_ANIMATION*	m_pMappedTextureAnimation = NULL;

	CTexturedRectMesh*		m_pFireParticleMesh{ nullptr };
	CTexture*							m_pFireParticleTexture{ nullptr };
	CMaterial*						m_pFireParticleMaterial{ nullptr };
	list<CGameObject*>		m_FireParticleList;

	CFramePlayer*					m_pFramePlayer{ nullptr };

	const double					MaxBulletDistance = 250.f;
		
	int										m_DeleteID = 0;
};

