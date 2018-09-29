#include "stdafx.h"
#include "TexturedRectMesh.h"
#include "Vertex.h"

CTexturedRectMesh::CTexturedRectMesh()
{
}

CTexturedRectMesh::CTexturedRectMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth, float fHeight, float fDepth, float fxPosition, float fyPosition, float fzPosition)
	: CMesh(pd3dDevice, pd3dCommandList)
{
	m_nVertices = 6;
	m_nStride = sizeof(CTexturedVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
	CTexturedVertex pVertices[6];

	float fx = (fWidth * 0.5f) + fxPosition;
	float fy = (fHeight * 0.5f) + fyPosition;
	float fz = (fDepth * 0.5f) + fzPosition;

	if (fWidth == 0.f) 
	{
		if (fxPosition > 0.f)
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(fx, +fy, -fz), XMFLOAT2(1.f, 0.f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(fx, -fy, -fz), XMFLOAT2(1.f, 1.f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(fx, -fy, +fz), XMFLOAT2(0.f, 1.f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(fx, -fy, +fz), XMFLOAT2(0.f, 1.f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(fx, +fy, +fz), XMFLOAT2(0.f, 0.f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(fx, +fy, -fz), XMFLOAT2(1.f, 0.f));
		}
		else 
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(fx, +fy, +fz), XMFLOAT2(1.f, 0.f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(fx, -fy, +fz), XMFLOAT2(1.f, 1.f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(fx, -fy, -fz), XMFLOAT2(0.f, 1.f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(fx, -fy, -fz), XMFLOAT2(0.f, 1.f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(fx, +fy, -fz), XMFLOAT2(0.f, 0.f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(fx, +fy, +fz), XMFLOAT2(1.f, 0.f));
		}
	}

	else if (fHeight == 0.f) 
	{
		if (fyPosition > 0.f) 
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(+fx, fy, -fz), XMFLOAT2(1.f, 0.f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(+fx, fy, +fz), XMFLOAT2(1.f, 1.f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(-fx, fy, +fz), XMFLOAT2(0.f, 1.f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(-fx, fy, +fz), XMFLOAT2(0.f, 1.f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(-fx, fy, -fz), XMFLOAT2(0.f, 0.f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(+fx, fy, -fz), XMFLOAT2(1.f, 0.f));
		}
		else 
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(+fx, fy, +fz), XMFLOAT2(1.f, 0.f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(+fx, fy, -fz), XMFLOAT2(1.f, 1.f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(-fx, fy, -fz), XMFLOAT2(0.f, 1.f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(-fx, fy, -fz), XMFLOAT2(0.f, 1.f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(-fx, fy, +fz), XMFLOAT2(0.f, 0.f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(+fx, fy, +fz), XMFLOAT2(1.f, 0.f));
		}
	}

	else if (fDepth == 0.f)
	{
		if (fzPosition > 0.f)
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(1.f, 0.f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(1.f, 1.f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(0.f, 1.f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(0.f, 1.f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(0.f, 0.f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(1.f, 0.f));
		}
		else 
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(1.f, 0.f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(1.f, 1.f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(0.f, 1.f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(0.f, 1.f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(0.f, 0.f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(1.f, 0.f));
		}
	}

	m_pd3dVertexBuffer = ::CreateBufferResource
	(
		pd3dDevice, 
		pd3dCommandList, 
		pVertices,
		m_nStride * m_nVertices, 
		D3D12_HEAP_TYPE_DEFAULT, 
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		&m_pd3dVertexUploadBuffer
	);
	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = m_nStride;
	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;
}

CTexturedRectMesh::~CTexturedRectMesh()
{
}
