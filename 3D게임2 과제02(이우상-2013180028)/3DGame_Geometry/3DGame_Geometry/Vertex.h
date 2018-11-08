#pragma once

class CVertex
{
public:
	XMFLOAT3	m_xmf3Position;

public:
	CVertex();
	CVertex(XMFLOAT3 xmf3Position);
	~CVertex();
};

class CDiffusedVertex : public CVertex
{
public:
	XMFLOAT4	m_xmf4Diffuse;

public:
	CDiffusedVertex();
	CDiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse);
	CDiffusedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	~CDiffusedVertex();
};

class CTexturedVertex : public CVertex
{
public:
	XMFLOAT2	m_xmf2TexCoord;

public:
	CTexturedVertex();
	CTexturedVertex(float x, float y, float z, XMFLOAT2 xmf2TexCoord);
	CTexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT2 xmf2TexCoord = XMFLOAT2(0.0f, 0.0f));
	~CTexturedVertex();
};

class CDiffusedTexturedVertex : public CDiffusedVertex
{
public:
	XMFLOAT2	m_xmf2TexCoord;

public:
	CDiffusedTexturedVertex();
	CDiffusedTexturedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse, XMFLOAT2 xmf2TexCoord);
	CDiffusedTexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT2 xmf2TexCoord = XMFLOAT2(0.0f, 0.0f));
	~CDiffusedTexturedVertex();
};

class CDiffused2TexturedVertex : public CDiffusedVertex
{
public:
	CDiffused2TexturedVertex();
	CDiffused2TexturedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse, XMFLOAT2 xmf2TexCoord0, XMFLOAT2 xmf2TexCoord1, XMFLOAT2 xmf2TexCoord2);
	CDiffused2TexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT2 xmf2TexCoord0 = XMFLOAT2(0.0f, 0.0f), XMFLOAT2 xmf2TexCoord1 = XMFLOAT2(0.0f, 0.0f), XMFLOAT2 xmf2TexCoord2 = XMFLOAT2(0.0f, 0.0f));
	~CDiffused2TexturedVertex();

	XMFLOAT2						m_xmf2TexCoord0;
	XMFLOAT2						m_xmf2TexCoord1;
	// Èæ uv ÁÂÇ¥
	XMFLOAT2						m_xmf2TexCoord2;
};

class CBillboardVertex
{
public:
	CBillboardVertex();
	CBillboardVertex(XMFLOAT3 xmf3Position, XMFLOAT2 xmf2Size);
	~CBillboardVertex();

	XMFLOAT3 m_xmf3Position;
	XMFLOAT2 m_xmf2Size;
};