#include "pch.h"
#include "GConstBuffer.h"

#include "GDevice.h"

GConstBuffer::GConstBuffer() :
	m_Desc{},
	m_Type(CB_TYPE::END)
{
}

GConstBuffer::~GConstBuffer()
{
}

int GConstBuffer::Create(CB_TYPE _Type, UINT _BufferSize)
{
	m_Type = _Type;

	// ��ü�� ��ġ���� ���޽�Ű�� ���� ������� ����
	D3D11_BUFFER_DESC m_Desc = {};

	m_Desc.ByteWidth = _BufferSize;
	// ���� ���Ŀ� ������ �̵� ����
	m_Desc.Usage =			D3D11_USAGE_DYNAMIC;
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_Desc.BindFlags =		D3D11_BIND_CONSTANT_BUFFER;

	if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, m_Buffer.GetAddressOf())))
	{
		return E_FAIL;
	}
    return S_OK;
}

void GConstBuffer::SetData(void* _Data, UINT _DataSize)
{
	// _Data�� ������۷� �̵���Ų��.
	D3D11_MAPPED_SUBRESOURCE tMapSub = {};
	CONTEXT->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMapSub);
	memcpy(tMapSub.pData, _Data, _DataSize);
	CONTEXT->Unmap(m_Buffer.Get(), 0);
}

void GConstBuffer::Binding()
{
	CONTEXT->VSSetConstantBuffers((UINT)m_Type, 1, m_Buffer.GetAddressOf());
	CONTEXT->HSSetConstantBuffers((UINT)m_Type, 1, m_Buffer.GetAddressOf());
	CONTEXT->DSSetConstantBuffers((UINT)m_Type, 1, m_Buffer.GetAddressOf());
	CONTEXT->GSSetConstantBuffers((UINT)m_Type, 1, m_Buffer.GetAddressOf());
	CONTEXT->PSSetConstantBuffers((UINT)m_Type, 1, m_Buffer.GetAddressOf());
}

void GConstBuffer::Binding_CS()
{
	CONTEXT->CSSetConstantBuffers((UINT)m_Type, 1, m_Buffer.GetAddressOf());
}
