#include "Stdafx.h"
#include "BlurringShader.h"

CBlurringShader::CBlurringShader() 
{
}

CBlurringShader::~CBlurringShader() 
{
	ReleaseObjects();
}

void CBlurringShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	m_pTexture = (CTexture *)pContext;

	CreateCbvSrvUavDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 2, 2);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture, 0, false);
	CreateUnorderedAccessViews(pd3dDevice, pd3dCommandList, m_pTexture, 1, false);
}

D3D12_SHADER_BYTECODE CBlurringShader::CreateHorzBlurShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "HorzBlurCS", "cs_5_1", ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE CBlurringShader::CreateVertBlurShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VertBlurCS", "cs_5_1", ppd3dShaderBlob));
}

void CBlurringShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dRootSignature, UINT nRenderTargets)
{
	m_nPipelineStates = 2;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	ID3DBlob *pd3dComputeShaderBlob = NULL;

	D3D12_COMPUTE_PIPELINE_STATE_DESC d3dComputePipelineStateDesc;
	::ZeroMemory(&d3dComputePipelineStateDesc, sizeof(D3D12_COMPUTE_PIPELINE_STATE_DESC));
	d3dComputePipelineStateDesc.pRootSignature = pd3dRootSignature;
	d3dComputePipelineStateDesc.CS = CreateHorzBlurShader(&pd3dComputeShaderBlob);
	d3dComputePipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	HRESULT hResult = pd3dDevice->CreateComputePipelineState(&d3dComputePipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[0]);

	d3dComputePipelineStateDesc.CS = CreateVertBlurShader(&pd3dComputeShaderBlob);
	hResult = pd3dDevice->CreateComputePipelineState(&d3dComputePipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[1]);
	if (pd3dComputeShaderBlob) pd3dComputeShaderBlob->Release();
}

void CBlurringShader::ReleaseObjects()
{
	if (m_pTexture) 
		m_pTexture->Release();
}

void CBlurringShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, UINT num, ID3D12Resource* input)
{

	//렌더타겟을 그리는것에서 카피로.
	::SynchronizeResourceTransition(
		pd3dCommandList,
		input,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_COPY_SOURCE);

	// 텍스처0번을 디폴트에서 카피받는걸로
	::SynchronizeResourceTransition(
		pd3dCommandList,
		m_pTexture->GetTexture(0),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_COPY_DEST);

	// 0번텍스처에 렌더타겟정보 카피
	pd3dCommandList->CopyResource(m_pTexture->GetTexture(0), input);

	// 0번텍스처를 카피 받는거에서 리드로.
	::SynchronizeResourceTransition(
		pd3dCommandList,
		m_pTexture->GetTexture(0),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ);

	// 1번 텍스처를 디폴트에서 언오더드 액세스로
	// 텍스처0번을 디폴트에서 카피받는걸로
	::SynchronizeResourceTransition(
		pd3dCommandList,
		m_pTexture->GetTexture(1),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvUavDescriptorHeap);
	for (int i = 0; i < 5; ++i) 
	{
		// 블러링 수평부터.
		pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);

		pd3dCommandList->SetComputeRootDescriptorTable(0, m_pTexture->GetSrvHandle(0));
		pd3dCommandList->SetComputeRootDescriptorTable(1, m_pTexture->GetSrvHandle(1));
		UINT numGroupsX = (UINT)ceilf(FRAME_BUFFER_WIDTH / 256.0f);

		pd3dCommandList->Dispatch(numGroupsX, FRAME_BUFFER_HEIGHT, 1);
		//pd3dCommandList->Dispatch(256, 1, 1);
		// 수평 끝. 0번텍스처를 이제 리드에서 언오더드로
		::SynchronizeResourceTransition(
			pd3dCommandList,
			m_pTexture->GetTexture(0),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

		// 1번텍스처를 언오더드에서 리드로.
		::SynchronizeResourceTransition(
			pd3dCommandList,
			m_pTexture->GetTexture(1),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_GENERIC_READ);

		// 수직파이프라인으로 셋
		pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[1]);
		pd3dCommandList->SetComputeRootDescriptorTable(0, m_pTexture->GetSrvHandle(1));
		pd3dCommandList->SetComputeRootDescriptorTable(1, m_pTexture->GetSrvHandle(0));
		UINT numGroupsY = (UINT)ceilf(FRAME_BUFFER_HEIGHT / 256.0f);
		//cout << "y  " << numGroupsY << endl;
		pd3dCommandList->Dispatch(FRAME_BUFFER_WIDTH, numGroupsY, 1);
		//pd3dCommandList->Dispatch(1, 256, 1);
		//// 수직 끝 0번텍스처를 언오더드에서 리드로.
		::SynchronizeResourceTransition(
			pd3dCommandList,
			m_pTexture->GetTexture(0),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_GENERIC_READ);

		// 1번텍스처를 리드에서 언오더드로.
		::SynchronizeResourceTransition(
			pd3dCommandList,
			m_pTexture->GetTexture(1),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	}

	::SynchronizeResourceTransition(
		pd3dCommandList,
		m_pTexture->GetTexture(0),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_RESOURCE_STATE_COPY_SOURCE);

	//렌더타겟을 카피받는거에서 카피로.
	::SynchronizeResourceTransition(pd3dCommandList,
		input,
		D3D12_RESOURCE_STATE_COPY_SOURCE,
		D3D12_RESOURCE_STATE_COPY_DEST);

	pd3dCommandList->CopyResource(input, m_pTexture->GetTexture(0));

	// 줬으니까 다시 코몬으로 만들어 둘다.

	::SynchronizeResourceTransition(
		pd3dCommandList,
		m_pTexture->GetTexture(0),
		D3D12_RESOURCE_STATE_COPY_SOURCE,
		D3D12_RESOURCE_STATE_COMMON);

	::SynchronizeResourceTransition(
		pd3dCommandList,
		m_pTexture->GetTexture(1),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_COMMON);
}