
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "JobSetting.h"
#include "OutputControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

JobSetting::JobSetting()
{
}

bool JobSetting::Run()
{
	return true;
}

bool JobSetting::Load(FILE* pFile)
{
	m_bShowSubJob = rdInt(pFile) == 1 ? true : false;
	m_bUseProgramPath = rdInt(pFile) == 1 ? true : false;
	m_strWorkingPath = rdString(pFile);
	return true;
}

bool JobSetting::Save(FILE* pFile)
{
	wrInt(pFile, m_bShowSubJob ? 1 : 0);
	wrInt(pFile, m_bUseProgramPath ? 1 : 0);
	wrString(pFile, m_strWorkingPath);
	return true;
}