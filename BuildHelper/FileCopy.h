#pragma once

#include "FileUtils.h"

class FileCopy
{
public:
	FileCopy();

	// Set Functions...
	void	SetSourcePath(const CString& strPath) { m_strSourcePath = strPath; }
	void	SetDestPath(const CString& strPath) { m_strDestPath = strPath; }
	void	SetTargetFilter(const CString& strFilter) { m_strTargetFilter = strFilter; }
	void	SetIncludeSubFolder(bool bSet) { m_bIncludeSubFolder = bSet; }

	// Get Functions...
	CString GetSourcePath() const  { return m_strSourcePath; }
	CString GetDestPath() const  { return m_strDestPath; }
	CString GetTargetFilter() const  { return m_strTargetFilter; }
	bool	GetIncludeSubFolder() const { return m_bIncludeSubFolder; }

	// Main Functions...
	bool	Copy();

protected:
	bool	MakeDir(const TCHAR* pPath);

protected:
	CString m_strSourcePath;		// c:\test\* 
	CString m_strDestPath;			// c:\target
	CString m_strTargetFilter;		// *.txt
	bool	m_bIncludeSubFolder;
};