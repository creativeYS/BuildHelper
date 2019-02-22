#pragma once

class JobFileCopyDlg : public CDialogEx
{
public:
	JobFileCopyDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	enum { IDD = IDD_BUILDHELPER_JOB_FILECOPY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
