#include "pch.h"
#include "GFlipbook.h"

#include "GPathManager.h"
#include "GAssetManager.h"

GFlipbook::GFlipbook(bool _EngineAsset)
	: GAsset(ASSET_TYPE::FLIPBOOK, _EngineAsset)
{
}

GFlipbook::~GFlipbook()
{
}

/// <summary>
/// 게임 실행 중 바뀔 때 오류 생길 수가 있음
/// </summary>
/// <param name="_Idx"></param>
void GFlipbook::RemoveAnimation(int _Idx)
{
	// 인덱스는 0보다 작거나 플립북 사이즈 크기보다 같거나 크다면
	if (_Idx < 0 || m_vecAnimation.size() <= _Idx)
	{
		MessageBox(nullptr, L"삭제할 위치가 잘못되었습니다.", L"플립북 에셋 오류", MB_OK);
		assert(nullptr);
		return;
	}

	vector<pair<Ptr<GSprite>, tAnimationInfo>>::iterator iter = m_vecAnimation.begin();

	for (int i = 0; i < _Idx; ++i)
		++iter;

	m_vecAnimation.erase(iter);
}

void GFlipbook::SetAnimation(Ptr<GSprite> _Sprite, const tAnimationInfo& _Info, int _Idx)
{
	// 인덱스는 0보다 작거나 플립북 사이즈 크기보다 같거나 크다면
	if (_Idx < 0 || m_vecAnimation.size() <= _Idx)
	{
		MessageBox(nullptr, L"설정할 위치가 잘못되었습니다.", L"플립북 에셋 오류", MB_OK);
		assert(nullptr);
		return;
	}
	m_vecAnimation[_Idx].first = _Sprite;
	m_vecAnimation[_Idx].second = _Info;
}

void GFlipbook::SetSprite(Ptr<GSprite> _Sprite, int _Idx)
{
	// 인덱스는 0보다 작거나 플립북 사이즈 크기보다 같거나 크다면
	if (_Idx < 0 || m_vecAnimation.size() <= _Idx)
	{
		MessageBox(nullptr, L"설정할 위치가 잘못되었습니다.", L"플립북 에셋 오류", MB_OK);
		assert(nullptr);
		return;
	}

	m_vecAnimation[_Idx].first = _Sprite;
}

void GFlipbook::SetInfo(const tAnimationInfo& _Info, int _Idx)
{
	if (_Idx < 0 || m_vecAnimation.size() <= _Idx)
	{
		MessageBox(nullptr, L"설정할 위치가 잘못되었습니다.", L"플립북 에셋 오류", MB_OK);
		assert(nullptr);
		return;
	}

	m_vecAnimation[_Idx].second = _Info;

}

int GFlipbook::Save(const wstring& _FilePath)
{
	// 입력된 전체경로에서 ContentPath 까지의 경로를 덜어내고, 상대경로만 잘라내서 설정해준다.
	wstring strContentPath = GPathManager::GetContentPath();
	wstring RelativePath = _FilePath.substr(strContentPath.length(), _FilePath.length());
	SetRelativePath(RelativePath);

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	assert(pFile);

	// 스프라이트 개수 저장
	size_t SpriteCount = m_vecAnimation.size();
	fwrite(&SpriteCount, sizeof(size_t), 1, pFile);

	// 스프라이트 키,경로 저장
	for (size_t i = 0; i < SpriteCount; ++i)
	{
		SaveAssetRef(m_vecAnimation[i].first, pFile);
		fwrite(&(m_vecAnimation[i].second), sizeof(tAnimationInfo), 1, pFile);
	}

	fclose(pFile);

	return S_OK;
}

int GFlipbook::Load(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");
	if (pFile == nullptr)
		return E_FAIL;

	// 스프라이트 개수 불러오기
	size_t SpriteCount = 0;
	fread(&SpriteCount, sizeof(size_t), 1, pFile);

	// 스프라이트 키,경로 불러오기
	for (size_t i = 0; i < SpriteCount; ++i)
	{
		pair<Ptr<GSprite>, tAnimationInfo> Animation;
		Animation.first = LoadAssetRef<GSprite>(pFile);
		fread(&(Animation.second), sizeof(tAnimationInfo), 1, pFile);
		m_vecAnimation.push_back(Animation);
	}

	fclose(pFile);

	return S_OK;
}
