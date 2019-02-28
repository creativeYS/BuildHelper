#include "stdafx.h"
#include "JobCreateFileListDlg.h"
#include "CreateFileList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

JobCreateFileListDlg::JobCreateFileListDlg(CreateFileList* pImpl, CWnd* pParent /*=nullptr*/)
	: CDialogEx(JobCreateFileListDlg::IDD, pParent), m_pImpl(pImpl)
{
}

void JobCreateFileListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

bool JobCreateFileListDlg::dlg2data()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT2)->GetWindowText(strTemp);
	m_pImpl->SetTargetPath(strTemp);
	GetDlgItem(IDC_EDIT4)->GetWindowText(strTemp);
	m_pImpl->SetTargetFilter(strTemp);
	m_pImpl->SetIncludeSubFolder(((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() ? true : false);
	return true;
}

void JobCreateFileListDlg::data2dlg()
{
	GetDlgItem(IDC_EDIT2)->SetWindowText(m_pImpl->GetTargetPath());
	GetDlgItem(IDC_EDIT4)->SetWindowText(m_pImpl->GetTargetFilter());
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(m_pImpl->GetIncludeSubFolder() ? TRUE : FALSE);
}

BEGIN_MESSAGE_MAP(JobCreateFileListDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &JobCreateFileListDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &JobCreateFileListDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL JobCreateFileListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	data2dlg();

	return TRUE;
}

void JobCreateFileListDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	dlg2data();
	CDialogEx::OnOK();
}

void JobCreateFileListDlg::OnBnClickedButton1()
{
	FileUtils::OnBrowseFolder(GetDlgItem(IDC_EDIT2), FileUtils::GetCurrentModulePath());
}