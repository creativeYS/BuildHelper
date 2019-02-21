#pragma once

class JobBase
{
public:
	virtual bool	Run() = 0;

	virtual bool	Load(FILE* pFile) = 0;
	virtual bool	Save(FILE* pFile) = 0;

	virtual int		GetType() = 0;

public:
	enum EN_JOB_TYPE
	{
		EN_JOB_TYPE_FILECOPY,
		EN_JOB_TYPE_FILEEXECUTE,
	};
	static JobBase* CreateImpl(int enType);

	static FILE*	Open(const TCHAR* pFilePath, bool bWrite);
	static void		Close(FILE* pFile);

	static int		rdInt(FILE* pFile);
	static double	rdDouble(FILE* pFile);
	static CString	rdString(FILE* pFile);

	static void		wrInt(FILE* pFile, int nVal);
	static void		wrDouble(FILE* pFile, double dVal);
	static void		wrString(FILE* pFile, const CString& strVal);
};