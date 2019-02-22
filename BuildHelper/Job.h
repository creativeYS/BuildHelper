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

	CString		GetJobName() { return m_strJobName; };
	void		SetJobName(const TCHAR* pStr) { m_strJobName = pStr; }

	JobBase*	GetImpl() { return m_pImpl;  }

protected:
	CString		m_strJobName;
	JobBase*	m_pImpl;
};