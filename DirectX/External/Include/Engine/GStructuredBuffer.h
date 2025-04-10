#pragma once
#include "GEntity.h"

enum class SB_TYPE
{
	SRV_ONLY,
	SRV_UAV
};

// 용량이 큰 버퍼 (기본적으로 텍스쳐 register를 응용)
class GStructuredBuffer :
	public GEntity
{
private:
	ComPtr<ID3D11Buffer>				m_SB;	// 메인 버퍼
	ComPtr<ID3D11Buffer>				m_SB_Write;	// 쓰기 버퍼
	ComPtr<ID3D11Buffer>				m_SB_Read;

	ComPtr<ID3D11ShaderResourceView>	m_SRV;	// t 레지스터 바인딩
	ComPtr<ID3D11UnorderedAccessView>	m_UAV;	// u 레지스터 바인딩(ComputeShader 전용)
	D3D11_BUFFER_DESC					m_Desc;	// 버퍼 생성 옵션

	SB_TYPE								m_Type;
	bool								m_SysMemMove;

	UINT								m_ElementSize; // 데이터 1개의 크기
	UINT								m_ElementCount; // 데이터 개수

	int                                 m_RecentSRV_CS;	// Binding 위치 저장 용도
	int                                 m_RecentUAV_CS; // Binding 위치 저장 용도

public:
	UINT GetElementSize() { return m_ElementSize; }
	UINT GetElementCount() { return m_ElementCount; }
	UINT GetBufferSize() { return m_ElementSize * m_ElementCount; }

public:
	int Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _SystemMemMove, void* _InitialData = nullptr);
	void SetData(void* _Data, UINT _ElementSize = 0, UINT _ElementCount = 0);
	void GetData(void* _Dst, UINT _ElementSize = 0, UINT _ElementCount = 0);

	void Binding(int _RegisterNum);

	// ComputeShader, t 레지스터
	void Binding_CS_SRV(UINT _RegisterNum);

	// ComputeShader, u 레지스터
	void Binding_CS_UAV(UINT _RegisterNum);


	void Clear(int _RegisterNum);
	void Clear_CS_SRV();
	void Clear_CS_UAV();

public:
	CLONE(GStructuredBuffer);
	GStructuredBuffer();
	~GStructuredBuffer();

};

