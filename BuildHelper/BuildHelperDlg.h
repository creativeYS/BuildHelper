
// BuildHelperDlg.h: 헤더 파일
//

#pragma once
#include <map>

class Job;
class JobSetting;
struct T_SETTING;

// CBuildHelperDlg 대화 상자
class CBuildHelperDlg : public CDialogEx
{
// 생성입니다.
public:
	CBuildHelperDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	virtual ~CBuildHelperDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUILDHELPER_DIALOG };
#endif
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	bool SetSettingData(const CString& strName = L"");

	void Dlg2DataSub(T_SETTING* pSetting);
	void Data2DlgSub(const T_SETTING* pSetting);

// 구현입니다.
protected:
	HICON			m_hIcon;
	CListCtrl		m_List;
	CComboBox		m_cbxSetting;
	Job*			m_pSettingJob = NULL;
	JobSetting*		m_pSetting = NULL;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
		
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

	void UpdateList();
	void UpdateJobs();
	void UpdateCombo();
	void ClearJobs();

private:
	std::map<CString, Job*> m_Jobs;

public:
	afx_msg void OnBnClickedUpdateEditEnable();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedNewJob();
	afx_msg void OnBnClickedRunJob();
	afx_msg void OnBnClickedDeleteJob();
	afx_msg void OnBnClickedShowSub();
	afx_msg void OnBnClickedJobModify();
	afx_msg void OnEnChangeEditFilter();
	afx_msg void OnBnClickedSaveSetting();
	afx_msg void OnBnClickedDeleteSetting();
	afx_msg void OnCbnSelchangeSettomgCombo();
	afx_msg void OnBnClickedCloneJob();
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
};
