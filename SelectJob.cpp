
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SelectJob.h"
#include "OutputControl.h"
#include "Job.h"
#include "JobSetting.h"
#include "JobComboDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool SelectJob::Run()
{
	JobComboDlg dlg(m_strFilter, m_bIncludeUnitJob);
	if (dlg.DoModal() == IDOK)
	{
		CString strName = dlg.GetSelectedName();

		CString strJobPath;
		strJobPath.Format(L"%s%s%s", FileUtils::GetSettingPath(), strName, L".job");

		Job tempJob;
		if (!tempJob.Load(strJobPath)) return false;

		for (CString& strOpt : m_Options)
		{
			tempJob.SetParamOption(strOpt);
		}
		
		return tempJob.Run();
	}

	return false;
}

bool SelectJob::Load(FILE* pFile)
{
	m_strFilter = rdString(pFile);
	m_bIncludeUnitJob = rdInt(pFile) == 1 ? true : false;
	return true;
}

bool SelectJob::Save(FILE* pFile)
{
	wrString(pFile, m_strFilter);
	wrInt(pFile, m_bIncludeUnitJob == 1 ? true : false);
	return true;
}