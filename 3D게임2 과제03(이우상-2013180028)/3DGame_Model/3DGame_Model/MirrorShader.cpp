#include "stdafx.h"
#include "MirrorShader.h"

CMirrorShader::CMirrorShader()
{
}

CMirrorShader::~CMirrorShader()
{
}

void CMirrorShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 4;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dPixelShaderBlob = NULL;

	// 스텐실 거울 영역 표시용
	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[0]);

	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) 
		delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;

	// 거울에 반사된 객체
	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dReflectionPipelineStateDesc;
	d3dReflectionPipelineStateDesc.DepthStencilState = CreateReflectionDepthStencilState();
	d3dReflectionPipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	d3dReflectionPipelineStateDesc.RasterizerState.FrontCounterClockwise = true;
	hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dReflectionPipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[1]);

	// 거울을 랜더링
	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dTransparentPipelineStateDesc;
	::ZeroMemory(&d3dTransparentPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dTransparentPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dTransparentPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
	d3dTransparentPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
	d3dTransparentPipelineStateDesc.RasterizerState = CreateRasterizerState();

	d3dTransparentPipelineStateDesc.BlendState = CreateTransparentBlendState();

	d3dTransparentPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dTransparentPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dTransparentPipelineStateDesc.SampleMask = UINT_MAX;
	d3dTransparentPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dTransparentPipelineStateDesc.NumRenderTargets = 1;
	d3dTransparentPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dTransparentPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dTransparentPipelineStateDesc.SampleDesc.Count = 1;
	d3dTransparentPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dTransparentPipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[2]);

	// 거울 뒷면
	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dMirrorBackPipelineStateDesc;
	::ZeroMemory(&d3dMirrorBackPipelineStateDesc.DepthStencilState, sizeof(D3D12_DEPTH_STENCIL_DESC));
	d3dMirrorBackPipelineStateDesc.DepthStencilState.DepthEnable = true;
	d3dMirrorBackPipelineStateDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	d3dMirrorBackPipelineStateDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dMirrorBackPipelineStateDesc.DepthStencilState.StencilEnable = false;
	hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dMirrorBackPipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[3]);

}

D3D12_DEPTH_STENCIL_DESC CMirrorShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	d3dDepthStencilDesc.DepthEnable = true;
	// 깊이 버퍼쓰기 비활성화
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dDepthStencilDesc.StencilEnable = true;
	d3dDepthStencilDesc.StencilReadMask = 0xff;
	d3dDepthStencilDesc.StencilWriteMask = 0xff;

	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	// 비교 연산자 설정
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
	// ALWAYS : 스텐실 판정이 항상 성공하도록 설정
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	// 후면은 랜더링하지 않으므로 설정이 중요하지 않음
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	return(d3dDepthStencilDesc);
}

D3D12_DEPTH_STENCIL_DESC CMirrorShader::CreateReflectionDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dReflectionDepthStencilDesc;
	d3dReflectionDepthStencilDesc.DepthEnable = true;
	d3dReflectionDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	d3dReflectionDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dReflectionDepthStencilDesc.StencilEnable = true;
	d3dReflectionDepthStencilDesc.StencilReadMask = 0xff;
	d3dReflectionDepthStencilDesc.StencilWriteMask = 0xff;

	d3dReflectionDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dReflectionDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dReflectionDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dReflectionDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;

	// 후면은 랜더링하지 않으므로 설정이 중요하지 않음
	d3dReflectionDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dReflectionDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dReflectionDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dReflectionDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;

	return d3dReflectionDepthStencilDesc;
}

D3D12_RASTERIZER_DESC CMirrorShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	d3dRasterizerDesc.FrontCounterClockwise = true;
	d3dRasterizerDesc.DepthClipEnable = true;

	return(d3dRasterizerDesc);
}

D3D12_BLEND_DESC CMirrorShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = false;
	d3dBlendDesc.IndependentBlendEnable = false;
	d3dBlendDesc.RenderTarget[0].BlendEnable = false;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = false;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	// 랜더 타겟 변경하지 않음
	// => 색상이 후면버퍼에 기록되지 않음
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = 0;	

	return(d3dBlendDesc);
}

D3D12_BLEND_DESC CMirrorShader::CreateTransparentBlendState()
{
	D3D12_BLEND_DESC d3dTransparentBlendDesc;
	::ZeroMemory(&d3dTransparentBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dTransparentBlendDesc.AlphaToCoverageEnable = false;
	d3dTransparentBlendDesc.IndependentBlendEnable = false;
	d3dTransparentBlendDesc.RenderTarget[0].BlendEnable = true;
	d3dTransparentBlendDesc.RenderTarget[0].LogicOpEnable = false;
	d3dTransparentBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	d3dTransparentBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	d3dTransparentBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dTransparentBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dTransparentBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dTransparentBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dTransparentBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dTransparentBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return d3dTransparentBlendDesc;
}

D3D12_INPUT_LAYOUT_DESC CMirrorShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CMirrorShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSFirstPersonUI", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CMirrorShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSFirstPersonUI", "ps_5_1", ppd3dShaderBlob));
}

void CMirrorShader::ReleaseShaderVariables()
{
	CTexturedShader::ReleaseShaderVariables();
}

void CMirrorShader::ReleaseUploadBuffers()
{
	//m_pFirstPersonUI->ReleaseUploadBuffers();
}

void CMirrorShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
}

void CMirrorShader::ReleaseObjects()
{
	//m_pFirstPersonUIMesh->Release();
	//DeleteUI();
}

void CMirrorShader::OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList, int nPipelineState)
{
	if (m_ppd3dPipelineStates)
		pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[nPipelineState]);
	
	if (m_pd3dCbvSrvDescriptorHeap)
		pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);
}

void CMirrorShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[3]);
	// 거울 뒤에 그릴 불투명한 객체 랜더
	//m_pMirrorBackObject->Render();

	pd3dCommandList->ClearDepthStencilView(d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
	
	// 스텐실 참조값을 1로 설정한다.
	pd3dCommandList->OMSetStencilRef(1);
	pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);
	// 거울을 스텐실 버퍼로 랜더링
	// m_pMirrorObject->Render();

	pd3dCommandList->OMSetStencilRef(1);
	pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[1]);
	// 거울에 반사된 객체들을 랜더링
	//for (int i = 0; i < m_pScene->m_nObject; ++i)
	//{
	//	Render();
	//}

	pd3dCommandList->OMSetStencilRef(0);
	pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[2]);
	// m_pMirrorObject->Render();

	//CTexturedShader::Render(pd3dCommandList, pCamera);
	//m_pFirstPersonUI->Render(pd3dCommandList, pCamera);
}
