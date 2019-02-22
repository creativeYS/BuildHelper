#include "stdafx.h"
#include "JobFileCopyDlg.h"

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

BEGIN_MESSAGE_MAP(JobFileCopyDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL JobFileCopyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}