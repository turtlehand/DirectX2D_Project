#include "pch.h"
#include "GAsset.h"
#include "GAssetManager.h"

GAsset::GAsset(ASSET_TYPE _Type, bool _EngineAsset) :
	m_Type(_Type),
	m_RefCount(0),
	m_EngineAsset(_EngineAsset)
{
}

GAsset::~GAsset()
{
}