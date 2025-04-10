#include "pch.h"
#include "GPathManager.h"

wchar_t GPathManager::g_ContentPath[255] = {};
wchar_t GPathManager::g_BinPath[255] = {};
wchar_t GPathManager::g_SolutionPath[255] = {};

void GPathManager::Init()
{
	wchar_t szBuff[255] = {};

	// ���� ���μ����� ���(����� ���� ������ ��� ����ó�� �ʿ�, ������Ʈ ����->Debug->�۾����͸� ����)
	GetCurrentDirectory(255, szBuff);

	// Bin ��� ����
	wcscpy_s(g_BinPath, 255, szBuff);
	wcscat_s(g_BinPath, 255, L"\\");

	for (size_t i = wcslen(szBuff); 0 <= i; --i)
	{
		if (L'\\' == szBuff[i])
		{
			szBuff[i] = L'\0';
			break;
		}
	}

	wcscat_s(szBuff, 255, L"\\Content\\");
	wcscpy_s(g_ContentPath, 255, szBuff);


}

/// <summary>
/// ��ü ��ο��� ������ ������ ������ ����θ� ����Ѵ�.
/// </summary>
/// <param name="_FullPath">��ü ���</param>
/// <returns></returns>
const wstring GPathManager::GetRelativePath(const wstring& _FullPath)
{
	wstring contentPaht = GetContentPath();

	// _FullPath���� ���������� ��θ� ã�� ���Ͽ��ٸ� 
	if (-1 == _FullPath.find(contentPaht.c_str()))
	{
		return wstring();
	}

	// ��ü ��ο��� ������ ��θ� ������ ��θ� ��ȯ�Ѵ�.
	return _FullPath.substr(contentPaht.length(), _FullPath.length());
}
