
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "CancelJob.h"
#include "OutputControl.h"
#include "Job.h"
#include "JobSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool CancelJob::Run()
{
	bool bYes = false;
	CString strPrompt = L"작업을 계속하시겠습니까?";

	if (m_strMsg.GetLength() > 0) strPrompt = m_strMsg;

	OutputControl::Instance()->ScanIn(strPrompt, bYes);
	if(!bYes) JobSetting::SetJobCanceled(true);

	return true;
}

bool CancelJob::Load(FILE* pFile)
{
	m_strMsg = rdString(pFile);
	return true;
}

bool CancelJob::Save(FILE* pFile)
{
	wrString(pFile, m_strMsg);
	return true;
}