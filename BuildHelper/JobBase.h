#pragma once

#include <vector>
#include <map>

typedef std::vector<int>		VecInt;
typedef std::vector<CString>	VecStr;
typedef std::map<CString, bool>	MapStrBool;

class Job;


template <class T>
void SetOptionSubFunc(T& tVar, const CString& strOpt)
{
	tVar = decltype(tVar)strOpt;
}
template <class T>
void SetOptionSubFunc(CString& tVar, const CString& strOpt)
{
	tVar = strOpt;
}
template <class T>
void SetOptionSubFunc(bool& tVar, const CString& strOpt)
{
	tVar = _wtoi(strOpt) == 0 ? false : true;
}
template <class T>
void SetOptionSubFunc(int& tVar, const CString& strOpt)
{
	tVar = _wtoi(strOpt);
}

#define DEF_SETOPTION(VAR) { \
CString strKey##VAR = L#VAR; \
if (strOption.Find(L"/" + strKey##VAR + L"=") == 0) { \
SetOptionSubFunc<decltype(VAR)>(VAR, strOption.Mid(strKey##VAR.GetLength() + 2)); }}

class JobBase
{
public:
	virtual ~JobBase() {}

	virtual bool	Run() = 0;

	virtual bool	Load(FILE* pFile) = 0;
	virtual bool	Save(FILE* pFile) = 0;

	virtual int		GetType() = 0;
	virtual void	SetParamOption(const CString& strOption) {};

public:
	enum EN_JOB_TYPE
	{
		EN_JOB_TYPE_FILECOPY,
		EN_JOB_TYPE_FILEEXECUTE,
		EN_JOB_TYPE_FILEBATCH,
		EN_JOB_TYPE_JOBSETTING,
		EN_JOB_TYPE_CREATEFILELIST,
		EN_JOB_TYPE_CLOSEUI,
		EN_JOB_TYPE_COPYFILESOLUTION,
		EN_JOB_TYPE_CANCELJOB,
		EN_JOB_TYPE_NUMBER,
	};
	static JobBase* CreateImpl(int enType);
	static CString	GetJobTypeName(int enType, bool bDisplay = false);
	static int		GetJobTypeCode(const CString& strName);
	static UINT		DoModal(int nType, void* pImpl);

	static FILE*	Open(const TCHAR* pFilePath, bool bWrite);
	static void		Close(FILE* pFile);

	static int		rdInt(FILE* pFile);
	static double	rdDouble(FILE* pFile);
	static CString	rdString(FILE* pFile);

	static void		wrInt(FILE* pFile, int nVal);
	static void		wrDouble(FILE* pFile, double dVal);
	static void		wrString(FILE* pFile, const CString& strVal);
};