
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "JobBase.h"
#include "OutputControl.h"
#include "FileCopy.h"
#include "FileExecute.h"
#include "FileBatch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// Job Impl
JobBase* JobBase::CreateImpl(int enType)
{
	JobBase* pImpl = nullptr;
	switch (enType)
	{
	case EN_JOB_TYPE_FILECOPY:
		pImpl = new FileCopy();
		break;
	case EN_JOB_TYPE_FILEEXECUTE:
		pImpl = new FileExecute();
		break;
	case EN_JOB_TYPE_FILEBATCH:
		pImpl = new FileBatch();
	default:
		ASSERT(0);
	}
	return pImpl;
}

CString JobBase::GetJobName(int enType)
{
	CString strTemp;
	switch (enType)
	{
	case EN_JOB_TYPE_FILECOPY:
		strTemp = L"FILECOPY";
		break;
	case EN_JOB_TYPE_FILEEXECUTE:
		strTemp = L"FILEEXECUTE";
		break;
	case EN_JOB_TYPE_FILEBATCH:
		strTemp = L"FILEBATCH";
	default:
		ASSERT(0);
	}
	return strTemp;
}

int JobBase::GetJobCode(const CString& strName)
{
	for (int i = 0; i < EN_JOB_TYPE_NUMBER; i++)
	{
		if (GetJobName(i) == strName) return i;
	}
	return EN_JOB_TYPE_NUMBER;
}

FILE* JobBase::Open(const TCHAR* pFilePath, bool bWrite)
{
	FILE* pFile = NULL;
	try
	{
		FILE* pFile = NULL;
		_wfopen_s(&pFile, pFilePath, bWrite ? L"wb, ccs=UNICODE" : L"r, ccs=UNICODE");
		if (pFile == NULL) DEF_OUT_RETURN_FALSE(L"파일을 열수 없습니다.");

		WORD mark = 0xFEFF;
		fwrite(&mark, sizeof(WORD), 1, pFile);
	}
	catch (CException*)
	{
		fclose(pFile);
	}
	fclose(pFile);

	return pFile;
}

void JobBase::Close(FILE* pFile)
{
	if (pFile)
	{
		fclose(pFile);
	}
}

int JobBase::rdInt(FILE* pFile)
{
	if (pFile == nullptr) return 0;

	if (!feof(pFile))
	{
		TCHAR  buff[1024];
		TCHAR* pStr;

		pStr = fgetws(buff, sizeof(buff), pFile);
		return _wtoi(buff);
	}
	return 0;
}

double JobBase::rdDouble(FILE* pFile)
{
	if (pFile == nullptr) return 0.0;

	if (!feof(pFile))
	{
		TCHAR  buff[1024];
		TCHAR* pStr;

		pStr = fgetws(buff, sizeof(buff), pFile);
		return _wtof(buff);
	}
	return 0.0;
}

CString JobBase::rdString(FILE* pFile)
{
	if (pFile == nullptr) return CString();

	if (!feof(pFile))
	{
		TCHAR  buff[1024];
		TCHAR* pStr;

		pStr = fgetws(buff, sizeof(buff), pFile);
		CString strRet = buff;
		return strRet;
	}
	return CString();
}

void JobBase::wrInt(FILE* pFile, int nVal)
{
	if (pFile)
	{
		CString strTemp;
		strTemp.Format(_T("%d\n"), nVal);
		fputws(strTemp, pFile);
	}
}

void JobBase::wrDouble(FILE* pFile, double dVal)
{
	if (pFile)
	{
		CString strTemp;
		strTemp.Format(_T("%f\n"), dVal);
		fputws(strTemp, pFile);
	}
}

void JobBase::wrString(FILE* pFile, const CString& strVal)
{
	if (pFile)
	{
		CString strTemp;
		strTemp.Format(_T("%s\n"), strVal);
		fputws(strTemp, pFile);
	}
}