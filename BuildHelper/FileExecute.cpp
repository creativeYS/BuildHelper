
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FileExecute.h"
#include "OutputControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

FileExecute::FileExecute()
{
}

bool FileExecute::Run()
{
	return true;
}

bool FileExecute::Load(FILE* pFile)
{
	m_strExecuteFile = rdString(pFile);
	m_strParam = rdString(pFile);
	return true;
}

bool FileExecute::Save(FILE* pFile)
{
	wrString(pFile, m_strExecuteFile);
	wrString(pFile, m_strParam);
	return true;
}