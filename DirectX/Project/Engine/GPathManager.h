#pragma once

class GPathManager
{
private:
	static wchar_t g_ContentPath[255];
	static wchar_t g_BinPath[255];
	static wchar_t g_SolutionPath[255];

public:
	static void Init();
	static const wchar_t* GetContentPath() { return g_ContentPath; }
	static const wchar_t* GetBinPath() { return g_BinPath; }
	static const wchar_t* GetSolutionPath() { return g_SolutionPath; }
	static const wstring GetRelativePath(const wstring& _FullPath);

private:
	GPathManager() {}
	~GPathManager() {}

};

