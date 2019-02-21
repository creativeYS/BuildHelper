#pragma once

class JobBase;
class Job
{
public:
	Job();
	virtual ~Job();
	
	bool Run();

	void Init();
	bool Load(const TCHAR* pFilePath);
	bool Save(const TCHAR* pFilePath);

	CString	GetJobName() { return m_strJobName; };
	void	SetJobName(const TCHAR* pStr) { m_strJobName = pStr; }

protected:
	CString		m_strJobName;
	JobBase*	m_pImpl;
};