#pragma once
#include "GShader.h"

struct tScalarParam
{
	wstring          Desc;
	SCALAR_PARAM    Type;

	DWORD_PTR		Param0;
	DWORD_PTR		Param1;
};

struct tTexParam
{
	wstring          Desc;
	TEX_PARAM       Type;

	DWORD_PTR		Param0;
	DWORD_PTR		Param1;
};

// �׸��� ���̴�
class GGraphicShader :
	public GShader
{
private:
	// Shader�� �̿��ؼ� �������Ҷ� �ʿ���ϴ� ������ �� �ؽ��� ���ҽ� ����
	vector<tScalarParam>            m_ScalarParam;
	vector<tTexParam>               m_TexParam;

	ComPtr<ID3DBlob>				m_VSBlob;
	ComPtr<ID3D11VertexShader>		m_VS;

	ComPtr<ID3DBlob>				m_PSBlob;
	ComPtr<ID3D11PixelShader>		m_PS;

	ComPtr<ID3DBlob>				m_GSBlob;
	ComPtr<ID3D11GeometryShader>	m_GS;

	ComPtr<ID3D11InputLayout>		m_Layout;
		
	D3D11_PRIMITIVE_TOPOLOGY		m_Topology;
	SHADER_DOMAIN					m_Domain;

	RS_TYPE							m_RSType;
	DS_TYPE							m_DSType;
	BS_TYPE							m_BSType;

public:
	int CreateVertexShader(const wstring& _RelativePath, const string& _FuncName);
	int CreatePixelShader(const wstring& _RelativePath, const string& _FuncName);
	int CreateGeometryShader(const wstring& _RelativePath, const string& _FuncName);

	virtual int Save(const wstring& _FilePath) override;
	virtual int Load(const wstring& _FilePath) override;

	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_Topology = _Topology; }
	void SetRSType(RS_TYPE _Type) { m_RSType = _Type; }
	void SetDSType(DS_TYPE _Type) { m_DSType = _Type; }
	void SetBSType(BS_TYPE _Type) { m_BSType = _Type; }
	void Binding();

	void SetDomain(SHADER_DOMAIN _Domain) { m_Domain = _Domain; }
	SHADER_DOMAIN GetDomain() { return m_Domain; }

	void AddScalarParam(SCALAR_PARAM _Type, const wstring& _Desc, DWORD_PTR _Param0 = 0, DWORD_PTR _Param1 = 0);
	void AddTexParam(TEX_PARAM _Type, const wstring& _Desc, DWORD_PTR _Param0 = 0, DWORD_PTR _Param1 = 0);
	const vector<tScalarParam>& GetScalarParam() { return m_ScalarParam; }
	const vector<tTexParam>& GetTexParam() { return m_TexParam; }

public:
	GGraphicShader();
	~GGraphicShader();


};

