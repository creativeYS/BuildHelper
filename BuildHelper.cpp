
// BuildHelper.cpp: 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "BuildHelper.h"
#include "BuildHelperDlg.h"
#include "FileCopy.h"
#include <locale.h>
#include "OutputControl.h"
#include "Job.h"
#include "JobSetting.h"

#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBuildHelperApp

BEGIN_MESSAGE_MAP(CBuildHelperApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

// CBuildHelperApp 생성

CBuildHelperApp::CBuildHelperApp()
{111111111111111111111
	111
	int xxxx = 3;

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	int a = 3;
	a++;
	int x = 3;
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;

	ㄴㅇㄹㄴㅇㄹㄴㅇㄹ
	SetAppID(_T("RTManager.AppID.NoVersion"));
	double dd = 10.0;
	AfxEnableControlContainer()
	dd += 10000.0;

	sdfsdfs

	ㄴㄴㄴ

		ㄴㄴㄴㄴ

	int ddd = 32;
	int val1 = 1 << 32;
	int val2 = 1 << ddd;
	ddd = 1;
	int val3 = 0x80000000 << ddd;
	a++;
	a++;
// 	_asm
// 	{
// 		mov         eax, 1
// 		mov         ecx, dword ptr[ddd]
	dfsdf

// 		shl         eax, cl
// 		mov         dword ptr[val2], eax
// 
// 		mov         eax, 1
// 		mov         ecx, dword ptr[ddd]
// 		sal         eax, cl
// 		mov         dword ptr[val2], eax
// 	}

	int a1 = 3;
	a1++;
}


// 유일한 CBuildHelperApp 개체입니다.

CBuildHelperApp theApp;


// CBuildHelperApp 초기화

bool ConsoleMode(bool& bRetValue);

BOOL CBuildHelperApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// MFC 컨트롤의 테마를 사용하기 위해 "Windows 원형" 비주얼 관리자 활성화
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

	bool bRetValue = true;
	if (ConsoleMode(bRetValue))
	{
		if (!bRetValue) m_nExitCode = 1;
		return FALSE;
	}

	OutputControl::Instance()->SetType(OutputControl::EN_TYPE_MSGBOX);

	CBuildHelperDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 응용 프로그램이 예기치 않게 종료됩니다.\n");
		TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	}

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
		int a = 3;
		a++;
		a++;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
	a
}

int CBuildHelperApp::ExitInstance()
{
	int nOrgVal = CWinApp::ExitInstance();
	if (m_nExitCode != 0) return m_nExitCode;
	return nOrgVal;
}

bool CBuildHelperApp::CmdRun(int nNum, const CString* pParams)
{
	CString strPath;
	strPath.Format(L"%s*", FileUtils::GetSettingPath());
	VecStr files;
	FileUtils::FileList(strPath, L"*.job", files);

	Job Setting;
	CString strSettingFileName;
	strSettingFileName.Format(L"%sSetting.dat", FileUtils::GetSettingPath());
	Setting.Load(strSettingFileName);

	JobSetting* pSetting = (JobSetting*)Setting.GetImpl();
	if (pSetting)
	{
		const T_SETTING* pSettingData = pSetting->GetSettingData();
		if (pSettingData && pSettingData->nUsePgmPath == 0)
		{
			JobSetting::SetCurrentWorkingPath(pSettingData->strWorkingPath);
		}
	}

	if (JobSetting::GetCurrentWorkingPath().GetLength() <= 0)
	{
		JobSetting::SetCurrentWorkingPath(FileUtils::GetCurrentModulePath());
	}

	if (nNum < 2)
	{
		DEF_OUT(L"Error> [Job Name] required.");
	}
	else
	{
		const CString& strJobName = pParams[1];

		for (CString str : files)
		{
			CString strName = FileUtils::GetOnlyFileName(str, false, true);
			if (strName.CompareNoCase(strJobName) == 0)
			{
				Job job;
				job.SetJobName(strName);
				job.Load(str);
				for (int i = 2; i < nNum; i++)
				{
					const CString& strParam = pParams[i];
					if (strParam.GetLength() <= 0) continue;
					if (strParam[0] == '/')
						job.SetParamOption(strParam);
				}
				
				bool bRet = job.Run();
				DEF_OUT(L"Executed.");
				return bRet;
			}
		}
		CString strPrompt;
		strPrompt.Format(L"Error > Can not find %s", strJobName);
		DEF_OUT(strPrompt);
	}
	return true;
}

bool ConsoleMode(bool& bRetValue)
{
	int iCnt = __argc;
	if (iCnt <= 1)
	{
		FreeConsole();
		return false;
	}

	OutputControl::Instance()->SetType(OutputControl::EN_TYPE_CONSOLE);

	std::vector<CString> params;
	params.reserve(iCnt);

	for (int i = 1; i < iCnt; i++)
	{
		LPCTSTR pszParam = __targv[i];
		params.push_back(CString(pszParam));
	}



	CString strPath;
	strPath.Format(L"%s*", FileUtils::GetSettingPath());
	VecStr files;
	FileUtils::FileList(strPath, L"*.job", files);

	if (params[0].CompareNoCase(L"?") == 0)
	{
		DEF_OUT(L"1. [program] list [options : /os = only sub job, /es = except sub job]");
		DEF_OUT(L"2. [program] create [Job Name] [JobType] [SubJob:0,1] \"val1\" \"val2\" \"val3\" ...");
		CString strTypes;
		for (int i = 0; i < JobBase::EN_JOB_TYPE::EN_JOB_TYPE_NUMBER; i++)
		{
			CString strTypeName = JobBase::GetJobTypeName(i, false);
			if (strTypes.GetLength() == 0)	strTypes.Append(L" - JobType : ");
			else							strTypes.Append(L", ");
			strTypes.Append(strTypeName);
		}
		DEF_OUT(strTypes);
		DEF_OUT(L"3. [program] run [Job Name]");
		DEF_OUT(L"4. [program] delete [Job Name]");
		DEF_OUT(L"5. [program] setting [Setting Name]")
			return true;
	}
	else if (params[0].CompareNoCase(L"list") == 0)
	{
		int nIndex = 1;
		bool bSubOnly = false;
		bool bExceptOnly = false;
		if (params.size() >= 2 && params[1].CompareNoCase(L"/os") == 0) bSubOnly = true;
		if (params.size() >= 2 && params[1].CompareNoCase(L"/es") == 0) bExceptOnly = true;

		for (CString str : files)
		{
			CString strName = FileUtils::GetOnlyFileName(str, false, true);

			Job job;
			job.SetJobName(strName);
			job.Load(str);

			CString strPrompt;
			if (bSubOnly)
			{
				if (!job.GetSubJob()) continue;
			}
			else if (bExceptOnly)
			{
				if (job.GetSubJob()) continue;
			}

			strPrompt.Format(_T("%d. %s (%s)"), nIndex++, strName, str);
			DEF_OUT(strPrompt);
		}
		DEF_OUT(L"Done");
	}
	else if (params[0].CompareNoCase(L"run") == 0)
	{
		bRetValue = CBuildHelperApp::CmdRun((int)params.size(), &params[0]);
	}
	else if (params[0].CompareNoCase(L"delete") == 0)
	{
		if (params.size() < 2)
		{
			DEF_OUT(L"Error> [Job Name] required.");
		}
		else
		{
			CString& strJobName = params[1];

			bool bFind = false;
			for (CString str : files)
			{
				CString strName = FileUtils::GetOnlyFileName(str, false, true);
				if (strName.CompareNoCase(strJobName) == 0)
				{
					::DeleteFile(str);
					bFind = true;
					DEF_OUT(L"Deleted.");
					break;
				}
			}
			if (!bFind)
			{
				CString strPrompt;
				strPrompt.Format(L"Error > Can not find \"%s\"", strJobName);
				DEF_OUT(strPrompt);
			}
		}
	}
	else if (params[0].CompareNoCase(L"create") == 0 && params.size() > 3)
	{
		CString strTempFileName;
		strTempFileName.Format(L"%s%s.job", FileUtils::GetSettingPath(), params[1]);

		FILE* pFile = JobBase::Open(strTempFileName, true);
		if (pFile == NULL) DEF_OUT_RETURN_FALSE(L"파일을 열 수 없습니다.");

		WORD mark = 0xFEFF;
		fwrite(&mark, sizeof(WORD), 1, pFile);

		bool bExecute = JobBase::GetJobTypeCode(params[2]) == JobBase::EN_JOB_TYPE::EN_JOB_TYPE_FILEEXECUTE;
		for (int i = 2; i < (int)params.size(); i++)
		{
			CString strWriteTemp;
			if (bExecute && params[i].Find(L" ") >= 0)
				strWriteTemp.Format(_T("\"%s\""), params[i]);
			else
				strWriteTemp = params[i];

			JobBase::wrString(pFile, strWriteTemp);
		}

		JobBase::Close(pFile);
	}
	else if (params[0].CompareNoCase(L"setting") == 0)
	{
		Job Setting;
		CString strSettingFileName;
		strSettingFileName.Format(L"%sSetting.dat", FileUtils::GetSettingPath());
		Setting.Load(strSettingFileName);

		JobSetting* pSetting = (JobSetting*)Setting.GetImpl();
		if (pSetting == nullptr) return false;
		const T_SETTING* pSettingData = pSetting->GetSettingData(params[1]);
		if (pSettingData)
		{
			pSetting->SetLastSetting(params[1]);
			Setting.Save(strSettingFileName);
		}
		else DEF_OUT_RETURN_FALSE(L"확인할 수 없는 설정입니다.");
	}
	double dTest = 1.0;
	int aaaaaa = 30;
	aaaaaa++;
	// 한글 주석 테스트.

	return true;
	int a = 3;
	a++;

}