#pragma once
#include "Mesh.h"

class CTexturedRectMesh : public CMesh
{
public:
	CTexturedRectMesh();
	CTexturedRectMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth, float fHeight, float fDepth, float fxPosition, float fyPosition, float fzPosition);
	~CTexturedRectMesh();
};

