#pragma once

#include "FileUtils.h"
#include "JobBase.h"

class CopyFileSolution : public JobBase
{
public:
	CopyFileSolution() {}
	virtual ~CopyFileSolution() {}

	enum EN_COPYFILETYPE
	{
		OUTPUT,
		LIB,
		PDB,
		NUMBER,
	};
	static CString GetTypeName(EN_COPYFILETYPE enType);

	// Set Functions...
	void	SetCopyFileType(EN_COPYFILETYPE enType) { m_enCopyFileType = enType; }
	void	SetTargetSolution(const CString& strTarget) { m_strTargetSolution = strTarget; }
	void	SetDestCopyPath(const CString& strDest) { m_strDestCopyPath = strDest; }
	void	SetOptions(const VecStr& strOptsKey, const VecStr& strOptsVal)
	{
		m_strOptionsKey = strOptsKey;
		m_strOptionsVal = strOptsVal;
	}

	// Get Functions...
	EN_COPYFILETYPE	GetCopyFileType() { return m_enCopyFileType; }
	CString	GetTargetSolution() { return m_strTargetSolution; }
	CString	GetDestCopyPath() { return m_strDestCopyPath; }
	int	GetOptions(VecStr& strKey, VecStr& strVal)
	{
		strKey = m_strOptionsKey;
		strVal = m_strOptionsVal;
		return (int)m_strOptionsKey.size();
	}

	// Main Functions...
	virtual bool	Run();

	virtual bool	Load(FILE* pFile);
	virtual bool	Save(FILE* pFile);

	virtual int		GetType() { return JobBase::EN_JOB_TYPE::EN_JOB_TYPE_COPYFILESOLUTION; }
	virtual void	SetParamOption(const CString& strOption);

protected:
	bool			Run_Solution(CString strTargetPath);

	bool			GetProjects(const CString strSolutionPath, MapStrBool& mapProjects);
	bool			GetBuildResultName(const CString &strProjectPath, CString& strRet);
	bool			IsInvalidValue(const CString& strVal);

	EN_COPYFILETYPE	m_enCopyFileType = OUTPUT;
	CString			m_strTargetSolution;
	CString			m_strDestCopyPath;
	VecStr			m_strOptionsKey;
	VecStr			m_strOptionsVal;
};