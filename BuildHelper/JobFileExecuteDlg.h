﻿#pragma once

class JobFileExecuteDlg : public CDialogEx
{
public:
	JobFileExecuteDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	enum { IDD = IDD_BUILDHELPER_JOB_FILEEXECUTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
