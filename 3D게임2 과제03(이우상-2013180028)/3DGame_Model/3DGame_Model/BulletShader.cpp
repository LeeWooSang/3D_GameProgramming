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
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256�� ���
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
		// �ڡڡڡڡ�
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
	// �Ѿ˷� ����� ť�� �޽� ����
	m_pBulletTexturedMesh = new CCubeMeshTextured(pd3dDevice, pd3dCommandList, 2.5f, 2.5f, 5.f);
	// �Ѿ˷� ����� �ؽ�ó ����
	m_pBulletTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pBulletTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/Box.dds", 0);
	// �Ѿ��� ������ ����� ��
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
			// �Ѿ� �߻� Ű
		case VK_CONTROL:
		{
			if (m_pFramePlayer)
			{
				m_pBullet = new CBullet;
				m_pBullet->SetMesh(m_pBulletTexturedMesh);
				m_pBullet->SetMaterial(m_pBulletMaterial);

				m_pBullet->SetLook(m_pFramePlayer->GetLookVector());
				// ���� �÷��̾��� Up����, Right���͵� �Ȱ��� �������־�� �÷��̾ ȸ������ ��,
				// �Ѿ� ��絵 ȸ���� �� ������� �ٲ��.
				m_pBullet->SetUp(m_pFramePlayer->GetUpVector());
				m_pBullet->SetRight(m_pFramePlayer->GetRightVector());
				// �Ѿ��� ������ġ�� �÷��̾��� ��ġ�� ����
				m_pBullet->SetPosition(m_pFramePlayer->GetPosition());
				// �Ѿ��� ���ư��� ������ �Ѿ��� �ٶ󺸴� �������� �ش�.
				m_pBullet->SetMovingDirection(m_pBullet->GetLook());
				m_BulletList.push_back(m_pBullet);
				cout << "�Ѿ� ���� ��" << endl;
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

	// �÷��̾��� �Ѿ� ����Ʈ�� ������ ���� ��ȸ�ϸ鼭, �ִϸ���Ʈ �����ش�.
	for (auto iter = m_BulletList.begin(); iter != m_BulletList.end();)
	{
		// �÷��̾� ��ġ�� �Ѿ��� ��ġ �Ÿ��� ����ϴ� �����̴�. 
		distance = sqrt( (pow(((*iter)->GetPosition().x - m_pFramePlayer->GetPosition().x), 2.0)
				+ pow(((*iter)->GetPosition().y - m_pFramePlayer->GetPosition().y), 2.0)
				+ pow(((*iter)->GetPosition().z - m_pFramePlayer->GetPosition().z), 2.0)) );

		// �÷��̾�� �Ѿ��� �Ÿ��� 250m���� Ŀ����, �Ѿ��� ��ȿ��Ÿ��� ����ŹǷ�
			// �Ѿ��� ��� �׸��� �ʰ�, �����־�� �����ӷ���Ʈ�� �ø� �� �ִ�.
		if (distance >= MaxBulletDistance)
		{
			cout << "�÷��̾� �Ѿ� �Ÿ���� ����" << endl;
			if (m_pExplosionParticleShader)
			{
				// �������� ������ ��ƼŬ�� �Ѿ��� ��ġ�� ����
				m_pExplosionParticleShader->Initialize((*iter)->GetPosition());
			}
			// �Ѿ� ����
			delete (*iter);
			iter = m_BulletList.erase(iter);
		}
		else
		{
			((CBullet*)(*iter))->Animate(fTimeElapsed);
			++iter;
		}
			
		//cout << "�Ѿ� ��ġ : " << (*iter)->GetPosition().x
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