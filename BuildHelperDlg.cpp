
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
#include "NameDlg.h"
#include <algorithm>
#include "ProgressDlg.h"

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

CBuildHelperDlg::~CBuildHelperDlg()
{
	ClearJobs();
}

void CBuildHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_COMBO1, m_cbxSetting);
}

bool CBuildHelperDlg::SetSettingData(const CString& strName)
{
	if (m_pSetting == nullptr) { ASSERT(0); return false; }

	VecSetting settings;

	int nCnt = m_pSetting->GetSetting(settings);

	CString strSaveTarget;
	if(strName.GetLength() > 0) strSaveTarget = strName;
	else
	{
		int nIdx = m_cbxSetting.GetCurSel();
		if (nIdx >= 0) m_cbxSetting.GetLBText(nIdx, strSaveTarget);
	}
	if (strSaveTarget.GetLength() <= 0)
	{
		DEF_OUT_RETURN_FALSE(L"저장할 대상을 확인할 수 없습니다.");
	}

	T_SETTING* pSaveTarget = nullptr;
	for (T_SETTING& rSetting : settings)
	{
		if (rSetting.strName.CompareNoCase(strSaveTarget) == 0)
		{
			pSaveTarget = &rSetting;
			break;
		}
	}

	if (pSaveTarget == nullptr)
	{
		DEF_OUT_RETURN_FALSE(L"저장할 대상을 확인할 수 없습니다.");
	}
	
	Dlg2DataSub(pSaveTarget);	
	m_pSetting->SetSetting(settings);

	return true;
}

void CBuildHelperDlg::Dlg2DataSub(T_SETTING* pSetting)
{
	if (pSetting == nullptr) return;

	GetDlgItem(IDC_EDIT1)->GetWindowText(pSetting->strFilter);
	pSetting->nShowSub = (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()) ? 1 : 0;
	pSetting->nUsePgmPath = (((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck()) ? 1 : 0;
	GetDlgItem(IDC_EDIT2)->GetWindowText(pSetting->strWorkingPath);
}

void CBuildHelperDlg::Data2DlgSub(const T_SETTING* pSetting)
{
	if (pSetting == nullptr) return;

	GetDlgItem(IDC_EDIT1)->SetWindowText(pSetting->strFilter);
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(pSetting->nShowSub == 1 ? TRUE : FALSE);
	((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(pSetting->nUsePgmPath == 1 ? TRUE : FALSE);
	GetDlgItem(IDC_EDIT2)->SetWindowText(pSetting->strWorkingPath);

	// check에 따른 edit 박스 업데이트
	OnBnClickedUpdateEditEnable();
}

BEGIN_MESSAGE_MAP(CBuildHelperDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK4, &CBuildHelperDlg::OnBnClickedUpdateEditEnable)
	ON_BN_CLICKED(IDC_BUTTON5, &CBuildHelperDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_BUTTON1, &CBuildHelperDlg::OnBnClickedNewJob)
	ON_BN_CLICKED(IDOK, &CBuildHelperDlg::OnBnClickedRunJob)
	ON_BN_CLICKED(IDC_BUTTON4, &CBuildHelperDlg::OnBnClickedDeleteJob)
	ON_BN_CLICKED(IDC_CHECK1, &CBuildHelperDlg::OnBnClickedShowSub)
	ON_BN_CLICKED(IDC_BUTTON2, &CBuildHelperDlg::OnBnClickedJobModify)
	ON_EN_CHANGE(IDC_EDIT1, &CBuildHelperDlg::OnEnChangeEditFilter)
	ON_BN_CLICKED(IDC_BUTTON6, &CBuildHelperDlg::OnBnClickedSaveSetting)
	ON_BN_CLICKED(IDC_BUTTON7, &CBuildHelperDlg::OnBnClickedDeleteSetting)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CBuildHelperDlg::OnCbnSelchangeSettomgCombo)
	ON_BN_CLICKED(IDC_BUTTON9, &CBuildHelperDlg::OnBnClickedCloneJob)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CBuildHelperDlg::OnNMDblclkList)
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
	
	//List Ctrl 초기화
	m_List.SetExtendedStyle(m_List.GetExtendedStyle()
		| LVS_EX_GRIDLINES
		| LVS_EX_FULLROWSELECT
		| TVS_SHOWSELALWAYS);

	int nWidth = 620;
	m_List.InsertColumn(0, _T("#"), LVCFMT_CENTER, (int)(nWidth * 0.1));
	m_List.InsertColumn(1, _T("작업 이름"), LVCFMT_LEFT, (int)(nWidth * 0.3));
	m_List.InsertColumn(2, _T("작업 종류"), LVCFMT_CENTER, (int)(nWidth * 0.2));
	m_List.InsertColumn(3, _T("설명"), LVCFMT_LEFT, (int)(nWidth * 0.4));

	// 설정 읽기
	static Job gSetting;
	if(m_pSetting == NULL)
	{
		CString strPath;
		strPath.Format(_T("%s%s"), FileUtils::GetSettingPath(), FileUtils::GetSettingFileName());
		if (FileUtils::FileExistOnly(strPath))
		{
			gSetting.Load(strPath);
		}
		if (gSetting.GetImpl() == nullptr ||
			gSetting.GetImpl()->GetType() != JobBase::EN_JOB_TYPE_JOBSETTING)
		{
			gSetting.Init(JobBase::EN_JOB_TYPE_JOBSETTING);
		}

		m_pSetting = (JobSetting*)gSetting.GetImpl();
		m_pSettingJob = &gSetting;
	}

	// combo, list 업데이트
	UpdateCombo();
	const T_SETTING* pSetting = m_pSetting->GetSettingData();
	T_SETTING settingDummy(L"");
	settingDummy.nShowSub = 1;
	Data2DlgSub(pSetting == nullptr ? &settingDummy : pSetting);
	UpdateList();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

BOOL CBuildHelperDlg::PreTranslateMessage(MSG* pMsg)
{
	return CDialogEx::PreTranslateMessage(pMsg);
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



void CBuildHelperDlg::UpdateList()
{
	UpdateJobs();

	bool bShowSub = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ? true : false;

	VecStr strKwd;
	CString strTemp;
	GetDlgItem(IDC_EDIT1)->GetWindowText(strTemp);
	CString token;
	int pos = 0;
	while ((token = strTemp.Tokenize(L" ", pos)) != L"")
	{
		strKwd.push_back(token);
	}

	m_List.DeleteAllItems();
	int nIdx = 0;
	for (auto itr = m_Jobs.begin(); itr != m_Jobs.end(); itr++)
	{
		nIdx++;
		Job* pJob = itr->second;
		if(!bShowSub)
		{
			if (pJob->GetSubJob()) continue;
		}

		CString strItemName = itr->first;
		CString strItemType = JobBase::GetJobTypeName(pJob->GetJobType(), true);
		bool bShow = true;
		if (strKwd.size() > 0)
		{
			bShow = false;
			int nFindCnt = 0;
			for (CString& kwd : strKwd)
			{
				if (strItemName.Find(kwd) >= 0) { nFindCnt++; continue; }
				if (strItemType.Find(kwd) >= 0) { nFindCnt++; continue; }
				int nVal = _wtoi(kwd);
				CString strTemp;
				strTemp.Format(_T("%d"), nVal);
				if(strTemp == kwd && nVal != 0 && nVal == nIdx) { nFindCnt++; continue; }
			}
			if(nFindCnt == (int)strKwd.size())
			{
				bShow = true;
			}
		}
		if (!bShow) continue;

		CString strTemp;
		strTemp.Format(_T("%d"), nIdx);
		int nIndex = m_List.InsertItem(m_List.GetItemCount(), strTemp);
		m_List.SetItemText(nIndex, 1, strItemName);
		m_List.SetItemText(nIndex, 2, strItemType);
		m_List.SetItemText(nIndex, 3, pJob->GetDesc());
		m_List.SetItemData(nIndex, (DWORD_PTR)pJob);
	}
}

void CBuildHelperDlg::UpdateJobs()
{
	ClearJobs();

	CString strPath;
	strPath.Format(L"%s*", FileUtils::GetSettingPath());
	VecStr files;
	FileUtils::FileList(strPath, L"*.job", files);
	for (CString str : files)
	{
		CString strName = FileUtils::GetOnlyFileName(str, false, true);
		Job* pJob = new Job();
		
		pJob->SetJobName(strName);
		if (!pJob->Load(str)) continue;
		
		m_Jobs[strName] = pJob;
	}
}

void CBuildHelperDlg::UpdateCombo()
{
	m_cbxSetting.ResetContent();

	CString strLastSetting = m_pSetting->GetLastSetting();
	VecSetting vecSettings;
	int nCnt = m_pSetting->GetSetting(vecSettings);
	int nIndex = -1;
	VecStr strNames;
	strNames.reserve(nCnt);
	for (int i = 0; i < nCnt; i++)
	{
		strNames.push_back(vecSettings[i].strName);
	}
	std::sort(strNames.begin(), strNames.end(), std::greater<CString>());

	for(int i = 0 ; i < nCnt; i++)
	{
		if (strLastSetting.CompareNoCase(strNames[i]) == 0) nIndex = i;
		m_cbxSetting.AddString(strNames[i]);
	}

	// 자동 정렬이라... 그냥 마지막에 오라고 ㅋㅋㅋ~
	m_cbxSetting.AddString(L"새 설정");

	m_cbxSetting.SetCurSel(nIndex);
}

void CBuildHelperDlg::ClearJobs()
{
	for (auto itr : m_Jobs)
	{
		Job* pJob = itr.second;
		delete pJob;
	}
	m_Jobs.clear();
}

void CBuildHelperDlg::OnBnClickedUpdateEditEnable()
{
	BOOL bEnable = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck() ? FALSE : TRUE;

	GetDlgItem(IDC_EDIT2)->EnableWindow(bEnable);
}


void CBuildHelperDlg::OnBnClickedClose()
{
	CDialogEx::OnOK();
}


void CBuildHelperDlg::OnBnClickedNewJob()
{
	CString strJobTempName;
	static int nIdx = 0;
	while (nIdx++ < 100)
	{
		strJobTempName.Format(L"작업 %02d", nIdx);

		bool bExist = false;
		for (auto itr = m_Jobs.begin(); itr != m_Jobs.end(); itr++)
		{
			CString strJobName = itr->second->GetJobName();
			if (strJobName.CompareNoCase(strJobTempName) == 0)
			{
				bExist = true;
				break;
			}
		}

		if (bExist) continue;
		break;
	}

	JobTypeDlg dlg;
	dlg.SetJobName(strJobTempName);
	if (dlg.DoModal() == IDOK)
	{
		Job jobNew;
		jobNew.Init(dlg.GetType());
		jobNew.SetJobName(dlg.GetJobName());
		jobNew.SetSubJob(dlg.GetSubJob());
		jobNew.SetDesc(dlg.GetJobDesc());
		if (jobNew.DoModal() != IDOK)
		{
			return;
		}

		CString strTemp;
		strTemp.Format(_T("%s%s.job"), FileUtils::GetSettingPath(), dlg.GetJobName());
		jobNew.Save(strTemp);

		UpdateList();
	}
}


void CBuildHelperDlg::OnBnClickedRunJob()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_List.GetSelectedCount() != 1)
	{
		DEF_OUT(L"한번에 한개만 실행할 수 있습니다.\n(연속 실행을 이용하세요.)");
		return;
	}

	POSITION pos = m_List.GetFirstSelectedItemPosition();
	int nIndex = m_List.GetNextSelectedItem(pos);
	Job* pJob = (Job*)m_List.GetItemData(nIndex);
	if(pJob == nullptr)
	{
		DEF_OUT(L"작업을 확인할 수 없습니다.");
	}
	
	CString strWorkingPath;
	if (((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck())
	{
		strWorkingPath = FileUtils::GetCurrentModulePath();
	}
	else
	{
		GetDlgItem(IDC_EDIT2)->GetWindowText(strWorkingPath);
	}

	JobSetting::SetCurrentWorkingPath(strWorkingPath);

	if (OutputControl::Instance()->GetType() == OutputControl::EN_TYPE_MSGBOX)
	{
		OutputControl::Instance()->SetType(OutputControl::EN_TYPE_PROGRESS);
		
		int nTotalCount = pJob->GetTotalCount();
		ProgressDlg::ShowProgress();
		ProgressDlg::SetProgressRange(true, nTotalCount);

		pJob->Run();

		ProgressDlg::DoneProgress();
		OutputControl::Instance()->SetType(OutputControl::EN_TYPE_MSGBOX);
	}
	else
	{
		pJob->Run();
	}

	if (JobSetting::GetUIClosed())
	{
		OnOK();
		return;
	}
}


void CBuildHelperDlg::OnBnClickedDeleteJob()
{
	int nCount = m_List.GetSelectedCount();
	if (nCount == 0)
	{
		DEF_OUT(L"선택된 작업이 없습니다.");
	}
	else
	{
		CString strTemp;
		strTemp.Format(_T("%d개의 작업을 삭제하시겠습니까?"), nCount);
		bool bDelete = false;
		OutputControl::Instance()->ScanIn(strTemp, bDelete);
		if (!bDelete) return;

		int nFailCnt = 0;
		POSITION pos = m_List.GetFirstSelectedItemPosition();
		while (pos)
		{
			int nIndex = m_List.GetNextSelectedItem(pos);
			Job* pJob = (Job*)m_List.GetItemData(nIndex);
			if (pJob->GetLoadedFilePath().GetLength() > 0)
			{
				if (!::DeleteFile(pJob->GetLoadedFilePath())) nFailCnt++;
			}
		}

		if (nFailCnt == 0)	strTemp.Format(_T("선택된 %d개의 작업중 %d개가 삭제되었습니다."), nCount, nCount - nFailCnt);
		else				strTemp.Format(_T("선택된 %d개의 작업이 삭제되었습니다."), nCount);
		DEF_OUT(strTemp);

		UpdateList();
	}
}


void CBuildHelperDlg::OnBnClickedShowSub()
{
	UpdateList();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CBuildHelperDlg::OnBnClickedJobModify()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_List.GetSelectedCount() != 1)
	{
		DEF_OUT(L"한번에 한개만 수정할 수 있습니다.");
		return;
	}

	POSITION pos = m_List.GetFirstSelectedItemPosition();
	int nIndex = m_List.GetNextSelectedItem(pos);
	Job* pJob = (Job*)m_List.GetItemData(nIndex);
	if (pJob == nullptr)
	{
		DEF_OUT(L"작업을 확인할 수 없습니다.");
	}

	JobTypeDlg dlg;
	dlg.SetType(pJob->GetJobType());
	dlg.SetJobName(pJob->GetJobName());
	dlg.SetSubJob(pJob->GetSubJob());
	dlg.SetJobDesc(pJob->GetDesc());
	if (dlg.DoModal() == IDOK)
	{
		Job jobTemp;
		jobTemp.Load(pJob->GetLoadedFilePath());
		if (jobTemp.GetJobType() != dlg.GetType())
		{
			jobTemp.Init(dlg.GetType());
		}
		jobTemp.SetJobName(dlg.GetJobName());
		jobTemp.SetSubJob(dlg.GetSubJob());
		jobTemp.SetDesc(dlg.GetJobDesc());

		if (jobTemp.DoModal() == IDOK)
		{
			::DeleteFile(pJob->GetLoadedFilePath());
			CString strFilePath = pJob->GetLoadedFilePath();
			CString strOrg, strNew;
			strOrg.Format(_T("\\%s."), pJob->GetJobName());
			strNew.Format(_T("\\%s."), dlg.GetJobName());
			strFilePath.Replace(strOrg, strNew);
			jobTemp.Save(strFilePath);
			UpdateList();
		}
	}
}


void CBuildHelperDlg::OnEnChangeEditFilter()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateList();
}

void CBuildHelperDlg::OnBnClickedSaveSetting()
{
	if (!SetSettingData())
	{
		DEF_OUT(L"성정값을 저장할 수 없습니다.");
		return;
	}

	CString strPath;
	strPath.Format(_T("%s%s"), FileUtils::GetSettingPath(), FileUtils::GetSettingFileName());
	m_pSettingJob->Save(strPath);
	DEF_OUT(L"성정을 저장하였습니다.");
}


void CBuildHelperDlg::OnBnClickedDeleteSetting()
{
	VecSetting settings;
	VecSetting settings2;
	m_pSetting->GetSetting(settings);

	settings2.reserve(settings.size());

	CString strTemp;
	m_cbxSetting.GetLBText(m_cbxSetting.GetCurSel(), strTemp);
	for (auto& rSet : settings)
	{
		if (rSet.strName.CompareNoCase(strTemp) == 0) continue;

		settings2.push_back(rSet);
	}
	m_pSetting->SetSetting(settings2);
	CString strCurName = settings2.size() > 0 ? settings2[0].strName : L"";
	m_pSetting->SetLastSetting(strCurName);

	UpdateCombo();
}

void CBuildHelperDlg::OnCbnSelchangeSettomgCombo()
{
	// 마지막 선택시
	if(m_cbxSetting.GetCurSel() == m_cbxSetting .GetCount() - 1)
	{
		VecSetting vecSettings;

		int nCnt = m_pSetting->GetSetting(vecSettings);
		int nIdx = 1;

		CString strName;
		while (true)
		{
			strName.Format(_T("설정 %02d"), nIdx++);

			if (m_pSetting->GetSettingData(strName) == NULL) break;
		}


		NameDlg dlg;
		dlg.SetDlgVal(L"설정 이름", L"설정 이름", L"설정 이름");
		if (strName.GetLength() > 0) dlg.SetName(strName);
		if (dlg.DoModal() == IDOK)
		{
			strName = dlg.GetName();

			T_SETTING rSetting(strName);
			vecSettings.push_back(rSetting);

			m_pSetting->SetSetting(vecSettings);
			m_pSetting->SetLastSetting(strName);

			UpdateCombo();
			Data2DlgSub(&rSetting);
		}
		else
		{
			UpdateCombo();
			Data2DlgSub(m_pSetting->GetSettingData());
		}
	}
	else if(m_pSetting->GetLastSetting().GetLength() > 0) // 바꿀때
	{
		// 이전의 것 저장하고
		SetSettingData(m_pSetting->GetLastSetting());

		// 새로 선택된것으로 바꾼다.
		int nCur = m_cbxSetting.GetCurSel();
		if (nCur >= 0)
		{
			CString strTemp;
			m_cbxSetting.GetLBText(nCur, strTemp);
			
			auto pData = m_pSetting->GetSettingData(strTemp);
			if(pData)
			{
				m_pSetting->SetLastSetting(strTemp);
				Data2DlgSub(pData);
				UpdateList();
			}
		}
	}
}

void CBuildHelperDlg::OnBnClickedCloneJob()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_List.GetSelectedCount() != 1)
	{
		DEF_OUT(L"한번에 한개만 복제할 수 있습니다.");
		return;
	}

	POSITION pos = m_List.GetFirstSelectedItemPosition();
	int nIndex = m_List.GetNextSelectedItem(pos);
	Job* pJob = (Job*)m_List.GetItemData(nIndex);
	if (pJob == nullptr)
	{
		DEF_OUT(L"작업을 확인할 수 없습니다.");
	}

	CString strFilePath = pJob->GetLoadedFilePath();
	CString strOrg, strNew;
	strOrg.Format(_T("\\%s."), pJob->GetJobName());
	strNew.Format(_T("\\%s."), pJob->GetJobName() + L"_Copy");
	strFilePath.Replace(strOrg, strNew);
	::CopyFile(pJob->GetLoadedFilePath(), strFilePath, FALSE);

	Job jobTemp;
	if(jobTemp.Load(strFilePath))
	{
		UpdateList();
	}
}

void CBuildHelperDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{	
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_List.GetSelectedCount() == 1)
	{
		OnBnClickedRunJob();
	}

	*pResult = 0;
}
