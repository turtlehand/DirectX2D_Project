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
	// Vertex 버퍼
	m_VtxCount = _VertextCount;

	// 버퍼의 종류 설정
	// 버퍼크기 설정
	m_VBDesc.ByteWidth = sizeof(Vertex) * m_VtxCount;

	// D3D11_USAGE_DEFAULT : 생성 이후엔 데이터 이동 없음
	// D3D11_USAGE_DYNAMIC : 생성 이후 데이터 이동 가능
	m_VBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA Sub = {};
	Sub.pSysMem = _VtxSysMem;

	// 디바이스를 사용하여 g_VB에 버퍼 생성
	DEVICE->CreateBuffer(&m_VBDesc, &Sub, m_VB.GetAddressOf());

	//============================================================================

	// Index 버퍼
	m_IdxCount = _IdxCount;

	// 버퍼의 종류 설정
	// 버퍼크기 설정
	m_IBDesc.ByteWidth = sizeof(UINT) * m_IdxCount;

	// D3D11_USAGE_DEFAULT : 생성 이후엔 데이터 이동 없음
	// D3D11_USAGE_DYNAMIC : 생성 이후 데이터 이동 가능
	m_IBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	Sub.pSysMem = _IdxSysMem;

	// 디바이스를 사용하여 g_IB에 버퍼 생성
	DEVICE->CreateBuffer(&m_IBDesc, &Sub, m_IB.GetAddressOf());

	// System 메모리에 정점과 인덱스 정보 기록
	m_VtxSysMem = new Vertex[m_VtxCount];
	m_IdxSysMem = new UINT[m_IdxCount];

	// _SysMem은 배열이므로 m_VtxSysMem을 복사한다.
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


