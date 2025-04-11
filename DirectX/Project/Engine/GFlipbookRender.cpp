#include "pch.h"
#include "GFlipbookRender.h"

#include "GTimeManager.h"
#include "GAssetManager.h"

#include "GTransform.h"
#include "GFlipbook.h"

GFlipbookRender::GFlipbookRender()
	:GRenderComponent(COMPONENT_TYPE::FLIPBOOK_RENDER)
	, m_vecFlipbook{}
	, m_CurFlipbookIndex(-1)
	, m_SpriteIdx(0)
	, m_Time(0)
	, m_Repeat(true)
	, m_Finish(false)
{
	SetMesh(GAssetManager::GetInst()->FindAsset<GMesh>(L"RectMesh"));
	CreateFlipbookMaterial();
}

GFlipbookRender::~GFlipbookRender()
{
}

void GFlipbookRender::CreateFlipbookMaterial()
{
	if (nullptr == GAssetManager::GetInst()->FindAsset<GGraphicShader>(L"FlipbookShader"))
	{
		// FlipbookShader
		Ptr<GGraphicShader> pShader = new GGraphicShader;
		pShader->CreateVertexShader(L"HLSL\\flipbook.fx", "VS_Flipbook");
		pShader->CreatePixelShader(L"HLSL\\flipbook.fx", "PS_Flipbook");
		pShader->SetRSType(RS_TYPE::CULL_NONE);
		pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
		GAssetManager::GetInst()->AddAsset(L"FlipbookShader", pShader.Get());

		pShader->AddTexParam(TEX_PARAM::TEX_0, L"Atlas");

		pShader->AddScalarParam(SCALAR_PARAM::VEC2_0, L"LeftTop");
		pShader->AddScalarParam(SCALAR_PARAM::VEC2_1, L"Slice");
		pShader->AddScalarParam(SCALAR_PARAM::VEC2_2, L"Offset");
		pShader->AddScalarParam(SCALAR_PARAM::VEC2_3, L"BackGround");
		pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, L"Color");
	}

	if (nullptr == GAssetManager::GetInst()->FindAsset<GMaterial>(L"FlipbookMtrl"))
	{
		// FlipbookMtrl
		Ptr<GMaterial> pMtrl = new GMaterial;
		pMtrl->SetShader(GAssetManager::GetInst()->FindAsset<GGraphicShader>(L"FlipbookShader"));
		GAssetManager::GetInst()->AddAsset(L"FlipbookMtrl", pMtrl.Get());
	}

	SetMaterial(GAssetManager::GetInst()->FindAsset<GMaterial>(L"FlipbookMtrl"));
}

void GFlipbookRender::AddFlipBook(Ptr<GFlipbook> _Flipbook)
{
	m_vecFlipbook.push_back(_Flipbook);
}

void GFlipbookRender::SetFlipbook(Ptr<GFlipbook> _Flipbook, int _Idx)
{
	if (_Idx < 0 || m_vecFlipbook.size() <= _Idx)
	{
		MessageBox(nullptr, L"오류", L"잘못된 위치입니다", MB_OK);
		return;
	}

	m_vecFlipbook[_Idx] = _Flipbook;
}

void GFlipbookRender::RemoveFlipbbok(int _Idx)
{
	if (_Idx < 0 || m_vecFlipbook.size() <= _Idx)
	{
		MessageBox(nullptr, L"오류", L"잘못된 위치입니다", MB_OK);
		return;
	}

	vector<Ptr<GFlipbook>>::iterator iter = m_vecFlipbook.begin();

	for (int i = 0; i < _Idx; ++i)
		++iter;

	m_vecFlipbook.erase(iter);

	m_CurFlipbookIndex = -1;
	m_SpriteIdx = 0;
}

void GFlipbookRender::FinalUpdate()
{
	// 현재 플립북을 선택하지 않았거나, 해당 플립북에 애니메이션이 없거나, 중단 상태라면
	if (m_CurFlipbookIndex < 0)
		return;
	if (m_vecFlipbook[m_CurFlipbookIndex]->GetAnimationCount() < 1 || m_Pause)
		return;

	if (m_Finish)
	{
		if (m_Repeat)
		{
			m_SpriteIdx = 0;
			m_Finish = false;
		}
		else
		{
			return;
		}
	}

	m_Time += DT;

	// AnimationTransform을 켜면 저장된 위치로 고정됩니다.
	if (m_vecFlipbook[m_CurFlipbookIndex]->GetInfo(m_SpriteIdx).AnimationTransform)
	{
		Transform()->SetRelativePos(m_vecFlipbook[m_CurFlipbookIndex]->GetInfo(m_SpriteIdx).Position);
		Transform()->SetRelativeScale(m_vecFlipbook[m_CurFlipbookIndex]->GetInfo(m_SpriteIdx).Scale);
		Transform()->SetRelativeRotation(m_vecFlipbook[m_CurFlipbookIndex]->GetInfo(m_SpriteIdx).Rotation);
	}

	if (m_vecFlipbook[m_CurFlipbookIndex]->GetInfo(m_SpriteIdx).Duration < m_Time)
	{
		m_Time -= m_vecFlipbook[m_CurFlipbookIndex]->GetInfo(m_SpriteIdx).Duration;

		++m_SpriteIdx;

		if (m_vecFlipbook[m_CurFlipbookIndex]->GetAnimationCount() <= m_SpriteIdx)
		{
			m_Finish = true;
			m_SpriteIdx -= 1;
		}
	}
}

void GFlipbookRender::Render()
{

	// 현재 플립북을 선택하지 않았을 수도 있음 m_CurIndex가 0 미만이라면 선택되지 않았음
	// 선택된 플립북의 애니메이션이 없었을 수도 있음
	if (m_CurFlipbookIndex < 0 || m_vecFlipbook[m_CurFlipbookIndex]->GetAnimationCount() < 1)
		return;

	// 현재 진행중인 프레임 스프라이트를 출력
	Ptr<GSprite> pCurSprite = m_vecFlipbook[m_CurFlipbookIndex]->GetSprite(m_SpriteIdx);

	// 플립북 특정 애니메이션에 스프라이트를 채우지 않았을 수도 있음
	if (pCurSprite == nullptr)
		return;

	Ptr<GTexture> pAtlas = pCurSprite->GetAtlasTex();
	Vector2 vLeftTop = pCurSprite->GetLeftTop();
	Vector2 vSlice = pCurSprite->GetSlice();
	Vector2 vOffset = pCurSprite->GetOffset();
	Vector2 vBackGround = pCurSprite->GetBackGround();
	const tAnimationInfo& tInfo = m_vecFlipbook[m_CurFlipbookIndex]->GetInfo(m_SpriteIdx);

	Transform()->Binding();

	if (nullptr != pCurSprite)
	{
		GetMaterial()->SetTexture(TEX_PARAM::TEX_0, pAtlas);

		GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, vLeftTop);
		GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, vSlice);
		GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_2, vOffset);
		GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_3, vBackGround);	
		GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, tInfo.Color);
	}
	else
	{
		GetMaterial()->SetTexture(TEX_PARAM::TEX_0, nullptr);
	}

	GetMaterial()->Binding();

	GetMesh()->Render();
}

void GFlipbookRender::SaveToFile(FILE* _File)
{
	GRenderComponent::SaveToFile(_File);

	size_t FlipbookCount = m_vecFlipbook.size();
	fwrite(&FlipbookCount, sizeof(size_t), 1, _File);
	for (size_t i = 0; i < FlipbookCount; ++i)
	{
		SaveAssetRef(m_vecFlipbook[i], _File);
	}
	fwrite(&m_CurFlipbookIndex, sizeof(int), 1, _File);

	fwrite(&m_SpriteIdx, sizeof(int), 1, _File);
	fwrite(&m_Repeat, sizeof(bool), 1, _File);
}

void GFlipbookRender::LoadFromFile(FILE* _File)
{
	GRenderComponent::LoadFromFile(_File);

	size_t FlipbookCount = 0;
	fread(&FlipbookCount, sizeof(size_t), 1, _File);
	m_vecFlipbook.clear();
	for (size_t i = 0; i < FlipbookCount; ++i)
	{
		m_vecFlipbook.push_back(LoadAssetRef<GFlipbook>(_File));
	}
	fread(&m_CurFlipbookIndex, sizeof(int), 1, _File);

	fread(&m_SpriteIdx, sizeof(int), 1, _File);
	fread(&m_Repeat, sizeof(bool), 1, _File);
}



