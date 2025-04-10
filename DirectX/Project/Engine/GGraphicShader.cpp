#include "pch.h"
#include "GGraphicShader.h"

#include "GPathManager.h"
#include "GDevice.h"

GGraphicShader::GGraphicShader() :
	GShader(ASSET_TYPE::GRAPHIC_SHADER),
	m_VSBlob(nullptr),
	m_PSBlob(nullptr),
	m_VS(nullptr),
	m_PS(nullptr),
	m_Layout(nullptr),
	m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	m_Domain(SHADER_DOMAIN::DOMAIN_MASKED),
	m_RSType(RS_TYPE::CULL_NONE),
	m_DSType(DS_TYPE::LESS),
	m_BSType(BS_TYPE::DEFAULT)
{

}

GGraphicShader::~GGraphicShader()
{

}

int GGraphicShader::CreateVertexShader(const wstring& _RelativePath, const string& _FuncName)
{
	// Vertex Shader �����ϱ�
	// fx �ҷ��� ���
	wstring strFilePath = CONTENTPATH + _RelativePath;

	UINT Flag = 0;
#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	// ��θ� ,0 ,0 ,�Լ� ,����
	if (FAILED(D3DCompileFromFile(strFilePath.c_str()
		, nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_FuncName.c_str()
		, "vs_5_0"
		, Flag
		, 0
		, m_VSBlob.GetAddressOf()
		, m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "���̴� ������ ����", MB_OK);
		}
		else
		{
			MessageBoxA(nullptr, "���̴� ���ϰ�ΰ� �Źٸ��� �ʽ��ϴ�.", "���̴� ������ ����", MB_OK);

		}
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), nullptr, m_VS.GetAddressOf())))
	{
		return E_FAIL;
	}

	// Input Layout
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[3] = {};

	LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;	// ũ��		
	LayoutDesc[0].InputSlot = 0;						// ������ �ε���
	LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[0].InstanceDataStepRate = 0;
	LayoutDesc[0].SemanticName = "POSITION";
	LayoutDesc[0].SemanticIndex = 0;
	LayoutDesc[0].AlignedByteOffset = 0;				// ���� ��ġ

	LayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	// ũ��		
	LayoutDesc[1].InputSlot = 0;							// ������ �ε���
	LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[1].InstanceDataStepRate = 0;
	LayoutDesc[1].SemanticName = "COLOR";
	LayoutDesc[1].SemanticIndex = 0;
	LayoutDesc[1].AlignedByteOffset = 12;					// ���� ��ġ

	LayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;	// ũ��		
	LayoutDesc[2].InputSlot = 0;						// ������ �ε���
	LayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[2].InstanceDataStepRate = 0;
	LayoutDesc[2].SemanticName = "TEXCOORD";
	LayoutDesc[2].SemanticIndex = 0;
	LayoutDesc[2].AlignedByteOffset = 28;			// ���� ��ġ

	if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 3
		, m_VSBlob->GetBufferPointer()
		, m_VSBlob->GetBufferSize(), m_Layout.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int GGraphicShader::CreatePixelShader(const wstring& _RelativePath, const string& _FuncName)
{
	// Pixel Shader �����ϱ�
	// fx �ҷ��� ���
	wstring strFilePath = CONTENTPATH + _RelativePath;
	UINT Flag = 0;

	// PixelShader
#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	// ��θ� ,0 ,0 ,�Լ� ,����
	if (FAILED(D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_FuncName.c_str(), "ps_5_0",
		Flag, 0,
		m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "���̴� ������ ����", MB_OK);
		}
		else
		{
			MessageBoxA(nullptr, "���̴� ���ϰ�ΰ� �Źٸ��� �ʽ��ϴ�.", "���̴� ������ ����", MB_OK);

		}
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf())))
	{
		return E_FAIL;
	}
	return S_OK;
}

int GGraphicShader::CreateGeometryShader(const wstring& _RelativePath, const string& _FuncName)
{
	// Pixel Shader �����ϱ�
	// fx �ҷ��� ���
	wstring strFilePath = CONTENTPATH + _RelativePath;
	UINT Flag = 0;

	// PixelShader
#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	// ��θ� ,0 ,0 ,�Լ� ,����
	if (FAILED(D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_FuncName.c_str(), "gs_5_0",
		Flag, 0,
		m_GSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "���̴� ������ ����", MB_OK);
		}
		else
		{
			MessageBoxA(nullptr, "���̴� ���ϰ�ΰ� �Źٸ��� �ʽ��ϴ�.", "���̴� ������ ����", MB_OK);

		}
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateGeometryShader(m_GSBlob->GetBufferPointer(), m_GSBlob->GetBufferSize(), nullptr, m_GS.GetAddressOf())))
	{
		return E_FAIL;
	}
	return S_OK;
}

int GGraphicShader::Save(const wstring& _FilePath)
{
	return 0;
}

int GGraphicShader::Load(const wstring& _FilePath)
{
	return 0;
}

void GGraphicShader::Binding()
{
	// InputLayout
	CONTEXT->IASetInputLayout(m_Layout.Get());

	// Topology
	CONTEXT->IASetPrimitiveTopology(m_Topology);

	// VertexShader
	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0);

	// GeomatryShader
	CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);
	
	// PixelShader
	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);

	

	// Resterize(Cull Type)
	CONTEXT->RSSetState(GDevice::GetInst()->GetRSState(m_RSType).Get());

	// DepthStencil
	CONTEXT->OMSetDepthStencilState(GDevice::GetInst()->GetDSState(m_DSType).Get(), 0);

	// BlendState
	CONTEXT->OMSetBlendState(GDevice::GetInst()->GetBSState(m_BSType).Get(),nullptr,0xffffff);
}

void GGraphicShader::AddScalarParam(SCALAR_PARAM _Type, const wstring& _Desc, DWORD_PTR _Param0, DWORD_PTR _Param1)
{
	tScalarParam ScalarP;
	ScalarP.Desc = _Desc;
	ScalarP.Type = _Type;
	ScalarP.Param0 = _Param0;
	ScalarP.Param1 = _Param1;
	m_ScalarParam.push_back(ScalarP);
}

void GGraphicShader::AddTexParam(TEX_PARAM _Type, const wstring& _Desc, DWORD_PTR _Param0, DWORD_PTR _Param1)
{
	tTexParam TextureP;
	TextureP.Desc = _Desc;
	TextureP.Type = _Type;
	TextureP.Param0 = _Param0;
	TextureP.Param1 = _Param1;
	m_TexParam.push_back(TextureP);
}
