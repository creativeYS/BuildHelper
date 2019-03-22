
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "CloseUI.h"
#include "OutputControl.h"
#include "Job.h"
#include "JobSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool CloseUI::Run()
{
	JobSetting::SetUIClosed(true);

	return true;
}

bool CloseUI::Load(FILE* pFile)
{
	return true;
}

bool CloseUI::Save(FILE* pFile)
{
	return true;
}