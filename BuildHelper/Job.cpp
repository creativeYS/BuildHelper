
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Job.h"
#include "OutputControl.h"
#include "FileCopy.h"
#include "FileExecute.h"
#include "JobFileCopyDlg.h"
#include "JobFileExecuteDlg.h"
#include "JobBatchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// Job
Job::Job() : m_pImpl(nullptr)
{
}

Job::~Job()
{
	Init();
}

bool Job::Run()
{
	if (m_pImpl) return m_pImpl->Run();
	return false;
}

void Job::Init(int enType)
{
	m_strLoadedFilePath = L"";
	if (m_pImpl) delete m_pImpl;
	m_pImpl = nullptr;

	if (enType == -1) return;

	m_pImpl = JobBase::CreateImpl(enType);
}

bool Job::Load(const TCHAR* pFilePath)
{
	if (m_pImpl != nullptr)
	{
		Init();
	}

	FILE* pFile = JobBase::Open(pFilePath, false);
	if (pFile == NULL) DEF_OUT_RETURN_FALSE(L"파일을 열 수 없습니다.");

	CString strName = JobBase::rdString(pFile);
	int nType = JobBase::GetJobCode(strName);
	if(nType == JobBase::EN_JOB_TYPE::EN_JOB_TYPE_NUMBER) DEF_OUT_RETURN_FALSE(L"확인할 수 없는 작업입니다.");

	m_bSubJob = JobBase::rdInt(pFile) == 1 ? true : false;
	
	m_pImpl = JobBase::CreateImpl(nType);

	bool bRet = m_pImpl->Load(pFile);
	JobBase::Close(pFile);
	m_strLoadedFilePath = pFilePath;

	return bRet;
}

bool Job::Save(const TCHAR* pFilePath)
{
	if (m_pImpl == nullptr) DEF_OUT_RETURN_FALSE(L"로드되지 않은 작업입니다.");

	CString strPathOnly = FileUtils::GetOnlyPath(pFilePath);
	if (!FileUtils::PathExist(strPathOnly))
	{
		_wmkdir(strPathOnly);
	}

	FILE* pFile = JobBase::Open(pFilePath, true);
	if (pFile == NULL) DEF_OUT_RETURN_FALSE(L"파일을 열 수 없습니다.");

	WORD mark = 0xFEFF;
	fwrite(&mark, sizeof(WORD), 1, pFile);

	CString strName = JobBase::GetJobName(m_pImpl->GetType());
	JobBase::wrString(pFile, strName);

	JobBase::wrInt(pFile, m_bSubJob ? 1 : 0);

	bool bRet = m_pImpl->Save(pFile);
	JobBase::Close(pFile);
	m_strLoadedFilePath = pFilePath;

	return bRet;
}

int Job::GetJobType()
{
	if (m_pImpl)
	{
		return m_pImpl->GetType();
	}
	return -1;
}

UINT Job::DoModal()
{
	switch (GetJobType())
	{
	case JobBase::EN_JOB_TYPE_FILECOPY:
	{
		JobFileCopyDlg dlg2((FileCopy*)GetImpl());
		return dlg2.DoModal();
	} break;
	case JobBase::EN_JOB_TYPE_FILEEXECUTE:
	{
		JobFileExecuteDlg dlg2((FileExecute*)GetImpl());
		return dlg2.DoModal();
	} break;
	case JobBase::EN_JOB_TYPE_FILEBATCH:
	{
		JobBatchDlg dlg2((FileBatch*)GetImpl());
		return dlg2.DoModal();
	} break;
	default:
		ASSERT(0);
		break;
	}
	return IDCANCEL;
}
