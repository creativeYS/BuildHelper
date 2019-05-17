#include "stdafx.h"
#include "SelectJobDlg.h"
#include "SelectJob.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SelectJobDlg::SelectJobDlg(SelectJob* pImpl, CWnd* pParent /*=nullptr*/)
	: CDialogEx(SelectJobDlg::IDD, pParent), m_pImpl(pImpl)
{
}

void SelectJobDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

bool SelectJobDlg::dlg2data()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT4)->GetWindowText(strTemp);
	m_pImpl->SetFilter(strTemp);
	m_pImpl->SetIncludeUnitJob(((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() ? true : false);
	return true;
}

void SelectJobDlg::data2dlg()
{
	GetDlgItem(IDC_EDIT4)->SetWindowText(m_pImpl->GetFilter());
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(m_pImpl->GetIncludeUnitJob() ? TRUE : FALSE);
}

BEGIN_MESSAGE_MAP(SelectJobDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SelectJobDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &SelectJobDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &SelectJobDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL SelectJobDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	data2dlg();

	return TRUE;
}

void SelectJobDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	dlg2data();
	CDialogEx::OnOK();
}

void SelectJobDlg::OnBnClickedButton1()
{
	FileUtils::OnBrowseFolder(GetDlgItem(IDC_EDIT2), FileUtils::GetCurrentModulePath());
}


void SelectJobDlg::OnBnClickedButton2()
{
	FileUtils::OnBrowseFolder(GetDlgItem(IDC_EDIT3), FileUtils::GetCurrentModulePath());
}