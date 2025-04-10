#pragma once
#include "GShader.h"

#include "GStructuredBuffer.h"

#include "GPathManager.h"
class GComputeShader :
	public GShader
{
protected:
	ComPtr<ID3DBlob>            m_CSBlob;
	ComPtr<ID3D11ComputeShader> m_CS;

	tMtrlConst					m_Const;
	const UINT m_GroupPerThreadX;
	const UINT m_GroupPerThreadY;
	const UINT m_GroupPerThreadZ;

	UINT m_GroupX;
	UINT m_GroupY;
	UINT m_GroupZ;



protected:
	int CreateComputeShader(const wstring& _RelativePath, const string& _FuncName);
	
public:
	int Excute();

private:
	virtual int Binding() = 0;
	virtual void CalcGroupNum() = 0;
	virtual void Clear() = 0;

public:
	virtual int Save(const wstring& _FilePath) { return S_OK; };
	virtual int Load(const wstring& _FilePath) { return S_OK; };

public:
	GComputeShader();
	GComputeShader(const wstring& _RelativePath, const string& _FuncName
		, UINT _GroupPerX, UINT _GroupPerY, UINT _GroupPerZ);
	~GComputeShader();
};

