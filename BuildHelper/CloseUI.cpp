
// BuildHelperDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "CloseUI.h"
#include "OutputControl.h"
#include "Job.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool CloseUI::Run()
{
// 	int nSuccessCnt = 0;
// 	int nTotalCnt = (int)m_vecTargetJobs.size();
// 	if (nTotalCnt == 0)
// 	{
// 		DEF_OUT_RETURN_FALSE(L"작업을 확인할 수 없습니다.");
// 	}
// 
// 	CString strPath;
// 	strPath.Format(L"%s*", FileUtils::GetSettingPath());
// 	VecStr files;
// 	FileUtils::FileList(strPath, L"*.job", files);
// 
// 	for (const CString& str : m_vecTargetJobs)
// 	{
// 		CString strJobPath;
// 		strJobPath.Format(L"%s%s%s", FileUtils::GetSettingPath(), str, L".job");
// 
// 		Job tempJob;
// 		if (!tempJob.Load(strJobPath)) continue;
// 
// 		if (tempJob.Run()) nSuccessCnt++;
// 	}
// 	if(nSuccessCnt == nTotalCnt)
// 	{
// 		CString strTemp;
// 		strTemp.Format(L"%d개의 작업을 모두 완료하였습니다.", nTotalCnt);
// 		DEF_OUT(strTemp);
// 	}
// 	else
// 	{
// 		CString strTemp;
// 		strTemp.Format(L"%d개의 작업 중 %d개가 실패하였습니다.", nTotalCnt, nTotalCnt - nSuccessCnt);
// 		DEF_OUT(strTemp);
// 	}

	return true;
}

bool CloseUI::Load(FILE* pFile)
{
// 	int nCnt = rdInt(pFile);
// 	m_vecTargetJobs.clear();
// 	m_vecTargetJobs.reserve(nCnt);
// 	for(int i = 0 ; i < nCnt; i++)
// 	{
// 		m_vecTargetJobs.push_back(rdString(pFile));
// 	}
	return true;
}

bool CloseUI::Save(FILE* pFile)
{
// 	int nCnt = (int)m_vecTargetJobs.size();
// 	wrInt(pFile, nCnt);
// 	for(int i = 0 ; i < nCnt; i++)
// 	{
// 		wrString(pFile, m_vecTargetJobs[i]);
// 	}
	return true;
}