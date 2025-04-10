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
/// ���� ���� �� �ٲ� �� ���� ���� ���� ����
/// </summary>
/// <param name="_Idx"></param>
void GFlipbook::RemoveAnimation(int _Idx)
{
	// �ε����� 0���� �۰ų� �ø��� ������ ũ�⺸�� ���ų� ũ�ٸ�
	if (_Idx < 0 || m_vecAnimation.size() <= _Idx)
	{
		MessageBox(nullptr, L"������ ��ġ�� �߸��Ǿ����ϴ�.", L"�ø��� ���� ����", MB_OK);
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
	// �ε����� 0���� �۰ų� �ø��� ������ ũ�⺸�� ���ų� ũ�ٸ�
	if (_Idx < 0 || m_vecAnimation.size() <= _Idx)
	{
		MessageBox(nullptr, L"������ ��ġ�� �߸��Ǿ����ϴ�.", L"�ø��� ���� ����", MB_OK);
		assert(nullptr);
		return;
	}
	m_vecAnimation[_Idx].first = _Sprite;
	m_vecAnimation[_Idx].second = _Info;
}

void GFlipbook::SetSprite(Ptr<GSprite> _Sprite, int _Idx)
{
	// �ε����� 0���� �۰ų� �ø��� ������ ũ�⺸�� ���ų� ũ�ٸ�
	if (_Idx < 0 || m_vecAnimation.size() <= _Idx)
	{
		MessageBox(nullptr, L"������ ��ġ�� �߸��Ǿ����ϴ�.", L"�ø��� ���� ����", MB_OK);
		assert(nullptr);
		return;
	}

	m_vecAnimation[_Idx].first = _Sprite;
}

void GFlipbook::SetInfo(const tAnimationInfo& _Info, int _Idx)
{
	if (_Idx < 0 || m_vecAnimation.size() <= _Idx)
	{
		MessageBox(nullptr, L"������ ��ġ�� �߸��Ǿ����ϴ�.", L"�ø��� ���� ����", MB_OK);
		assert(nullptr);
		return;
	}

	m_vecAnimation[_Idx].second = _Info;

}

int GFlipbook::Save(const wstring& _FilePath)
{
	// �Էµ� ��ü��ο��� ContentPath ������ ��θ� �����, ����θ� �߶󳻼� �������ش�.
	wstring strContentPath = GPathManager::GetContentPath();
	wstring RelativePath = _FilePath.substr(strContentPath.length(), _FilePath.length());
	SetRelativePath(RelativePath);

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	assert(pFile);

	// ��������Ʈ ���� ����
	size_t SpriteCount = m_vecAnimation.size();
	fwrite(&SpriteCount, sizeof(size_t), 1, pFile);

	// ��������Ʈ Ű,��� ����
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

	// ��������Ʈ ���� �ҷ�����
	size_t SpriteCount = 0;
	fread(&SpriteCount, sizeof(size_t), 1, pFile);

	// ��������Ʈ Ű,��� �ҷ�����
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
