#pragma once

#include "FileUtils.h"
#include "JobBase.h"
#include <vector>

class FileBatch : public JobBase
{
public:
	FileBatch() {}
	virtual ~FileBatch() {}

	// Set Functions...
	void	SetJobs(int nCnt, const CString* pVals);
	void	AddJob(const CString& strJob);

	// Get Functions...
	int		GetJobs(std::vector<CString>& rVals) const;

	// Main Functions...
	virtual bool	Run();

	virtual bool	Load(FILE* pFile);
	virtual bool	Save(FILE* pFile);

	virtual int		GetType() { return JobBase::EN_JOB_TYPE::EN_JOB_TYPE_FILEBATCH; }

protected:

protected:
	std::vector<CString> m_vecTargetJobs;		// JobFile Path { c:\test\aadf.job, c:\good.job, ... }
};