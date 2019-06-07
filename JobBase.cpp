
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "JobBase.h"
#include "OutputControl.h"
#include "FileCopy.h"
#include "FileExecute.h"
#include "FileBatch.h"
#include "JobSetting.h"
#include "CreateFileList.h"
#include "CloseUI.h"
#include "CopyFileSolution.h"
#include "JobCreateFileListDlg.h"
#include "JobBatchDlg.h"
#include "JobFileExecuteDlg.h"
#include "JobFileCopyDlg.h"
#include "CopyFileSolutionDlg.h"
#include "CancelJob.h"
#include "CancelJobDlg.h"
#include "SelectJob.h"
#include "SelectJobDlg.h"

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
		break;
	case EN_JOB_TYPE_JOBSETTING:
		pImpl = new JobSetting();
		break;
	case EN_JOB_TYPE_CREATEFILELIST:
		pImpl = new CreateFileList();
		break;
	case EN_JOB_TYPE_CLOSEUI:
		pImpl = new CloseUI();
		break;
	case EN_JOB_TYPE_COPYFILESOLUTION:
		pImpl = new CopyFileSolution();
		break;
	case EN_JOB_TYPE_CANCELJOB:
		pImpl = new CancelJob();
		break;
	case EN_JOB_TYPE_SELECTJOB:
		pImpl = new SelectJob();
		break;
	default:
		ASSERT(0);
	}
	return pImpl;
}

CString JobBase::GetJobTypeName(int enType, bool bDisplay)
{
	CString strTemp;
	switch (enType)
	{
	case EN_JOB_TYPE_FILECOPY:
		strTemp = bDisplay ? L"복사 작업" : L"COPY";
		break;
	case EN_JOB_TYPE_FILEEXECUTE:
		strTemp = bDisplay ? L"실행 작업" : L"EXECUTE";
		break;
	case EN_JOB_TYPE_FILEBATCH:
		strTemp = bDisplay ? L"연속 실행" : L"BATCH";
		break;
	case EN_JOB_TYPE_JOBSETTING:
		strTemp = bDisplay ? L"설정" : L"SETTING";
		break;
	case EN_JOB_TYPE_CREATEFILELIST:
		strTemp = bDisplay ? L"파일 리스트 생성" : L"FILELIST";
		break;
	case EN_JOB_TYPE_CLOSEUI:
		strTemp = bDisplay ? L"UI 닫기" : L"CLOSEUI";
		break;
	case EN_JOB_TYPE_COPYFILESOLUTION:
		strTemp = bDisplay ? L"솔루션 결과 복사 작업" : L"COPYFILESOLUTION";
		break;
	case EN_JOB_TYPE_CANCELJOB:
		strTemp = bDisplay ? L"작업 취소 도구" : L"JOBCANCEL";
		break;
	case EN_JOB_TYPE_SELECTJOB:
		strTemp = bDisplay ? L"작업 선택" : L"SELECTJOB";
		break;
	default:
		ASSERT(0);
	}
	return strTemp;
}

int JobBase::GetJobTypeCode(const CString& strName)
{
	for (int i = 0; i < EN_JOB_TYPE_NUMBER; i++)
	{
		if (GetJobTypeName(i) == strName) return i;
	}
	return EN_JOB_TYPE_NUMBER;
}

UINT JobBase::DoModal(int nType, void* pImpl)
{
	switch (nType)
	{
	case JobBase::EN_JOB_TYPE_FILECOPY:
	{
		JobFileCopyDlg dlg((FileCopy*)pImpl);
		return (UINT)dlg.DoModal();
	} break;
	case JobBase::EN_JOB_TYPE_FILEEXECUTE:
	{
		JobFileExecuteDlg dlg((FileExecute*)pImpl);
		return (UINT)dlg.DoModal();
	} break;
	case JobBase::EN_JOB_TYPE_FILEBATCH:
	{
		JobBatchDlg dlg((FileBatch*)pImpl);
		return (UINT)dlg.DoModal();
	} break;
	case JobBase::EN_JOB_TYPE_CREATEFILELIST:
	{
		JobCreateFileListDlg dlg((CreateFileList*)pImpl);
		return (UINT)dlg.DoModal();
	} break;
	case JobBase::EN_JOB_TYPE_CLOSEUI:
	{
		return IDOK;
	} break;
	case JobBase::EN_JOB_TYPE_COPYFILESOLUTION:
	{
		CopyFileSolutionDlg dlg((CopyFileSolution*)pImpl);
		return (UINT)dlg.DoModal();
	} break;
	case JobBase::EN_JOB_TYPE_CANCELJOB:
	{
		CancelJobDlg dlg((CancelJob*)pImpl);
		return (UINT)dlg.DoModal();
	} break;
	case JobBase::EN_JOB_TYPE_SELECTJOB:
	{
		SelectJobDlg dlg((SelectJob*)pImpl);
		return (UINT)dlg.DoModal();
	} break;
	default:
		ASSERT(0);
		return IDOK;
		break;
	}
	return IDCANCEL;
}

FILE* JobBase::Open(const TCHAR* pFilePath, bool bWrite)
{
	FILE* pFile = NULL;
	try
	{
		_wfopen_s(&pFile, pFilePath, bWrite ? L"wb, ccs=UNICODE" : L"r, ccs=UNICODE");
	}
	catch (CException*)
	{
		if(pFile) fclose(pFile);
	}

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
		TCHAR  buff[2048];
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
		TCHAR  buff[2048];
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
		TCHAR  buff[2048];
		TCHAR* pStr;

		pStr = fgetws(buff, sizeof(buff), pFile);
		CString strRet = buff;
		strRet.Replace(L"\n", L"");
		strRet.Replace(L"\r", L"");
		return strRet;
	}
	return CString();
}

void JobBase::wrInt(FILE* pFile, int nVal)
{
	if (pFile)
	{
		CString strTemp;
		strTemp.Format(_T("%d\r\n"), nVal);
		fputws(strTemp, pFile);
	}
}

void JobBase::wrDouble(FILE* pFile, double dVal)
{
	if (pFile)
	{
		CString strTemp;
		strTemp.Format(_T("%f\r\n"), dVal);
		fputws(strTemp, pFile);
	}
}

void JobBase::wrString(FILE* pFile, const CString& strVal)
{
	if (pFile)
	{
		CString strTemp;
		strTemp.Format(_T("%s\r\n"), strVal);
		fputws(strTemp, pFile);
	}
}