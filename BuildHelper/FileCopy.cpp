
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FileCopy.h"
#include "OutputControl.h"
#include "JobSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool FileCopy::Run()
{
	if (m_strSourcePath.GetLength() <= 0)	DEF_OUT_RETURN_FALSE(L"원본 폴더 경로를 확인할 수 없습니다.");
	if (m_strDestPath.GetLength() <= 0)		DEF_OUT_RETURN_FALSE(L"목표 폴더 경로를 확인할 수 없습니다.");
	if (m_strTargetFilter.GetLength() <= 0)	DEF_OUT_RETURN_FALSE(L"목표 파일 필터를 확인할 수 없습니다.");

	CString strSourcePath = m_strSourcePath;
	CString strDestPath = m_strDestPath;
	CString strTargetFilter = m_strTargetFilter;

	if (strSourcePath[strSourcePath.GetLength() - 2] != '\\')
		strSourcePath.AppendChar('\\');
	if (strSourcePath[strSourcePath.GetLength() - 1] != '*')
		strSourcePath.AppendChar('*');

	if (strDestPath[strDestPath.GetLength() - 1] != '\\' &&
		strDestPath[strDestPath.GetLength() - 1] != '/')
	{
		strDestPath.AppendChar('\\');
	}

	CString strWorkingPath = JobSetting::GetCurrentWorkingPath();
	if (strWorkingPath.GetLength() <= 0) DEF_OUT_RETURN_FALSE(L"작업 경로를 확인할 수 없습니다.");
	if (strWorkingPath[strWorkingPath.GetLength() - 1] != '\\')
		strWorkingPath.AppendChar('\\');

	FileUtils::ConvertRelativeFileName(strWorkingPath, strSourcePath);


	VecStr strResult;
	FileUtils::FileList(strSourcePath.GetBuffer(), strTargetFilter.GetBuffer(), strResult, m_bIncludeSubFolder);
	
	if (strResult.size() == 0) DEF_OUT_RETURN_FALSE(L"목표 파일을 찾을 수 없습니다.");

	if (!MakeDir(strDestPath))  DEF_OUT_RETURN_FALSE(L"목표 폴더를 생성할 수 없습니다.");

	for (CString& strFileOrg : strResult)
	{
		CString strFile = strFileOrg.Right(strFileOrg.GetLength() - strSourcePath.GetLength() + 1);
		
		CString strCopiedFilePath;
		strCopiedFilePath.Format(_T("%s%s"), strDestPath, strFile);

		CString strPathMid = FileUtils::GetOnlyPath(strCopiedFilePath);

		if (!MakeDir(strPathMid)) continue;

		::CopyFile(strFileOrg, strCopiedFilePath, FALSE);
	}

	return true;
}

bool FileCopy::Load(FILE* pFile)
{
	m_strSourcePath = rdString(pFile);
	m_strDestPath = rdString(pFile);
	m_strTargetFilter = rdString(pFile);
	m_bIncludeSubFolder = rdInt(pFile) == 1 ? true : false;
	return true;
}

bool FileCopy::Save(FILE* pFile)
{
	wrString(pFile, m_strSourcePath);
	wrString(pFile, m_strDestPath);
	wrString(pFile, m_strTargetFilter);
	wrInt(pFile, m_bIncludeSubFolder ? 1 : 0);
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