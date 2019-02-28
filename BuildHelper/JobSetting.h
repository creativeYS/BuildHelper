#pragma once

#include "FileUtils.h"
#include "JobBase.h"


struct T_SETTING
{
	CString strName;
	CString strFilter;
	int		nShowSub;
	int		nUsePgmPath;
	CString strWorkingPath;

	T_SETTING(const CString& str) : strName(str), nShowSub(0), nUsePgmPath(1)
	{}
};
typedef std::vector<T_SETTING> VecSetting;

class JobSetting : public JobBase
{
public:
	JobSetting() {}
	virtual ~JobSetting() {}

	// Set Functions...
	void	SetSetting(const VecSetting& settings);

	// Get Functions...
	int		GetSetting(VecSetting& settings);

	const T_SETTING*	GetSettingData(const CString& strName = L"") const;

	CString	GetLastSetting() { return m_strLastSetting;  }
	void	SetLastSetting(const CString& strName) { m_strLastSetting = strName; }

	// Main Functions...
	virtual bool	Run();

	virtual bool	Load(FILE* pFile);
	virtual bool	Save(FILE* pFile);

	virtual int		GetType() { return JobBase::EN_JOB_TYPE::EN_JOB_TYPE_JOBSETTING; }

public:
	static CString	GetCurrentWorkingPath() { return m_strCurrentWorkingPath; }
	static void		SetCurrentWorkingPath(const CString& str) { m_strCurrentWorkingPath = str; }

protected:
	CString		m_strLastSetting;
	VecSetting	m_vecSettings;

	static CString m_strCurrentWorkingPath;
};