#pragma once

class JobBase;
class Job
{
public:
	Job();
	virtual ~Job();
	
	bool Run();

	void Init(int enType = -1); // enType == -1 : just init
	bool Load(const TCHAR* pFilePath);
	bool Save(const TCHAR* pFilePath);

	// Property
	CString		GetJobName() { return m_strJobName; };
	void		SetJobName(const TCHAR* pStr) { m_strJobName = pStr; }

	bool		GetSubJob() { return m_bSubJob; }
	void		SetSubJob(bool bSet) { m_bSubJob = bSet; }

	// ReadOnly
	CString		GetLoadedFilePath() { return m_strLoadedFilePath; }
	int			GetJobType();

	UINT		DoModal();
	JobBase*	GetImpl() { return m_pImpl;  }

protected:
	CString		m_strJobName;
	bool		m_bSubJob = true;
	CString		m_strLoadedFilePath;
	JobBase*	m_pImpl;
};