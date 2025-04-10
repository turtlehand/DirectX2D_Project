#pragma once
#include "GAsset.h"

// 모양
class GMesh :
	public GAsset
{
private:
	// 버텍스
	ComPtr<ID3D11Buffer> m_VB;
	D3D11_BUFFER_DESC m_VBDesc;
	UINT m_VtxCount;
	Vertex* m_VtxSysMem;

	// 인덱스
	ComPtr<ID3D11Buffer> m_IB;
	D3D11_BUFFER_DESC m_IBDesc;
	UINT m_IdxCount;
	UINT* m_IdxSysMem;

public:
	int Create(Vertex* _VerSysMem, UINT _VertextCount, UINT* _IdxSysMem, UINT _IdxCount);

	virtual int Save(const wstring& _FilePath) override;
	virtual int Load(const wstring& _FilePath) override;
	
	void Render();
	void Render_Particle(UINT _Count);

private:
	void Binding();

public:
	CLONE_DELETE(GMesh);
public:
	GMesh(bool _EngineAsset = false);
	~GMesh();


};

