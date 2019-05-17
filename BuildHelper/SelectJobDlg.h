#pragma once

class SelectJob;
class SelectJobDlg : public CDialogEx
{
public:
	SelectJobDlg(SelectJob* pImpl, CWnd* pParent = nullptr);	// 표준 생성자입니다.

	enum { IDD = IDD_BUILDHELPER_JOB_SELECTJOB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	bool dlg2data();
	void data2dlg();
protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	SelectJob* m_pImpl = nullptr;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
