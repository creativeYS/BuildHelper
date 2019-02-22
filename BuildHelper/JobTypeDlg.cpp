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
}

void JobTypeDlg::data2dlg()
{
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(m_enType == JobBase::EN_JOB_TYPE_FILECOPY ? TRUE : FALSE);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(m_enType == JobBase::EN_JOB_TYPE_FILEEXECUTE ? TRUE : FALSE);
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(m_enType == JobBase::EN_JOB_TYPE_FILEBATCH ? TRUE : FALSE);

	GetDlgItem(IDC_EDIT1)->SetWindowText(m_strJobName);

	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(m_bSubJob ? TRUE : FALSE);
}

bool JobTypeDlg::dlg2data()
{
	m_enType = JobBase::EN_JOB_TYPE_NUMBER;
	if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck()) m_enType = JobBase::EN_JOB_TYPE_FILECOPY;
	else if (((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck()) m_enType = JobBase::EN_JOB_TYPE_FILEEXECUTE;
	else if (((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck()) m_enType = JobBase::EN_JOB_TYPE_FILEBATCH;

	if(m_enType == JobBase::EN_JOB_TYPE_NUMBER) DEF_OUT_RETURN_FALSE(L"작업의 종류를 선택해주세요");

	GetDlgItem(IDC_EDIT1)->GetWindowText(m_strJobName);
	if (m_strJobName.GetLength() <= 0) DEF_OUT_RETURN_FALSE(L"작업 이름을 입력해주세요");

	m_bSubJob = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ? true : false;
	return true;
}

BEGIN_MESSAGE_MAP(JobTypeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &JobTypeDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL JobTypeDlg::OnInitDialog()
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

void JobTypeDlg::OnBnClickedOk()
{
	if (!dlg2data())
	{
		return;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
