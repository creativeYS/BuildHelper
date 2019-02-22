#include "stdafx.h"
#include "JobFileCopyDlg.h"
#include "FileCopy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

JobFileCopyDlg::JobFileCopyDlg(FileCopy* pImpl, CWnd* pParent /*=nullptr*/)
	: CDialogEx(JobFileCopyDlg::IDD, pParent), m_pImpl(pImpl)
{
}

void JobFileCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

bool JobFileCopyDlg::dlg2data()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT2)->GetWindowText(strTemp);
	m_pImpl->SetSourcePath(strTemp);
	GetDlgItem(IDC_EDIT3)->GetWindowText(strTemp);
	m_pImpl->SetDestPath(strTemp);
	GetDlgItem(IDC_EDIT4)->GetWindowText(strTemp);
	m_pImpl->SetTargetFilter(strTemp);
	m_pImpl->SetIncludeSubFolder(((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() ? true : false);
	return true;
}

void JobFileCopyDlg::data2dlg()
{
	GetDlgItem(IDC_EDIT2)->SetWindowText(m_pImpl->GetSourcePath());
	GetDlgItem(IDC_EDIT3)->SetWindowText(m_pImpl->GetDestPath());
	GetDlgItem(IDC_EDIT4)->SetWindowText(m_pImpl->GetTargetFilter());
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(m_pImpl->GetIncludeSubFolder() ? TRUE : FALSE);
}

BEGIN_MESSAGE_MAP(JobFileCopyDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &JobFileCopyDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL JobFileCopyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	data2dlg();

	return TRUE;
}

void JobFileCopyDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	dlg2data();
	CDialogEx::OnOK();
}
