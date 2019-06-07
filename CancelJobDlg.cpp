#include "stdafx.h"
#include "CancelJobDlg.h"
#include "CancelJob.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CancelJobDlg::CancelJobDlg(CancelJob* pImpl, CWnd* pParent /*=nullptr*/)
	: CDialogEx(CancelJobDlg::IDD, pParent), m_pImpl(pImpl)
{
}

void CancelJobDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

bool CancelJobDlg::dlg2data()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT2)->GetWindowText(strTemp);
	m_pImpl->SetMessage(strTemp);

	return true;
}

void CancelJobDlg::data2dlg()
{
	GetDlgItem(IDC_EDIT2)->SetWindowText(m_pImpl->GetMessage());
}

BEGIN_MESSAGE_MAP(CancelJobDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CancelJobDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CancelJobDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	data2dlg();

	return TRUE;
}

void CancelJobDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	dlg2data();
	CDialogEx::OnOK();
}