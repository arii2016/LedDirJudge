//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "afxcmn.h"
#include "grabpagedlg.h"
#include "PatternPageDlg.h"
#include "judgepagedlg.h"
//---------------------------------------------------------------------------
#define MANUAL_GRAB_DLG			0
#define MANUAL_PATTERN_DLG		1
#define MANUAL_JUDGE_DLG		2
//---------------------------------------------------------------------------
// CManualPageDlg �_�C�A���O
class CManualPageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualPageDlg)

public:
	CManualPageDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CManualPageDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_MANUAL_PAGEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_clsTab;
	CGrabPageDlg m_dlgGrab;
	CPatternPageDlg m_dlgPattern;
	CJudgePageDlg m_dlgJudge;

	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabManual(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	void ShowPara();
	void StopContinGrab();
};
//---------------------------------------------------------------------------
