
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "BuildHelper.h"
#include "BuildHelperDlg.h"
#include "afxdialogex.h"
#include "FileUtils.h"
#include "Job.h"
#include "JobSetting.h"
#include "OutputControl.h"
#include "JobTypeDlg.h"
#include "JobFileCopyDlg.h"
#include "JobFileExecuteDlg.h"
#include "JobBatchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBuildHelperDlg 대화 상자



CBuildHelperDlg::CBuildHelperDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BUILDHELPER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBuildHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

bool CBuildHelperDlg::Dlg2Data()
{
	if (m_pSetting == nullptr) { ASSERT(0); return false; }
	JobSetting* pSetting = (JobSetting*)m_pSetting->GetImpl();
	if (pSetting == nullptr) { ASSERT(0); return false; }

	bool bChk1 = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	pSetting->SetShowSubJob(bChk1);
	bool bChk2 = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck();
	pSetting->SetUseProgramPath(bChk2);
	CString strTemp;
	GetDlgItem(IDC_EDIT2)->GetWindowText(strTemp);
	pSetting->SetWorkingPath(strTemp);
	return true;
}

void CBuildHelperDlg::Data2Dlg()
{
	if (m_pSetting == nullptr) { ASSERT(0); return; }
	JobSetting* pSetting = (JobSetting*)m_pSetting->GetImpl();
	if (pSetting == nullptr) { ASSERT(0); return; }

	if (pSetting->GetShowSubJob()) ((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(TRUE);
	if (pSetting->GetUseProgramPath()) ((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(TRUE);
	GetDlgItem(IDC_EDIT2)->SetWindowText(pSetting->GetWorkingPath());
}

BEGIN_MESSAGE_MAP(CBuildHelperDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK4, &CBuildHelperDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_BUTTON5, &CBuildHelperDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_BUTTON1, &CBuildHelperDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CBuildHelperDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBuildHelperDlg 메시지 처리기

BOOL CBuildHelperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	static Job gSetting;
	if(m_pSetting == NULL)
	{
		CString strPath;
		strPath.Format(_T("%s%s"), FileUtils::GetCurrentModulePath(), FileUtils::GetSettingFileName());
		gSetting.Load(strPath);
		if (gSetting.GetImpl() == nullptr ||
			gSetting.GetImpl()->GetType() != JobBase::EN_JOB_TYPE_JOBSETTING)
		{
			gSetting.Init(JobBase::EN_JOB_TYPE_JOBSETTING);
		}

		m_pSetting = &gSetting;
	}
	Data2Dlg();
	OnBnClickedCheck4();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CBuildHelperDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CBuildHelperDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBuildHelperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBuildHelperDlg::OnBnClickedCheck4()
{
	BOOL bEnable = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck() ? FALSE : TRUE;

	GetDlgItem(IDC_EDIT2)->EnableWindow(bEnable);
}


void CBuildHelperDlg::OnBnClickedClose()
{
	if (!Dlg2Data())
	{
		DEF_OUT(L"성정값을 저장할 수 없습니다.");
		return;
	}
	CString strPath;
	strPath.Format(_T("%s%s"), FileUtils::GetCurrentModulePath(), FileUtils::GetSettingFileName());
	m_pSetting->Save(strPath);
	CDialogEx::OnOK();
}


void CBuildHelperDlg::OnBnClickedButton1()
{
	JobTypeDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		Job jobNew;
		jobNew.Init(dlg.GetType());
		switch(dlg.GetType())
		{
		case JobBase::EN_JOB_TYPE_FILECOPY:
		{
			JobFileCopyDlg dlg2((FileCopy*)jobNew.GetImpl());
			dlg2.DoModal();
		} break;
		case JobBase::EN_JOB_TYPE_FILEEXECUTE:
		{
			JobFileExecuteDlg dlg2((FileExecute*)jobNew.GetImpl());
			dlg2.DoModal();
		} break;
		case JobBase::EN_JOB_TYPE_FILEBATCH:
		{
			JobBatchDlg dlg2((FileBatch*)jobNew.GetImpl());
			dlg2.DoModal();
		} break;
		default:
			ASSERT(0);
			break;
		}
	}
}


void CBuildHelperDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}
