#include "stdafx.h"
#include "Vertex.h"

CVertex::CVertex()
{ 
	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); 
}

CVertex::CVertex(XMFLOAT3 xmf3Position) 
{ 
	m_xmf3Position = xmf3Position; 
}

CVertex::~CVertex()
{
}

//=========================================================================

CDiffusedVertex::CDiffusedVertex()
{ 
	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); 
	m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); 
}

CDiffusedVertex::CDiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse) 
{ 
	m_xmf3Position = XMFLOAT3(x, y, z); 
	m_xmf4Diffuse = xmf4Diffuse; 
}

CDiffusedVertex::CDiffusedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse) 
{ 
	m_xmf3Position = xmf3Position; 
	m_xmf4Diffuse = xmf4Diffuse; 
}

CDiffusedVertex::~CDiffusedVertex() 
{
}

//=========================================================================

CTexturedVertex::CTexturedVertex()
{
	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); 
	m_xmf2TexCoord = XMFLOAT2(0.0f, 0.0f);
}

CTexturedVertex::CTexturedVertex(float x, float y, float z, XMFLOAT2 xmf2TexCoord) 
{
	m_xmf3Position = XMFLOAT3(x, y, z); 
	m_xmf2TexCoord = xmf2TexCoord;
}

CTexturedVertex::CTexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT2 xmf2TexCoord) 
{ 
	m_xmf3Position = xmf3Position; 
	m_xmf2TexCoord = xmf2TexCoord; 
}

CTexturedVertex::~CTexturedVertex() 
{
}

//=========================================================================

CDiffusedTexturedVertex::CDiffusedTexturedVertex() 
{ 
	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); 
	m_xmf2TexCoord = XMFLOAT2(0.0f, 0.0f); 
}

CDiffusedTexturedVertex::CDiffusedTexturedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse, XMFLOAT2 xmf2TexCoord) 
{ 
	m_xmf3Position = XMFLOAT3(x, y, z); 
	m_xmf4Diffuse = xmf4Diffuse; 
	m_xmf2TexCoord = xmf2TexCoord;
}

CDiffusedTexturedVertex::CDiffusedTexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse, XMFLOAT2 xmf2TexCoord)
{ 
	m_xmf3Position = xmf3Position; 
	m_xmf4Diffuse = xmf4Diffuse; 
	m_xmf2TexCoord = xmf2TexCoord;
}

CDiffusedTexturedVertex::~CDiffusedTexturedVertex() 
{
}

//=========================================================================

CDiffused2TexturedVertex::CDiffused2TexturedVertex() 
{
	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); 
	m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); 
	m_xmf2TexCoord0 = XMFLOAT2(0.0f, 0.0f);
	m_xmf2TexCoord1 = XMFLOAT2(0.0f, 0.0f);
	m_xmf2TexCoord2 = XMFLOAT2(0.0f, 0.0f);
}

CDiffused2TexturedVertex::CDiffused2TexturedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse, XMFLOAT2 xmf2TexCoord0, XMFLOAT2 xmf2TexCoord1, XMFLOAT2 xmf2TexCoord2) 
{
	m_xmf3Position = XMFLOAT3(x, y, z); 
	m_xmf4Diffuse = xmf4Diffuse; 
	m_xmf2TexCoord0 = xmf2TexCoord0; 
	m_xmf2TexCoord1 = xmf2TexCoord1; 
	m_xmf2TexCoord2 = xmf2TexCoord2;
}

CDiffused2TexturedVertex::CDiffused2TexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse, XMFLOAT2 xmf2TexCoord0, XMFLOAT2 xmf2TexCoord1, XMFLOAT2 xmf2TexCoord2)
{
	m_xmf3Position = xmf3Position; 
	m_xmf4Diffuse = xmf4Diffuse; 
	m_xmf2TexCoord0 = xmf2TexCoord0; 
	m_xmf2TexCoord1 = xmf2TexCoord1; 
	m_xmf2TexCoord2 = xmf2TexCoord2;
}

CDiffused2TexturedVertex::~CDiffused2TexturedVertex() 
{
}