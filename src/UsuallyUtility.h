#pragma once

#ifndef __USUALLYUTILITY_H_
#define __USUALLYUTILITY_H_

#include <map>
#include <regex>
#include <string>
#include <vector>
#include <algorithm>

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <shellapi.h>
#include <commdlg.h>

#define SYSTEM_PATH "SYSTEM32\\"
#define CMD_NAME    "CMD.EXE"

#include <codecvt>

//#define AToW_EX(s,l) std::wstring_convert<std::codecvt_byname<wchar_t, char, std::mbstate_t>>(new std::codecvt_byname<wchar_t, char, std::mbstate_t>(l)).from_bytes(s)
//#define WToA_EX(s,l) std::wstring_convert<std::codecvt_byname<wchar_t, char, std::mbstate_t>>(new std::codecvt_byname<wchar_t, char, std::mbstate_t>(l)).to_bytes(s)

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

//通用版将wstring转化为string
__inline std::string W_To_A(const std::wstring &wstr, unsigned int codepage = CP_ACP)
{
	int nwstrlen = WideCharToMultiByte(codepage, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	if (nwstrlen > 0)
	{
		std::string str(nwstrlen + 1, '\0');
		WideCharToMultiByte(codepage, 0, wstr.c_str(), -1, (LPSTR)str.c_str(), nwstrlen, NULL, NULL);
		return str;
	}

	return ("");
}

//通用版将string转化为wstring
__inline std::wstring A_To_W(const std::string &str, unsigned int codepage = CP_ACP)
{
	int nstrlen = MultiByteToWideChar(codepage, 0, str.c_str(), -1, NULL, 0);
	if (nstrlen > 0)
	{
		std::wstring wstr(nstrlen + 1, L'\0');
		MultiByteToWideChar(codepage, 0, str.c_str(), -1, (LPWSTR)wstr.c_str(), nstrlen);
		return wstr;
	}

	return (L"");
}

__inline static
#if !defined(UNICODE) && !defined(_UNICODE)
std::string
#else
std::wstring
#endif
A_To_T(const std::string& str)
{
#if !defined(UNICODE) && !defined(_UNICODE)
	return str;
#else
	return A_To_W(str);
#endif
}

__inline static
#if !defined(UNICODE) && !defined(_UNICODE)
std::string
#else
std::wstring
#endif
W_To_T(const std::wstring &wstr)
{
#if !defined(UNICODE) && !defined(_UNICODE)
	return W_To_A(wstr);
#else
	return wstr;
#endif
}

__inline static std::string T_To_A(
	const
#if !defined(UNICODE) && !defined(_UNICODE)
	std::string
#else
	std::wstring
#endif
	&tsT)
{
#if !defined(UNICODE) && !defined(_UNICODE)
	return tsT;
#else
	return W_To_A(tsT);
#endif
}

__inline static std::wstring T_To_W(
	const
#if !defined(UNICODE) && !defined(_UNICODE)
	std::string
#else
	std::wstring
#endif
	&tsT)
{
#if !defined(UNICODE) && !defined(_UNICODE)
	return A_To_W(tsT);
#else
	return tsT;
#endif
}

//将From编码转化为To编码
__inline static std::string CodePage_FromTo(const std::string &str,
	unsigned int from_codepage, unsigned int to_codepage)
{
	return W_To_A(A_To_W(str, from_codepage), to_codepage);
}

//将UTF8转化为ANSI
__inline static std::string UTF8ToANSI(const std::string& str)
{
	return CodePage_FromTo(str, CP_UTF8, CP_ACP);
}

//将ANSI转化为UTF8
__inline static std::string ANSIToUTF8(const std::string &str)
{
	return CodePage_FromTo(str, CP_ACP, CP_UTF8);
}

__inline static bool string_regex_valid(const std::string& data, const std::string& pattern)
{
	return std::regex_match(data, std::regex(pattern));
}
__inline static size_t string_regex_replace_all(std::string & result, std::string & data, const std::string& replace, const std::string& pattern, const std::regex_constants::match_flag_type& matchflagtype = std::regex_constants::match_default)
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
__inline static size_t string_regex_find(std::string & result, std::vector<std::vector<std::string>> & svv, const std::string & data, const std::string& pattern)
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
__inline static std::string::size_type string_reader(std::string &result, const std::string &strData,
	const std::string& strStart, const std::string& strFinal, std::string::size_type stPos = 0,
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
__inline static std::string string_reader(const std::string& strData,
	const std::string& strStart, const std::string& strFinal,
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
__inline static std::string string_replace_all(std::string &strData, const std::string& strDst, const std::string& strSrc, std::string::size_type stPos = 0)
{
	while ((stPos = strData.find(strSrc, stPos)) != std::string::npos)
	{
		strData.replace(stPos, strSrc.length(), strDst);
		stPos += strDst.length() - strSrc.length();
	}

	return strData;
}
__inline static size_t string_split_to_vector(std::vector<std::string> & sv, const std::string& strData, const std::string& strSplitter, std::string::size_type stPos = 0)
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

__inline static bool wstring_regex_valid(const std::wstring & data, const std::wstring &pattern)
{
	return std::regex_match(data, std::wregex(pattern));
}
__inline static size_t wstring_regex_find(std::wstring & result, std::vector<std::vector<std::wstring>> & svv, const std::wstring& data, const std::wstring& pattern)
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
		//result = std::wstring_convert<std::codecvt_byname<wchar_t, char, std::mbstate_t>>(new std::codecvt_byname<wchar_t, char, std::mbstate_t>("chs")).from_bytes(e.what());
		//result = std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>(new std::codecvt<wchar_t, char, std::mbstate_t>("chs")).from_bytes(e.what());
	}

	return svv.size();
}
__inline static size_t wstring_regex_replace_all(std::wstring & result, std::wstring & data, const std::wstring& replace, const std::wstring& pattern, std::regex_constants::match_flag_type matchflagtype = std::regex_constants::match_default)
{
	try
	{
		data = std::regex_replace(data, std::wregex(pattern), replace, matchflagtype);
	}
	catch (const std::exception & e)
	{
		//result = std::wstring_convert<std::codecvt_byname<wchar_t, char, std::mbstate_t>>(new std::codecvt_byname<wchar_t, char, std::mbstate_t>("chs")).from_bytes(e.what());
		//result = std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>(new std::codecvt<wchar_t, char, std::mbstate_t>("chs")).from_bytes(e.what());
	}
	return data.length();
}
__inline static std::wstring::size_type wstring_reader(std::wstring &result, const std::wstring& wstrData,
	const std::wstring& wstrStart, const std::wstring& wstrFinal, std::wstring::size_type stPos = 0,
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
__inline static std::wstring wstring_reader(const std::wstring& wstrData,
	const std::wstring& wstrStart, const std::wstring& wstrFinal,
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
__inline static std::wstring wstring_replace_all(std::wstring &wstrData, const std::wstring& wstrDst, const std::wstring& wstrSrc, std::wstring::size_type stPos = 0)
{
	while ((stPos = wstrData.find(wstrSrc, stPos)) != std::wstring::npos)
	{
		wstrData.replace(stPos, wstrSrc.length(), wstrDst);
		stPos += wstrDst.length() - wstrSrc.length();
	}

	return wstrData;
}
__inline static size_t wstring_split_to_vector(std::vector<std::wstring> & wsv, const std::wstring& wstrData, const std::wstring& wstrSplitter, std::wstring::size_type stPos = 0)
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
__inline static std::string STRING_FORMAT_A(const CHAR * paFormat, ...)
{
	INT nAS = 0;
	std::string A = ("");

	va_list valist = { 0 };

	va_start(valist, paFormat);

	nAS = _vscprintf_p(paFormat, valist);
	if (nAS > 0)
	{
		A.resize((nAS + sizeof(CHAR)) * sizeof(CHAR), ('\0'));
		_vsnprintf_s((CHAR *)A.c_str(), (nAS + sizeof(CHAR)) * sizeof(CHAR), nAS * sizeof(CHAR), paFormat, valist);
	}

	va_end(valist);

	return A.c_str();
}
__inline static std::wstring STRING_FORMAT_W(const WCHAR * pwFormat, ...)
{
	INT nWS = 0;
	std::wstring W = (L"");

	va_list valist = { 0 };

	va_start(valist, pwFormat);

	nWS = _vscwprintf_p(pwFormat, valist);
	if (nWS > 0)
	{
		W.resize((nWS + sizeof(WCHAR)) * sizeof(WCHAR), (L'\0'));
		_vsnwprintf_s((WCHAR *)W.c_str(), (nWS + sizeof(WCHAR)) * sizeof(WCHAR), nWS * sizeof(WCHAR), pwFormat, valist);
	}

	va_end(valist);

	return W.c_str();
}
__inline static size_t file_reader(char ** ppdata, size_t &size, std::string filename, std::string mode = "rb")
{
#define DATA_BASE_SIZE	0x10000

	FILE * pF = 0;
	size_t sizeofread = 0;

	char * pdata = 0;

	pF = fopen(filename.c_str(), mode.c_str());
	if (pF)
	{
		size = 0;
		(*ppdata) = (char *)malloc((size + DATA_BASE_SIZE) * sizeof(char));
		while (!feof(pF))
		{
			sizeofread = fread((void *)((*ppdata) + size), sizeof(char), DATA_BASE_SIZE, pF);
			size += sizeofread;
			if (sizeofread >= DATA_BASE_SIZE)
			{
				break;
			}
			pdata = (*ppdata);
			(*ppdata) = (char *)realloc(pdata, (size + DATA_BASE_SIZE) * sizeof(char));
			if (!(*ppdata) || GetLastError() != ERROR_SUCCESS)
			{
				if (pdata)
				{
					free(pdata);
					pdata = 0;
				}
				break;
			}
		}

		fclose(pF);
		pF = 0;
	}

	return size;

#undef DATA_BASE_SIZE
}

__inline static size_t file_reader(std::string&data, const std::string &filename, const std::string& mode = "rb")
{
#define DATA_BASE_SIZE	0x10000

	FILE * pF = 0;
	size_t size = 0;

	pF = fopen(filename.c_str(), mode.c_str());
	if (pF)
	{
		while (!feof(pF))
		{
			data.resize(data.size() + DATA_BASE_SIZE);
			size += fread((void *)(data.c_str() + data.size() - DATA_BASE_SIZE), sizeof(char), DATA_BASE_SIZE, pF);
		}
		data.resize(size);
		fclose(pF);
		pF = 0;
	}

	return size;

#undef DATA_BASE_SIZE
}

__inline static size_t file_writer(const std::string& data, const std::string &filename, const std::string & mode = "wb")
{
	FILE * pF = 0;
	size_t size = 0;

	pF = fopen(filename.c_str(), mode.c_str());
	if (pF)
	{
		size = fwrite((void *)(data.c_str()), sizeof(char), data.size(), pF);
		fclose(pF);
		pF = 0;
	}

	return size;
}

__inline static size_t file_writer(const char * data, size_t size, const std::string& filename, const std::string& mode = "wb")
{
	FILE * pF = 0;
	size_t ssize = 0;

	pF = fopen(filename.c_str(), mode.c_str());
	if (pF)
	{
		ssize = fwrite((void *)(data), sizeof(char), size, pF);
		fclose(pF);
		pF = 0;
	}

	return ssize;
}
__inline static std::string GetFilePathDriveA(LPCSTR lpFileName)
{
	CHAR szDrive[_MAX_DRIVE] = { 0 };
	CHAR szDir[_MAX_DIR] = { 0 };
	CHAR szFname[_MAX_FNAME] = { 0 };
	CHAR szExt[_MAX_EXT] = { 0 };

	_splitpath_s(lpFileName, szDrive, szDir, szFname, szExt);

	return szDrive;
}
__inline static std::wstring GetFilePathDriveW(LPCWSTR lpFileName)
{
	WCHAR szDrive[_MAX_DRIVE] = { 0 };
	WCHAR szDir[_MAX_DIR] = { 0 };
	WCHAR szFname[_MAX_FNAME] = { 0 };
	WCHAR szExt[_MAX_EXT] = { 0 };

	_wsplitpath_s(lpFileName, szDrive, szDir, szFname, szExt);

	return szDrive;
}
__inline static std::string GetFilePathDirA(LPCSTR lpFileName)
{
	CHAR szDrive[_MAX_DRIVE] = { 0 };
	CHAR szDir[_MAX_DIR] = { 0 };
	CHAR szFname[_MAX_FNAME] = { 0 };
	CHAR szExt[_MAX_EXT] = { 0 };

	_splitpath_s(lpFileName, szDrive, szDir, szFname, szExt);

	return szDir;
}
__inline static std::wstring GetFilePathDirW(LPCWSTR lpFileName)
{
	WCHAR szDrive[_MAX_DRIVE] = { 0 };
	WCHAR szDir[_MAX_DIR] = { 0 };
	WCHAR szFname[_MAX_FNAME] = { 0 };
	WCHAR szExt[_MAX_EXT] = { 0 };

	_wsplitpath_s(lpFileName, szDrive, szDir, szFname, szExt);

	return szDir;
}
__inline static std::string GetFilePathExtA(LPCSTR lpFileName)
{
	CHAR szDrive[_MAX_DRIVE] = { 0 };
	CHAR szDir[_MAX_DIR] = { 0 };
	CHAR szFname[_MAX_FNAME] = { 0 };
	CHAR szExt[_MAX_EXT] = { 0 };

	_splitpath_s(lpFileName, szDrive, szDir, szFname, szExt);

	return szExt;
}
__inline static std::wstring GetFilePathExtW(LPCWSTR lpFileName)
{
	WCHAR szDrive[_MAX_DRIVE] = { 0 };
	WCHAR szDir[_MAX_DIR] = { 0 };
	WCHAR szFname[_MAX_FNAME] = { 0 };
	WCHAR szExt[_MAX_EXT] = { 0 };

	_wsplitpath_s(lpFileName, szDrive, szDir, szFname, szExt);

	return szExt;
}
__inline static std::string GetFilePathFnameA(LPCSTR lpFileName)
{
	CHAR szDrive[_MAX_DRIVE] = { 0 };
	CHAR szDir[_MAX_DIR] = { 0 };
	CHAR szFname[_MAX_FNAME] = { 0 };
	CHAR szExt[_MAX_EXT] = { 0 };

	_splitpath_s(lpFileName, szDrive, szDir, szFname, szExt);

	return szFname;
}
__inline static std::wstring GetFilePathFnameW(LPCWSTR lpFileName)
{
	WCHAR szDrive[_MAX_DRIVE] = { 0 };
	WCHAR szDir[_MAX_DIR] = { 0 };
	WCHAR szFname[_MAX_FNAME] = { 0 };
	WCHAR szExt[_MAX_EXT] = { 0 };

	_wsplitpath_s(lpFileName, szDrive, szDir, szFname, szExt);

	return szFname;
}
__inline static void SplitFilePathA(LPCSTR lpFileName, std::string & strDrive,
	std::string & strDir, std::string & strFname, std::string & strExt)
{
	CHAR szDrive[_MAX_DRIVE] = { 0 };
	CHAR szDir[_MAX_DIR] = { 0 };
	CHAR szFname[_MAX_FNAME] = { 0 };
	CHAR szExt[_MAX_EXT] = { 0 };

	_splitpath_s(lpFileName, szDrive, szDir, szFname, szExt);
	strDrive = szDrive;
	strDir = szDir;
	strFname = szFname;
	strExt = szExt;
}
__inline static void SplitFilePathW(LPCWSTR lpFileName, std::wstring & strDrive,
	std::wstring & strDir, std::wstring & strFname, std::wstring & strExt)
{
	WCHAR szDrive[_MAX_DRIVE] = { 0 };
	WCHAR szDir[_MAX_DIR] = { 0 };
	WCHAR szFname[_MAX_FNAME] = { 0 };
	WCHAR szExt[_MAX_EXT] = { 0 };

	_wsplitpath_s(lpFileName, szDrive, szDir, szFname, szExt);
	strDrive = szDrive;
	strDir = szDir;
	strFname = szFname;
	strExt = szExt;
}

__inline static std::string ToLowerA(const std::string& s)
{
	std::string r("");
	std::transform(s.begin(), s.end(), r.begin(), tolower);
	return r;
}
__inline static std::wstring ToLowerW(const std::wstring& ws)
{
	std::wstring wr(L"");
	std::transform(ws.begin(), ws.end(), wr.begin(), tolower);
	return wr;
}
__inline static std::string ToUpperA(const std::string& s)
{
	std::string r("");
	std::transform(s.begin(), s.end(), r.begin(), toupper);
	return r;
}
__inline static std::wstring ToUpperW(const std::wstring& ws)
{
	std::wstring wr(L"");
	std::transform(ws.begin(), ws.end(), wr.begin(), toupper);
	return wr;
}

#if !defined(UNICODE) && !defined(_UNICODE)
#define TO_LOWER ToLowerA
#define TO_UPPER ToUpperA
#define TSTRING std::string
#define TO_TSTRING std::to_string
#define TSTRING_REGEX_VALID string_regex_valid
#define TSTRING_REGEX_REPLACE_ALL string_regex_replace_all
#define TSTRING_REGEX_FIND string_regex_find
#define TSTRING_READER string_reader
#define TSTRING_REPLACE_ALL string_replace_all
#define TSTRING_SPLIT_TO_VECTOR string_split_to_vector

#define STRING_FORMAT	STRING_FORMAT_A
#define ParseError				ParseErrorA
#define GetFilePathDrive		GetFilePathDriveA
#define GetFilePathDir			GetFilePathDirA
#define GetFilePathExt			GetFilePathExtA
#define GetFilePathFname		GetFilePathFnameA
#define SplitFilePath			SplitFilePathA
#else
#define TO_LOWER ToLowerW
#define TO_UPPER ToUpperW
#define TSTRING std::wstring
#define TO_TSTRING std::to_wstring
#define TSTRING_REGEX_VALID wstring_regex_valid
#define TSTRING_REGEX_REPLACE_ALL wstring_regex_replace_all
#define TSTRING_REGEX_FIND wstring_regex_find
#define TSTRING_READER wstring_reader
#define TSTRING_REPLACE_ALL wstring_replace_all
#define TSTRING_SPLIT_TO_VECTOR wstring_split_to_vector

#define STRING_FORMAT	STRING_FORMAT_W
#define ParseError				ParseErrorW
#define GetFilePathDrive		GetFilePathDriveW
#define GetFilePathDir			GetFilePathDirW
#define GetFilePathExt			GetFilePathExtW
#define GetFilePathFname		GetFilePathFnameW
#define SplitFilePath			SplitFilePathW
#endif

#define To_Lower TO_LOWER
#define To_Upper TO_UPPER
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

static FILE * STD_IN = NULL;
static FILE * STD_OUT = NULL;
static FILE * STD_ERR = NULL;
//初始化调试窗口显示
__inline static void InitDebugConsole(const _TCHAR * ptText = _T("TraceDebugWindow"))
{
	if (::AllocConsole())
	{
		::SetConsoleTitle(ptText);

		_tfreopen_s(&STD_IN, _T("CONIN$"), _T("rb"), stdin);
		_tfreopen_s(&STD_OUT, _T("CONOUT$"), _T("wb"), stdout);
		_tfreopen_s(&STD_ERR, _T("CONOUT$"), _T("wb"), stderr);

		_tsetlocale(LC_ALL, _T("chs"));	
	}
}

//释放掉调试窗口显示
__inline static void ExitDebugConsole()
{
    fclose(STD_ERR);
    fclose(STD_OUT);
    fclose(STD_IN);

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
__inline static size_t GetDropFiles(std::vector<TSTRING> * ptv, HDROP hDropInfo)
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
		ptv->push_back(tszFilePathName);
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

__inline static DWORD GetProgramPath(_TCHAR tFilePath[MAX_PATH])
{
	DWORD dwSize = 0;
	_TCHAR * pT = NULL;
	dwSize = ::GetModuleFileName(GetModuleHandle(NULL), tFilePath, MAX_PATH);
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

__inline static UINT GetSystemPath(_TCHAR tFilePath[MAX_PATH])
{
	UINT uSize = 0;
	uSize = ::GetSystemDirectory(tFilePath, MAX_PATH);
	if (uSize)
	{
		*(tFilePath + uSize) = _T('\\');
		*(tFilePath + (++uSize)) = _T('\0');
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
		lpt = (LPSTR)malloc((size+sizeof(CHAR)) * sizeof(CHAR));
		if (lpt)
		{
			memset(lpt, 0, (size + sizeof(CHAR)) * sizeof(CHAR));
			_vsnprintf_s(lpt, (size + sizeof(CHAR)), size, lpszFormat, args);
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
		lpt = (LPWSTR)malloc((size + sizeof(WCHAR)) * sizeof(WCHAR));
		if (lpt)
		{
			memset(lpt, 0, (size + sizeof(WCHAR)) * sizeof(WCHAR));
			_vsnwprintf_s(lpt, (size + sizeof(WCHAR)), size, lpszFormat, args);
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
		lpt = (LPSTR)malloc((size + sizeof(CHAR)) * sizeof(CHAR));
		if (lpt)
		{
			memset(lpt, 0, (size + sizeof(CHAR)) * sizeof(CHAR));
			_vsnprintf_s(lpt, (size + sizeof(CHAR)), size, lpszFormat, args);
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
		lpt = (LPWSTR)malloc((size + sizeof(WCHAR)) * sizeof(WCHAR));
		if (lpt)
		{
			memset(lpt, 0, (size + sizeof(WCHAR)) * sizeof(WCHAR));
			_vsnwprintf_s(lpt, (size + sizeof(WCHAR)), size, lpszFormat, args);
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

//////////////////////////////////////////////////////////////////////////
// 函数说明：执行程序命令并获取执行打印信息
// 参    数：输出的文件行内容数据、要执行的命令
// 返 回 值：bool返回类型，成功返回true；失败返回false
// 编 写 者: ppshuai 20141126
//////////////////////////////////////////////////////////////////////////
__inline static bool ExecuteCommand(std::vector<tstring> * pStringVector, const tstring& tCommandLine)
{
	bool result = false;
	FILE * ppipe = NULL;
	tstring tItemText = TEXT("");
	_TCHAR tItemChar[2] = { TEXT('\0') };

	if (pStringVector)
	{
		// Open pipe to execute command line
		ppipe = _tpopen(tCommandLine.c_str(), TEXT("rb"));
		if (ppipe)
		{
			/* Read pipe until end of file, or an error occurs. */
			while (fread(&tItemChar, sizeof(_TCHAR), 1, ppipe))
			{
				if ((*tItemChar != TEXT('\n'))
					&& (*tItemChar != TEXT('\0')))
				{
					*(tItemChar + 1) = TEXT('\0');
					tItemText.append(tItemChar);
				}
				else
				{
					pStringVector->push_back(tItemText);
					tItemText.empty();
					tItemText = TEXT("");
				}
			}

			pStringVector->push_back(tItemText);

			/* Close pipe and print return value of pPipe. */
			if (feof(ppipe))
			{
				result = true;
				_pclose(ppipe);
				ppipe = NULL;
			}
		}
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////
// 函数说明：执行程序命令并获取执行打印信息
// 参    数：输出的文件行内容数据、要执行的命令
// 返 回 值：bool返回类型，成功返回true；失败返回false
// 编 写 者: ppshuai 20141126
//////////////////////////////////////////////////////////////////////////
__inline static bool ExecuteCommandEx(std::vector<tstring> * pStdOutputStringVector,
	std::vector<tstring> * pStdErrorStringVector,
	const tstring &tExecuteFile,
	const tstring &tCommandLine)
{
	bool result = false;
	STARTUPINFO si = { 0 };
	HANDLE hStdErrorRead = NULL;
	HANDLE hStdOutputRead = NULL;
	HANDLE hStdErrorWrite = NULL;
	HANDLE hStdOutputWrite = NULL;
	SECURITY_ATTRIBUTES sa = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	tstring tItemText = TEXT("");
	_TCHAR tItemChar[2] = { TEXT('\0') };
	unsigned long ulBytesOfRead = 0;
	unsigned long ulBytesOfWritten = 0;

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;



	// Set the bInheritHandle flag so pipe handles are inherited.
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	// Create a pipe for the child process's STDOUT.
	if (CreatePipe(&hStdOutputRead, &hStdOutputWrite, &sa, 0) &&
		CreatePipe(&hStdErrorRead, &hStdErrorWrite, &sa, 0))
	{
		// Ensure the read handle to the pipe for STDOUT is not inherited.
		if (SetHandleInformation(hStdOutputRead, HANDLE_FLAG_INHERIT, 0) &&
			SetHandleInformation(hStdErrorRead, HANDLE_FLAG_INHERIT, 0))
		{
			si.hStdOutput = hStdOutputWrite;
			si.hStdError = hStdErrorWrite;
			si.wShowWindow = SW_HIDE;
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

			// Start the child process.
			if (CreateProcess(tExecuteFile.size() ? tExecuteFile.c_str() : NULL,   // No module name (use command line)
				(TCHAR *)tCommandLine.c_str(),        // Command line
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				TRUE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory
				&si,            // Pointer to STARTUPINFO structure
				&pi))           // Pointer to PROCESS_INFORMATION structure
			{
				// Wait until child process exits.
				//WaitForSingleObject( pi.hProcess, INFINITE );

				// Close process and thread handles.
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				if (hStdOutputWrite)
				{
					CloseHandle(hStdOutputWrite);
					hStdOutputWrite = NULL;
				}
				if (hStdErrorWrite)
				{
					CloseHandle(hStdErrorWrite);
					hStdErrorWrite = NULL;
				}

				if (pStdOutputStringVector)
				{
					while (ReadFile(hStdOutputRead, tItemChar, sizeof(_TCHAR),
						&ulBytesOfRead, NULL) && ulBytesOfRead)
					{
						if ((*tItemChar != TEXT('\n')) &&
							(*tItemChar != TEXT('\0')))
						{
							*(tItemChar + 1) = TEXT('\0');
							tItemText.append(tItemChar);
						}
						else
						{
							pStdOutputStringVector->push_back(tItemText);
							tItemText.empty();
							tItemText = TEXT("");
						}
					}
					pStdOutputStringVector->push_back(tItemText);
				}

				if (pStdErrorStringVector)
				{
					while (ReadFile(hStdErrorRead, tItemChar, sizeof(_TCHAR),
						&ulBytesOfRead, NULL) && ulBytesOfRead)
					{
						if ((*tItemChar != TEXT('\n')) &&
							(*tItemChar != TEXT('\0')))
						{
							*(tItemChar + 1) = TEXT('\0');
							tItemText.append(tItemChar);
						}
						else
						{
							pStdErrorStringVector->push_back(tItemText);
							tItemText.empty();
							tItemText = TEXT("");
						}
					}
					pStdErrorStringVector->push_back(tItemText);
				}
				result = true;
			}
		}
	}

	if (hStdErrorWrite)
	{
		CloseHandle(hStdErrorWrite);
		hStdErrorWrite = NULL;
	}
	if (hStdOutputWrite)
	{
		CloseHandle(hStdOutputWrite);
		hStdOutputWrite = NULL;
	}
	if (hStdErrorRead)
	{
		CloseHandle(hStdErrorRead);
		hStdErrorRead = NULL;
	}
	if (hStdOutputRead)
	{
		CloseHandle(hStdOutputRead);
		hStdOutputRead = NULL;
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////
// 函数说明：执行程序命令并获取执行打印信息
// 参    数：输出的文件行内容数据、要执行的命令
// 返 回 值：BOOL返回类型，成功返回TRUE；失败返回FALSE
// 编 写 者: ppshuai 20141126
//////////////////////////////////////////////////////////////////////////
__inline static BOOL ExecuteProcess(const _TCHAR * pProcName, const _TCHAR * pCmdLine,
	BOOL bShowFlag = FALSE, DWORD dwMilliseconds = INFINITE)
{
	BOOL bResult = FALSE;
	DWORD dwShowFlag = 0;
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	DWORD dwCmdLineSizeLength = 0;
	_TCHAR szCmdLine[MAX_PATH * 4 + 1] = { 0 };

	si.cb = sizeof(si);

	dwShowFlag = bShowFlag ? NULL : CREATE_NO_WINDOW;
	if (pCmdLine != NULL)
	{
		dwCmdLineSizeLength = (_tcslen(pCmdLine) < sizeof(szCmdLine)) ?
			_tcslen(pCmdLine) : sizeof(szCmdLine);
		lstrcpyn(szCmdLine, pCmdLine, dwCmdLineSizeLength);
		bResult = ::CreateProcess(pProcName, szCmdLine, NULL, NULL, FALSE,
			NORMAL_PRIORITY_CLASS | dwShowFlag, NULL, NULL, &si, &pi);
	}
	else
	{
		bResult = ::CreateProcess(pProcName, szCmdLine, NULL, NULL, FALSE,
			NORMAL_PRIORITY_CLASS | dwShowFlag, NULL, NULL, &si, &pi);
	}

	if (bResult)
	{
		WaitForSingleObject(pi.hProcess, dwMilliseconds);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	return bResult;
}

__inline static
BOOL FileIsExists(LPCTSTR pFileName)
{
	WIN32_FILE_ATTRIBUTE_DATA wfad = { 0 };

	return (GetFileAttributesEx(pFileName, GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &wfad)
		? ((wfad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) : FALSE);
}
__inline static
BOOL PathIsExists(LPCTSTR pFileName)
{
	WIN32_FILE_ATTRIBUTE_DATA wfad = { 0 };
	return (GetFileAttributesEx(pFileName, GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &wfad)
		? !((wfad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) : FALSE);
}
__inline static
BOOL IsPathExists(LPCTSTR pFileName)
{
	WIN32_FILE_ATTRIBUTE_DATA wfad = { 0 };
	return (GetFileAttributesEx(pFileName, GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &wfad)
		? !((wfad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) : FALSE);
}
__inline static
BOOL IsFileExist(LPCTSTR fileName)
{
	HANDLE hFindFile = NULL;
	WIN32_FIND_DATA	findData = { 0 };

	hFindFile = FindFirstFile(fileName, &findData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		FindClose(hFindFile);
		hFindFile = NULL;
		return TRUE;
	}

	return FALSE;
}
__inline static
BOOL IsFileExistEx(LPCTSTR lpFileName)
{
	WIN32_FILE_ATTRIBUTE_DATA wfad = { 0 };
	GET_FILEEX_INFO_LEVELS gfil = GetFileExInfoStandard;

	if (GetFileAttributes(lpFileName) != INVALID_FILE_ATTRIBUTES)
	{
		return TRUE;
	}
	else
	{
		if (GetFileAttributesEx(lpFileName, gfil, &wfad) &&
			wfad.dwFileAttributes != INVALID_FILE_ATTRIBUTES)
		{
			return TRUE;
		}
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
// 函数说明：执行程序命令并获取执行打印信息
// 参    数：要删除的文件名
// 返 回 值：bool返回类型，成功返回true；失败返回false
// 编 写 者: ppshuai 20141126
//////////////////////////////////////////////////////////////////////////
__inline static BOOL ForceDeleteFile(const TCHAR * pszFileName)
{
	BOOL bResult = FALSE;
	TCHAR tSystemPath[MAX_PATH] = { 0 };
	TCHAR tCmdLine[MAX_PATH * 4 + 1] = { 0 };

	if (pszFileName && (*pszFileName))
	{
		GetSystemPath(tSystemPath);
		_sntprintf_s(tCmdLine, sizeof(tCmdLine) / sizeof(*tCmdLine), sizeof(tCmdLine) / sizeof(*tCmdLine) - sizeof(char), TEXT("%sCMD.EXE /c DEL /F /S /Q \"%s\""), tSystemPath, pszFileName);
		bResult = ExecuteProcess(NULL, tCmdLine);
	}

	return bResult;
}
//////////////////////////////////////////////////////////////////////////
// 函数说明：遍历目录获取指定文件列表
// 参    数：输出的文件行内容数据、过滤后缀名、过滤的前缀字符
// 返 回 值：bool返回类型，成功返回true；失败返回false
// 编 写 者: ppshuai 20141112
//////////////////////////////////////////////////////////////////////////
__inline static BOOL DirectoryTraversal(std::vector<TSTRING> * pTV, LPCTSTR lpRootPath = _T("."), LPCTSTR lpExtension = _T("*.*"))
{
	BOOL bResult = FALSE;
	HANDLE hFindFile = NULL;
	WIN32_FIND_DATA wfd = { 0 };
	_TCHAR tFindPath[MAX_PATH + 1] = { 0 };

	//构建遍历根目录
	wsprintf(tFindPath, TEXT("%s%s"), lpRootPath, lpExtension);

	hFindFile = FindFirstFileEx(tFindPath, FindExInfoStandard, &wfd, FindExSearchNameMatch, NULL, 0);
	//hFindFile = FindFirstFile(tRootPath, &wfd);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
			{
				pTV->push_back(TSTRING(TSTRING(lpRootPath) + wfd.cFileName));
			}
			else
			{
				if (_tcscmp(wfd.cFileName, _T(".")) && _tcscmp(wfd.cFileName, _T("..")))
				{
					bResult = DirectoryTraversal(pTV, TSTRING(TSTRING(lpRootPath) + wfd.cFileName + _T("\\")).c_str(), lpExtension);
				}
			}
		} while (FindNextFile(hFindFile, &wfd));

		FindClose(hFindFile);
		hFindFile = NULL;
		bResult = TRUE;
	}

	return bResult;
}
//////////////////////////////////////////////////////////////////////////
// 函数说明：遍历目录获取指定文件列表
// 参    数：输出的文件行内容数据、过滤后缀名、过滤的前缀字符
// 返 回 值：bool返回类型，成功返回true；失败返回false
// 编 写 者: ppshuai 20141112
//////////////////////////////////////////////////////////////////////////
__inline static BOOL DirectoryTraversal(std::map<SIZE_T, TSTRING> * pSTMAP, LPCTSTR lpRootPath = _T("."), LPCTSTR lpExtension = _T("*.*"))
{
	BOOL bResult = FALSE;
	HANDLE hFindFile = NULL;
	WIN32_FIND_DATA wfd = { 0 };
	_TCHAR tFindPath[MAX_PATH + 1] = { 0 };

	//构建遍历根目录
	wsprintf(tFindPath, TEXT("%s%s"), lpRootPath, lpExtension);

	hFindFile = FindFirstFileEx(tFindPath, FindExInfoStandard, &wfd, FindExSearchNameMatch, NULL, 0);
	//hFindFile = FindFirstFile(tRootPath, &wfd);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			pSTMAP->insert(std::map<SIZE_T, TSTRING>::value_type(pSTMAP->size(), TSTRING(TSTRING(lpRootPath) + wfd.cFileName)));

			if (((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) && (_tcscmp(wfd.cFileName, _T(".")) && _tcscmp(wfd.cFileName, _T(".."))))
			{
				bResult = DirectoryTraversal(pSTMAP, TSTRING(TSTRING(lpRootPath) + wfd.cFileName + _T("\\")).c_str(), lpExtension);
			}

		} while (FindNextFile(hFindFile, &wfd));

		FindClose(hFindFile);
		hFindFile = NULL;
		bResult = TRUE;
	}

	return bResult;
}
//////////////////////////////////////////////////////////////////////////
// 函数说明：遍历目录获取指定文件列表
// 参    数：输出的文件行内容数据、过滤后缀名、过滤的前缀字符
// 返 回 值：bool返回类型，成功返回true；失败返回false
// 编 写 者: ppshuai 20141112
//////////////////////////////////////////////////////////////////////////
__inline static BOOL DirectoryTraversal(std::map<TSTRING, TSTRING> * pTTMAP, LPCTSTR lpFindPath = _T(".\\"), LPCTSTR lpRootPath = _T(".\\"), LPCTSTR lpExtension = _T("*.*"))
{
	BOOL bResult = FALSE;
	HANDLE hFindFile = NULL;
	WIN32_FIND_DATA wfd = { 0 };
	_TCHAR tChildPath[MAX_PATH + 1] = { 0 };
	_TCHAR tFindFileName[MAX_PATH + 1] = { 0 };

	if ((_tcslen(lpFindPath) >= _tcslen(lpRootPath)) && _tcsstr(TO_LOWER(lpFindPath).c_str(), TO_LOWER(lpRootPath).c_str()))
	{
		_sntprintf_s(tChildPath, sizeof(tChildPath) / sizeof(*tChildPath), sizeof(tChildPath) / sizeof(*tChildPath) - sizeof(char), _T("%s"), lpFindPath + _tcslen(lpRootPath));
	}

	//构建遍历根目录
	_sntprintf_s(tFindFileName, sizeof(tFindFileName) / sizeof(*tFindFileName), sizeof(tFindFileName) / sizeof(*tFindFileName) - sizeof(char), TEXT("%s%s"), lpFindPath, lpExtension);

	hFindFile = FindFirstFileEx(tFindFileName, FindExInfoStandard, &wfd, FindExSearchNameMatch, NULL, 0);
	//hFindFile = FindFirstFile(tRootPath, &wfd);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
			{
				pTTMAP->insert(std::map<TSTRING, TSTRING>::value_type((TSTRING(lpFindPath) + wfd.cFileName), TSTRING(tChildPath) + wfd.cFileName));
			}
			else
			{
				if (_tcscmp(wfd.cFileName, _T(".")) && _tcscmp(wfd.cFileName, _T("..")))
				{
					pTTMAP->insert(std::map<TSTRING, TSTRING>::value_type(TSTRING(lpFindPath) + wfd.cFileName, TSTRING(tChildPath) + wfd.cFileName + _T("\\")));
					bResult = DirectoryTraversal(pTTMAP, TSTRING(TSTRING(lpFindPath) + wfd.cFileName + _T("\\")).c_str(), lpRootPath, lpExtension);
				}
			}
		} while (FindNextFile(hFindFile, &wfd));

		FindClose(hFindFile);
		hFindFile = NULL;
		bResult = TRUE;
	}

	return bResult;
}
//判断目录是否存在，若不存在则创建
__inline static BOOL CreateCascadeDirectory(LPCTSTR lpPathName, //Directory name
	LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL  // Security attribute
)
{
	_TCHAR *pToken = NULL;
	_TCHAR *pContext = NULL;
	_TCHAR tPathTemp[MAX_PATH] = { 0 };
	_TCHAR tPathName[MAX_PATH] = { 0 };

	_tcscpy_s(tPathName, lpPathName);
	pToken = _tcstok_s(tPathName, _T("\\"), &pContext);
	while (pToken)
	{
		_sntprintf_s(tPathTemp, sizeof(tPathTemp) / sizeof(*tPathTemp), sizeof(tPathTemp) / sizeof(*tPathTemp) - sizeof(char), _T("%s%s\\"), tPathTemp, pToken);
		if (!IsFileExistEx(tPathTemp))
		{
			//创建失败时还应删除已创建的上层目录，此次略
			if (!CreateDirectory(tPathTemp, lpSecurityAttributes))
			{
				_tprintf_s(_T("CreateDirectory Failed: %d\n"), GetLastError());
				return FALSE;
			}
		}
		pToken = _tcstok_s(NULL, _T("\\"), &pContext);
	}
	return TRUE;
}

// 文件内存映射
__inline static LPVOID MapViewOfFileAgain(HANDLE hFileMapping, LPVOID * lppBaseAddress, ULARGE_INTEGER * pui, SIZE_T stNumberOfBytesToMap = 0, LPVOID lpBaseAddress = 0)
{
	if (lppBaseAddress && (*lppBaseAddress))
	{
		::UnmapViewOfFile((*lppBaseAddress));
		(*lppBaseAddress) = NULL;
	}
	return ((*lppBaseAddress) = ::MapViewOfFileEx(hFileMapping, FILE_MAP_ALL_ACCESS, pui->HighPart, pui->LowPart, stNumberOfBytesToMap, lpBaseAddress));
}

__inline static void MapRelease(HANDLE * phFileMapping, LPVOID * lpBaseAddress)
{
	if (lpBaseAddress && (*lpBaseAddress))
	{
		// 从进程的地址空间撤消文件数据映像
		::UnmapViewOfFile((*lpBaseAddress));
		(*lpBaseAddress) = NULL;
	}

	if (phFileMapping && (*phFileMapping))
	{
		// 关闭文件映射对象
		::CloseHandle((*phFileMapping));
		(*phFileMapping) = NULL;
	}
}

__inline static HANDLE MapFileCreate(LPVOID * lpFileData, LPCTSTR lpFileName)
{
	DWORD dwResult = 0;
	PBYTE pbFile = NULL;
	BOOL bLoopFlag = FALSE;
	HANDLE hWaitEvent[] = { 0, 0 };
	DWORD dwWaitEventNum = sizeof(hWaitEvent) / sizeof(HANDLE);

	SYSTEM_INFO si = { 0 };
	HANDLE hFileMapping = NULL;
	LPVOID lpBaseAddress = NULL;
	ULONGLONG ullFileVolume = 0LL;
	SIZE_T stNumberOfBytesToMap = 0;
	ULARGE_INTEGER uiFileSize = { 0, 0 };

	// 创建文件内核对象，其句柄保存于hFile
	HANDLE hFile = ::CreateFile(lpFileName,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	uiFileSize.LowPart = ::GetFileSize(hFile, &uiFileSize.HighPart);
	// 创建文件映射内核对象，句柄保存于hFileMapping
	hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE,
		uiFileSize.HighPart, uiFileSize.LowPart, NULL);
	if (hFile)
	{
		// 释放文件内核对象
		CloseHandle(hFile);
		hFile = NULL;
	}

	if (hFileMapping)
	{
		// 设定大小、偏移量等参数	
		//SystemKernel::GetNativeSystemInformation(&si);
		//ullFileVolume = si.dwAllocationGranularity;

		// 将文件数据映射到进程的地址空间
		(*lpFileData) = MapViewOfFileEx(hFileMapping, FILE_MAP_ALL_ACCESS,
			uiFileSize.HighPart, uiFileSize.LowPart, stNumberOfBytesToMap, lpBaseAddress);
	}

	return hFileMapping;
}

__inline static HANDLE MapCreate(LPVOID * lpData, LPCTSTR lpMapName, ULARGE_INTEGER * puiFileSize)
{
	SYSTEM_INFO si = { 0 };
	HANDLE hFileMapping = NULL;
	LPVOID lpBaseAddress = NULL;
	ULONGLONG ullFileVolume = 0LL;
	SIZE_T stNumberOfBytesToMap = 0;

	// 创建文件映射内核对象，句柄保存于hFileMapping
	hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
		puiFileSize->HighPart, puiFileSize->LowPart, lpMapName);
	if (hFileMapping)
	{
		// 设定大小、偏移量等参数	
		//PPSHUAI::SystemKernel::GetNativeSystemInformation(&si);
		//ullFileVolume = si.dwAllocationGranularity;

		// 将文件数据映射到进程的地址空间
		if (sizeof(SIZE_T) < sizeof(ULONGLONG))
		{
			stNumberOfBytesToMap = puiFileSize->LowPart;
		}
		else
		{
			stNumberOfBytesToMap = puiFileSize->QuadPart;
		}
		(*lpData) = MapViewOfFileEx(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, stNumberOfBytesToMap, lpBaseAddress);
	}

	return hFileMapping;
}
__inline static BOOL SelectSaveFile(_TCHAR(&tFileName)[MAX_PATH], const _TCHAR * ptFilter = _T("Execute Files (*.EXE)\0*.EXE\0All Files (*.*)\0*.*\0\0"), HWND hWndOwner = NULL, DWORD dwFlags = OFN_EXPLORER | OFN_ENABLEHOOK | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST, LPOFNHOOKPROC lpofnHookProc = NULL)
{
	BOOL bResult = FALSE;
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = ptFilter;
	ofn.lpstrFile = tFileName;
	ofn.hwndOwner = hWndOwner;
	ofn.lpfnHook = lpofnHookProc;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = dwFlags;
	bResult = GetSaveFileName(&ofn);
	if (bResult == FALSE)
	{
		//dwError = CommDlgExtendedError();
		//return bResult;
	}
	return bResult;
}
__inline static BOOL SelectOpenFile(_TCHAR(&tFileName)[MAX_PATH], const _TCHAR * ptFilter = _T("Execute Files (*.EXE)\0*.EXE\0All Files (*.*)\0*.*\0\0"), HWND hWndOwner = NULL, DWORD dwFlags = OFN_EXPLORER | OFN_ENABLEHOOK | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST, LPOFNHOOKPROC lpofnHookProc = NULL)
{
	BOOL bResult = FALSE;
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = ptFilter;
	ofn.lpstrFile = tFileName;
	ofn.hwndOwner = hWndOwner;
	ofn.lpfnHook = lpofnHookProc;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = dwFlags;
	bResult = GetOpenFileName(&ofn);
	if (bResult == FALSE)
	{
		//dwError = CommDlgExtendedError();
		//return bResult;
	}
	return bResult;
}
//获取程序工作路径
__inline static tstring GetWorkPath()
{
	tstring tsWorkPath = _T("");
	_TCHAR tWorkPath[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, tWorkPath);
	if (*tWorkPath)
	{
		tsWorkPath = tstring(tWorkPath) + _T("\\");
	}
	return tsWorkPath;
}

//获取系统临时路径
__inline static tstring GetTempPath()
{
	_TCHAR tTempPath[MAX_PATH] = { 0 };
	::GetTempPath(MAX_PATH, tTempPath);
	return tstring(tTempPath);
}

//获取程序文件路径
__inline static tstring GetProgramPath()
{
	tstring tsFilePath = _T("");
	_TCHAR * pFoundPosition = 0;
	_TCHAR tFilePath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, tFilePath, MAX_PATH);
	if (*tFilePath)
	{
		pFoundPosition = _tcsrchr(tFilePath, _T('\\'));
		if (*(++pFoundPosition))
		{
			*pFoundPosition = _T('\0');
		}
		tsFilePath = tFilePath;
	}
	return tsFilePath;
}

__inline static //获取系统路径
tstring GetSystemPath()
{
	tstring tsSystemPath = _T("");
	_TCHAR tSystemPath[MAX_PATH] = { 0 };
	GetSystemDirectory(tSystemPath, MAX_PATH);
	if (*tSystemPath)
	{
		tsSystemPath = tstring(tSystemPath) + _T("\\");
	}
	return tsSystemPath;
}

__inline static //获取系统路径
tstring GetSystemPathX64()
{
	tstring tsSystemPath = _T("");
	_TCHAR tSystemPath[MAX_PATH] = { 0 };
	GetSystemWow64Directory(tSystemPath, MAX_PATH);
	if (*tSystemPath)
	{
		tsSystemPath = tstring(tSystemPath) + _T("\\");
	}
	return tsSystemPath;
}
#endif //__USUALLYUTILITY_H_
