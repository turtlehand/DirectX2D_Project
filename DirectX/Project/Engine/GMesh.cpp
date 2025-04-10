#include "pch.h"
#include "GMesh.h"

#include "GDevice.h"

GMesh::GMesh(bool _EngineAsset) :
	GAsset(ASSET_TYPE::MESH, _EngineAsset),
	m_VB(nullptr),
	m_VBDesc{},
	m_VtxCount(0),
	m_VtxSysMem(nullptr),

	m_IB(nullptr),
	m_IBDesc{},
	m_IdxCount(0),
	m_IdxSysMem(nullptr)
{
}

GMesh::~GMesh()
{
	if (nullptr != m_VtxSysMem)
		delete m_VtxSysMem;
	if (nullptr != m_IdxSysMem)
		delete m_IdxSysMem;
}

int GMesh::Create(Vertex* _VtxSysMem, UINT _VertextCount, UINT* _IdxSysMem, UINT _IdxCount)
{
	// Vertex ����
	m_VtxCount = _VertextCount;

	// ������ ���� ����
	// ����ũ�� ����
	m_VBDesc.ByteWidth = sizeof(Vertex) * m_VtxCount;

	// D3D11_USAGE_DEFAULT : ���� ���Ŀ� ������ �̵� ����
	// D3D11_USAGE_DYNAMIC : ���� ���� ������ �̵� ����
	m_VBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA Sub = {};
	Sub.pSysMem = _VtxSysMem;

	// ����̽��� ����Ͽ� g_VB�� ���� ����
	DEVICE->CreateBuffer(&m_VBDesc, &Sub, m_VB.GetAddressOf());

	//============================================================================

	// Index ����
	m_IdxCount = _IdxCount;

	// ������ ���� ����
	// ����ũ�� ����
	m_IBDesc.ByteWidth = sizeof(UINT) * m_IdxCount;

	// D3D11_USAGE_DEFAULT : ���� ���Ŀ� ������ �̵� ����
	// D3D11_USAGE_DYNAMIC : ���� ���� ������ �̵� ����
	m_IBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	Sub.pSysMem = _IdxSysMem;

	// ����̽��� ����Ͽ� g_IB�� ���� ����
	DEVICE->CreateBuffer(&m_IBDesc, &Sub, m_IB.GetAddressOf());

	// System �޸𸮿� ������ �ε��� ���� ���
	m_VtxSysMem = new Vertex[m_VtxCount];
	m_IdxSysMem = new UINT[m_IdxCount];

	// _SysMem�� �迭�̹Ƿ� m_VtxSysMem�� �����Ѵ�.
	memcpy(m_VtxSysMem, _VtxSysMem, sizeof(Vertex) * m_VtxCount);
	memcpy(m_IdxSysMem, _IdxSysMem, sizeof(UINT) * m_IdxCount);

	return S_OK;
}

int GMesh::Save(const wstring& _FilePath)
{
	return 0;
}

int GMesh::Load(const wstring& _FilePath)
{
	return 0;
}

void GMesh::Binding()
{
	UINT Stride = sizeof(Vertex);
	UINT Offset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &Stride, &Offset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void GMesh::Render()
{
	Binding();
	CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}

void GMesh::Render_Particle(UINT _Count)
{
	Binding();
	CONTEXT->DrawIndexedInstanced(m_IdxCount, _Count, 0, 0, 0);
}


