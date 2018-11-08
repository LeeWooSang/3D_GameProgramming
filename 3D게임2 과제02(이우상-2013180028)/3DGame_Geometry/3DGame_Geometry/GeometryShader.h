#pragma once
#include "ObjectsShader.h"

class CGeometryShader : public CTexturedShader
{
public:
	CGeometryShader();
	virtual ~CGeometryShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);

	virtual D3D12_SHADER_BYTECODE CreateGeometryShader(ID3DBlob **ppd3dShaderBlob);

	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);

	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

	virtual void ReleaseShaderVariables();
	virtual void ReleaseUploadBuffers();

	// 빌보드 정점 리소스 생성
	ID3D12Resource*						m_pd3dVertexUploadBuffer = nullptr;
	ID3D12Resource*						m_pd3dVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_d3dVertexBufferView;
	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dCbvGPUDescriptorHandle;

	int									m_nVertices;
	int									m_nStride;
	CMaterial*					m_pMaterial[5];

	//CMaterial*					m_pMaterial;
#ifdef _WITH_BATCH_MATERIAL
	CMaterial						*m_ppGrassMaterials[2] = { NULL, NULL };
	CMaterial						*m_ppFlowerMaterials[2] = { NULL, NULL };
#endif
};

