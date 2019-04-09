#pragma once

class FileExecute_Base
{
public:
	void SetExeFile(const TCHAR* pExeFile) { m_strExeFile = pExeFile; }
	void SetParams(const TCHAR* pParams) { m_strParams = pParams; }

	virtual bool	Run() = 0;

protected:
	CString m_strExeFile;
	CString m_strParams;
};

class FileExecute_ShellExecute : public FileExecute_Base
{
public:
	FileExecute_ShellExecute() {}
	virtual ~FileExecute_ShellExecute() {}

	bool	Run() override;
};

class FileExecute_CreateProcess : public FileExecute_Base
{
public:
	FileExecute_CreateProcess() {}
	virtual ~FileExecute_CreateProcess() {}

	bool	Run() override;
};