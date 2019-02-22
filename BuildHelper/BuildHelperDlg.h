
// BuildHelperDlg.h: 헤더 파일
//

#pragma once

class Job;

// CBuildHelperDlg 대화 상자
class CBuildHelperDlg : public CDialogEx
{
// 생성입니다.
public:
	CBuildHelperDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUILDHELPER_DIALOG };
#endif
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	bool Dlg2Data();
	void Data2Dlg();

// 구현입니다.
protected:
	HICON	m_hIcon;
	Job*	m_pSetting = NULL;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
		
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedOk();
};
