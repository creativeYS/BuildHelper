
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Job.h"
#include "OutputControl.h"
#include "FileCopy.h"
#include "FileExecute.h"

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
	m_pImpl = JobBase::CreateImpl(nType);

	bool bRet = m_pImpl->Load(pFile);
	JobBase::Close(pFile);

	return bRet;
}

bool Job::Save(const TCHAR* pFilePath)
{
	if (m_pImpl == NULL) DEF_OUT_RETURN_FALSE(L"로드되지 않은 작업입니다.");

	FILE* pFile = JobBase::Open(pFilePath, true);
	if (pFile == NULL) DEF_OUT_RETURN_FALSE(L"파일을 열 수 없습니다.");

	WORD mark = 0xFEFF;
	fwrite(&mark, sizeof(WORD), 1, pFile);

	CString strName = JobBase::GetJobName(m_pImpl->GetType());
	JobBase::wrString(pFile, strName);

	bool bRet = m_pImpl->Save(pFile);
	JobBase::Close(pFile);

	return bRet;
}