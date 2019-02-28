
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "CreateFileList.h"
#include "OutputControl.h"
#include "JobSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool CreateFileList::Run()
{
	if (m_strTargetPath.GetLength() <= 0)	DEF_OUT_RETURN_FALSE(L"대상 폴더 경로를 확인할 수 없습니다.");
	if (m_strTargetFilter.GetLength() <= 0)	DEF_OUT_RETURN_FALSE(L"목표 파일 필터를 확인할 수 없습니다.");

	CString strTargetPath = m_strTargetPath;
	CString strTargetFilter = m_strTargetFilter;

	if (strTargetPath[strTargetPath.GetLength() - 2] != '\\')
		strTargetPath.AppendChar('\\');
	if (strTargetPath[strTargetPath.GetLength() - 1] != '*')
		strTargetPath.AppendChar('*');

	CString strWorkingPath = JobSetting::GetCurrentWorkingPath();
	if (strWorkingPath.GetLength() <= 0) DEF_OUT_RETURN_FALSE(L"작업 경로를 확인할 수 없습니다.");
	if (strWorkingPath[strWorkingPath.GetLength() - 1] != '\\')
		strWorkingPath.AppendChar('\\');

	FileUtils::ConvertRelativeFileName(strWorkingPath, strTargetPath);

	VecStr strResult;
	FileUtils::FileList(strTargetPath.GetBuffer(), strTargetFilter.GetBuffer(), strResult, m_bIncludeSubFolder);
	
	if (strResult.size() == 0) DEF_OUT_RETURN_FALSE(L"목표 파일을 찾을 수 없습니다.");
	
	CString strTargetFileName;
	strTargetFileName.Format(L"%sfileList.txt", FileUtils::GetCurrentModulePath());
	FILE* pFile = JobBase::Open(strTargetFileName, true);
	if (pFile == NULL) DEF_OUT_RETURN_FALSE(L"파일을 열 수 없습니다.");

	WORD mark = 0xFEFF;
	fwrite(&mark, sizeof(WORD), 1, pFile);

	for (const CString& strFile : strResult)
	{
		JobBase::wrString(pFile, strFile);
	}
	
	JobBase::Close(pFile);

	::ShellExecute(NULL, L"open", strTargetFileName, L"", NULL, SW_SHOWNORMAL);

	return true;
}

bool CreateFileList::Load(FILE* pFile)
{
	m_strTargetPath = rdString(pFile);
	m_strTargetFilter = rdString(pFile);
	m_bIncludeSubFolder = rdInt(pFile) == 1 ? true : false;
	return true;
}

bool CreateFileList::Save(FILE* pFile)
{
	wrString(pFile, m_strTargetPath);
	wrString(pFile, m_strTargetFilter);
	wrInt(pFile, m_bIncludeSubFolder ? 1 : 0);
	return true;
}