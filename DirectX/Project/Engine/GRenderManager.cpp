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
	// 임시 DebugObject 객체 삭제
	if (nullptr != m_DebugObject)
		delete m_DebugObject;

	if (nullptr != m_Light2DBuffer)
		delete m_Light2DBuffer;
}

void GRenderManager::RegisterCamera(GCamera* _Cam, int _Priority)
{
	// 우선 순위가 -1 이라면 등록하지 않는다.
	if (_Priority == -1)
	{
		_Cam->m_Priority = -1;
		return;
	}

	// 우선순위 값이 카메라벡터 보다 크다면 카메라벡터의 사이즈를 그만큼 증가시킨다.
	if (m_vecCam.size() <= _Priority)
	{
		m_vecCam.resize(_Priority + 1);
	}

	// 이미 해당 우선순위의 카메라가 존재한다면 카메라를 미등록으로 바꾼다.
	if (m_vecCam[_Priority] != nullptr)
	{
		m_vecCam[_Priority]->m_Priority = -1;
	}

	// 카메라를 등록한다.
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
	// 출력 렌더타겟 및 출력 깊이타겟 설정
	Ptr<GTexture> pRTTex = GAssetManager::GetInst()->FindAsset<GTexture>(L"RenderTargetTex");
	Ptr<GTexture> pDSTex = GAssetManager::GetInst()->FindAsset<GTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	// Light2D Binding
	if (m_Light2DBuffer->GetElementCount() < m_vecLight2D.size())
	{
		// 구조화 버퍼 수정 X, SRV_ONLY
		// 구조화 버퍼 이동 O
		m_Light2DBuffer->Create(sizeof(tLight2DInfo), (UINT)m_vecLight2D.size(), SB_TYPE::SRV_ONLY, true);
	}

	// 각 광원의 tLight 구조체 정보를 벡터에 모음
	static vector<tLight2DInfo> vecLight2DInfo;
	vecLight2DInfo.clear();

	for (size_t i = 0;i < m_vecLight2D.size();++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetInfo());
	}

	// 광원이 없다면 바인딩하지 않는다.
	// 그럼 광원이 1개 였다가 사라지면?
	if (!vecLight2DInfo.empty())
	{
		// tLight2DInfo 정보를 구조체 적용
		m_Light2DBuffer->SetData(vecLight2DInfo.data(), sizeof(tLight2DInfo), (UINT)vecLight2DInfo.size());

		// t12에 바인딩
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
	// 카메라의 Render를 호출한다.
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
		// 디버그 모양에 따른 메쉬 선택
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

		// 깊이 판정에 따라서 DepthStencilState를 골라줌
		if ((*iter).DepthTest)
			m_DebugObject->Renderer()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_WRITE);
		else
			m_DebugObject->Renderer()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WIRTE);
	
		// 색상값을 재질을 통해서 전달
		m_DebugObject->MeshRender()->SetColor((*iter).Color);

		// 위치정보 세팅 및 월드행렬 계산
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

		// 디버그 오브젝트 렌더링
		m_DebugObject->Render();

		// 노출 시간이 넘어선 정보는 제거한다.
		(*iter).CurTime += ENGINEDT;
		if ((*iter).Duration < (*iter).CurTime)
			iter = m_vecDebugInfo.erase(iter);
		else
			++iter;
	}
}
