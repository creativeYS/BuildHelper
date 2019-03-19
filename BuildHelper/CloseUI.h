#pragma once

#include "FileUtils.h"
#include "JobBase.h"
#include <vector>

class CloseUI : public JobBase
{
public:
	CloseUI() {}
	virtual ~CloseUI() {}

	// Main Functions...
	virtual bool	Run();

	virtual bool	Load(FILE* pFile);
	virtual bool	Save(FILE* pFile);

	virtual int		GetType() { return JobBase::EN_JOB_TYPE::EN_JOB_TYPE_CLOSEUI; }

protected:
};