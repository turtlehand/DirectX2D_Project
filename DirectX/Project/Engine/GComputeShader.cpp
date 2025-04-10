#include "pch.h"
#include "GComputeShader.h"

#include "GDevice.h"
#include "GPathManager.h"

#include "GConstBuffer.h"

GComputeShader::GComputeShader() 
    :GShader(ASSET_TYPE::COMPUTE_SHADER)
	, m_GroupPerThreadX(1)
	, m_GroupPerThreadY(1)
	, m_GroupPerThreadZ(1)
	, m_GroupX(1)
	, m_GroupY(1)
	, m_GroupZ(1)
{

}

GComputeShader::GComputeShader(const wstring& _RelativePath, const string& _FuncName
	, UINT _GroupPerX, UINT _GroupPerY, UINT _GroupPerZ)
	: GShader(ASSET_TYPE::COMPUTE_SHADER)
	, m_GroupPerThreadX(_GroupPerX)
	, m_GroupPerThreadY(_GroupPerY)
	, m_GroupPerThreadZ(_GroupPerZ)
	, m_GroupX(1)
	, m_GroupY(1)
	, m_GroupZ(1)
{
	CreateComputeShader(_RelativePath, _FuncName);
}

GComputeShader::~GComputeShader()
{

}

int GComputeShader::CreateComputeShader(const wstring& _RelativePath, const string& _FuncName)
{
	wstring strFilePath = GPathManager::GetContentPath() + _RelativePath;

	UINT Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strFilePath.c_str()
		, nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName.c_str(), "cs_5_0", Flag, 0
		, m_CSBlob.GetAddressOf()
		, m_ErrBlob.GetAddressOf())))
	{
		// ������ �����ϸ� �޼��� �ڽ��� ������ ���г��� ����
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "���̴� ������ ����", MB_OK);
		}
		else
		{
			MessageBoxA(nullptr, "���̴� ���� ��ΰ� �Źٸ��� �ʽ��ϴ�.", "���̴� ������ ����", MB_OK);
		}
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer()
		, m_CSBlob->GetBufferSize()
		, nullptr
		, m_CS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int GComputeShader::Excute()
{
	if (FAILED(Binding()))
	{
		assert(nullptr);
		return E_FAIL;
	}

	CalcGroupNum();

	// ������� ������ۿ� ���� �� CS ���ε�
	static GConstBuffer* pBuffer = GDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pBuffer->SetData(&m_Const, sizeof(tMtrlConst));
	pBuffer->Binding_CS();

	// ��ǻ�� ���̴� ���ε� �� ����
	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);
	CONTEXT->Dispatch(m_GroupX, m_GroupY, m_GroupZ);

	// ���ҽ� �� ���̴� Ŭ����
	Clear();
	CONTEXT->CSSetShader(nullptr, nullptr, 0);
}


