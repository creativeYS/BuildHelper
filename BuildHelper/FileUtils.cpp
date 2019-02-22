
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FileUtils.h"
#include "OutputControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void FileUtils::FileList(const TCHAR* pPath, const TCHAR* pExt, VecStr& vecTarget, bool bSubFolder)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	BOOL bResult = TRUE;
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[MAX_PATH];

	const TCHAR* path = pPath;
	hSrch = FindFirstFile(path, &wfd);
	if (hSrch != INVALID_HANDLE_VALUE && bSubFolder)
	{
		_tsplitpath_s(path, drive, _MAX_DRIVE, dir, MAX_PATH, NULL, 0, NULL, 0);
		while (bResult)
		{
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (_tcscmp(wfd.cFileName, _T(".")) && _tcscmp(wfd.cFileName, _T("..")))
				{
					CString strTemp;
					strTemp.Format(_T("%s%s%s\\*"), drive, dir, wfd.cFileName);

					FileList(strTemp.GetBuffer(), pExt, vecTarget);
				}
			}
			bResult = FindNextFile(hSrch, &wfd);
		}
		FindClose(hSrch);
	}

	CString strPathNew;
	strPathNew.Format(_T("%s%s"), path, pExt + 1);
	hSrch = FindFirstFile(strPathNew.GetBuffer(), &wfd);
	bResult = TRUE;
	if (hSrch != INVALID_HANDLE_VALUE)
	{
		_tsplitpath_s(path, drive, _MAX_DRIVE, dir, MAX_PATH, NULL, 0, NULL, 0);
		while (bResult)
		{
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				CString strTemp;
				strTemp.Format(_T("%s%s%s"), drive, dir, wfd.cFileName);
				vecTarget.push_back(strTemp);
			}
			bResult = FindNextFile(hSrch, &wfd);
		}
		FindClose(hSrch);
	}
}

bool FileUtils::FileExistOnly(const TCHAR* pFilePath)
{
	if (pFilePath == nullptr) return FALSE;
	CStdioFile File;
	if (!File.Open(pFilePath, CFile::modeRead | CFile::shareDenyNone | CFile::typeText)) return false;
	File.Close();

	return true;
}

bool FileUtils::PathExist(const TCHAR* pFolderPath)
{
	int nResult = _waccess(pFolderPath, 0);

	if (nResult == 0)
	{
		return true;
	}

	return false;
}

BOOL DeleteDirectoryFile(LPCTSTR RootDir)
{
	if (RootDir == NULL)
	{
		return FALSE;
	}

	BOOL bRval = FALSE;

	CString szNextDirPath = _T("");
	CString szRoot = _T("");


	// 해당 디렉토리의 모든 파일을 검사한다.
	szRoot.Format(_T("%s\\*.*"), RootDir);

	CFileFind find;

	bRval = find.FindFile(szRoot);

	if (bRval == FALSE)
	{
		return bRval;
	}

	while (bRval)
	{
		bRval = find.FindNextFile();

		// . or .. 인 경우 무시 한다.  
		if (find.IsDots() == FALSE)
		{
			// Directory 일 경우 재귀호출 한다.
			if (find.IsDirectory())
			{
				DeleteDirectoryFile(find.GetFilePath());
			}
			// file일 경우 삭제 
			else
			{
				bRval = DeleteFile(find.GetFilePath());
			}
		}
	}
	find.Close();
	bRval = RemoveDirectory(RootDir);
	return bRval;
}

bool FileUtils::FolderDelete(const TCHAR* pPath)
{
	if (!PathExist(pPath) || FileExistOnly(pPath)) DEF_OUT_RETURN_FALSE(L"폴더를 확인할 수 없습니다.");

	bool bYes = false;
	CString strPrompt;
	strPrompt.Format(L"\'%s\' 폴더 및 내부 파일들이 모두 삭제됩니다 계속하시겠습니까?", pPath);

	OutputControl::Instance()->ScanIn(strPrompt, bYes);

	if (bYes)
	{
		if (DeleteDirectoryFile(pPath)) return true;
	}
	return false;
}

CString FileUtils::GetOnlyFileName(const CString& strPath, bool bRevSlash, bool bWithOutExt)
{
	int nPos = strPath.ReverseFind('\\');
	CString strTemp = strPath.Right(strPath.GetLength() - nPos - 1);

	if (bRevSlash)
	{
		CString strNewPath = strTemp;
		nPos = strNewPath.ReverseFind('/');
		strTemp = strNewPath.Right(strNewPath.GetLength() - nPos - 1);
	}
	if(bWithOutExt)
	{
		CString strFileNameOnly = strTemp;
		strTemp = strFileNameOnly.Left(strFileNameOnly.ReverseFind('.'));
	}

	return strTemp;
}

CString FileUtils::GetOnlyPath(const CString& strPath)
{
	if (PathExist(strPath))
	{
		if (FileExistOnly(strPath))
		{
			int nPos = strPath.ReverseFind('\\');
			CString strTemp = strPath.Left(nPos);
			return strTemp;
		}
		return strPath;
	}
	else
	{
		int nPos1 = strPath.ReverseFind('\\');
		int nPos2 = strPath.ReverseFind('/');
		int nPos = nPos1 > nPos2 ? nPos1 : nPos2;
		if(nPos >= 0)
		{
			CString strTemp = strPath.Left(nPos);
			return strTemp;
		}
	}

	return CString();
}

CString FileUtils::GetCurrentModulePath()
{
	wchar_t tmpStr[MAX_PATH];
	DWORD status = GetModuleFileName(NULL, tmpStr, MAX_PATH);

	CString strTemp = tmpStr;
	return strTemp.Left(strTemp.ReverseFind('\\') + 1);

}

CString FileUtils::GetSettingPath()
{
	CString strTemp;
	strTemp.Format(_T("%sSetting\\"), GetCurrentModulePath());
	return strTemp;
}
