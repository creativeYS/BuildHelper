#include "stdafx.h"
#include "JobFileExecuteDlg.h"

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

BEGIN_MESSAGE_MAP(JobFileExecuteDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL JobFileExecuteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}