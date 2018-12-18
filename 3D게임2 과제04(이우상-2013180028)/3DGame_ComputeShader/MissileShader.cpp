#include "Stdafx.h"
#include "MissileShader.h"

// static ����
int CMissileShader::m_MissileCount = 0;
CMissileShader::CMissileShader()
{
}

CMissileShader::~CMissileShader()
{
}

void CMissileShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
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

void CMissileShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	int i = 0;
	for (auto iter = m_MissileList.begin(); iter != m_MissileList.end(); ++iter)
	{
		// �ڡڡڡڡ�
		(*iter)->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
		CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedGameObjects + (i * ncbElementBytes));
		XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&(*iter)->m_xmf4x4World)));	
		++i;
	}
}

void CMissileShader::ReleaseShaderVariables()
{
	if (m_pd3dcbGameObjects)
	{
		m_pd3dcbGameObjects->Unmap(0, NULL);
		m_pd3dcbGameObjects->Release();
	}

	CTexturedShader::ReleaseShaderVariables();
}

D3D12_SHADER_BYTECODE CMissileShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSMissile", "ps_5_1", ppd3dShaderBlob));
}

void CMissileShader::ReleaseUploadBuffers()
{
	for (auto iter = m_MissileList.begin(); iter != m_MissileList.end(); ++iter)
		(*iter)->ReleaseUploadBuffers();
}

void CMissileShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	// �Ѿ˷� ����� ť�� �޽� ����
	m_pMissileTexturedMesh = new CCubeMeshTextured(pd3dDevice, pd3dCommandList, 2.5f, 2.5f, 5.f);
	// �Ѿ˷� ����� �ؽ�ó ����
	m_pMissileTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pMissileTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"../Textures/GameObject/Missile.dds", 0);
	// �Ѿ��� ������ ����� ��
	m_pMissileMaterial = new CMaterial;
	m_pMissileMaterial->SetTexture(m_pMissileTexture);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);
	m_nObjects = 100;
	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, 1);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pMissileTexture, 16, false);
}

bool CMissileShader::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	static int i = 0;
	static int j = 0;
	static bool check_i = false;
	static bool check_j = false;

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
				CGameObject* pMissile = new CMissile;
				// ID�� 1������ ����
				((CMissile*)pMissile)->SetID(++m_MissileCount);
				pMissile->SetMesh(m_pMissileTexturedMesh);
				pMissile->SetMaterial(m_pMissileMaterial);
				pMissile->SetLook(m_pFramePlayer->GetLookVector());

				// ���� �÷��̾��� Up����, Right���͵� �Ȱ��� �������־�� �÷��̾ ȸ������ ��,
				// �Ѿ� ��絵 ȸ���� �� ������� �ٲ��.
				pMissile->SetUp(m_pFramePlayer->GetUpVector());
				pMissile->SetRight(m_pFramePlayer->GetRightVector());
				// �Ѿ��� ������ġ�� �÷��̾��� ��ġ�� ����
				pMissile->SetPosition(m_pFramePlayer->GetPosition());
				//pMissile->SetPosition(m_pFramePlayer->GetCamera()->GetPosition());
				// �Ѿ��� ���ư��� ������ �Ѿ��� �ٶ󺸴� �������� �ش�.
				pMissile->SetMovingDirection(pMissile->GetLook());
				m_MissileList.push_back(pMissile);
				cout << "�Ѿ� ���� ��" << endl;
				m_pFireParticleShader->Initialize(pMissile, m_MissileCount);
				
				++i;
				if (i <= 9)
				{
					if (check_i == false)
						m_pFontShader->Initialize(i);
					else
						m_pFontShader->SetNumber(i);
				}
				else
				{
					// ���� �Ѿ���� ���̻� �����ڸ� ���ڸ� �������� �ʾƵ� ��
					check_i = true;					
					// ���� �ڸ��� �ϳ� �÷��� �������ش�.
					if (check_j == false)
						m_pTenthFontShader->Initialize(++j);
					i = 0;
					m_pFontShader->SetNumber(i);
				}
			}
			return true;
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

void CMissileShader::ReleaseObjects()
{
	for (auto iter = m_MissileList.begin(); iter != m_MissileList.end(); )
	{
		(*iter)->DeleteMesh();
		delete (*iter);
		iter = m_MissileList.erase(iter);
	}
	m_MissileList.clear();

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) delete m_pMaterial;
#endif
}

void CMissileShader::AnimateObjects(float fTimeElapsed)
{
	double distance = 0.f;

	if (m_pExplosionParticleShader)
	{
		// �÷��̾��� �Ѿ� ����Ʈ�� ������ ���� ��ȸ�ϸ鼭, �ִϸ���Ʈ �����ش�.
		for (auto iter = m_MissileList.begin(); iter != m_MissileList.end();)
		{
			// �Ѿ��� �浹�̶��
			if (((CMissile*)(*iter))->GetCollision() == true)
			{
				// �����ؾ��ϴ� Fire��ƼŬ ID�� �Ѱ��ش�.
				m_pFireParticleShader->SetDeleteFireParticleID(((CMissile*)(*iter))->GetID());

				// �������� ������ ��ƼŬ�� �Ѿ��� ��ġ�� ����
				m_pExplosionParticleShader->Initialize((*iter)->GetPosition());

				// �Ѿ� ����
				delete (*iter);
				iter = m_MissileList.erase(iter);
			}
			// �浹�� �Ѿ��� �ƴϸ�
			else
			{
				// �÷��̾� ��ġ�� �Ѿ��� ��ġ �Ÿ��� ����ϴ� �����̴�. 
				distance = sqrt((pow(((*iter)->GetPosition().x - m_pFramePlayer->GetPosition().x), 2.0)
					+ pow(((*iter)->GetPosition().y - m_pFramePlayer->GetPosition().y), 2.0)
					+ pow(((*iter)->GetPosition().z - m_pFramePlayer->GetPosition().z), 2.0)));

				// �÷��̾�� �Ѿ��� �Ÿ��� 250m���� Ŀ����, �Ѿ��� ��ȿ��Ÿ��� ����ŹǷ�
					// �Ѿ��� ��� �׸��� �ʰ�, �����־�� �����ӷ���Ʈ�� �ø� �� �ִ�.
				if (distance >= MaxMissileDistance)
				{
					// �����ؾ��ϴ� Fire��ƼŬ ID�� �Ѱ��ش�.
					m_pFireParticleShader->SetDeleteFireParticleID(((CMissile*)(*iter))->GetID());

					// �������� ������ ��ƼŬ�� �Ѿ��� ��ġ�� ����
					m_pExplosionParticleShader->Initialize((*iter)->GetPosition());

					// �Ѿ� ����
					delete (*iter);
					iter = m_MissileList.erase(iter);
					cout << "�÷��̾� �Ѿ� �Ÿ���� ����" << endl;
				}
				else
				{
					((CMissile*)(*iter))->Animate(fTimeElapsed);
					++iter;
				}
			}
		}
	}
}

void CMissileShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CTexturedShader::Render(pd3dCommandList, pCamera);
	UpdateShaderVariables(pd3dCommandList);

	for (auto iter = m_MissileList.begin(); iter != m_MissileList.end(); ++iter)
	{
		((*iter))->Render(pd3dCommandList, pCamera);
	}
}