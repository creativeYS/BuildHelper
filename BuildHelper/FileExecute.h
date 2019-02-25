#pragma once

#include "FileUtils.h"
#include "JobBase.h"

class FileExecute : public JobBase
{
public:
	FileExecute();

	// Set Functions...
	void	SetExecuteFile(const CString& str) { m_strExecuteFile = str; }
	void	SetParam(const CString& str) { m_strParam = str; }

	// Get Functions...
	CString GetExecuteFile() const  { return m_strExecuteFile; }
	CString GetParam() const  { return m_strParam; }

	// Main Functions...
	virtual bool	Run();

	virtual bool	Load(FILE* pFile);
	virtual bool	Save(FILE* pFile);

	virtual int		GetType() { return JobBase::EN_JOB_TYPE::EN_JOB_TYPE_FILEEXECUTE; }

protected:
	CString m_strExecuteFile;		// c:\test\run.exe
	CString m_strParam;				// "c:\param1\test.txt" good 2
};