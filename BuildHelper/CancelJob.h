#pragma once

#include "FileUtils.h"
#include "JobBase.h"
#include <vector>

class CancelJob : public JobBase
{
public:
	CancelJob() {}
	virtual ~CancelJob() {}

	// Set Functions...
	void	SetMessage(const CString& strMsg) { m_strMsg = strMsg; }

	// Get Functions...
	CString	GetMessage() { return m_strMsg; }

	// Main Functions...
	virtual bool	Run();

	virtual bool	Load(FILE* pFile);
	virtual bool	Save(FILE* pFile);

	virtual int		GetType() { return JobBase::EN_JOB_TYPE::EN_JOB_TYPE_CANCELJOB; }
	virtual void	SetParamOption(const CString& strOption)
	{
		DEF_SETOPTION(m_strMsg);
	}

protected:
	CString m_strMsg;
};