#include "pch.h"
#include "GRenderManager.h"

#include "GDevice.h"
#include "GLevel.h"
#include "GCamera.h"
#include "GConstBuffer.h"

#include "GTimeManager.h"
#include "GAssetManager.h"
#include "GLevelManager.h"

#include "assets.h"
#include "components.h"

GRenderManager::GRenderManager()
	: m_EditorCam(nullptr)
	, m_DebugObject(nullptr)
	, m_Light2DBuffer(nullptr)
	, m_RenderFunc(nullptr)
{
	m_Light2DBuffer = new GStructuredBuffer;

	m_RenderFunc = &GRenderManager::Render_Editor;
}

GRenderManager::~GRenderManager()
{
	// �ӽ� DebugObject ��ü ����
	if (nullptr != m_DebugObject)
		delete m_DebugObject;

	if (nullptr != m_Light2DBuffer)
		delete m_Light2DBuffer;
}

void GRenderManager::RegisterCamera(GCamera* _Cam, int _Priority)
{
	// �켱 ������ -1 �̶�� ������� �ʴ´�.
	if (_Priority == -1)
	{
		_Cam->m_Priority = -1;
		return;
	}

	// �켱���� ���� ī�޶��� ���� ũ�ٸ� ī�޶����� ����� �׸�ŭ ������Ų��.
	if (m_vecCam.size() <= _Priority)
	{
		m_vecCam.resize(_Priority + 1);
	}

	// �̹� �ش� �켱������ ī�޶� �����Ѵٸ� ī�޶� �̵������ �ٲ۴�.
	if (m_vecCam[_Priority] != nullptr)
	{
		m_vecCam[_Priority]->m_Priority = -1;
	}

	// ī�޶� ����Ѵ�.
	m_vecCam[_Priority] = _Cam;
}

void GRenderManager::DeRegisterCamera(GCamera* _Cam)
{
	vector<GCamera*>::iterator iter = find(m_vecCam.begin(), m_vecCam.end(), _Cam);

	if (iter == m_vecCam.end())
		return;
	m_vecCam.erase(iter);

}

void GRenderManager::CopyRenderTarget()
{
	Ptr<GTexture> pRTTex = GAssetManager::GetInst()->FindAsset<GTexture>(L"RenderTargetTex");

	// RenderTarget -> Copy -> PostProcessTex
	CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void GRenderManager::Progress()
{
	// Resource GPU Binding
	Binding();

	//Main Rendering
	GDevice::GetInst()->ClearTarget();
	(this->*m_RenderFunc)();

	// Debug Rendering
	DebugRender();

	// Resource Clear
	Clear();
}

void GRenderManager::Binding()
{
	// ��� ����Ÿ�� �� ��� ����Ÿ�� ����
	Ptr<GTexture> pRTTex = GAssetManager::GetInst()->FindAsset<GTexture>(L"RenderTargetTex");
	Ptr<GTexture> pDSTex = GAssetManager::GetInst()->FindAsset<GTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	// Light2D Binding
	if (m_Light2DBuffer->GetElementCount() < m_vecLight2D.size())
	{
		// ����ȭ ���� ���� X, SRV_ONLY
		// ����ȭ ���� �̵� O
		m_Light2DBuffer->Create(sizeof(tLight2DInfo), (UINT)m_vecLight2D.size(), SB_TYPE::SRV_ONLY, true);
	}

	// �� ������ tLight ����ü ������ ���Ϳ� ����
	static vector<tLight2DInfo> vecLight2DInfo;
	vecLight2DInfo.clear();

	for (size_t i = 0;i < m_vecLight2D.size();++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetInfo());
	}

	// ������ ���ٸ� ���ε����� �ʴ´�.
	// �׷� ������ 1�� ���ٰ� �������?
	if (!vecLight2DInfo.empty())
	{
		// tLight2DInfo ������ ����ü ����
		m_Light2DBuffer->SetData(vecLight2DInfo.data(), sizeof(tLight2DInfo), (UINT)vecLight2DInfo.size());

		// t12�� ���ε�
		m_Light2DBuffer->Binding(12);
	}

	// GlobalData Biding
	static GConstBuffer* pGlobal = GDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);

	g_Global.g_Light2DCount = vecLight2DInfo.size();

	pGlobal->SetData(&g_Global, sizeof(tGlobal));
	pGlobal->Binding();
	pGlobal->Binding_CS();
}

void GRenderManager::Render_Play()
{
	// ī�޶��� Render�� ȣ���Ѵ�.
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		if (m_vecCam[i] == nullptr)
			continue;
		m_vecCam[i]->Render();
	}
}

void GRenderManager::Render_Editor()
{
	if (nullptr == m_EditorCam)
		return;

	m_EditorCam->Render();
}

void GRenderManager::Clear()
{
	m_Light2DBuffer->Clear(12);
	m_vecLight2D.clear();
}

void GRenderManager::DebugRender()
{
	if (!m_DrawDebug)
		m_vecDebugInfo.clear();

	vector<tDebugShapeInfo>::iterator iter = m_vecDebugInfo.begin();
	for (; iter != m_vecDebugInfo.end();)
	{
		// ����� ��翡 ���� �޽� ����
		switch ((*iter).Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DebugObject->Renderer()->SetMesh(GAssetManager::GetInst()->FindAsset<GMesh>(L"RectMesh_Debug"));
			m_DebugObject->Renderer()->GetMaterial()->SetShader(GAssetManager::GetInst()->FindAsset<GGraphicShader>(L"DebugShapeShader"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugObject->Renderer()->SetMesh(GAssetManager::GetInst()->FindAsset<GMesh>(L"CircleMesh_Debug"));
			m_DebugObject->Renderer()->GetMaterial()->SetShader(GAssetManager::GetInst()->FindAsset<GGraphicShader>(L"DebugShapeShader"));
			break;
		case DEBUG_SHAPE::LINE:
			m_DebugObject->Renderer()->SetMesh(GAssetManager::GetInst()->FindAsset<GMesh>(L"PointMesh"));
			m_DebugObject->Renderer()->GetMaterial()->SetShader(GAssetManager::GetInst()->FindAsset<GGraphicShader>(L"DebugLineShader"));
			m_DebugObject->Renderer()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_1, (*iter).WorldDirction);
			break;
		default:
			break;
		}

		// ���� ������ ���� DepthStencilState�� �����
		if ((*iter).DepthTest)
			m_DebugObject->Renderer()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_WRITE);
		else
			m_DebugObject->Renderer()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WIRTE);
	
		// ������ ������ ���ؼ� ����
		m_DebugObject->MeshRender()->SetColor((*iter).Color);

		// ��ġ���� ���� �� ������� ���
		if ((*iter).MatWorld == Matrix::Identity)
		{
			m_DebugObject->Transform()->SetRelativePos((*iter).WorldPos);
			m_DebugObject->Transform()->SetRelativeScale((*iter).WorldScale);
			m_DebugObject->Transform()->SetRelativeRotation((*iter).WorldRotation);
			m_DebugObject->Transform()->SetMatrix();
		}
		else
		{
			m_DebugObject->Transform()->SetWorldMat((*iter).MatWorld);
		}

		// ����� ������Ʈ ������
		m_DebugObject->Render();

		// ���� �ð��� �Ѿ ������ �����Ѵ�.
		(*iter).CurTime += ENGINEDT;
		if ((*iter).Duration < (*iter).CurTime)
			iter = m_vecDebugInfo.erase(iter);
		else
			++iter;
	}
}
