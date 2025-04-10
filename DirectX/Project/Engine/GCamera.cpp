#include "pch.h"
#include "GCamera.h"

#include "GDevice.h"
#include "GRenderManager.h"
#include "GLevelManager.h"

#include "GLevel.h"
#include "GLayer.h"
#include "GGameObject.h"

#include "components.h"


GCamera::GCamera() :
	GComponent(COMPONENT_TYPE::CAMERA),
	m_ProjType(PROJECTION_TYPE::PERSPECTIVE),
	m_OrthoScaleX(1.f),
	m_AspectRatio(1.f),
	m_FOV(XM_PI / 2.f),
	m_Far(10000.f),
	m_Priority(-1),
	m_LayerCheck(0)
{
	CheckLayerAll();
	CheckLayer(31);

	Vector2 vResoultion = GDevice::GetInst()->GetRenderResolution();

	m_OrthoScaleX = vResoultion.x;					// ���� :1280
	m_AspectRatio = vResoultion.x / vResoultion.y;	// 1280 / 768 = 1.6666
}

GCamera::GCamera(const GCamera& _Origin)
	: GComponent(_Origin)
	, m_ProjType(_Origin.m_ProjType)
	, m_OrthoScaleX(_Origin.m_OrthoScaleX)
	, m_AspectRatio(_Origin.m_AspectRatio)
	, m_FOV(_Origin.m_FOV)
	, m_Far(_Origin.m_Far)
	, m_Priority(_Origin.m_Priority)
	, m_LayerCheck(_Origin.m_LayerCheck)

{

}

GCamera::~GCamera()
{

}

void GCamera::Begin()
{
	GRenderManager::GetInst()->RegisterCamera(this, m_Priority);
}

void GCamera::FinalUpdate()
{
	// �� ��� ���
	// �̵�
	Vector3 vCamPos = -Transform()->GetRelativePos();			// ī�޶��� ��ġ��ŭ ������ ���ؼ� ������
	Matrix matTrans = XMMatrixTranslation(vCamPos.x, vCamPos.y, vCamPos.z);

	Vector3 vRight	=	Transform()->GetWorldDirection(DIRECTION_TYPE::RIGHT);
	Vector3 vUp		=	Transform()->GetWorldDirection(DIRECTION_TYPE::UP);
	Vector3 vFront	=	Transform()->GetWorldDirection(DIRECTION_TYPE::FRONT);

	Matrix matRotation = XMMatrixIdentity();
	matRotation._11 = vRight.x; matRotation._12 = vUp.x; matRotation._13 = vFront.x;
	matRotation._21 = vRight.y; matRotation._22 = vUp.y; matRotation._23 = vFront.y;
	matRotation._31 = vRight.z; matRotation._32 = vUp.z; matRotation._33 = vFront.z;

	m_matView = matTrans * matRotation;

	// ������� ����ϱ�
	// XMMatrixOrthographicLH(1280, 1280 / 1.6666, 1.f, m_Far);
	if (m_ProjType == PROJECTION_TYPE::ORTHOGRAPHIC)
	{
		m_matProj = XMMatrixOrthographicLH(m_OrthoScaleX, m_OrthoScaleX / m_AspectRatio, 1.f, m_Far);
	}
	else if (m_ProjType == PROJECTION_TYPE::PERSPECTIVE)
	{
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}
}

void GCamera::Render()
{
	// ī�޶��� View, Proj ����� ����
	g_Trans.matView = m_matView;
	g_Trans.matProj = m_matProj;

	SortObject();

	// ��ü ������
	// Opaque
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->Render();
	}

	// Masked
	for (size_t i = 0; i < m_vecMasked.size(); ++i)
	{
		m_vecMasked[i]->Render();
	}

	// Transparent
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->Render();
	}

	// PostProcess
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		// ����Ÿ������ ����� PostProcess Ÿ������ ����
		GRenderManager::GetInst()->CopyRenderTarget();

		// ����� PostProcess �ؽ��ĸ� t14(����)���� ���ε�
		GRenderManager::GetInst()->GetPostProcessTex()->Binding(14);

		m_vecPostProcess[i]->Render();
	}
}

void GCamera::End()
{
	GRenderManager::GetInst()->DeRegisterCamera(this);
}

void GCamera::SortObject()
{

	m_vecOpaque.clear();
	m_vecMasked.clear();
	m_vecTransparent.clear();
	m_vecPostProcess.clear();

	GLevel* CurLevel = GLevelManager::GetInst()->GetCurrentLevel();

	// ��ü �з�
	// 1. Camera�� Rendering ���� �ʴ� ���̾�� �Ÿ���.
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// Camera�� Rendering ���� �ʴ� ���̾�� �Ÿ���.
		if (!(m_LayerCheck & (1 << i)))
			continue;

		// ���̾ ���� ��ü���� �����´�.
		const vector<GGameObject*>& vecObjects = CurLevel->GetLayer(i)->GetObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			// 2. ������Ʈ�� ����ϴ� ���̴��� �����ο� ���� �з��Ѵ�.
			// ������Ʈ�� �޽�, ���̴��� ���ٸ�
			if (vecObjects[j]->Renderer() == nullptr ||
				vecObjects[j]->Renderer()->GetMesh() == nullptr ||
				vecObjects[j]->Renderer()->GetMaterial() == nullptr ||
				vecObjects[j]->Renderer()->GetMaterial()->GetShader() == nullptr)
				continue;

			switch (vecObjects[j]->Renderer()->GetMaterial()->GetShader()->GetDomain())
			{
				case SHADER_DOMAIN::DOMAIN_MASKED:
					m_vecMasked.push_back(vecObjects[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
					m_vecOpaque.push_back(vecObjects[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
					m_vecTransparent.push_back(vecObjects[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
					m_vecPostProcess.push_back(vecObjects[j]);
					break;
			}
		}
	}

	
}

void GCamera::SaveToFile(FILE* _File)
{
	fwrite(&m_ProjType, sizeof(PROJECTION_TYPE), 1, _File);
	fwrite(&m_OrthoScaleX, sizeof(float), 1, _File);
	fwrite(&m_AspectRatio, sizeof(float), 1, _File);
	fwrite(&m_FOV, sizeof(float), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_Priority, sizeof(int), 1, _File);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _File);
}

void GCamera::LoadFromFile(FILE* _File)
{
	fread(&m_ProjType, sizeof(PROJECTION_TYPE), 1, _File);
	fread(&m_OrthoScaleX, sizeof(float), 1, _File);
	fread(&m_AspectRatio, sizeof(float), 1, _File);
	fread(&m_FOV, sizeof(float), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_Priority, sizeof(int), 1, _File);
	fread(&m_LayerCheck, sizeof(UINT), 1, _File);
}