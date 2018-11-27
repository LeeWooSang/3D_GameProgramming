#pragma once
#include "Shader.h"
#include "Particle.h"
#include "FramePlayer.h"
#include "Bullet.h"

class CExplosionParticleShader : public CTexturedShader
{
public:
public:
	CExplosionParticleShader();
	virtual ~CExplosionParticleShader();

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void ReleaseUploadBuffers();

	void Initialize(XMFLOAT3 position);
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void ReleaseObjects();
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

	CFramePlayer* GetFramePlayer() const { return m_pFramePlayer; }
	void SetFramePlayer(CFramePlayer* p) { m_pFramePlayer = p; }

	void SetBullet(CGameObject* pBullet) { m_pBullet = pBullet; }

private:
	int											m_nObjects = 0;

	ID3D12Resource*					m_pd3dcbGameObjects = NULL;
	CB_GAMEOBJECT_INFO*		m_pcbMappedGameObjects = NULL;

	ID3D12Resource*					m_pTextureAnimation = NULL;
	CB_TEXTURE_ANIMATION*	m_pMappedTextureAnimation = NULL;

	CTexturedRectMesh*		m_pExplosionParticleMesh{ nullptr };
	CTexture*							m_pExplosionParticleTexture{ nullptr };
	CMaterial*						m_pExplosionParticleMaterial{ nullptr };
	list<CGameObject*>		m_ExplosionParticleList;

	CGameObject*					m_pBullet{ nullptr };
	CFramePlayer*					m_pFramePlayer{ nullptr };

	const double MaxBulletDistance = 250.f;
};

