#pragma once
#include "Mesh.h"

class CRawFormatImage
{
public:
	CRawFormatImage(LPCTSTR pFileName, int nWidth, int nLength, bool bFlipY = false);
	~CRawFormatImage(void);

	BYTE GetRawImagePixel(int x, int z) { return(m_pRawImagePixels[x + (z*m_nWidth)]); }
	void SetRawImagePixel(int x, int z, BYTE nPixel) { m_pRawImagePixels[x + (z*m_nWidth)] = nPixel; }

	BYTE *GetRawImagePixels() { return(m_pRawImagePixels); }

	int GetRawImageWidth() { return(m_nWidth); }
	int GetRawImageLength() { return(m_nLength); }

protected:
	BYTE						*m_pRawImagePixels = NULL;

	int							m_nWidth;
	int							m_nLength;

};

class CHeightMapImage : public CRawFormatImage
{
public:
	CHeightMapImage(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale);
	~CHeightMapImage(void);

	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	float GetHeight(float x, float z, bool bReverseQuad = false);
	XMFLOAT3 GetHeightMapNormal(int x, int z);

protected:
	XMFLOAT3					m_xmf3Scale;

};

class CHeightMapGridMesh : public CMesh
{
public:
	CHeightMapGridMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int xStart, int zStart, int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), void *pContext = NULL);
	virtual ~CHeightMapGridMesh();

	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return(m_nLength); }

	virtual float OnGetHeight(int x, int z, void *pContext);
	virtual XMFLOAT4 OnGetColor(int x, int z, void *pContext);

protected:
	int							m_nWidth;
	int							m_nLength;
	XMFLOAT3			m_xmf3Scale;

	UINT						m_nStride = 0;
	UINT						m_nIndices = 0;

	ID3D12Resource*						m_pd3dVertexBuffer = NULL;
	ID3D12Resource*						m_pd3dVertexUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW	m_d3dVertexBufferView;

	ID3D12Resource*						m_pd3dIndexBuffer = NULL;
	ID3D12Resource*						m_pd3dIndexUploadBuffer = NULL;
	D3D12_INDEX_BUFFER_VIEW	m_d3dIndexBufferView;
};
