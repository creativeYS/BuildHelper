#pragma once

class FileCopy;
class JobFileCopyDlg : public CDialogEx
{
public:
	JobFileCopyDlg(FileCopy* pImpl, CWnd* pParent = nullptr);	// 표준 생성자입니다.

	enum { IDD = IDD_BUILDHELPER_JOB_FILECOPY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	bool dlg2data();
	void data2dlg();
protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	FileCopy* m_pImpl = nullptr;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
