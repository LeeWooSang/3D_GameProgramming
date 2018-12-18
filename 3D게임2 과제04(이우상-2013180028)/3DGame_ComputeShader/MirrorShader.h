#pragma once
#include "TexturedShader.h"

class CMirrorShader : public CTexturedShader
{
public:
	CMirrorShader();
	virtual ~CMirrorShader();

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	D3D12_DEPTH_STENCIL_DESC CreateReflectionDepthStencilState();

	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();

	virtual D3D12_BLEND_DESC CreateBlendState();
	D3D12_BLEND_DESC CreateTransparentBlendState();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void ReleaseShaderVariables();
	virtual void ReleaseUploadBuffers();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void ReleaseObjects();

	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList, int nPipelineState);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

private:
	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle;
};

