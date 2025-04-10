#pragma once
#include "GAsset.h"
class GShader :
	public GAsset
{
protected:
	ComPtr<ID3DBlob> m_ErrBlob;

public:
	CLONE_DELETE(GShader);

public:
	GShader(ASSET_TYPE _TYPE);
	~GShader();
};

