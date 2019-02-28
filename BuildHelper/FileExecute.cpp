
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FileExecute.h"
#include "OutputControl.h"
#include "JobSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void FileExecute::AddParam(const CString& strName, const CString& strParam)
{
	m_vecParamName.push_back(strName);
	m_vecParam.push_back(strParam);
}

void FileExecute::SetParam(int nCount, const CString* pStrName, const CString* strParam)
{
	m_vecParamName.clear();
	m_vecParam.clear();
	m_vecParamName.reserve(nCount);
	m_vecParam.reserve(nCount);
	for(int i = 0; i < nCount; i++)
	{
		m_vecParamName.push_back(pStrName[i]);
		m_vecParam.push_back(strParam[i]);
	}
}

int FileExecute::GetParams(VecStr& strName, VecStr& strParam) const
{
	strName = m_vecParamName;
	strParam = m_vecParam;
	return (int)m_vecParamName.size();
}

int FileExecute::GetParamNum() const
{
	return (int)m_vecParamName.size();
}

bool FileExecute::Run()
{
	CString strParam;
	for (CString strItem : m_vecParam)
	{
		// 상대경로인놈은 업데이트...
		if ((strItem.Find(L".\\") >= 0 || strItem.Find(L"..\\") >= 0)
			&& strItem.GetLength() > 2)
		{
			CString strTemp = strItem;
			CString strWorkingPath = JobSetting::GetCurrentWorkingPath();
			if (strWorkingPath.GetLength() <= 0) DEF_OUT_RETURN_FALSE(L"작업 경로를 확인할 수 없습니다.");
			if (strWorkingPath[strWorkingPath.GetLength() - 1] != '\\')
				strWorkingPath.AppendChar('\\');

			bool bDdaom = strTemp[0] == '\"' && strTemp[strTemp.GetLength() - 1] == '\"';
			if (bDdaom)
			{
				CString str1 = strTemp.Mid(1, strTemp.GetLength() - 2);
				strTemp = str1;
			}

			FileUtils::ConvertRelativeFileName(strWorkingPath, strTemp);
			strItem = strTemp;
		}

		if (strParam.GetLength() <= 0) strParam = strItem;
		else
		{
			strParam.AppendChar(' ');
			strParam.Append(strItem);
		}
	}
	::ShellExecute(NULL, L"open", m_strExecuteFile, strParam, NULL, SW_SHOWNORMAL);

	return true;
}

bool FileExecute::Load(FILE* pFile)
{
	m_strExecuteFile = rdString(pFile);
	int nCount = rdInt(pFile);
	m_vecParamName.clear();
	m_vecParam.clear();
	m_vecParamName.reserve(nCount);
	m_vecParam.reserve(nCount);

	while (nCount--)
	{
		m_vecParamName.push_back(rdString(pFile));
		m_vecParam.push_back(rdString(pFile));
	}
	return true;
}

bool FileExecute::Save(FILE* pFile)
{
	wrString(pFile, m_strExecuteFile);
	int nCount = (int)m_vecParamName.size();
	wrInt(pFile, nCount);
	for (int i = 0; i < nCount; i++)
	{
		wrString(pFile, m_vecParamName[i]);
		wrString(pFile, m_vecParam[i]);
	}
	return true;
}