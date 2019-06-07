#pragma once

class FileBatch;
class JobBatchDlg : public CDialogEx
{
public:
	JobBatchDlg(FileBatch* pImpl, CWnd* pParent = nullptr);	// 표준 생성자입니다.

	enum { IDD = IDD_BUILDHELPER_JOB_BATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	bool dlg2data();
	void data2dlg();

	void UpdateListAll();
	void UpdateListTarget();

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	FileBatch*	m_pImpl = nullptr;
	CListBox	m_ListAll;
	CListBox	m_ListTarget;
public:
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedCheck3();
};
