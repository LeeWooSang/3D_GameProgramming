#include "stdafx.h"
#include "BulletShader.h"

CBulletShader::CBulletShader()
{
}

CBulletShader::~CBulletShader()
{
}

void CBulletShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbGameObjects = ::CreateBufferResource
	(
		pd3dDevice, 
		pd3dCommandList, 
		NULL, 
		ncbElementBytes * m_nObjects, 
		D3D12_HEAP_TYPE_UPLOAD, 
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, 
		NULL
	);

	m_pd3dcbGameObjects->Map(0, NULL, (void **)&m_pcbMappedGameObjects);
}

void CBulletShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	int i = 0;
	for (auto iter = m_BulletList.begin(); iter != m_BulletList.end(); ++iter)
	{
		// ★★★★★
		(*iter)->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
		CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedGameObjects + (i * ncbElementBytes));
		XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&(*iter)->m_xmf4x4World)));	
		++i;
	}
}

void CBulletShader::ReleaseShaderVariables()
{
	if (m_pd3dcbGameObjects)
	{
		m_pd3dcbGameObjects->Unmap(0, NULL);
		m_pd3dcbGameObjects->Release();
	}

	CTexturedShader::ReleaseShaderVariables();
}

D3D12_SHADER_BYTECODE CBulletShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSBullet", "ps_5_1", ppd3dShaderBlob));
}

void CBulletShader::ReleaseUploadBuffers()
{
	for (auto iter = m_BulletList.begin(); iter != m_BulletList.end(); ++iter)
		(*iter)->ReleaseUploadBuffers();
}

void CBulletShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	// 총알로 사용할 큐브 메쉬 생성
	m_pBulletTexturedMesh = new CCubeMeshTextured(pd3dDevice, pd3dCommandList, 2.5f, 2.5f, 5.f);
	// 총알로 사용할 텍스처 생성
	m_pBulletTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pBulletTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Box.dds", 0);
	// 총알의 재질을 만들어 줌
	m_pBulletMaterial = new CMaterial;
	m_pBulletMaterial->SetTexture(m_pBulletTexture);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);
	m_nObjects = 100;
	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, 1);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pBulletTexture, 16, false);
}

bool CBulletShader::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
			// 총알 발사 키
		case VK_CONTROL:
		{
			if (m_pFramePlayer)
			{
				m_pBullet = new CBullet;
				m_pBullet->SetMesh(m_pBulletTexturedMesh);
				m_pBullet->SetMaterial(m_pBulletMaterial);

				m_pBullet->SetLook(m_pFramePlayer->GetLookVector());
				// 또한 플레이어의 Up벡터, Right벡터도 똑같이 설정해주어야 플레이어가 회전했을 때,
				// 총알 모양도 회전이 된 모양으로 바뀐다.
				m_pBullet->SetUp(m_pFramePlayer->GetUpVector());
				m_pBullet->SetRight(m_pFramePlayer->GetRightVector());
				// 총알의 생성위치는 플레이어의 위치로 설정
				m_pBullet->SetPosition(m_pFramePlayer->GetPosition());
				// 총알이 나아가는 방향은 총알이 바라보는 방향으로 준다.
				m_pBullet->SetMovingDirection(m_pBullet->GetLook());
				m_BulletList.push_back(m_pBullet);
				cout << "총알 생성 됨" << endl;
			}
			break;
		}
		default:
			break;
		}
		break;
	default:
		break;
	}
	return(false);
}

void CBulletShader::ReleaseObjects()
{
	for (auto iter = m_BulletList.begin(); iter != m_BulletList.end(); )
	{
		(*iter)->DeleteMesh();
		delete (*iter);
		iter = m_BulletList.erase(iter);
	}
	m_BulletList.clear();

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) delete m_pMaterial;
#endif
}

void CBulletShader::AnimateObjects(float fTimeElapsed)
{
	double distance = 0.f;

	// 플레이어의 총알 리스트를 루프를 통해 순회하면서, 애니메이트 시켜준다.
	for (auto iter = m_BulletList.begin(); iter != m_BulletList.end();)
	{
		// 플레이어 위치와 총알의 위치 거리를 계산하는 공식이다. 
		distance = sqrt( (pow(((*iter)->GetPosition().x - m_pFramePlayer->GetPosition().x), 2.0)
				+ pow(((*iter)->GetPosition().y - m_pFramePlayer->GetPosition().y), 2.0)
				+ pow(((*iter)->GetPosition().z - m_pFramePlayer->GetPosition().z), 2.0)) );

		// 플레이어와 총알의 거리가 250m보다 커지면, 총알의 유효사거리를 벗어난거므로
			// 총알을 계속 그리지 않고, 지워주어야 프레임레이트를 올릴 수 있다.
		if (distance >= MaxBulletDistance)
		{
			cout << "플레이어 총알 거리벗어남 삭제" << endl;
			delete (*iter);
			iter = m_BulletList.erase(iter);
		}
		else
		{
			((CBullet*)(*iter))->Animate(fTimeElapsed);
			++iter;
		}
			
		//cout << "총알 위치 : " << (*iter)->GetPosition().x
		//	<< ", " << (*iter)->GetPosition().y
		//	<< ", " << (*iter)->GetPosition().z << endl;
	}
}

void CBulletShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CTexturedShader::Render(pd3dCommandList, pCamera);
	UpdateShaderVariables(pd3dCommandList);
	//for (int j = 0; j < m_nObjects; j++)
	//{
	//	if (m_ppObjects[j])
	//		m_ppObjects[j]->Render(pd3dCommandList, pCamera);
	//}
	for (auto iter = m_BulletList.begin(); iter != m_BulletList.end(); ++iter)
	{
		((*iter))->Render(pd3dCommandList, pCamera);
	}
}