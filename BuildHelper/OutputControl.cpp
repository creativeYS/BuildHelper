
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "OutputControl.h"
#include <iostream>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

OutputControl* OutputControl::Instance()
{
	static OutputControl gInstance;
	return &gInstance;
}

void OutputControl::ScanIn(const TCHAR* pPrompt, bool& bYes)
{
	if (m_enType == OutputControl::EN_TYPE::EN_TYPE_CONSOLE)
	{
		wcout << pPrompt << L"[yes/no]";
		TCHAR chIn[1024];
		wcin >> chIn;
		CString strTemp = chIn;
		if (strTemp.CompareNoCase(L"yes") == 0 ||
			strTemp.CompareNoCase(L"y") == 0)
		{
			bYes = true;
		}
	}
	else if (m_enType == OutputControl::EN_TYPE::EN_TYPE_CONSOLE)
	{
		if (AfxMessageBox(pPrompt, MB_YESNO) == IDYES) bYes = true;
	}
}

void OutputControl::PrintOut(const TCHAR* pStr)
{
	if (m_enType == OutputControl::EN_TYPE::EN_TYPE_CONSOLE)
	{
		wcout << pStr << endl;
	}
	else if (m_enType == OutputControl::EN_TYPE::EN_TYPE_CONSOLE)
	{
		AfxMessageBox(pStr);
	}
}

OutputControl::OutputControl()
	: m_enType(OutputControl::EN_TYPE::EN_TYPE_CONSOLE)
{

}
