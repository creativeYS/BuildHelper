#pragma once

class JobTypeDlg : public CDialogEx
{
public:
	JobTypeDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	enum { IDD = IDD_BUILDHELPER_JOB_TYPENAME };

	int		GetType() { return m_enType; }
	CString GetJobName() { return m_strJobName; }
	CString GetJobDesc() { return m_strJobDesc; }
	bool	GetSubJob() { return m_bSubJob; }

	void	SetType(int nType) { m_enType = nType; }
	void	SetJobName(const CString& strName) { m_strJobName = strName; }
	void	SetJobDesc(const CString& strDesc) { m_strJobDesc = strDesc; }
	void	SetSubJob(bool bSub) { m_bSubJob = bSub; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	void data2dlg();
	bool dlg2data();

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	int			m_enType = 0;
	CString		m_strJobName = L"작업";	
	CString		m_strJobDesc = L"";
	bool		m_bSubJob = true;

	CComboBox	m_cbxType;

public:
	afx_msg void OnBnClickedOk();
};
