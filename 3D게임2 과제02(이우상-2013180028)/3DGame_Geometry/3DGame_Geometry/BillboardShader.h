#pragma once
#include "ObjectsShader.h"

class CBillboardShader : public CObjectsShader
{
public:
	CBillboardShader();
	virtual ~CBillboardShader();

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);

	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

	virtual void ReleaseObjects();
	virtual void ReleaseUploadBuffers();

#ifdef _WITH_BATCH_MATERIAL
	CMaterial						*m_ppGrassMaterials[2] = { NULL, NULL };
	CMaterial						*m_ppFlowerMaterials[2] = { NULL, NULL };
#endif
};
