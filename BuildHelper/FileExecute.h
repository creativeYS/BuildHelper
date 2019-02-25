#pragma once

#include "FileUtils.h"
#include "JobBase.h"

class FileExecute : public JobBase
{
public:
	FileExecute();

	// Set Functions...
	void	SetExecuteFile(const CString& str) { m_strExecuteFile = str; }
	void	AddParam(const CString& strName, const CString& strParam);
	void	SetParam(int nCount, const CString* pStrName, const CString* strParam);

	// Get Functions...
	CString GetExecuteFile() const  { return m_strExecuteFile; }
	int		GetParams(VecStr& strName, VecStr& strParam) const;
	int		GetParamNum() const;

	// Main Functions...
	virtual bool	Run();

	virtual bool	Load(FILE* pFile);
	virtual bool	Save(FILE* pFile);

	virtual int		GetType() { return JobBase::EN_JOB_TYPE::EN_JOB_TYPE_FILEEXECUTE; }

protected:
	CString m_strExecuteFile;			// c:\test\run.exe
	std::vector<CString> m_vecParamName;	// Param Name
	std::vector<CString> m_vecParam;		// Param
};