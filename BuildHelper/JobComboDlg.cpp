#include "stdafx.h"
#include "JobComboDlg.h"
#include "JobBase.h"
#include "OutputControl.h"
#include "FileUtils.h"
#include "Job.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

JobComboDlg::JobComboDlg(const CString& strFilter, bool bIncludeUnitJob, CWnd* pParent)
	: CDialogEx(JobComboDlg::IDD, pParent), m_strFilter(strFilter), m_bIncludeUnitJob(bIncludeUnitJob)
{
}

void JobComboDlg::SetDlgVal(const TCHAR* pTitle, const TCHAR* pGroup, const TCHAR* pLabel)
{
	m_strDlgTitle = pTitle;
	m_strDlgGroup = pGroup;
	m_strDlgLabel = pLabel;
}

void JobComboDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void JobComboDlg::data2dlg()
{
	if (m_strDlgTitle.GetLength() > 0) SetWindowText(m_strDlgTitle);
	if (m_strDlgGroup.GetLength() > 0) GetDlgItem(IDC_STATIC3)->SetWindowText(m_strDlgGroup);
	if (m_strDlgLabel.GetLength() > 0) GetDlgItem(IDC_STATIC2)->SetWindowText(m_strDlgLabel);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
	if (pCombo == nullptr) {
		ASSERT(0);  return;
	}

	for (int i = 0; i < (int)pCombo->GetCount(); i++)
	{
		CString strText;
		pCombo->GetLBText(i, strText);

		if (strText.GetLength() <= 0) continue;
		if (strText.CompareNoCase(m_strName) == 0)
		{
			pCombo->SetCurSel(i);
			break;
		}
	}
}

bool JobComboDlg::dlg2data()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
	if (pCombo == nullptr) return false;
	if (pCombo->GetCurSel() < 0) return false;

	pCombo->GetLBText(pCombo->GetCurSel(), m_strName);
	return true;
}

void JobComboDlg::InitCtrl()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
	if (pCombo == nullptr) return;

	m_strFilter.MakeLower();

	CString strPath;
	strPath.Format(L"%s*", FileUtils::GetSettingPath());
	VecStr files;
	FileUtils::FileList(strPath, L"*.job", files);
	for (CString str : files)
	{
		CString strName = FileUtils::GetOnlyFileName(str, false, true);
		if (!m_bIncludeUnitJob)
		{
			Job jobTemp;
			if (jobTemp.Load(str))
			{
				if (jobTemp.GetSubJob()) continue;
			}
		}
		bool bContinue = true;
		if (m_strFilter == L"*" || m_strFilter.GetLength() <= 0)
			bContinue = false;
		else
		{
			CString strNameLower = strName;
			strNameLower.MakeLower();

			int nFind = 0;
			int nTotal = 0;
			CString token;
			int pos = 0;
			while ((token = m_strFilter.Tokenize(L" ", pos)) != L"")
			{
				nTotal++;
				if (strNameLower.Find(token) >= 0) nFind++;
			}

			if (nFind == nTotal) bContinue = false;
		}

		if (bContinue) continue;
		pCombo->AddString(strName);
	}
	if(files.size() > 0) pCombo->SetCurSel(0);
}

BEGIN_MESSAGE_MAP(JobComboDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &JobComboDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL JobComboDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitCtrl();
	data2dlg();

	return FALSE;
}

void JobComboDlg::OnBnClickedOk()
{
	if (!dlg2data())
	{
		AfxMessageBox(L"Select Job");
		return;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
