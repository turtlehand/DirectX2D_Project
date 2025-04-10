#pragma once
#include "GSingleton.h"
#include "GTexture.h"

class GConstBuffer;

class GDevice :
	public GSingleton<GDevice>
{
	SINGLE(GDevice);
private:
	HWND								m_hMainWnd;
	Vector2								m_RenderResolution;			// 렌더타겟 해상도

	ComPtr<ID3D11Device>				m_Device;					// GPU 메모리 제어, DirectX 객체 생성
	ComPtr<ID3D11DeviceContext>			m_Context;					// GPU 명령 수행

	ComPtr<IDXGISwapChain>				m_SwapChain;				// Window 화면에 렌더링

	Ptr<GTexture>						m_RTTex;					// 렌더 타겟용 텍스쳐

	Ptr<GTexture>						m_DSTex;					// 뎁스 스텐실용 텍스쳐

	GConstBuffer*						m_ConstBuffer[(UINT)CB_TYPE::END];

	ComPtr<ID3D11RasterizerState>		m_RSState[(UINT)RS_TYPE::END];
	ComPtr<ID3D11DepthStencilState>		m_DSState[(UINT)DS_TYPE::END];
	ComPtr<ID3D11BlendState>			m_BSState[(UINT)BS_TYPE::END];

	ComPtr<ID3D11SamplerState>			m_Sampler[2];				// 샘플링(색상 추출)

public:
	int Init(HWND _OutputWnd, Vector2 _vResolution);

	void ClearTarget();
	void Present();

	ComPtr<ID3D11Device> GetDevice() { return m_Device; }
	ComPtr<ID3D11DeviceContext> GetContext() { return m_Context; }
	GConstBuffer* GetConstBuffer(CB_TYPE _Type) { return m_ConstBuffer[(UINT)_Type]; }
	Vector2 GetRenderResolution() { return m_RenderResolution; }
	ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _Type) { return m_RSState[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_DSState[(UINT)_Type]; }
	ComPtr<ID3D11BlendState> GetBSState(BS_TYPE _Type) { return m_BSState[(UINT)_Type]; }

private:
	int CreateSwapChain();
	int CreateView();
	void CreateConstBuffer();
	void CreateRasterizerState();
	void CreateSamplerState();
	void CreateDepthStencilState();
	void CreateBlendState();
};

