#pragma once

class SelectJob;
class ProgressDlg : public CDialogEx
{
public:
	ProgressDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	enum { IDD = IDD_BUILDHELPER_PROGRESS };

	// static Functions...
	static ProgressDlg* GetDlgInstance();
	static void			ShowProgress();
	static void			HideProgress();

	static void			SetProgressRange(bool bTotal, int nSum);
	static void			StepProgress(bool bTotal);
	static void			DoneProgress();

	static void			AddString(const CString& strPrompt);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
};
