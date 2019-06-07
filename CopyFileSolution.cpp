
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "CopyFileSolution.h"
#include "OutputControl.h"
#include "JobSetting.h"
#include "Job.h"
#include "FileCopy.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString CopyFileSolution::GetTypeName(EN_COPYFILETYPE enType)
{
	switch (enType)
	{
	case LIB:
		return L"Library Export";
	case OUTPUT:
		return L"Output File";
	case PDB:
		return L"Program Database";
	}
	return L"";
}

bool CopyFileSolution::Run()
{
	if (m_strTargetSolution.GetLength() <= 0)	DEF_OUT_RETURN_FALSE(L"솔루션 파일을 확인할 수 없습니다.");
	if (m_strDestCopyPath.GetLength() <= 0)	DEF_OUT_RETURN_FALSE(L"대상 폴더 경로를 확인할 수 없습니다.");
	
	if (m_strTargetSolution.Right(4).CompareNoCase(L".sln") != 0) DEF_OUT_RETURN_FALSE(L"솔루션 파일을 확인할 수 없습니다.");

	CString strWorkingPath = JobSetting::GetCurrentWorkingPath();
	if (strWorkingPath.GetLength() <= 0) DEF_OUT_RETURN_FALSE(L"작업 경로를 확인할 수 없습니다.");
	if (strWorkingPath[strWorkingPath.GetLength() - 1] != '\\')
		strWorkingPath.AppendChar('\\');

	FileUtils::ConvertRelativeFileName(strWorkingPath, m_strTargetSolution);

	return Run_Solution(m_strTargetSolution);
}

bool CopyFileSolution::Load(FILE* pFile)
{
	m_enCopyFileType = (EN_COPYFILETYPE)rdInt(pFile);
	m_strTargetSolution = rdString(pFile);
	m_strDestCopyPath = rdString(pFile);
	int nSize = rdInt(pFile);
	m_strOptionsKey.clear();
	m_strOptionsKey.reserve(nSize);
	m_strOptionsVal.clear();
	m_strOptionsVal.reserve(nSize);
	for (int i = 0; i < nSize; i++)
	{
		m_strOptionsKey.push_back(rdString(pFile));
		m_strOptionsVal.push_back(rdString(pFile));
	}
	return true;
}

bool CopyFileSolution::Save(FILE* pFile)
{
	wrInt(pFile, (int)m_enCopyFileType);
	wrString(pFile, m_strTargetSolution);
	wrString(pFile, m_strDestCopyPath);
	int nSize = (int)m_strOptionsKey.size();
	wrInt(pFile, nSize);
	for (int i = 0; i < nSize; i++)
	{
		wrString(pFile, m_strOptionsKey[i]);
		wrString(pFile, m_strOptionsVal[i]);
	}
	return true;
}

void CopyFileSolution::SetParamOption(const CString& strOption)
{
	CString m_strOptions;
	DEF_SETOPTION(m_strOptions);
	DEF_SETOPTION(m_strDestCopyPath);

	CString token;
	int pos = 0;
	while ((token = m_strOptions.Tokenize(L";", pos)) != L"")
	{
		CString strKey = token.Left(token.Find(L"="));
		CString strVal = token.Mid(token.Find(L"=") + 1);

		int nTargetIndex = -1;
		for (auto i = 0; i < m_strOptionsKey.size(); i++)
		{
			if (m_strOptionsKey[i].CompareNoCase(strKey) == 0)
			{
				nTargetIndex = i;
			}
		}

		if(nTargetIndex >= 0)
		{
			m_strOptionsVal[nTargetIndex] = strVal;
		}
		else
		{
			m_strOptionsKey.push_back(strKey);
			m_strOptionsVal.push_back(strVal);
		}
	}
}

bool CopyFileSolution::Run_Solution(CString strTargetPath)
{
	CString strWorkingPath = JobSetting::GetCurrentWorkingPath();
	if (strWorkingPath.GetLength() <= 0) DEF_OUT_RETURN_FALSE(L"작업 경로를 확인할 수 없습니다.");
	if (strWorkingPath[strWorkingPath.GetLength() - 1] != '\\')
		strWorkingPath.AppendChar('\\');

	FileUtils::ConvertRelativeFileName(strWorkingPath, strTargetPath);

	MapStrBool mapProjects;
	if (!GetProjects(strTargetPath, mapProjects)) DEF_OUT_RETURN_FALSE(L"프로젝트 정보를 읽을 수 없습니다.");

	CString strInvalid;
	int nCount = 0;
	for(auto& itr : mapProjects)
	{
		if(!itr.second)
		{
			strInvalid += itr.first;
			strInvalid += L"\n";
			strInvalid += L"\n";
			nCount++;
		}
	}
	if (nCount > 0)
	{
		CString strPrompt;
		strPrompt.Format(L"\n\n총 %d 개의 항목이 제외되었습니다.\n%s", nCount, strInvalid);
		DEF_OUT(strPrompt);
	}

	if (mapProjects.size() - nCount <= 0) DEF_OUT_RETURN_FALSE(L"목표 파일을 찾을 수 없습니다.");

	CString strTargetFileName;
	strTargetFileName.Format(L"%sfileList.txt", FileUtils::GetCurrentModulePath());
	FILE* pFile = JobBase::Open(strTargetFileName, true);
	if (pFile == NULL) DEF_OUT_RETURN_FALSE(L"파일을 열 수 없습니다.");

	WORD mark = 0xFEFF;
	fwrite(&mark, sizeof(WORD), 1, pFile);

	for (auto& itr : mapProjects)
	{
		if (!itr.second) continue;

		const CString& strFile = itr.first;
		JobBase::wrString(pFile, strFile);
	}

	JobBase::Close(pFile);
	// 파일 출력하고 실행하지는 않음.
	// ::ShellExecute(NULL, L"open", strTargetFileName, L"", NULL, SW_SHOWNORMAL);

	if (m_strDestCopyPath[m_strDestCopyPath.GetLength() - 1] != '\\' &&
		m_strDestCopyPath[m_strDestCopyPath.GetLength() - 1] != '/')
	{
		m_strDestCopyPath.AppendChar('\\');
	}
	
	ProgressDlg::SetProgressRange(false, (int)mapProjects.size());
	CString strPrompt;
	strPrompt.Format(_T("%d개의 파일복사를 시작합니다."), mapProjects.size());
	DEF_OUT(strPrompt);

	int nCopiedCnt = 0;
	int nCopyTargetCnt = 0;
	for (auto& itr : mapProjects)
	{
		ProgressDlg::StepProgress(false);

		if (!itr.second) continue;

		const CString& strFile = itr.first;
		
		CString strFileName = FileUtils::GetOnlyFileName(strFile, false, false);

		CString strCopiedFilePath;
		strCopiedFilePath.Format(_T("%s%s"), m_strDestCopyPath, strFileName);

		FileUtils::MakeDirBeforeCopy(strCopiedFilePath);

		::CopyFile(strFile, strCopiedFilePath, FALSE);
		nCopyTargetCnt++;
		if (FileUtils::FileExistOnly(strCopiedFilePath)) nCopiedCnt++;
	}

	if(nCopyTargetCnt == nCopiedCnt)
		strPrompt.Format(_T("%d개의 파일이 모두 복사되었습니다."), nCopiedCnt);
	else
		strPrompt.Format(_T("%d개의 의 대상 파일중 %d개의 파일이 복사되었습니다."), nCopyTargetCnt, nCopiedCnt);
	DEF_OUT(strPrompt);
	
	return true;
}

bool CopyFileSolution::GetProjects(const CString strSolutionPath, MapStrBool& mapProjects)
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
				CString strProjectFilePath = nRev >= 0 ? strPath.Left(nRev + 1) + strProject : L"";

				bool bProjectExist = FileUtils::FileExistOnly(strProjectFilePath);

				CString strResult;
				if (bProjectExist && GetBuildResultName(strProjectFilePath, strResult))
				{
					mapProjects[strResult] = true;
				}
				else
				{
					mapProjects[strProjectFilePath] = false;
					continue;
				}
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

//////////////////////////////////////////////////////////////////////////
//
struct T_Data {
	bool	bParsed;
	CString strValue;
};

#define DEF_DEFAULT L"#default#"

typedef std::map<CString, T_Data> MapParseData;

CString GetXmlString(CString strXml, const CString& strTag)
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

CString GetXmlKey(const CString& strKey)
{
	int nPos1 = strKey.Find(L"$(");
	if (nPos1 < 0) return L"";
	nPos1 += 2;
	int nPos2 = strKey.Find(L")");
	return strKey.Mid(nPos1, nPos2 - nPos1);
}

CString GetValueFromMap(const CString& strKey, MapParseData& mapData, const CString& strProjectPath)
{
	CString strTarget = strKey;
	// 필드 값 갱신
	int nLoop = 100;
	CString strKeyParse = GetXmlKey(strTarget);
	while (strKeyParse.GetLength() > 0 && nLoop--)
	{
		auto itrFind = mapData.find(strKeyParse);
		if (itrFind == mapData.end())
		{
			ASSERT(0);
			// project 파싱할때 알 수 없는 파일임.. 추가해야한다.
			OUT(L"확인할 수 없는 필드가 존재합니다.");
			break;
		}
		CString strValue = itrFind->second.strValue;
		if (strValue.Find(L"..\\") >= 0 || strValue.Find(L".\\") >= 0 ||
			strValue.Find(L"../") >= 0 || strValue.Find(L"./") >= 0)
		{
			CString strCurPath = strProjectPath.Left(strProjectPath.ReverseFind('\\'));
			FileUtils::ConvertRelativeFileName(strCurPath, strValue);
		}
		strTarget.Replace(L"$(" + strKeyParse + ")", strValue);
		strKeyParse = GetXmlKey(strTarget);
	}
	strTarget.Replace('/', '\\');

	CString strTargetAbs = FileUtils::RemoveRelative(strTarget);
	if (strTargetAbs.GetLength() > 0) strTarget = strTargetAbs;

	return strTarget;
}

void UpdateDefaultValueFromMap(const CString& strProjectPath, MapParseData& mapData)
{
	for (auto& itr : mapData)
	{
		T_Data& dataDefault = itr.second;
		if (dataDefault.strValue == DEF_DEFAULT)
		{
			if (itr.first.CompareNoCase(L"TargetExt") == 0)
			{
				const auto& itrFind = mapData.find(L"ConfigurationType");
				if (itrFind == mapData.end()) continue;

				const T_Data& dataConfingType = itrFind->second;

				if (dataConfingType.strValue.CompareNoCase(L"DynamicLibrary") == 0)
					dataDefault.strValue = L".dll";
				else if (dataConfingType.strValue.CompareNoCase(L"Application") == 0)
					dataDefault.strValue = L".exe";
				else if (dataConfingType.strValue.CompareNoCase(L"StaticLibrary") == 0)
					dataDefault.strValue = L".lib";
				else ASSERT(0);
			}
			else
			{
				// default 값인경우는 경우에 따라 값 설정.
				ASSERT(0);
			}
		}
	}
}
//
//////////////////////////////////////////////////////////////////////////

bool CopyFileSolution::GetBuildResultName(const CString &strProjectPath, CString& strRet)
{
	FILE *wfp;
	_wfopen_s(&wfp, strProjectPath, _T("r, ccs=UNICODE"));
	if (!wfp) return false;
	
	if(strProjectPath.Find(L"MitCrash") >= 0)
	{
		int a = 3;
		a++;
	}

	MapParseData mapData;
	mapData[L"OutputFile"]					= T_Data{ false, L"$(OutDir)$(TargetName)$(TargetExt)" };
	mapData[L"ImportLibrary"]				= T_Data{ false, L"" };
	mapData[L"ProgramDatabaseFile"]			= T_Data{ false, L"$(OutDir)$(TargetName).pdb" };
	mapData[L"ProjectName"]					= T_Data{ false, FileUtils::GetOnlyFileName(strProjectPath) };
	mapData[L"TargetName"]					= T_Data{ false, L"$(ProjectName)" };
	mapData[L"TargetExt"]					= T_Data{ false, DEF_DEFAULT };
	mapData[L"ConfigurationType"]			= T_Data{ false, L"" };
	mapData[L"GenerateDebugInformation"]	= T_Data{ false, L"false" };
	mapData[L"OutDir"]						= T_Data{ false, L"" };
	mapData[L"TargetDir"]					= T_Data{ false, L"$(OutDir)" };

	for (int i = 0; i < (int)m_strOptionsKey.size(); i++)
	{
		mapData[m_strOptionsKey[i]] = T_Data{ true, m_strOptionsVal[i] };
	}
	
	TCHAR  buff[2048];
	TCHAR* pStr;
	try
	{
		while (!feof(wfp))
		{
			pStr = fgetws(buff, 2047, wfp);
			buff[2047] = '\0';

			for (auto& itr : mapData)
			{
				const CString& strKey = itr.first;
				T_Data& data = itr.second;

				if (data.bParsed) continue;

				CString strTemp = GetXmlString(buff, strKey);

				if (strTemp.GetLength() <= 0)	continue;
				if (IsInvalidValue(strTemp))	continue;

				data.strValue = strTemp;
				data.bParsed = true;
			}
		}
		fclose(wfp);
	}
	catch (CException*)
	{
		fclose(wfp);
	}

	//////////////////////////////////////////////////////////////////////////
	// Default 값인경우는 업데이트.
	UpdateDefaultValueFromMap(strProjectPath, mapData);

	// 출력파일 결정.
	CString strTarget;

	switch (m_enCopyFileType)
	{
	case EN_COPYFILETYPE::OUTPUT:
		strTarget = mapData[L"OutputFile"].strValue;
		break;
	case EN_COPYFILETYPE::LIB:
		if (!mapData[L"ImportLibrary"].bParsed &&
			mapData[L"ImportLibrary"].strValue.GetLength() <= 0 &&
			mapData[L"ConfigurationType"].strValue.CompareNoCase(L"StaticLibrary") == 0)
		{
			strTarget = mapData[L"OutputFile"].strValue;
		}
		else
		{
			strTarget = mapData[L"ImportLibrary"].strValue;
		}
		break;
	case EN_COPYFILETYPE::PDB:
		strTarget = mapData[L"ProgramDatabaseFile"].strValue;
		break;
	default:
		ASSERT(0);
		break;
	}
	strTarget = GetValueFromMap(CString(strTarget), mapData, strProjectPath);
	if (strTarget.Find(DEF_DEFAULT) >= 0) return false;

	// output 일때 lib 는 수집하지 않음.
	if (m_enCopyFileType == EN_COPYFILETYPE::OUTPUT)
	{
		if (strTarget.Right(4).CompareNoCase(L".lib") == 0)
			return false;
	}
	if (strTarget.GetLength() <= 0) return false;

	// 모든 필드를 대체하고난 결과
	if (strTarget.Find(L"$(") >= 0)
	{
		ASSERT(0);
		return false;
	}

	strRet = strTarget;
	return true;

	return false;
}

bool CopyFileSolution::IsInvalidValue(const CString& strVal)
{
	if (strVal.Find(L".bsc") >= 0) return true;
	if (strVal.Find(L".BSC") >= 0) return true;
	if (strVal.Find(L".tlb") >= 0) return true;
	if (strVal.Find(L".TLB") >= 0) return true;
	return false;
}
