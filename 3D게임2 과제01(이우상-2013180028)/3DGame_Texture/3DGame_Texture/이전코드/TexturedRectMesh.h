#pragma once
#include "Mesh.h"

class CTexturedRectMesh : public CMesh
{
public:
	CTexturedRectMesh();
	CTexturedRectMesh(ID3D12Device*, ID3D12GraphicsCommandList*, float, float, float, float, float, float);
	~CTexturedRectMesh();
};

