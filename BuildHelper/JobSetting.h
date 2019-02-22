#pragma once

#include "FileUtils.h"
#include "JobBase.h"

class JobSetting : public JobBase
{
public:
	JobSetting();

	// Set Functions...
	void	SetShowSubJob(bool bSet) { m_bShowSubJob = bSet; }
	void	SetUseProgramPath(bool bSet) { m_bUseProgramPath = bSet; }
	void	SetWorkingPath(const CString& str) { m_strWorkingPath = str; }

	// Get Functions...
	bool	GetShowSubJob() const { return m_bShowSubJob; }
	bool	GetUseProgramPath() const { return m_bUseProgramPath; }
	CString GetWorkingPath() const  { return m_strWorkingPath; }

	// Main Functions...
	virtual bool	Run();

	virtual bool	Load(FILE* pFile);
	virtual bool	Save(FILE* pFile);

	virtual int		GetType() { return JobBase::EN_JOB_TYPE::EN_JOB_TYPE_JOBSETTING; }

protected:
	bool	m_bShowSubJob = true;
	bool	m_bUseProgramPath = true;
	CString	m_strWorkingPath;
};