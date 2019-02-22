﻿#include "stdafx.h"
#include "JobBatchDlg.h"

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
}

BEGIN_MESSAGE_MAP(JobBatchDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL JobBatchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}