
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Job.h"
#include "OutputControl.h"
#include "FileCopy.h"
#include "FileExecute.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// Total Count...
bool	g_bTotalCountMode = false;
int		g_nTotalCount = 0;


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
	if (g_bTotalCountMode)
	{
		g_nTotalCount++;
		if (m_pImpl->GetType() == JobBase::EN_JOB_TYPE::EN_JOB_TYPE_FILEBATCH)
		{
			m_pImpl->Run();
		}
		return true;
	}

	ProgressDlg::StepProgress(true);
	CString strPrompt;
	strPrompt.Format(_T("현재 작업 : %s (%s)"), GetJobName(), m_pImpl->GetJobTypeName(m_pImpl->GetType(), true));
	ProgressDlg::AddString(strPrompt);

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
	if (pFile == NULL)
	{
		JobBase::Close(pFile);
		DEF_OUT_RETURN_FALSE(L"파일을 열 수 없습니다.");
	}

	m_strLoadedFilePath = pFilePath;

	CString strName = JobBase::rdString(pFile);
	int nType = JobBase::GetJobTypeCode(strName);
	if (nType == JobBase::EN_JOB_TYPE::EN_JOB_TYPE_NUMBER)
	{
		JobBase::Close(pFile);
		DEF_OUT_RETURN_FALSE(L"확인할 수 없는 작업입니다.");
	}

	m_bSubJob = JobBase::rdInt(pFile) == 1 ? true : false;	
	m_pImpl = JobBase::CreateImpl(nType);

	bool bRet = m_pImpl->Load(pFile);
	JobBase::Close(pFile);

	if (m_strJobName.GetLength() <= 0)
	{
		CString strName = FileUtils::GetOnlyFileName(pFilePath, false, true);
		SetJobName(strName);
	}

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

	CString strName = JobBase::GetJobTypeName(m_pImpl->GetType());
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
	return JobBase::DoModal(GetJobType(), GetImpl());
}

void Job::SetParamOption(const CString& strOption)
{
	if (m_pImpl == nullptr) return;

	m_pImpl->SetParamOption(strOption);
}

int Job::GetTotalCount()
{
	if (m_pImpl == nullptr) return 0;

	g_bTotalCountMode = true;
	g_nTotalCount = 0;

	Run();

	g_bTotalCountMode = false;

	return g_nTotalCount;
}