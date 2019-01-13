#pragma once

#ifndef __USUALLYUTILITY_H_
#define __USUALLYUTILITY_H_

#include <map>
#include <regex>
#include <string>
#include <vector>

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <atlconv.h>
#include <shellapi.h>
#include <atlexcept.h>

#define SYSTEM_PATH "SYSTEM32\\"
#define ADB_PATH    "ADB\\"

#define ADB_NAME    "ADB.EXE"
#define CMD_NAME    "CMD.EXE"

#include <codecvt>

#define AToW_EX(s,l) std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>(new std::codecvt<wchar_t, char, std::mbstate_t>(l)).from_bytes(s)
#define WToA_EX(s,l) std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>(new std::codecvt<wchar_t, char, std::mbstate_t>(l)).to_bytes(s)
#define AToW(s) AToW_EX(s,"chs")
#define WToA(s) WToA_EX(s,"chs")

#define WToUTF8(s) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(s)
#define UTF8ToW(s) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(s)

#if !defined(_UNICODE) && !defined(UNICODE)
#define AToT_EX(s,l)	WToA_EX(AToW_EX(s,l),l)
#define TToA_EX(s,l)	WToA_EX(AToW_EX(s,l),l)
#define WToT_EX(s,l)	WToA_EX(s,l)
#define TToW_EX(s,l)	AToW_EX(s,l)
#define AToT(s)	s
#define TToA(s)	s
#define WToT(s)	WToA(s)
#define TToW(s)	AToW(s)
#define TToUTF8(s)	WToUTF8(AToW(s))
#define AToUTF8(s)	WToUTF8(AToW(s))
#define UTF8ToA(s)	WToA(UTF8ToW(s))
#define UTF8ToT(s)	WToA(UTF8ToW(s))
#else
#define AToT_EX(s,l)	AToW_EX(s,l)
#define TToA_EX(s,l)	WToA_EX(s,l)
#define WToT_EX(s,l)	AToW_EX(WToA_EX(s,l),l)
#define TToW_EX(s,l)	AToW_EX(WToA_EX(s,l),l)
#define AToT(s) AToW(s)
#define TToA(s)	WToA(s)
#define WToT(s)	s
#define TToW(s)	s
#define TToUTF8(s)	WToUTF8(s)
#define AToUTF8(s)	WToUTF8(AToW(s))
#define UTF8ToA(s)	WToA(UTF8ToW(s))
#define UTF8ToT(s)	UTF8ToW(s)
#endif

__inline static bool string_regex_valid(std::string data, std::string pattern)
{
	return std::regex_match(data, std::regex(pattern));
}
__inline static size_t string_regex_replace_all(std::string & result, std::string & data, std::string replace, std::string pattern, std::regex_constants::match_flag_type matchflagtype = std::regex_constants::match_default)
{
	try
	{
		data = std::regex_replace(data, std::regex(pattern), replace, matchflagtype);
	}
	catch (const std::exception & e)
	{
		result = e.what();
	}
	return data.length();
}
__inline static size_t string_regex_find(std::string & result, std::vector<std::vector<std::string>> & svv, std::string & data, std::string pattern)
{
	std::smatch smatch;
	std::string::const_iterator ite = data.end();
	std::string::const_iterator itb = data.begin();
	try
	{
		//如果匹配成功
		while (std::regex_search(itb, ite, smatch, std::regex(pattern)))
		{
			if (smatch.size() > 1)
			{
				for (size_t stidx = svv.size() + 1; stidx < smatch.size(); stidx++)
				{
					svv.push_back(std::vector<std::string>());
				}
				for (size_t stidx = 1; stidx < smatch.size(); stidx++)
				{
					svv.at(stidx - 1).push_back(std::string(smatch[stidx].first, smatch[stidx].second));
					itb = smatch[stidx].second;
				}
			}
		}
	}
	catch (const std::exception & e)
	{
		result = e.what();
	}

	return svv.size();
}
__inline static std::string::size_type string_reader(std::string &result, std::string strData,
	std::string strStart, std::string strFinal, std::string::size_type stPos = 0,
	bool bTakeStart = false, bool bTakeFinal = false)
{
	std::string::size_type stRetPos = std::string::npos;
	std::string::size_type stStartPos = stPos;
	std::string::size_type stFinalPos = std::string::npos;
	
	stStartPos = strData.find(strStart, stStartPos);
	if (stStartPos != std::string::npos)
	{
		stRetPos = stFinalPos = strData.find(strFinal, stStartPos + strStart.length());
		if (stFinalPos != std::string::npos)
		{
			if (!bTakeStart)
			{
				stStartPos += strStart.length();
			}
			if (bTakeFinal)
			{
				stFinalPos += strFinal.length();
			}
			result = strData.substr(stStartPos, stFinalPos - stStartPos);
		}
	}

	return stRetPos;
}
__inline static std::string string_reader(std::string strData,
	std::string strStart, std::string strFinal,
	bool bTakeStart = false, bool bTakeFinal = false)
{
	std::string strRet = ("");
	std::string::size_type stStartPos = std::string::npos;
	std::string::size_type stFinalPos = std::string::npos;
	stStartPos = strData.find(strStart);
	if (stStartPos != std::string::npos)
	{
		stFinalPos = strData.find(strFinal, stStartPos + strStart.length());
		if (stFinalPos != std::string::npos)
		{
			if (!bTakeStart)
			{
				stStartPos += strStart.length();
			}
			if (bTakeFinal)
			{
				stFinalPos += strFinal.length();
			}
			strRet = strData.substr(stStartPos, stFinalPos - stStartPos);
		}
	}

	return strRet;
}
__inline static std::string string_replace_all(std::string &strData, std::string strDst, std::string strSrc, std::string::size_type stPos = 0)
{
	while ((stPos = strData.find(strSrc, stPos)) != std::string::npos)
	{
		strData.replace(stPos, strSrc.length(), strDst);
	}

	return strData;
}
__inline static size_t string_split_to_vector(std::vector<std::string> & sv, std::string strData, std::string strSplitter, std::string::size_type stPos = 0)
{
	std::string strTmp = ("");
	std::string::size_type stIdx = 0;
	std::string::size_type stSize = strData.length();

	while ((stPos = strData.find(strSplitter, stIdx)) != std::string::npos)
	{
		strTmp = strData.substr(stIdx, stPos - stIdx);
		if (!strTmp.length())
		{
			strTmp = ("");
		}
		sv.push_back(strTmp);

		stIdx = stPos + strSplitter.length();
	}

	if (stIdx < stSize)
	{
		strTmp = strData.substr(stIdx, stSize - stIdx);
		if (!strTmp.length())
		{
			strTmp = ("");
		}
		sv.push_back(strTmp);
	}

	return sv.size();
}

__inline static bool wstring_regex_valid(std::wstring data, std::wstring pattern)
{
	return std::regex_match(data, std::wregex(pattern));
}
__inline static size_t wstring_regex_find(std::wstring & result, std::vector<std::vector<std::wstring>> & svv, std::wstring & data, std::wstring pattern)
{
	std::wsmatch smatch;
	std::wstring::const_iterator ite = data.end();
	std::wstring::const_iterator itb = data.begin();

	try
	{
		//如果匹配成功 
		while (std::regex_search(itb, ite, smatch, std::wregex(pattern))) 
		{
			if (smatch.size() > 1)
			{
				for (size_t stidx = svv.size() + 1; stidx < smatch.size(); stidx++)
				{
					svv.push_back(std::vector<std::wstring>());
				}
				for (size_t stidx = 1; stidx < smatch.size(); stidx++)
				{
					svv.at(stidx - 1).push_back(std::wstring(smatch[stidx].first, smatch[stidx].second));
					itb = smatch[stidx].second;
				}
			}
		}
	}
	catch (const std::exception & e)
	{
		result = std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>(new std::codecvt<wchar_t, char, std::mbstate_t>("chs")).from_bytes(e.what());
	}

	return svv.size();
}
__inline static size_t wstring_regex_replace_all(std::wstring & result, std::wstring & data, std::wstring replace, std::wstring pattern, std::regex_constants::match_flag_type matchflagtype = std::regex_constants::match_default)
{
	try
	{
		data = std::regex_replace(data, std::wregex(pattern), replace, matchflagtype);
	}
	catch (const std::exception & e)
	{
		result = std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>(new std::codecvt<wchar_t, char, std::mbstate_t>("chs")).from_bytes(e.what());
	}
	return data.length();
}
__inline static std::wstring::size_type wstring_reader(std::wstring &result, std::wstring wstrData,
	std::wstring wstrStart, std::wstring wstrFinal, std::wstring::size_type stPos = 0,
	bool bTakeStart = false, bool bTakeFinal = false)
{
	std::wstring::size_type stRetPos = std::wstring::npos;
	std::wstring::size_type stStartPos = stPos;
	std::wstring::size_type stFinalPos = std::wstring::npos;
	
	stStartPos = wstrData.find(wstrStart);
	if (stStartPos != std::wstring::npos)
	{
		stRetPos = stFinalPos = wstrData.find(wstrFinal, stStartPos + wstrStart.length());
		if (stFinalPos != std::wstring::npos)
		{
			if (!bTakeStart)
			{
				stStartPos += wstrStart.length();
			}
			if (bTakeFinal)
			{
				stFinalPos += wstrFinal.length();
			}
			result = wstrData.substr(stStartPos, stFinalPos - stStartPos);
		}
	}

	return stRetPos;
}
__inline static std::wstring wstring_reader(std::wstring wstrData,
	std::wstring wstrStart, std::wstring wstrFinal,
	bool bTakeStart = false, bool bTakeFinal = false)
{
	std::wstring wstrRet = (L"");
	std::wstring::size_type stStartPos = std::wstring::npos;
	std::wstring::size_type stFinalPos = std::wstring::npos;
	stStartPos = wstrData.find(wstrStart);
	if (stStartPos != std::wstring::npos)
	{
		stFinalPos = wstrData.find(wstrFinal, stStartPos + wstrStart.length());
		if (stFinalPos != std::wstring::npos)
		{
			if (!bTakeStart)
			{
				stStartPos += wstrStart.length();
			}
			if (bTakeFinal)
			{
				stFinalPos += wstrFinal.length();
			}
			wstrRet = wstrData.substr(stStartPos, stFinalPos - stStartPos);
		}
	}

	return wstrRet;
}
__inline static std::wstring wstring_replace_all(std::wstring &wstrData, std::wstring wstrDst, std::wstring wstrSrc, std::wstring::size_type stPos = 0)
{
	while ((stPos = wstrData.find(wstrSrc, stPos)) != std::wstring::npos)
	{
		wstrData.replace(stPos, wstrSrc.length(), wstrDst);
	}

	return wstrData;
}
__inline static size_t wstring_split_to_vector(std::vector<std::wstring> & wsv, std::wstring wstrData, std::wstring wstrSplitter, std::wstring::size_type stPos = 0)
{
	std::wstring wstrTemp = (L"");
	std::wstring::size_type stIdx = 0;
	std::wstring::size_type stSize = wstrData.length();

	while ((stPos = wstrData.find(wstrSplitter, stIdx)) != std::wstring::npos)
	{
		wstrTemp = wstrData.substr(stIdx, stPos - stIdx);
		if (!wstrTemp.length())
		{
			wstrTemp = (L"");
		}
		wsv.push_back(wstrTemp);

		stIdx = stPos + wstrSplitter.length();
	}

	if (stIdx < stSize)
	{
		wstrTemp = wstrData.substr(stIdx, stSize - stIdx);
		if (!wstrTemp.length())
		{
			wstrTemp = (L"");
		}
		wsv.push_back(wstrTemp);
	}
	return wsv.size();
}

#if !defined(UNICODE) && !defined(_UNICODE)
#define TO_LOWER tolower
#define TO_UPPER toupper
#define TSTRING std::string
#define TO_TSTRING std::to_string
#define TSTRING_REGEX_VALID string_regex_valid
#define TSTRING_REGEX_REPLACE_ALL string_regex_replace_all
#define TSTRING_REGEX_FIND string_regex_find
#define TSTRING_READER string_reader
#define TSTRING_REPLACE_ALL string_replace_all
#define TSTRING_SPLIT_TO_VECTOR string_split_to_vector
#else
#define TO_LOWER towlower
#define TO_UPPER towupper
#define TSTRING std::wstring
#define TO_TSTRING std::to_wstring
#define TSTRING_REGEX_VALID wstring_regex_valid
#define TSTRING_REGEX_REPLACE_ALL wstring_regex_replace_all
#define TSTRING_REGEX_FIND wstring_regex_find
#define TSTRING_READER wstring_reader
#define TSTRING_REPLACE_ALL wstring_replace_all
#define TSTRING_SPLIT_TO_VECTOR wstring_split_to_vector
#endif

#define _tstring TSTRING
#define tstring TSTRING
#define _to_tstring TO_TSTRING
#define to_tstring TO_TSTRING
#define tstring_regex_valid TSTRING_REGEX_VALID
#define _tstring_regex_valid TSTRING_REGEX_VALID
#define tstring_regex_replace_all TSTRING_REGEX_REPLACE_ALL
#define _tstring_regex_replace_all TSTRING_REGEX_REPLACE_ALL
#define tstring_regex_find TSTRING_REGEX_FIND
#define _tstring_regex_find TSTRING_REGEX_FIND
#define tstring_reader TSTRING_READER
#define _tstring_reader TSTRING_READER
#define tstring_replace_all TSTRING_REPLACE_ALL
#define _tstring_replace_all TSTRING_REPLACE_ALL
#define tstring_split_to_vector TSTRING_SPLIT_TO_VECTOR
#define _tstring_split_to_vector TSTRING_SPLIT_TO_VECTOR

//初始化调试窗口显示
__inline static void InitDebugConsole(const _TCHAR * ptText = _T("TraceDebugWindow"))
{
	if (::AllocConsole())
	{
		::SetConsoleTitle(ptText);

		_tfreopen(_T("CONIN$"), _T("rb"), stdin);
		_tfreopen(_T("CONOUT$"), _T("wb"), stdout);
		_tfreopen(_T("CONOUT$"), _T("wb"), stderr);
		_tsetlocale(LC_ALL, _T("chs"));	
	}
}

//释放掉调试窗口显示
__inline static void ExitDebugConsole()
{
    fclose(stderr);
    fclose(stdin);
    fclose(stdout);

	::FreeConsole();
}

__inline static void RegisterDropFilesEvent(HWND hWnd)
{
#ifndef WM_COPYGLOBALDATA
#define WM_COPYGLOBALDAYA	0x0049
#endif

#ifndef MSGFLT_ADD
#define MSGFLT_ADD 1
#endif

#ifndef MSGFLT_REMOVE
#define MSGFLT_REMOVE 2
#endif
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_ACCEPTFILES);
	typedef BOOL(WINAPI *LPFN_ChangeWindowMessageFilter)(__in UINT message, __in DWORD dwFlag);
	LPFN_ChangeWindowMessageFilter pfnChangeWindowMessageFilter = (LPFN_ChangeWindowMessageFilter)GetProcAddress(GetModuleHandle(_T("USER32.DLL")), "ChangeWindowMessageFilter");
	if (pfnChangeWindowMessageFilter)
	{
		pfnChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
		pfnChangeWindowMessageFilter(WM_COPYDATA, MSGFLT_ADD);
		pfnChangeWindowMessageFilter(WM_COPYGLOBALDAYA, MSGFLT_ADD);// 0x0049 == WM_COPYGLOBALDATA
	}
}
__inline static size_t GetDropFiles(std::map<TSTRING, TSTRING> * pttmap, HDROP hDropInfo)
{
	UINT nIndex = 0;
	UINT nNumOfFiles = 0;
	_TCHAR tszFilePathName[MAX_PATH + 1] = { 0 };

	//得到文件个数
	nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (nIndex = 0; nIndex < nNumOfFiles; nIndex++)
	{
		//得到文件名
		DragQueryFile(hDropInfo, nIndex, (LPTSTR)tszFilePathName, _MAX_PATH);
		pttmap->insert(std::map<TSTRING, TSTRING>::value_type(tszFilePathName, tszFilePathName));
	}

	DragFinish(hDropInfo);

	return nNumOfFiles;
}
__inline static size_t GetDropFiles(std::vector<TSTRING> * pttmap, HDROP hDropInfo)
{
	UINT nIndex = 0;
	UINT nNumOfFiles = 0;
	_TCHAR tszFilePathName[MAX_PATH + 1] = { 0 };

	//得到文件个数
	nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (nIndex = 0; nIndex < nNumOfFiles; nIndex++)
	{
		//得到文件名
		DragQueryFile(hDropInfo, nIndex, (LPTSTR)tszFilePathName, _MAX_PATH);
		pttmap->push_back(tszFilePathName);
	}

	DragFinish(hDropInfo);

	return nNumOfFiles;
}
//显示在屏幕中央
__inline static void CenterWindowInScreen(HWND hWnd)
{
	RECT rcWindow = { 0 };
	RECT rcScreen = { 0 };
	SIZE szAppWnd = { 300, 160 };
	POINT ptAppWnd = { 0, 0 };

	// Get workarea rect.
	BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA,   // Get workarea information
		0,              // Not used
		&rcScreen,    // Screen rect information
		0);             // Not used

	GetWindowRect(hWnd, &rcWindow);
	szAppWnd.cx = rcWindow.right - rcWindow.left;
	szAppWnd.cy = rcWindow.bottom - rcWindow.top;

	//居中显示
	ptAppWnd.x = (rcScreen.right - rcScreen.left - szAppWnd.cx) / 2;
	ptAppWnd.y = (rcScreen.bottom - rcScreen.top - szAppWnd.cy) / 2;
	MoveWindow(hWnd, ptAppWnd.x, ptAppWnd.y, szAppWnd.cx, szAppWnd.cy, TRUE);
}

//显示在父窗口中央
__inline static void CenterWindowInParent(HWND hWnd, HWND hParentWnd)
{
	RECT rcWindow = { 0 };
	RECT rcParent = { 0 };
	SIZE szAppWnd = { 300, 160 };
	POINT ptAppWnd = { 0, 0 };

	GetWindowRect(hParentWnd, &rcParent);
	GetWindowRect(hWnd, &rcWindow);
	szAppWnd.cx = rcWindow.right - rcWindow.left;
	szAppWnd.cy = rcWindow.bottom - rcWindow.top;

	//居中显示
	ptAppWnd.x = (rcParent.right - rcParent.left - szAppWnd.cx) / 2;
	ptAppWnd.y = (rcParent.bottom - rcParent.top - szAppWnd.cy) / 2;
	MoveWindow(hWnd, ptAppWnd.x, ptAppWnd.y, szAppWnd.cx, szAppWnd.cy, TRUE);
}

//关闭清理进程
__inline static VOID CleanupProgram(LPSTARTUPINFO lpsi, LPPROCESS_INFORMATION lppi)
{
	if (lpsi)
	{
		if (lpsi->hStdError && lpsi != INVALID_HANDLE_VALUE)
		{
			CloseHandle(lpsi->hStdError);
			lpsi->hStdError = NULL;
		}
		if (lpsi->hStdInput && lpsi != INVALID_HANDLE_VALUE)
		{
			CloseHandle(lpsi->hStdInput);
			lpsi->hStdInput = NULL;
		}
		if (lpsi->hStdOutput && lpsi != INVALID_HANDLE_VALUE)
		{
			CloseHandle(lpsi->hStdOutput);
			lpsi->hStdOutput = NULL;
		}
	}
	if (lppi)
	{
		if (lppi->hProcess)
		{
			TerminateProcess(lppi->hProcess, (0L));
			CloseHandle(lppi->hProcess);
			lppi->hProcess = NULL;
		}
		if (lppi->hThread)
		{
			TerminateThread(lppi->hThread, (0L));
			CloseHandle(lppi->hThread);
			lppi->hThread = NULL;
		}
	}
}

//启动进程(应用程序文件名称、参数、启动类型及等待时间)
__inline static BOOL StartupProgram(LPCTSTR lpApplicationName, LPTSTR lpCommandLine = NULL, WORD wShowWindow = SW_SHOW, DWORD dwMilliseconds = WAIT_TIMEOUT, LPPROCESS_INFORMATION lpProcessInformation = NULL, LPSTARTUPINFO lpStartupInfo = NULL)
{
	BOOL bResult = FALSE;
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	LPSTARTUPINFO lpsi = &si;
	LPPROCESS_INFORMATION lppi = &pi;
	if (lpStartupInfo)
	{
		lpsi = lpStartupInfo;
	}
	if (lpProcessInformation)
	{
		lppi = lpProcessInformation;
	}

	ZeroMemory(lpsi, sizeof(STARTUPINFO));
	lpsi->cb = sizeof(STARTUPINFO);
	lpsi->dwFlags = STARTF_USESHOWWINDOW;
	lpsi->wShowWindow = wShowWindow;
	
	ZeroMemory(lppi, sizeof(PROCESS_INFORMATION));

	// Start the child process.
	bResult = CreateProcess(lpApplicationName,   // No module name (use command line)
		lpCommandLine,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0L,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory
		lpsi,           // Pointer to STARTUPINFO structure
		lppi);          // Pointer to PROCESS_INFORMATION structure
	if (bResult)
	{
		// Wait until child process exits.
		WaitForSingleObject(lppi->hProcess, dwMilliseconds);

		if (!lpProcessInformation)
		{
			// Close process and thread handles.
			CleanupProgram(lpsi, lppi);
		}
	}

	return bResult;
}

__inline static DWORD GetProgramPath(_TCHAR tFilePath[MAX_PATH] = _T(""))
{
	DWORD dwSize = 0;
	_TCHAR * pT = NULL;
	dwSize = GetModuleFileName(GetModuleHandle(NULL), tFilePath, MAX_PATH);
	if (dwSize)
	{
		pT = _tcsrchr(tFilePath, _T('\\'));
		if (pT)
		{
			*(pT + sizeof(BYTE)) = _T('\0');
			dwSize = (DWORD)(pT - tFilePath);
		}
	}
	return dwSize;
}

__inline static UINT GetSystemPath(_TCHAR tFilePath[MAX_PATH] = _T(""))
{
	UINT uSize = 0;
	_TCHAR * pT = NULL;
	uSize = GetSystemDirectory(tFilePath, MAX_PATH);
	if (uSize)
	{
		pT = _tcsrchr(tFilePath, _T('\\'));
		if (pT)
		{
			*(pT + sizeof(BYTE)) = _T('\0');
			uSize = (DWORD)(pT - tFilePath);
		}
	}
	return uSize;
}
// 判断是否非数字(10进制)
__inline static bool IsDigitsString(const _TCHAR * s)
{
	bool result = false;
	if (s)
	{
		do
		{
			if (*s != _T('\0'))
			{
				if ((*s < _T('0')) || (*s > _T('9')))
				{
					result = false;
					break;
				}
			}
			else
			{
				result = true;
				break;
			}
		} while (s++);
	}
	return result;
}
// 过滤尾部空格回车换行符( \r\n)
__inline static void RemoveTailerFilter(tstring & s)
{
	tstring::size_type n = s.length();
	tstring::size_type i = n;
	while (i-- && (s.at(i) == _T('\r') || s.at(i) == _T('\n') || s.at(i) == _T(' ')))
	{
		s.at(i) = _T('\0');
	}
}

__inline static void OUTPUT_DEBUG_TRACE_A(LPCSTR lpszFormat, ...)
{
	va_list args;
	size_t size = 0;
	LPSTR lpt = NULL;
	va_start(args, lpszFormat);
	size = _vscprintf(lpszFormat, args);
	if (size > 0)
	{
		size++;
		lpt = (LPSTR)malloc(size * sizeof(CHAR));
		if (lpt)
		{
			memset(lpt, 0, size * sizeof(CHAR));
			_vsnprintf(lpt, size, lpszFormat, args);
			OutputDebugStringA((LPCSTR)lpt);
			free(lpt);
			lpt = NULL;
		}
	}
	va_end(args);
}
__inline static void OUTPUT_DEBUG_TRACE_W(LPCWSTR lpszFormat, ...)
{
	va_list args;
	size_t size = 0;
	LPWSTR lpt = NULL;
	va_start(args, lpszFormat);
	size = _vscwprintf(lpszFormat, args);
	if (size > 0)
	{
		size++;
		lpt = (LPWSTR)malloc(size * sizeof(WCHAR));
		if (lpt)
		{
			memset(lpt, 0, size * sizeof(WCHAR));
			_vsnwprintf(lpt, size, lpszFormat, args);
			OutputDebugStringW((LPCWSTR)lpt);
			free(lpt);
			lpt = NULL;
		}
	}
	va_end(args);
}

////////////////////////////////////////////////////////////////////////////////////
// 五个级别
// DEBUG Level : 指出细粒度信息事件对调试应用程序是非常有帮助的, 就是输出debug的信息.
// INFO level : 表明消息在粗粒度级别上突出强调应用程序的运行过程, 就是输出提示信息.
// WARN level : 表明会出现潜在错误的情形, 就是显示警告信息.
// ERROR level : 指出虽然发生错误事件, 但仍然不影响系统的继续运行.就是显示错误信息.
// FATAL level : 指出每个严重的错误事件将会导致应用程序的退出.
// 两个开关
// ALL level : 是最低等级的, 用于打开所有日志记录.
// OFF level : 是最高等级的, 用于关闭所有日志记录.

__inline static void TRACE_PRINT_A(FILE * fStream, LPCSTR lpType, LPCSTR lpszFormat, ...)
{
	va_list args;
	size_t size = 0;
	LPSTR lpt = NULL;
	WORD wAttributes = 0;
	
	switch (*(lpType + sizeof(BYTE)))
	{
	case ('D'):
	{
		wAttributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	}
	break;
	case ('I'):
	{
		wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	}
	break;
	case ('W'):
	{
		wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	}
	break;
	case ('E'):
	{
		wAttributes = FOREGROUND_RED;
	}
	break;
	case ('F'):
	{
		wAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	}
	break;
	default:
	{
		return;
	}
	break;
	}
	
	va_start(args, lpszFormat);
	size = _vscprintf_p(lpszFormat, args);
	if (size > 0)
	{
		size++;
		lpt = (LPSTR)malloc(size * sizeof(CHAR));
		if (lpt)
		{
			memset(lpt, 0, size * sizeof(CHAR));
			_vsnprintf(lpt, size, lpszFormat, args);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wAttributes);
			fprintf(fStream, ("%s%s"), lpType, lpt);
			free(lpt);
			lpt = NULL;
		}
	}
	va_end(args);
}
__inline static void TRACE_PRINT_W(FILE * fStream, LPCWSTR lpType, LPCWSTR lpszFormat, ...)
{
	va_list args;
	size_t size = 0;
	LPWSTR lpt = NULL;
	WORD wAttributes = 0;

	switch (*(lpType + sizeof(BYTE)))
	{
	case (L'D'):
	{
		wAttributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	}
	break;
	case (L'I'):
	{
		wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	}
	break;
	case (L'W'):
	{
		wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	}
	break;
	case (L'E'):
	{
		wAttributes = FOREGROUND_RED;
	}
	break;
	case (L'F'):
	{
		wAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	}
	break;
	default:
	{
		return;
	}
	break;
	}

	va_start(args, lpszFormat);
	size = _vscwprintf(lpszFormat, args);
	if (size > 0)
	{
		size++;
		lpt = (LPWSTR)malloc(size * sizeof(WCHAR));
		if (lpt)
		{
			memset(lpt, 0, size * sizeof(WCHAR));
			_vsnwprintf(lpt, size, lpszFormat, args);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wAttributes);
			fwprintf(fStream, (L"%s%s"), lpType, lpt);
			free(lpt);
			lpt = NULL;
		}
	}
	va_end(args);
}

#if !defined(_UNICODE) && !defined(UNICODE)
#define OUTPUT_DEBUG_TRACE				OUTPUT_DEBUG_TRACE_A
#define INFO_PRINT(lpszFormat, ...)		TRACE_PRINT_A(stdout, ("[INFO]"), lpszFormat, __VA_ARGS__)
#define DEBUG_PRINT(lpszFormat, ...)	TRACE_PRINT_A(stdout, ("[DEBUG]"), lpszFormat, __VA_ARGS__)
#define WARN_PRINT(lpszFormat, ...)		TRACE_PRINT_A(stdout, ("[WARN]"), lpszFormat, __VA_ARGS__)
#define ERROR_PRINT(lpszFormat, ...)	TRACE_PRINT_A(stderr, ("[ERROR]"), lpszFormat, __VA_ARGS__)
#define FATAL_PRINT(lpszFormat, ...)	TRACE_PRINT_A(stderr, ("[FATAL]"), lpszFormat, __VA_ARGS__)
#else
#define OUTPUT_DEBUG_TRACE				OUTPUT_DEBUG_TRACE_W
#define INFO_PRINT(lpszFormat, ...)		TRACE_PRINT_W(stdout, (L"[INFO]"), lpszFormat, __VA_ARGS__)
#define DEBUG_PRINT(lpszFormat, ...)	TRACE_PRINT_W(stdout, (L"[DEBUG]"), lpszFormat, __VA_ARGS__)
#define WARN_PRINT(lpszFormat, ...)		TRACE_PRINT_W(stdout, (L"[WARN]"), lpszFormat, __VA_ARGS__)
#define ERROR_PRINT(lpszFormat, ...)	TRACE_PRINT_W(stderr, (L"[ERROR]"), lpszFormat, __VA_ARGS__)
#define FATAL_PRINT(lpszFormat, ...)	TRACE_PRINT_W(stderr, (L"[FATAL]"), lpszFormat, __VA_ARGS__)
#endif

#endif //__USUALLYUTILITY_H_
