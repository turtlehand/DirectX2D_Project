#pragma once
#include "GEntity.h"

enum class SB_TYPE
{
	SRV_ONLY,
	SRV_UAV
};

// �뷮�� ū ���� (�⺻������ �ؽ��� register�� ����)
class GStructuredBuffer :
	public GEntity
{
private:
	ComPtr<ID3D11Buffer>				m_SB;	// ���� ����
	ComPtr<ID3D11Buffer>				m_SB_Write;	// ���� ����
	ComPtr<ID3D11Buffer>				m_SB_Read;

	ComPtr<ID3D11ShaderResourceView>	m_SRV;	// t �������� ���ε�
	ComPtr<ID3D11UnorderedAccessView>	m_UAV;	// u �������� ���ε�(ComputeShader ����)
	D3D11_BUFFER_DESC					m_Desc;	// ���� ���� �ɼ�

	SB_TYPE								m_Type;
	bool								m_SysMemMove;

	UINT								m_ElementSize; // ������ 1���� ũ��
	UINT								m_ElementCount; // ������ ����

	int                                 m_RecentSRV_CS;	// Binding ��ġ ���� �뵵
	int                                 m_RecentUAV_CS; // Binding ��ġ ���� �뵵

public:
	UINT GetElementSize() { return m_ElementSize; }
	UINT GetElementCount() { return m_ElementCount; }
	UINT GetBufferSize() { return m_ElementSize * m_ElementCount; }

public:
	int Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _SystemMemMove, void* _InitialData = nullptr);
	void SetData(void* _Data, UINT _ElementSize = 0, UINT _ElementCount = 0);
	void GetData(void* _Dst, UINT _ElementSize = 0, UINT _ElementCount = 0);

	void Binding(int _RegisterNum);

	// ComputeShader, t ��������
	void Binding_CS_SRV(UINT _RegisterNum);

	// ComputeShader, u ��������
	void Binding_CS_UAV(UINT _RegisterNum);


	void Clear(int _RegisterNum);
	void Clear_CS_SRV();
	void Clear_CS_UAV();

public:
	CLONE(GStructuredBuffer);
	GStructuredBuffer();
	~GStructuredBuffer();

};

