#include "stdafx.h"
#include "CopyFileSolutionDlg.h"
#include "CopyFileSolution.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CopyFileSolutionDlg::CopyFileSolutionDlg(CopyFileSolution* pImpl, CWnd* pParent /*=nullptr*/)
	: CDialogEx(CopyFileSolutionDlg::IDD, pParent), m_pImpl(pImpl)
{
}

void CopyFileSolutionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbxType);
}

bool CopyFileSolutionDlg::dlg2data()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT2)->GetWindowText(strTemp);
	m_pImpl->SetTargetSolution(strTemp);
	GetDlgItem(IDC_EDIT3)->GetWindowText(strTemp);
	m_pImpl->SetDestCopyPath(strTemp);
	if (m_cbxType.GetCurSel() >= 0)
	{
		int nType = (int)m_cbxType.GetItemData(m_cbxType.GetCurSel());
		m_pImpl->SetCopyFileType((CopyFileSolution::EN_COPYFILETYPE)nType);
	}

	GetDlgItem(IDC_EDIT4)->GetWindowText(strTemp);
	CString token;
	int pos = 0;
	VecStr strKeys;
	VecStr strVals;
	while ((token = strTemp.Tokenize(L";", pos)) != L"")
	{
		CString strKey = token.Left(token.Find(L"="));
		CString strVal = token.Mid(token.Find(L"=") + 1);
		strKeys.push_back(strKey);
		strVals.push_back(strVal);
	}
	m_pImpl->SetOptions(strKeys, strVals);

	return true;
}

void CopyFileSolutionDlg::data2dlg()
{
	GetDlgItem(IDC_EDIT2)->SetWindowText(m_pImpl->GetTargetSolution());
	GetDlgItem(IDC_EDIT3)->SetWindowText(m_pImpl->GetDestCopyPath());
	int nType = (int)m_pImpl->GetCopyFileType();
	for (int i = 0; i < m_cbxType.GetCount(); i++)
	{
		if (m_cbxType.GetItemData(i) == nType)
		{
			m_cbxType.SetCurSel(i);
			break;
		}
	}
	VecStr strKeys;
	VecStr strVals;
	CString strOptions;
	int nSize = m_pImpl->GetOptions(strKeys, strVals);
	for (int i = 0; i < nSize; i++)
	{
		CString strParam;
		strParam.Format(_T("%s%s=%s;"), strOptions, strKeys[i], strVals[i]);
		strOptions = strParam;
	}
	GetDlgItem(IDC_EDIT4)->SetWindowText(strOptions);
}

BEGIN_MESSAGE_MAP(CopyFileSolutionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CopyFileSolutionDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CopyFileSolutionDlg::OnBnClickedButtonSolutionFile)
	ON_BN_CLICKED(IDC_BUTTON2, &CopyFileSolutionDlg::OnBnClickedButtonDestPath)
END_MESSAGE_MAP()

BOOL CopyFileSolutionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (auto i = 0; i < CopyFileSolution::NUMBER; i++)
	{
		int nIdx = m_cbxType.AddString(CopyFileSolution::GetTypeName((CopyFileSolution::EN_COPYFILETYPE)i));
		m_cbxType.SetItemData(nIdx, i);
	}

	data2dlg();

	return TRUE;
}

void CopyFileSolutionDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	dlg2data();
	CDialogEx::OnOK();
}

void CopyFileSolutionDlg::OnBnClickedButtonSolutionFile()
{
	FileUtils::OnBrowseFolder(GetDlgItem(IDC_EDIT2), FileUtils::GetCurrentModulePath(), true, _T("Solution File(*.sln)|*.sln|All Files(*.*)|*.*||"));
}

void CopyFileSolutionDlg::OnBnClickedButtonDestPath()
{
	FileUtils::OnBrowseFolder(GetDlgItem(IDC_EDIT3), FileUtils::GetCurrentModulePath());
}
