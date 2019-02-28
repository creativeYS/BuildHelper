#pragma once

#include <vector>

typedef std::vector<int>		VecInt;
typedef std::vector<CString>	VecStr;

class Job;

class JobBase
{
public:
	virtual ~JobBase() {}

	virtual bool	Run() = 0;

	virtual bool	Load(FILE* pFile) = 0;
	virtual bool	Save(FILE* pFile) = 0;

	virtual int		GetType() = 0;

public:
	enum EN_JOB_TYPE
	{
		EN_JOB_TYPE_FILECOPY,
		EN_JOB_TYPE_FILEEXECUTE,
		EN_JOB_TYPE_FILEBATCH,
		EN_JOB_TYPE_JOBSETTING,
		EN_JOB_TYPE_NUMBER,
	};
	static JobBase* CreateImpl(int enType);
	static CString	GetJobName(int enType, bool bDisplay = false);
	static int		GetJobCode(const CString& strName);

	static FILE*	Open(const TCHAR* pFilePath, bool bWrite);
	static void		Close(FILE* pFile);

	static int		rdInt(FILE* pFile);
	static double	rdDouble(FILE* pFile);
	static CString	rdString(FILE* pFile);

	static void		wrInt(FILE* pFile, int nVal);
	static void		wrDouble(FILE* pFile, double dVal);
	static void		wrString(FILE* pFile, const CString& strVal);
};