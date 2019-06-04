
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FileExecute.h"
#include "OutputControl.h"
#include "JobSetting.h"
#include "FileExecute_SubTypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void FileExecute::AddParam(const CString& strName, const CString& strParam)
{
	m_vecParamKey.push_back(strName);
	m_vecParamVal.push_back(strParam);
}

void FileExecute::SetParam(int nCount, const CString* pStrName, const CString* strParam)
{
	m_vecParamKey.clear();
	m_vecParamVal.clear();
	m_vecParamKey.reserve(nCount);
	m_vecParamVal.reserve(nCount);
	for(int i = 0; i < nCount; i++)
	{
		m_vecParamKey.push_back(pStrName[i]);
		m_vecParamVal.push_back(strParam[i]);
	}
}

int FileExecute::GetParams(VecStr& strName, VecStr& strParam) const
{
	strName = m_vecParamKey;
	strParam = m_vecParamVal;
	return (int)m_vecParamKey.size();
}

int FileExecute::GetParamNum() const
{
	return (int)m_vecParamKey.size();
}

bool FileExecute::Run()
{
	CString strWorkingPath = JobSetting::GetCurrentWorkingPath();
	if (strWorkingPath.GetLength() <= 0) DEF_OUT_RETURN_FALSE(L"작업 경로를 확인할 수 없습니다.");
	if (strWorkingPath[strWorkingPath.GetLength() - 1] != '\\')
		strWorkingPath.AppendChar('\\');

	CString strParam;
	for (CString strItem : m_vecParamVal)
	{
		// 상대경로인놈은 업데이트...
		if ((strItem.Find(L".\\") >= 0 || strItem.Find(L"..\\") >= 0)
			&& strItem.GetLength() > 2)
		{
			CString strTemp = strItem;
			bool bDdaom = strTemp[0] == '\"' && strTemp[strTemp.GetLength() - 1] == '\"';
			if (bDdaom)
			{
				CString str1 = strTemp.Mid(1, strTemp.GetLength() - 2);
				strTemp = str1;
			}

			FileUtils::ConvertRelativeFileName(strWorkingPath, strTemp);
			if (bDdaom) strItem.Format(_T("\"%s\""), strTemp);
			else		strItem = strTemp;
		}

		if (strParam.GetLength() <= 0) strParam = strItem;
		else
		{
			strParam.AppendChar(' ');
			strParam.Append(strItem);
		}
	}

	if ((m_strExecuteFile.CompareNoCase(L"del") == 0 ||
		m_strExecuteFile.CompareNoCase(L"delete") == 0) &&
		m_vecParamVal.size() > 0)
	{
		::DeleteFile(m_vecParamVal[0]);
		return true;
	}

	CString strExeFile = m_strExecuteFile;
	if(strExeFile.Find(L".\\") >= 0 || strExeFile.Find(L"..\\") >= 0)
	{
		CString strRelTemp = strExeFile;
		if (FileUtils::ConvertRelativeFileName(strWorkingPath, strRelTemp))
		{
			strExeFile = strRelTemp;
		}
	}

	enum EN_EXECUTE_TYPE
	{
		EN_SHELLEXECUTE,
		EN_CREATEPROCESS,
	};
	EN_EXECUTE_TYPE enExecuteType(EN_SHELLEXECUTE);

	// console 인경우는 CreateProcess, 표준입출력 redirection...
	if(OutputControl::Instance()->GetType() == OutputControl::EN_TYPE_CONSOLE)
	{
		enExecuteType = EN_CREATEPROCESS;
	}

	FileExecute_Base* pBase = nullptr;
	switch (enExecuteType)
	{
	case EN_SHELLEXECUTE:
	{
		static FileExecute_ShellExecute shellExecute;
		pBase = &shellExecute;
	} break;	
	case EN_CREATEPROCESS:
	{
		static FileExecute_CreateProcess createProcess;
		pBase = &createProcess;
	} break;
	default:
		ASSERT(0);
		break;
	}

	bool bRet = false;
	if (pBase)
	{
		pBase->SetExeFile(strExeFile);
		pBase->SetParams(strParam);
		bRet = pBase->Run();
	}

	return bRet;
}

bool FileExecute::Load(FILE* pFile)
{
	m_strExecuteFile = rdString(pFile);
	int nCount = rdInt(pFile);
	m_vecParamKey.clear();
	m_vecParamVal.clear();
	m_vecParamKey.reserve(nCount);
	m_vecParamVal.reserve(nCount);

	while (nCount--)
	{
		m_vecParamKey.push_back(rdString(pFile));
		m_vecParamVal.push_back(rdString(pFile));
	}
	return true;
}

bool FileExecute::Save(FILE* pFile)
{
	wrString(pFile, m_strExecuteFile);
	int nCount = (int)m_vecParamKey.size();
	wrInt(pFile, nCount);
	for (int i = 0; i < nCount; i++)
	{
		wrString(pFile, m_vecParamKey[i]);
		wrString(pFile, m_vecParamVal[i]);
	}
	return true;
}

void FileExecute::SetParamOption(const CString& strOption)
{
	CString m_strOptions;
	DEF_SETOPTION(m_strOptions);
	DEF_SETOPTION(m_strExecuteFile);

	CString token;
	int pos = 0;
	while ((token = m_strOptions.Tokenize(L";", pos)) != L"")
	{
		CString strKey = token.Left(token.Find(L"="));
		CString strVal = token.Mid(token.Find(L"=") + 1);

		int nTargetIndex = -1;
		for (auto i = 0; i < m_vecParamKey.size(); i++)
		{
			if (m_vecParamKey[i].CompareNoCase(strKey) == 0)
			{
				nTargetIndex = i;
			}
		}

		if (nTargetIndex >= 0)
		{
			CString& strParamOrg = m_vecParamVal[nTargetIndex];
			if (strParamOrg.GetLength() > 2 &&
				strParamOrg[0] == '\"' &&
				strParamOrg[strParamOrg.GetLength() - 1] == '\"')
			{
				m_vecParamVal[nTargetIndex].Format(_T("\"%s\""), strVal);
			}
			else
			{
				m_vecParamVal[nTargetIndex] = strVal;
			}
		}
		else
		{
			m_vecParamKey.push_back(strKey);
			m_vecParamVal.push_back(strVal);
		}
	}

}
