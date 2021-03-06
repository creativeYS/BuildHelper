﻿#pragma once

#include <vector>
#include <map>

typedef std::vector<CString> VecStr;
typedef std::map<CString, bool>	MapStr2Bool;

class FileUtils
{
public:
	static CString	GetOnlyFileName(const CString& strPath, bool bRevSlash = false, bool bWithOutExt = true);
	static CString	GetOnlyPath(const CString& strPath);
	static CString	GetCurrentModulePath();
	static CString	GetSettingPath();
	static CString	GetSettingFileName() { return L"Setting.dat"; }

	static BOOL		ConvertRelativeFileName(const TCHAR* szCurPath, CString& strPath);
	static CString	RemoveRelative(const TCHAR* szPath);

	static bool		IsLastChar(const CString& strCheck, TCHAR ch);

	static void		OnBrowseFolder(CWnd* pCtrl, const TCHAR* pBasePath, bool bFile = false, const TCHAR* pExt = nullptr);

	// pPath : c:\test\nice\*
	// pExt : *.txt
	static void		FileList(const TCHAR* pPath, const TCHAR* pExt, VecStr& vecTarget, bool bSubFolder = true);

	// 파일이 존재하는지 검사 (폴더인경우 return false)
	// pFilePath : c:\test\abc.txt
	static bool		FileExistOnly(const TCHAR* pFilePath);

	// 폴더 혹은 파일이 있는지 검사 (파일이든 폴더든 있으면 return true)
	// pFilePath : c:\test
	static bool		PathExist(const TCHAR* pFolderPath);

	// 폴더의 내용물을 포함하여 모두 지운다.
	static bool		FolderDelete(const TCHAR* pPath);

	static bool		MakeDir(const TCHAR* pPath);
	static bool		MakeDirBeforeCopy(const TCHAR* pFilePath);
};
