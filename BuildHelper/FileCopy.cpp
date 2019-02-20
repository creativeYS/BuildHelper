
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FileCopy.h"
#include "OutputControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

FileCopy::FileCopy()
	: m_bIncludeSubFolder(true)
{
}

bool FileCopy::Copy()
{
	if (m_strSourcePath.GetLength() <= 0)	DEF_OUT_RETURN_FALSE(L"원본 폴더 경로를 확인할 수 없습니다.");
	if (m_strDestPath.GetLength() <= 0)		DEF_OUT_RETURN_FALSE(L"목표 폴더 경로를 확인할 수 없습니다.");
	if (m_strTargetFilter.GetLength() <= 0)	DEF_OUT_RETURN_FALSE(L"목표 파일 필터를 확인할 수 없습니다.");

	if (m_strSourcePath[m_strSourcePath.GetLength() - 1] != '*')
		m_strSourcePath.AppendChar('*');

	if (m_strDestPath[m_strDestPath.GetLength() - 1] != '\\' &&
		m_strDestPath[m_strDestPath.GetLength() - 1] != '/')
	{
		m_strDestPath.AppendChar('\\');
	}

	VecStr strResult;
	FileUtils::FileList(m_strSourcePath.GetBuffer(), m_strTargetFilter.GetBuffer(), strResult, m_bIncludeSubFolder);
	
	if (strResult.size() == 0) DEF_OUT_RETURN_FALSE(L"목표 파일을 찾을 수 없습니다.");

	if (!MakeDir(m_strDestPath))  DEF_OUT_RETURN_FALSE(L"목표 폴더를 생성할 수 없습니다.");

	for (CString& strFileOrg : strResult)
	{
		CString strFile = strFileOrg.Right(strFileOrg.GetLength() - m_strSourcePath.GetLength() + 1);
		
		CString strCopiedFilePath;
		strCopiedFilePath.Format(_T("%s%s"), m_strDestPath, strFile);

		CString strPathMid = FileUtils::GetOnlyPath(strCopiedFilePath);

		if (!MakeDir(strPathMid)) continue;

		::CopyFile(strFileOrg, strCopiedFilePath, FALSE);
	}
	FileUtils::FolderDelete(L"D:\\_Temp\\test\\새 폴더");

	return true;
}

bool FileCopy::MakeDir(const TCHAR* pPath)
{
	if (FileUtils::FileExistOnly(pPath))
	{
		if (!FileUtils::PathExist(pPath)) return false;
	}
	
	if (!FileUtils::PathExist(pPath))
		_wmkdir(pPath);

	return true;

}