#include "stdafx.h"
#include "JobBatchDlg.h"
#include "FileUtils.h"
#include "FileBatch.h"
#include "Job.h"
#include "OutputControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

JobBatchDlg::JobBatchDlg(FileBatch* pImpl, CWnd* pParent /*=nullptr*/)
	: CDialogEx(JobBatchDlg::IDD, pParent) , m_pImpl(pImpl)
{
}

void JobBatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListAll);
	DDX_Control(pDX, IDC_LIST2, m_ListTarget);
}

bool JobBatchDlg::dlg2data()
{
	return true;
}

void JobBatchDlg::data2dlg()
{
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(TRUE);

	UpdateListAll();
	UpdateListTarget();
}

BEGIN_MESSAGE_MAP(JobBatchDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &JobBatchDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &JobBatchDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, &JobBatchDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, &JobBatchDlg::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON8, &JobBatchDlg::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_CHECK3, &JobBatchDlg::OnBnClickedCheck3)
END_MESSAGE_MAP()

void JobBatchDlg::UpdateListAll()
{
	bool bSubOnly = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck() ? true : false;

	m_ListAll.ResetContent();
	CString strPath;
	strPath.Format(L"%s*", FileUtils::GetSettingPath());
	VecStr files;
	FileUtils::FileList(strPath, L"*.job", files);
	for (CString str : files)
	{
		if (bSubOnly)
		{
			Job jobTemp;
			jobTemp.Load(str);
			if (!jobTemp.GetSubJob()) continue;
		}

		CString strName = FileUtils::GetOnlyFileName(str, false, true);
		m_ListAll.AddString(strName);
	}
}

void JobBatchDlg::UpdateListTarget()
{
	m_ListTarget.ResetContent();
	VecStr strJobs;
	m_pImpl->GetJobs(strJobs);
	for (CString& str : strJobs)
	{
		m_ListTarget.AddString(str);
	}
}

BOOL JobBatchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	data2dlg();

	return TRUE;
}

void JobBatchDlg::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.int count = m_listBox.GetSelCount();

	int lp[1025];
	int count2 = m_ListAll.GetSelItems(1024, lp);
	for(int i = 0 ; i < count2; i++)
	{
		CString strTemp;
		m_ListAll.GetText(lp[i], strTemp);
		if (strTemp.GetLength() <= 0) continue;
		m_pImpl->AddJob(strTemp);
	}
	UpdateListTarget();
}

void JobBatchDlg::OnBnClickedButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pImpl->SetJobs(0, NULL);

	UpdateListTarget();
}


void JobBatchDlg::OnBnClickedButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.int count = m_listBox.GetSelCount();
	VecStr jobs;
	m_pImpl->GetJobs(jobs);

	int lp[1025];
	int count2 = m_ListTarget.GetSelItems(1024, lp);

	if(jobs.size() == count2)
	{
		m_pImpl->SetJobs(0, nullptr);
		return;
	}

	std::vector<bool> vecDelFlag;
	vecDelFlag.reserve(jobs.size());
	int nSelIdx = 0;
	for (int i = 0; i < (int)jobs.size(); i++)
	{
		vecDelFlag.push_back(false);
	}

	for (int i = 0; i < count2; i++)
	{
		vecDelFlag[lp[i]] = true;
	}

	VecStr newJobs;
	newJobs.reserve(jobs.size());
	for (int i = 0; i < (int)jobs.size(); i++)
	{
		if (vecDelFlag[i]) continue;
		newJobs.push_back(jobs[i]);
	}
	m_pImpl->SetJobs((int)newJobs.size(), &newJobs[0]);

	UpdateListTarget();
}


void JobBatchDlg::OnBnClickedButtonUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int lp[1025];
	int count2 = m_ListTarget.GetSelItems(1024, lp);
	if (count2 == 0) return;

	VecStr jobs;
	m_pImpl->GetJobs(jobs);

	if (jobs.size() == 0) return;
	if (lp[0] == 0) { DEF_OUT(L"올릴 수 없습니다.") return; }


	for (int i = 0; i < count2; i++)
	{
		CString strTemp = jobs[lp[i] - 1];
		jobs[lp[i] - 1] = jobs[lp[i]];
		jobs[lp[i]] = strTemp;
	}
	m_pImpl->SetJobs((int)jobs.size(), &jobs[0]);

	UpdateListTarget();
	for (int i = 0; i < count2; i++)
	{
		m_ListTarget.SetSel(lp[i] - 1);
	}
}


void JobBatchDlg::OnBnClickedButtonDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int lp[1025];
	int count2 = m_ListTarget.GetSelItems(1024, lp);
	if (count2 == 0) return;

	VecStr jobs;
	m_pImpl->GetJobs(jobs);

	if (jobs.size() == 0) return;
	if (lp[count2 - 1] == jobs.size() - 1) { DEF_OUT(L"내릴 수 없습니다.") return; }

	for (int i = count2 - 1; i >= 0; i--)
	{
		CString strTemp = jobs[lp[i] + 1];
		jobs[lp[i] + 1] = jobs[lp[i]];
		jobs[lp[i]] = strTemp;
	}
	m_pImpl->SetJobs((int)jobs.size(), &jobs[0]);

	UpdateListTarget();
	for (int i = count2 - 1; i >= 0; i--)
	{
		m_ListTarget.SetSel(lp[i] + 1);
	}
}


void JobBatchDlg::OnBnClickedCheck3()
{
	UpdateListAll();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
