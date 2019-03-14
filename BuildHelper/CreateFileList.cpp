
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

	if (m_strTargetPath.Right(4).CompareNoCase(L".sln") == 0)
	{
		return Run_Solution(m_strTargetPath);
	}
	else
	{
		if (m_strTargetFilter.GetLength() <= 0)	DEF_OUT_RETURN_FALSE(L"목표 파일 필터를 확인할 수 없습니다.");

		return Run_Path(m_strTargetPath, m_strTargetFilter);
	}
	return false;
}

bool CreateFileList::Load(FILE* pFile)
{
	m_strTargetPath = rdString(pFile);
	m_strTargetFilter = rdString(pFile);
	m_bIncludeSubFolder = rdInt(pFile) == 1 ? true : false;
	m_bFindPdb = rdInt(pFile) == 1 ? true : false;
	return true;
}

bool CreateFileList::Save(FILE* pFile)
{
	wrString(pFile, m_strTargetPath);
	wrString(pFile, m_strTargetFilter);
	wrInt(pFile, m_bIncludeSubFolder ? 1 : 0);
	wrInt(pFile, m_bFindPdb ? 1 : 0);
	return true;
}

bool CreateFileList::Run_Path(CString strTargetPath, CString strTargetFilter)
{
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

bool CreateFileList::Run_Solution(CString strTargetPath)
{
	CString strWorkingPath = JobSetting::GetCurrentWorkingPath();
	if (strWorkingPath.GetLength() <= 0) DEF_OUT_RETURN_FALSE(L"작업 경로를 확인할 수 없습니다.");
	if (strWorkingPath[strWorkingPath.GetLength() - 1] != '\\')
		strWorkingPath.AppendChar('\\');

	FileUtils::ConvertRelativeFileName(strWorkingPath, strTargetPath);

	MapStrBool mapProjects;
	if (!GetProjects(strTargetPath, mapProjects)) DEF_OUT_RETURN_FALSE(L"프로젝트 정보를 읽을 수 없습니다.");

	if (mapProjects.size() == 0) DEF_OUT_RETURN_FALSE(L"목표 파일을 찾을 수 없습니다.");

	CString strTargetFileName;
	strTargetFileName.Format(L"%sfileList.txt", FileUtils::GetCurrentModulePath());
	FILE* pFile = JobBase::Open(strTargetFileName, true);
	if (pFile == NULL) DEF_OUT_RETURN_FALSE(L"파일을 열 수 없습니다.");

	WORD mark = 0xFEFF;
	fwrite(&mark, sizeof(WORD), 1, pFile);

	for (auto itr = mapProjects.begin(); itr != mapProjects.end(); itr++)
	{
		const CString& strFile = itr->first;
		JobBase::wrString(pFile, strFile);
	}

	JobBase::Close(pFile);

	::ShellExecute(NULL, L"open", strTargetFileName, L"", NULL, SW_SHOWNORMAL);
	return true;
}

bool CreateFileList::GetProjects(const CString strSolutionPath, MapStrBool& mapProjects)
{
	const CString strPath = strSolutionPath;

	FILE *wfp;
	_wfopen_s(&wfp, strPath, _T("r, ccs=UNICODE"));
	if (!wfp) return false;

	TCHAR  buff[2048];
	TCHAR* pStr;

	try
	{
		while (!feof(wfp))
		{
			pStr = fgetws(buff, 2047, wfp);
			buff[2047] = '\0';

			TCHAR* pFind = _tcsstr(buff, _T("Project(\""));
			if (pFind == NULL) continue;

			CString strTemp = pFind;
			int nStart = strTemp.Find(_T(",")) + 2;

			CString strExt = _T(".vcproj");
			int nExtSize = 0;
			int nEnd = strTemp.Find(strExt);
			if (nEnd >= 0) nExtSize = strExt.GetLength() - 1;

			if (nEnd < 0)
			{
				strExt = _T(".vcxproj");
				nEnd = strTemp.Find(strExt);
				if (nEnd >= 0) nExtSize = strExt.GetLength() - 1;
			}

			if (nStart >= 0 && nEnd >= 0 && nEnd > nStart)
			{
				CString strProject = strTemp.Mid(nStart + 1, nEnd - nStart + nExtSize);

				int nRev = strPath.ReverseFind('\\');
				if (nRev >= 0)
				{
					CString strTemp = strPath.Left(nRev + 1) + strProject;
					if (FileUtils::FileExistOnly(strTemp))
					{
						strProject = GetBuildResultName(strTemp, m_bFindPdb);

						// 이거는 추가하면 안되는 정적 라이브러리.
						if (strProject.GetLength() == 0) continue;
					}
				}
				
				if(strProject.Find('\\') >= 0)
				{
					strProject = FileUtils::GetOnlyFileName(strProject);
				}
				mapProjects[strProject] = true;
			}
		}
		fclose(wfp);
	}
	catch (CException*)
	{
		fclose(wfp);
		return false;
	}

	return true;
}

template<typename T, std::size_t N>
constexpr std::size_t arraySize(T(&)[N]) noexcept
{
	return N;
}

CString CreateFileList::GetBuildResultName(const CString &strProjectPath, bool bPdb)
{
	CString strRet = strProjectPath;

	FILE *wfp;
	_wfopen_s(&wfp, strProjectPath, _T("r, ccs=UNICODE"));
	if (!wfp) return strRet;
	
	struct T_Data {
		CString strKey;
		CString strValue;
		bool	bParsed;
	};
	T_Data strParse[] = { { L"ProjectName", FileUtils::GetOnlyFileName(strProjectPath), false },
	{L"TargetName", L"$(ProjectName)", false },
	{L"TargetExt", L"#deafult#", false },
	{L"OutputFile", L"$(OutDir)$(TargetName)$(TargetExt)", false },
	{L"ConfigurationType", L"", false},
	{L"ProgramDatabaseFile", L"$(OutDir)$(TargetName).pdb", false},
	{L"GenerateDebugInformation", L"false", false},
	};

	int nSize = arraySize(strParse);

	TCHAR  buff[2048];
	TCHAR* pStr;
	try
	{
		while (!feof(wfp))
		{
			pStr = fgetws(buff, 2047, wfp);
			buff[2047] = '\0';

			for (int i = 0; i < nSize; i++)
			{
				if (strParse[i].bParsed) continue;

				CString strTemp = GetXmlString(buff, strParse[i].strKey);
				if (strTemp.GetLength() > 0 && strTemp.Find(L".bsc") < 0)
				{
					strParse[i].strValue = strTemp;
					strParse[i].bParsed = true;
				}
			}
		}
		fclose(wfp);
	}
	catch (CException*)
	{
		fclose(wfp);
	}

	int nIdx = 0;
	CString& strProjectName = strParse[nIdx++].strValue;
	CString& strTargetName = strParse[nIdx++].strValue;
	CString& strTargetExt = strParse[nIdx++].strValue;
	CString& strOutputFile = strParse[nIdx++].strValue;
	CString& strConfigType = strParse[nIdx++].strValue;
	CString& strPdbFile = strParse[nIdx++].strValue;
	CString& strGenPdb = strParse[nIdx++].strValue;

	if (strTargetExt.CompareNoCase(L"#deafult#") == 0)
	{
		if (strConfigType.CompareNoCase(L"DynamicLibrary") == 0) strTargetExt = L".dll";
		if (strConfigType.CompareNoCase(L"Application") == 0) strTargetExt = L".exe";
	}
	if (strTargetName.CompareNoCase(L"$(ProjectName)") == 0)
	{
		strTargetName = strProjectName;
	}

	//////////////////////////////////////////////////////////////////////////
	if (bPdb)
	{
		if (strGenPdb.CompareNoCase(L"true") != 0) return CString();

		strPdbFile.Replace(L"$(OutDir)", L"");
		strPdbFile.Replace(L"$(TargetName)", strTargetName);

		if (strPdbFile.Find('\\') < 0 && strPdbFile.Find('\\') < 0)
		{
			strRet = strPdbFile;
		}
	}
	else
	{
		if (strConfigType.CompareNoCase(L"DynamicLibrary") != 0 &&
			strConfigType.CompareNoCase(L"Application") != 0)
		{
			return CString();
		}

		strOutputFile.Replace(L"$(OutDir)", L"");
		strOutputFile.Replace(L"$(TargetName)", strTargetName);
		strOutputFile.Replace(L"$(TargetExt)", strTargetExt);

		if (strOutputFile.Find('\\') < 0 && strOutputFile.Find('\\') < 0)
		{
			strRet = strOutputFile;
		}
	}

	return strRet;
}

CString CreateFileList::GetXmlString(CString strXml, const CString& strTag)
{
	CString strStart;
	strStart.Format(L"<%s>", strTag);
	if (strXml.Find(strStart) < 0) return CString();

	CString strEnd;
	strEnd.Format(L"</%s>", strTag);
	strXml.Replace(strStart, L"");
	strXml.Replace(strEnd, L"");
	strXml.Trim();
	return strXml;
}
