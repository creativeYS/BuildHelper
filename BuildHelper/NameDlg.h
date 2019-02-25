#pragma once

class NameDlg : public CDialogEx
{
public:
	NameDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	enum { IDD = IDD_BUILDHELPER_NAME };

	CString GetName() { return m_strName; }
	void	SetName(const CString& strName) { m_strName = strName; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	void data2dlg();
	bool dlg2data();

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	CString		m_strName = L"실행 인자";
public:
	afx_msg void OnBnClickedOk();
};
