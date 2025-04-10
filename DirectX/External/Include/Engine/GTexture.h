#pragma once
#include "GAsset.h"
class GTexture :
	public GAsset
{
private:
	ScratchImage							m_Image;	// DirectXTex ���̺귯������ ����, �̹��� ������ SystemMem�� �ε��ϴ� ����� �����Ǿ�����
	ComPtr<ID3D11Texture2D>					m_Tex2D;	// GPU �޸𸮿��� ����� �ؽ��ĸ� �����ϴ� ��ü
	D3D11_TEXTURE2D_DESC					m_Desc;		// Texture2D ����

	ComPtr<ID3D11RenderTargetView>			m_RTV;		// ����Ÿ������ �����ϴ� �뵵
	ComPtr<ID3D11DepthStencilView>			m_DSV;		// �X�� ���ٽ� �뵵�� �����ϴ� �뵵
	ComPtr<ID3D11ShaderResourceView>		m_SRV;		// GraphicShader ���� ���ҽ��� ����ϴ� �뵵(t)
	ComPtr<ID3D11UnorderedAccessView>		m_UAV;		// ComputeShader ���� �б� ���Ⱑ ������ �뵵(u)

	int                                 m_RecentSRV_CS;	// Binding ��ġ ���� �뵵
	int                                 m_RecentUAV_CS; // Binding ��ġ ���� �뵵

public:
	UINT GetWidth() { return m_Desc.Width; }
	UINT GetHeight() { return m_Desc.Height; }

	ComPtr<ID3D11RenderTargetView>		GetRTV() { return m_RTV; }		// ����Ÿ������ �����ϴ� �뵵
	ComPtr<ID3D11DepthStencilView>		GetDSV() { return m_DSV; }		// �X�� ���ٽ� �뵵�� �����ϴ� �뵵
	ComPtr<ID3D11ShaderResourceView>	GetSRV() { return m_SRV; }		// GraphicShader ���� ���ҽ��� ����ϴ� �뵵
	ComPtr<ID3D11UnorderedAccessView>	GetUAV() { return m_UAV; }		// ComputeShader ���� �б� ���Ⱑ ������ �뵵

	ComPtr<ID3D11Texture2D>				GetTex2D() { return m_Tex2D; }
public:
	// GraphicShader, t ��������
	void Binding(UINT _RegisterNum);

	// ComputeShader, t ��������
	void Binding_CS_SRV(UINT _RegisterNum);

	// ComputeShader, u ��������
	void Binding_CS_UAV(UINT _RegisterNum);

	void Clear_CS_SRV(int _RegisterNum = -1);
	void Clear_CS_UAV();

private:
	// _Flag : D3D11_BIND_FLAG
	int Create(UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _Flag, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
	int Create(ComPtr<ID3D11Texture2D> _Tex2D);
	int Load(const wstring& _FilePath) override;

public:
	int Save(const wstring& _FilePath) override;

public:
	CLONE_DELETE(GTexture);
public:
	GTexture(bool _EngineAsset = false);
	~GTexture();

	friend class GAssetManager;
};

