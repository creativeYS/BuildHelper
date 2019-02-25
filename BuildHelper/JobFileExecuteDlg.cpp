#include "stdafx.h"
#include "JobFileExecuteDlg.h"
#include "FileExecute.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

JobFileExecuteDlg::JobFileExecuteDlg(FileExecute* pImpl, CWnd* pParent /*=nullptr*/)
	: CDialogEx(JobFileExecuteDlg::IDD, pParent), m_pImpl(pImpl)
{
}

void JobFileExecuteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

bool JobFileExecuteDlg::dlg2data()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT2)->GetWindowText(strTemp);
	m_pImpl->SetExecuteFile(strTemp);
	GetDlgItem(IDC_EDIT3)->GetWindowText(strTemp);
	m_pImpl->SetParam(strTemp);
	return true;
}

void JobFileExecuteDlg::data2dlg()
{
	GetDlgItem(IDC_EDIT2)->SetWindowText(m_pImpl->GetExecuteFile());
	GetDlgItem(IDC_EDIT3)->SetWindowText(m_pImpl->GetParam());
}

BEGIN_MESSAGE_MAP(JobFileExecuteDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &JobFileExecuteDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &JobFileExecuteDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL JobFileExecuteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

void JobFileExecuteDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	dlg2data();
	CDialogEx::OnOK();
}


void JobFileExecuteDlg::OnBnClickedButton1()
{
	FileUtils::OnBrowseFolder(this, IDC_EDIT2, true);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
