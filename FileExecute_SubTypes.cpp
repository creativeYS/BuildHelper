
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FileExecute_SubTypes.h"
#include "OutputControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool FileExecute_ShellExecute::Run()
{
	SHELLEXECUTEINFO execinfo;

	// 실행을 위해 구조체 세트
	ZeroMemory(&execinfo, sizeof(execinfo));
	execinfo.cbSize = sizeof(execinfo);
	execinfo.lpVerb = L"open";
	execinfo.lpFile = m_strExeFile;
	execinfo.lpParameters = m_strParams;
	execinfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	execinfo.nShow = SW_SHOWNORMAL;

	// 프로그램을 실행한다.
	int r = (int)ShellExecuteEx(&execinfo);
	//프로세스가 종료될 때까지 무한정 기다림
	DWORD dwRet = WaitForSingleObject(execinfo.hProcess, 100);
	if (execinfo.hProcess)
	{
		while (dwRet)
		{
			MSG msg;
			while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
			}
			dwRet = WaitForSingleObject(execinfo.hProcess, 200);
		}
	}

	dwRet = 1;
	GetExitCodeProcess(execinfo.hProcess, &dwRet);

	if (dwRet == 0) return true;
	return false;
}

bool FileExecute_CreateProcess::Run()
{
	CString csExecute;
	csExecute = m_strExeFile + " " + m_strParams;

	SECURITY_ATTRIBUTES secattr;
	ZeroMemory(&secattr, sizeof(secattr));
	secattr.nLength = sizeof(secattr);
	secattr.bInheritHandle = TRUE;

	HANDLE rPipe, wPipe;

	//Create pipes to write and read data 

	CreatePipe(&rPipe, &wPipe, &secattr, 0);

	STARTUPINFO sInfo;
	ZeroMemory(&sInfo, sizeof(sInfo));
	PROCESS_INFORMATION pInfo;
	ZeroMemory(&pInfo, sizeof(pInfo));
	sInfo.cb = sizeof(sInfo);
	sInfo.dwFlags = STARTF_USESTDHANDLES;
	sInfo.hStdInput = NULL;
	sInfo.hStdOutput = wPipe;
	sInfo.hStdError = wPipe;

	//Create the process here. 

	if (CreateProcess(0, csExecute.GetBuffer(), 0, 0, TRUE,
		NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, 0, 0, &sInfo, &pInfo) == FALSE)
	{
		return false;
	}
	CloseHandle(wPipe);

	//now read the output pipe here. 
	char buf[2048] = { 0 };
	DWORD reDword;
	CString csTemp;
	BOOL res;
	do
	{
		res = ::ReadFile(rPipe, buf, 2047, &reDword, 0);
		buf[reDword] = '\0';
		csTemp = buf;
		csTemp.Replace(L"\r", L"");
		csTemp.Replace(L"\n", L"");
		DEF_OUT_CONSOLE(csTemp);
	} while (res);

	DWORD dwRet = 1;
	GetExitCodeProcess(pInfo.hProcess, &dwRet);

	if (dwRet == 0) return true;
	return false;
}