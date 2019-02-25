﻿#include "stdafx.h"
#include "NameDlg.h"
#include "JobBase.h"
#include "OutputControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NameDlg::NameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(NameDlg::IDD, pParent)
{
}

void NameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void NameDlg::data2dlg()
{
	GetDlgItem(IDC_EDIT1)->SetWindowText(m_strName);
}

bool NameDlg::dlg2data()
{
	GetDlgItem(IDC_EDIT1)->GetWindowText(m_strName);
	return true;
}

BEGIN_MESSAGE_MAP(NameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &NameDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL NameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	data2dlg();

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	if (pEdit)
	{
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
	}

	return FALSE;
}

void NameDlg::OnBnClickedOk()
{
	if (!dlg2data())
	{
		return;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
