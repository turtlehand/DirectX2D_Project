#pragma once
#include "GEntity.h"

class GAsset :
	public GEntity
{


private:
	const ASSET_TYPE	m_Type;
	UINT				m_RefCount;

	wstring m_Key;
	wstring m_RelativePath;

	const bool m_EngineAsset;		// true 시 엔진에서 자체 생산되는 에셋

public:
	ASSET_TYPE GetType() const { return m_Type; }
	const wstring& GetKey() { return m_Key; }
	const wstring& GetRelativePath() { return m_RelativePath; }

	bool IsEngineAsset() { return m_EngineAsset; }

protected:
	void SetKey(const wstring& _Key) { m_Key = _Key; }
	void SetRelativePath(const wstring& _Path) { m_RelativePath = _Path; }

public:
	virtual int Save(const wstring& _FilePath) = 0;
	virtual int Load(const wstring& _FilePath) = 0;

public:
	void AddRef()
	{
		++m_RefCount;
	}
	void Release()
	{
		if (--m_RefCount <= 0)
		{
			delete this;
		}
	}
	
public:
	GAsset(ASSET_TYPE _Type, bool _EngineAsset = false);
	~GAsset();

	template<typename T>
	friend class Ptr;
	friend class GAssetManager;
};

