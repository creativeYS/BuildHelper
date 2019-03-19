
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "JobSetting.h"
#include "OutputControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEF_CLEAR_RESERVE(VECTOR, SIZE) { VECTOR.clear(); VECTOR.reserve(SIZE); }

CString JobSetting::m_strCurrentWorkingPath;

void JobSetting::SetSetting(const VecSetting& settings)
{
	m_vecSettings = settings;
}

int JobSetting::GetSetting(VecSetting& settings)
{
	settings = m_vecSettings;
	return (int)m_vecSettings.size();
}

const T_SETTING* JobSetting::GetSettingData(const CString& strName) const
{
	CString strTarget = strName.GetLength() > 0 ? strName : m_strLastSetting;
	for (const T_SETTING& setting : m_vecSettings)
	{
		if (setting.strName.CompareNoCase(strTarget) == 0)
			return &setting;
	}
	return NULL;
}

bool JobSetting::Run()
{
	return true;
}

bool JobSetting::Load(FILE* pFile)
{
	m_strLastSetting = rdString(pFile);

	int nSize = rdInt(pFile);

	DEF_CLEAR_RESERVE(m_vecSettings, nSize);

	while (nSize--)
	{
		T_SETTING setting(rdString(pFile));
		setting.strFilter		= rdString(pFile);
		setting.nShowSub		= rdInt(pFile);
		setting.nUsePgmPath		= rdInt(pFile);
		setting.strWorkingPath	= rdString(pFile);

		m_vecSettings.push_back(setting);
	}
	return true;
}

bool JobSetting::Save(FILE* pFile)
{
	wrString(pFile, m_strLastSetting);

	wrInt(pFile, (int)m_vecSettings.size());

	for(int i = 0 ; i < (int)m_vecSettings.size(); i++)
	{
		const T_SETTING& setting = m_vecSettings[i];
		wrString(pFile, setting.strName);
		wrString(pFile, setting.strFilter);
		wrInt(pFile,	setting.nShowSub);
		wrInt(pFile,	setting.nUsePgmPath);
		wrString(pFile, setting.strWorkingPath);
	}
	return true;
}

CString JobSetting::GetCurrentWorkingPath()
{
	return m_strCurrentWorkingPath;
}

void JobSetting::SetCurrentWorkingPath(const CString& str)
{
	m_strCurrentWorkingPath = str;
	if (m_strCurrentWorkingPath.Find(L".\\") == 0)
	{
		m_strCurrentWorkingPath.Insert(0, FileUtils::GetCurrentModulePath());
	}
}
