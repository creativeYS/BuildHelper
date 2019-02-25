#include "stdafx.h"
#include "JobFileExecuteDlg.h"
#include "FileExecute.h"
#include "NameDlg.h"

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


	VecStr strName;
	VecStr strParam;
	UINT uiName = 0, uiParam = 0;
	int nCount = 0;
	for (int i = 0; i < 5; i++)
	{
		if (!GetParamNameValueCtrlId(i, uiName, uiParam)) continue;

		if (!GetDlgItem(uiParam)->IsWindowEnabled()) break;
		nCount++;
	}
	for(int i = 0 ; i < nCount; i++)
	{
		if (!GetParamNameValueCtrlId(i, uiName, uiParam)) continue;

		GetDlgItem(uiName)->GetWindowText(strTemp);
		GetName(strTemp, true);
		strName.push_back(strTemp);
		GetDlgItem(uiParam)->GetWindowText(strTemp);
		strParam.push_back(strTemp);
	}
	m_pImpl->SetParam((int)strName.size(), &strName[0], &strParam[0]);
	return true;
}

void JobFileExecuteDlg::data2dlg()
{
	GetDlgItem(IDC_EDIT2)->SetWindowText(m_pImpl->GetExecuteFile());
	VecStr strName;
	VecStr strParam;
	m_pImpl->GetParams(strName, strParam);
	SetEnableCheck((int)strName.size());
	for (int i = 0; i < (int)strName.size(); i++)
	{
		UINT uiCtrlName = 0;
		UINT uiCtrlParam = 0;
		if (!GetParamNameValueCtrlId(i, uiCtrlName, uiCtrlParam)) continue;

		CString strTemp = strName[i];
		GetName(strTemp, false);
		GetDlgItem(uiCtrlName)->SetWindowText(strTemp);
		((CButton*)GetDlgItem(uiCtrlName))->SetCheck(TRUE);
		GetDlgItem(uiCtrlParam)->SetWindowText(strParam[i]);
	}
}

bool JobFileExecuteDlg::GetParamNameValueCtrlId(int nIndex, UINT& uiName, UINT& uiParam)
{
	uiName = 0;
	uiParam = 0;

	switch (nIndex)
	{
	case 0: uiName = IDC_CHECK1; uiParam = IDC_EDIT3; return true;
	case 1: uiName = IDC_CHECK5; uiParam = IDC_EDIT4; return true;
	case 2: uiName = IDC_CHECK6; uiParam = IDC_EDIT5; return true;
	case 3: uiName = IDC_CHECK7; uiParam = IDC_EDIT6; return true;
	case 4: uiName = IDC_CHECK8; uiParam = IDC_EDIT7; return true;
	}
	return false;
}

void JobFileExecuteDlg::SetEnableCheck(int nCount)
{
	int nIndex = 0;
	UINT uiName = 0;
	UINT uiParam = 0;
	while (GetParamNameValueCtrlId(nIndex++, uiName, uiParam))
	{
		GetDlgItem(uiName)->EnableWindow(nIndex - 1 <= nCount ? TRUE : FALSE);
		GetDlgItem(uiParam)->EnableWindow(nIndex <= nCount ? TRUE : FALSE);

		if(nIndex > nCount)
		{
			((CButton*)GetDlgItem(uiName))->SetCheck(FALSE);
		}
	}
}

void JobFileExecuteDlg::OnClickedBtn(int nIdx)
{
	UINT uiName, uiParam;
	if (!GetParamNameValueCtrlId(nIdx, uiName, uiParam)) return;

	if (((CButton*)GetDlgItem(uiName))->GetCheck())
	{
		SetEnableCheck(nIdx + 1);

		NameDlg nameDlg;
		CString strName;
		GetDlgItem(uiName)->GetWindowText(strName);
		GetName(strName, true);
		nameDlg.SetName(strName);
		if (nameDlg.DoModal() == IDOK)
		{
			strName = nameDlg.GetName();
			GetName(strName, false);
			GetDlgItem(uiName)->SetWindowText(strName);
		}
	}
	else
	{
		SetEnableCheck(nIdx);
	}
}

void JobFileExecuteDlg::GetName(CString& strName, bool bRemove)
{
	if (bRemove)	strName.Replace(L" :", L"");
	else			strName.Append(L" :");
}

BEGIN_MESSAGE_MAP(JobFileExecuteDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &JobFileExecuteDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &JobFileExecuteDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &JobFileExecuteDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK5, &JobFileExecuteDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK6, &JobFileExecuteDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK7, &JobFileExecuteDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK8, &JobFileExecuteDlg::OnBnClickedCheck5)
END_MESSAGE_MAP()

BOOL JobFileExecuteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	data2dlg();
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

void JobFileExecuteDlg::OnBnClickedCheck1()
{
	OnClickedBtn(0);
}

void JobFileExecuteDlg::OnBnClickedCheck2()
{
	OnClickedBtn(1);
}

void JobFileExecuteDlg::OnBnClickedCheck3()
{
	OnClickedBtn(2);
}

void JobFileExecuteDlg::OnBnClickedCheck4()
{
	OnClickedBtn(3);
}

void JobFileExecuteDlg::OnBnClickedCheck5()
{
	OnClickedBtn(4);
}