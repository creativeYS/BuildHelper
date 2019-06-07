#include "stdafx.h"
#include "JobTypeDlg.h"
#include "JobBase.h"
#include "OutputControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

JobTypeDlg::JobTypeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(JobTypeDlg::IDD, pParent)
{
}

void JobTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbxType);
}

void JobTypeDlg::data2dlg()
{
	int nSelIndex = -1;
	for (int i = 0; i < m_cbxType.GetCount(); i++)
	{
		int nType = (int)m_cbxType.GetItemData(i);
		if (nType == m_enType) nSelIndex = i;
	}
	m_cbxType.SetCurSel(nSelIndex);

	GetDlgItem(IDC_EDIT1)->SetWindowText(m_strJobName);
	GetDlgItem(IDC_EDIT2)->SetWindowText(m_strJobDesc);

	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(m_bSubJob ? TRUE : FALSE);
}

bool JobTypeDlg::dlg2data()
{
	if(m_cbxType.GetCurSel() < 0) DEF_OUT_RETURN_FALSE(L"작업의 종류를 선택해주세요");
	m_enType = (int)m_cbxType.GetItemData(m_cbxType.GetCurSel());

	GetDlgItem(IDC_EDIT1)->GetWindowText(m_strJobName);
	if (m_strJobName.GetLength() <= 0) DEF_OUT_RETURN_FALSE(L"작업 이름을 입력해주세요");

	if (m_strJobName.FindOneOf(L"\\/:*?\"<>|") >= 0)
		DEF_OUT_RETURN_FALSE(L"사용할 수 없는 문자가 포함되었습니다.");

	GetDlgItem(IDC_EDIT2)->GetWindowText(m_strJobDesc);
	m_bSubJob = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ? true : false;
	return true;
}

BEGIN_MESSAGE_MAP(JobTypeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &JobTypeDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL JobTypeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int i = 0; i < (int)JobBase::EN_JOB_TYPE::EN_JOB_TYPE_NUMBER; i++)
	{
		// 설정은 제외
		if (i == (int)JobBase::EN_JOB_TYPE::EN_JOB_TYPE_JOBSETTING) continue;

		CString strJobTypeName = JobBase::GetJobTypeName(i, true);
		int nIndex = m_cbxType.AddString(strJobTypeName);
		m_cbxType.SetItemData(nIndex, i);
	}

	data2dlg();

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	if (pEdit)
	{
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
	}

	return FALSE;
}

void JobTypeDlg::OnBnClickedOk()
{
	if (!dlg2data())
	{
		return;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
