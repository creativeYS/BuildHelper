#include "stdafx.h"
#include "ProgressDlg.h"
#include "SelectJob.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ProgressDlg::ProgressDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(ProgressDlg::IDD, pParent)
{
}

ProgressDlg* ProgressDlg::GetDlgInstance()
{
	static ProgressDlg* s_pDlg = nullptr;
	if (s_pDlg == nullptr)
	{
		s_pDlg = new ProgressDlg();
	}
	return s_pDlg;
}

void ProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ProgressDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL ProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

void ProgressDlg::ShowProgress()
{
	if (!IsWindow(GetDlgInstance()->m_hWnd))
	{
		GetDlgInstance()->Create(ProgressDlg::IDD, AfxGetMainWnd());
	}
		
	GetDlgInstance()->GetDlgItem(IDC_EDIT8)->SetWindowText(L"");
	GetDlgInstance()->ShowWindow(SW_SHOW);
	GetDlgInstance()->GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgInstance()->GetDlgItem(IDOK)->RedrawWindow();

	MSG msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	}
}

void ProgressDlg::HideProgress()
{
	if (!IsWindow(GetDlgInstance()->m_hWnd)) return;

	GetDlgInstance()->ShowWindow(SW_HIDE);
}

void ProgressDlg::SetProgressRange(bool bTotal, int nSum)
{
	if (!IsWindow(GetDlgInstance()->m_hWnd)) return;

	if (!bTotal && nSum < 10) return;

	CProgressCtrl* pProgress = bTotal ? (CProgressCtrl*)GetDlgInstance()->GetDlgItem(IDC_PROGRESS1) :
		(CProgressCtrl*)GetDlgInstance()->GetDlgItem(IDC_PROGRESS2);

	pProgress->SetRange(0, nSum);
}

void ProgressDlg::StepProgress(bool bTotal)
{
	if (!IsWindow(GetDlgInstance()->m_hWnd)) return;

	CProgressCtrl* pProgress = bTotal ? (CProgressCtrl*)GetDlgInstance()->GetDlgItem(IDC_PROGRESS1) :
		(CProgressCtrl*)GetDlgInstance()->GetDlgItem(IDC_PROGRESS2);

	pProgress->SetPos(pProgress->GetPos() + 1);

	MSG msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	}
}

void ProgressDlg::DoneProgress()
{
	if (!IsWindow(GetDlgInstance()->m_hWnd)) return;

	GetDlgInstance()->AddString(L"");
	GetDlgInstance()->AddString(L"*** 작업이 완료되었습니다 ***");
	GetDlgInstance()->GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void ProgressDlg::AddString(const CString& strPrompt)
{
	if (!IsWindow(GetDlgInstance()->m_hWnd)) return;

	CString strTemp;
	GetDlgInstance()->GetDlgItem(IDC_EDIT8)->GetWindowText(strTemp);
	CString strNew;
	if (strTemp.GetLength() > 0)
		strNew.Format(_T("%s\r\n%s"), strTemp, strPrompt);
	else
		strNew = strPrompt;
	GetDlgInstance()->GetDlgItem(IDC_EDIT8)->SetWindowText(strNew);

	CEdit* pEdit = (CEdit*)GetDlgInstance()->GetDlgItem(IDC_EDIT8);
	pEdit->SetSel(-1);

}
