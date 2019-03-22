#pragma once

class CancelJob;
class CancelJobDlg : public CDialogEx
{
public:
	CancelJobDlg(CancelJob* pImpl, CWnd* pParent = nullptr);	// 표준 생성자입니다.

	enum { IDD = IDD_BUILDHELPER_JOB_CANCELJOB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	bool dlg2data();
	void data2dlg();
protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	CancelJob* m_pImpl = nullptr;
	CComboBox m_cbxType;
public:
	afx_msg void OnBnClickedOk();
};
