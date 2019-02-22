#pragma once

class FileBatch;
class JobBatchDlg : public CDialogEx
{
public:
	JobBatchDlg(FileBatch* pImpl, CWnd* pParent = nullptr);	// 표준 생성자입니다.

	enum { IDD = IDD_BUILDHELPER_JOB_BATCH };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	FileBatch* m_pImpl = nullptr;
};
