#pragma once

#include "FileUtils.h"
#include "JobBase.h"

class CreateFileList : public JobBase
{
public:
	CreateFileList() {}
	virtual ~CreateFileList() {}

	// Set Functions...
	void	SetTargetPath(const CString& strPath) { m_strTargetPath = strPath; }
	void	SetTargetFilter(const CString& strFilter) { m_strTargetFilter = strFilter; }
	void	SetIncludeSubFolder(bool bSet) { m_bIncludeSubFolder = bSet; }

	// Get Functions...
	CString GetTargetPath() const { return m_strTargetPath; }
	CString GetTargetFilter() const { return m_strTargetFilter; }
	bool	GetIncludeSubFolder() const { return m_bIncludeSubFolder; }

	// Main Functions...
	virtual bool	Run();

	virtual bool	Load(FILE* pFile);
	virtual bool	Save(FILE* pFile);

	virtual int		GetType() { return JobBase::EN_JOB_TYPE::EN_JOB_TYPE_CREATEFILELIST; }

protected:
	bool			Run_Path(CString strTargetPath, CString strTargetFilter);
	bool			Run_Solution(CString strTargetPath);

	bool			GetProjects(const CString strSolutionPath, MapStrBool& mapProjects);

	CString m_strTargetPath;
	CString m_strTargetFilter;
	bool	m_bIncludeSubFolder = true;
};