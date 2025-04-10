#include "pch.h"
#include "GMaterial.h"

#include "GDevice.h"
#include "GConstBuffer.h"

#include "GPathManager.h"
#include "GAssetManager.h"

GMaterial::GMaterial(bool _EngineAsset) :
	GAsset(ASSET_TYPE::MATERIAL, _EngineAsset),
	m_Const{}
{
}

GMaterial::~GMaterial()
{
}

void GMaterial::Binding()
{
	m_Shader->Binding();

	for (int i = 0;i < (UINT)TEX_PARAM::END;++i)
	{
		// 텍스쳐가 있다면
		if (nullptr != m_arrTex[i].Get())
		{
			m_arrTex[i]->Binding(i);
			m_Const.bTex[i] = true;
		}
		else
		{
			ID3D11ShaderResourceView* pSRV = nullptr;
			CONTEXT->VSSetShaderResources(i, 1, &pSRV);
			CONTEXT->HSSetShaderResources(i, 1, &pSRV);
			CONTEXT->DSSetShaderResources(i, 1, &pSRV);
			CONTEXT->GSSetShaderResources(i, 1, &pSRV);
			CONTEXT->PSSetShaderResources(i, 1, &pSRV);
			CONTEXT->PSSetShaderResources(i, 1, &pSRV);
			m_Const.bTex[i] = false;
		}
	}

	// 재질 상수값을 상수버퍼에 전달 및 바인딩
	static GConstBuffer* pBuffer = GDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pBuffer->SetData(&m_Const, sizeof(tMtrlConst));
	pBuffer->Binding();
}

int GMaterial::Save(const wstring& _FilePath)
{
	// 입력된 전체경로에서 ContentPath 까지의 경로를 덜어내고, 상대경로만 잘라내서 설정해준다.
	wstring strContentPath = GPathManager::GetContentPath();
	wstring RelativePath = _FilePath.substr(strContentPath.length(), _FilePath.length());
	SetRelativePath(RelativePath);

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	assert(pFile);

	// 셰이더 정보
	SaveAssetRef(m_Shader, pFile);

	// 텍스쳐 정보
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		SaveAssetRef(m_arrTex[i], pFile);
	}

	// 상수 데이터
	fwrite(&m_Const, sizeof(tMtrlConst), 1, pFile);

	fclose(pFile);

	return S_OK;
}
int GMaterial::Load(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");
	if (pFile == nullptr)
		return E_FAIL;

	// 셰이더 정보
	m_Shader = LoadAssetRef<GGraphicShader>(pFile);

	// 텍스쳐 정보
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		m_arrTex[i] = LoadAssetRef<GTexture>(pFile);
	}

	// 상수 데이터
	fread(&m_Const, sizeof(tMtrlConst), 1, pFile);

	fclose(pFile);

	return S_OK;
}