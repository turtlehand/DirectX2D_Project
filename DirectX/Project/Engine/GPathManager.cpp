#include "pch.h"
#include "GPathManager.h"

wchar_t GPathManager::g_ContentPath[255] = {};
wchar_t GPathManager::g_BinPath[255] = {};
wchar_t GPathManager::g_SolutionPath[255] = {};

void GPathManager::Init()
{
	wchar_t szBuff[255] = {};

	// 실행 프로세스의 경로(디버깅 모드로 실행할 경우 예외처리 필요, 프로젝트 구성->Debug->작업디렉터리 세팅)
	GetCurrentDirectory(255, szBuff);

	// Bin 경로 복사
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
/// 전체 경로에서 컨텐츠 폴더를 제외한 상대경로만 출력한다.
/// </summary>
/// <param name="_FullPath">전체 경로</param>
/// <returns></returns>
const wstring GPathManager::GetRelativePath(const wstring& _FullPath)
{
	wstring contentPaht = GetContentPath();

	// _FullPath에서 컨텐츠파일 경로를 찾지 못하였다면 
	if (-1 == _FullPath.find(contentPaht.c_str()))
	{
		return wstring();
	}

	// 전체 경로에서 컨텐츠 경로를 제외한 경로만 반환한다.
	return _FullPath.substr(contentPaht.length(), _FullPath.length());
}
