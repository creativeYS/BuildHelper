#pragma once

#include "FileUtils.h"
#include "JobBase.h"

class SelectJob : public JobBase
{
public:
	SelectJob() {}
	virtual ~SelectJob() {}

	// Set Functions...
	void	SetFilter(const CString& strFilter) { m_strFilter = strFilter; }
	void	SetIncludeUnitJob(bool bSet) { m_bIncludeUnitJob = bSet; }

	// Get Functions...
	CString	GetFilter() { return m_strFilter; }
	bool	GetIncludeUnitJob() { return m_bIncludeUnitJob; }

	// Main Functions...
	virtual bool	Run();

	virtual bool	Load(FILE* pFile);
	virtual bool	Save(FILE* pFile);

	virtual int		GetType() { return JobBase::EN_JOB_TYPE::EN_JOB_TYPE_SELECTJOB; }
	virtual void	SetParamOption(const CString& strOption)
	{
		m_Options.push_back(strOption);
		DEF_SETOPTION(m_strFilter);
		DEF_SETOPTION(m_bIncludeUnitJob);
	}

protected:
	CString m_strFilter = L"*";
	bool	m_bIncludeUnitJob = true;
	VecStr	m_Options;
};