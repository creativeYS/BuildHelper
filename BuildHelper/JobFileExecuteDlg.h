#pragma once

class FileExecute;
class JobFileExecuteDlg : public CDialogEx
{
public:
	JobFileExecuteDlg(FileExecute* pImpl, CWnd* pParent = nullptr);	// 표준 생성자입니다.

	enum { IDD = IDD_BUILDHELPER_JOB_FILEEXECUTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	bool dlg2data();
	void data2dlg();

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	FileExecute* m_pImpl = nullptr;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
