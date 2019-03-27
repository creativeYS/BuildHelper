
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FileUtils.h"
#include "OutputControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL FileUtils::ConvertRelativeFileName(const TCHAR* szCurPath, CString& strPath)
{
	CString strFindTemp = strPath;
	if (strFindTemp.Find(L"./") < 0 &&
		strFindTemp.Find(L".\\") < 0 &&
		strFindTemp.Find(L"../") < 0 &&
		strFindTemp.Find(L"..\\") < 0 ) return FALSE;

	CString szCurPathWithoutRelative = RemoveRelative(szCurPath);
	if (szCurPathWithoutRelative.GetLength() > 0) szCurPath = szCurPathWithoutRelative;

	CString strModulePath = szCurPath == nullptr ? GetCurrentModulePath() : szCurPath;
	strModulePath.Replace('/', '\\');
	if (FileUtils::IsLastChar(strModulePath, '\\'))
		strModulePath.Delete(strModulePath.GetLength() - 1, 1);

	szCurPath = strModulePath;

	if (strPath.GetLength() > 1)
	{
		CString strTemp;
		if ((strPath[0] == '.' && strPath[1] == '\\') ||
			(strPath[0] == '.' && strPath[1] == '/'))
		{
			strTemp = strPath.Right(strPath.GetLength() - 1);
		}
		else if ((strPath[0] == '.' && strPath[1] == '.'))
		{
			strTemp.Format(_T("\\%s"), strPath);
		}
		else if (strPath[0] == '\\' || strPath[0] == '/')
		{
			strTemp = strPath;
		}
		else // 상대경로가 아닌것으로 본다.
		{
			strTemp = strPath;
			return TRUE;
		}

		CString strFront = szCurPath;
		CString strRear = strTemp;

		while (strRear.Find(_T("../"), 0) >= 0 || strRear.Find(_T("..\\"), 0) >= 0)
		{
			int _left = strFront.ReverseFind(_T('\\'));
			strFront = strFront.Left(_left);

			int _len = (int)strlen("../");

			int _right = 0;

			if (strRear.Find(_T("../"), 0) >= 0)
				_right = strRear.GetLength() - strRear.Find(_T("../"), 0) - _len;
			else if (strRear.Find(_T("..\\"), 0) >= 0)
				_right = strRear.GetLength() - strRear.Find(_T("..\\"), 0) - _len;

			strRear = strRear.Right(_right);
		}

		if (strRear.GetLength() > 0 && strRear[0] == '\\')
			strTemp = strFront + strRear;
		else
			strTemp = strFront + _T("\\") + strRear;

		strPath = strTemp;
		return TRUE;
	}
	return FALSE;
}

CString FileUtils::RemoveRelative(const TCHAR* szPath)
{
	CString strTemp = szPath;
	int nPos = strTemp.Find(L"..\\") - 1;
	if (nPos < 0) return L"";

	while (nPos >= 0)
	{
		int nPosFirst = nPos + 3;
		if (nPos < 0) return L"";
		while (--nPos)
		{
			if (strTemp[nPos] == '\\')
			{
				CString strMidTemp = strTemp.Mid(nPos, nPosFirst - nPos);
				strTemp.Replace(strMidTemp, L"");
				break;
			}
		}
		nPos = strTemp.Find(L"..\\") - 1;
	}
	return strTemp;
}

bool FileUtils::IsLastChar(const CString& strCheck, TCHAR ch)
{
	if (strCheck.GetLength() <= 0) return false;

	if (strCheck[strCheck.GetLength() - 1] == ch) return true;

	return false;
}

void FileUtils::OnBrowseFolder(CWnd* pCtrl, const TCHAR* pBasePath, bool bFile, const TCHAR* pExt)
{
	CString strModulePath = pBasePath;
	strModulePath.Replace('/', '\\');
	if (FileUtils::IsLastChar(strModulePath, '\\'))
		strModulePath.Delete(strModulePath.GetLength() - 1, 1);

	CString strInit = strModulePath;

	CString strTemp;
	pCtrl->GetWindowText(strTemp);

	if (strTemp.GetLength() > 0)
	{
		if (!bFile && !FileUtils::IsLastChar(strTemp, '\\')) strTemp.AppendChar('\\');
		FileUtils::ConvertRelativeFileName(strInit, strTemp);
		strInit = strTemp;
		
// 		if (bFile && strInit.GetLength() > 0 &&
// 			strInit.ReverseFind('\\') < strInit.ReverseFind('.'))
// 		{
// 			strTemp = strInit.Left(strInit.ReverseFind('\\'));
// 			strInit = strTemp;
// 		}
	}
	if (!FileUtils::IsLastChar(strInit, '\\')) strInit.AppendChar('\\');

	if (bFile)
	{
		CString strFilter = _T("Execute File(*.exe)|*.exe|All Files(*.*)|*.*||");
		if (pExt)
		{
			strFilter = pExt;
		}
		CFileDialog dlg(TRUE, NULL, strInit, OFN_HIDEREADONLY, strFilter);
		if (dlg.DoModal() == IDOK)
		{
			CString strPathName = dlg.GetPathName();
			pCtrl->SetWindowText(strPathName);
		}
	}
	else
	{
		CFolderPickerDialog Picker(strInit, OFN_FILEMUSTEXIST, NULL, 0);
		if (Picker.DoModal() == IDOK)
		{
			CString strFolderPath = Picker.GetPathName();
			pCtrl->SetWindowText(strFolderPath);
		}
	}
}

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

bool FileUtils::MakeDir(const TCHAR* pPath)
{
	if (FileUtils::FileExistOnly(pPath))
	{
		if (!FileUtils::PathExist(pPath)) return false;
	}

	if (!FileUtils::PathExist(pPath))
		_wmkdir(pPath);

	return true;
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
