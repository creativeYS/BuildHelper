#pragma once

class JobComboDlg : public CDialogEx
{
public:
	JobComboDlg(const CString& strFilter, bool bIncludeUnitJob, CWnd* pParent = nullptr);	// 표준 생성자입니다.

	enum { IDD = IDD_BUILDHELPER_SELECT };

	CString GetSelectedName() { return m_strName; }
	void	SetSelectedName(const CString& strName) { m_strName = strName; }

	// 대화상자 타이틀, 그룹, 캡션
	void	SetDlgVal(const TCHAR* pTitle, const TCHAR* pGroup, const TCHAR* pLabel);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	void data2dlg();
	bool dlg2data();
	void InitCtrl();

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	CString		m_strName;
	CString		m_strFilter;
	bool		m_bIncludeUnitJob;

	CString		m_strDlgTitle;
	CString		m_strDlgGroup;
	CString		m_strDlgLabel;
public:
	afx_msg void OnBnClickedOk();
};
