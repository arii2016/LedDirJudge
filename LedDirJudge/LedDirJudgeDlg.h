//---------------------------------------------------------------------------
// LedDirJudgeDlg.h : ヘッダー ファイル
//
//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "autopagedlg.h"
#include "manualpagedlg.h"
#include "PassDlg.h"
#include "Define.h"
//---------------------------------------------------------------------------
// CLedDirJudgeDlg ダイアログ
class CLedDirJudgeDlg : public CDialogEx
{
// コンストラクション
public:
	CLedDirJudgeDlg(CWnd* pParent = NULL);	// 標準コンストラクター
	~CLedDirJudgeDlg();

// ダイアログ データ
	enum { IDD = IDD_LEDDIRJUDGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CAutoPageDlg	m_clAutoPage;
	CManualPageDlg	m_clManualPage;
	CPassDlg		m_clPassDlg;

	afx_msg void OnFileExit();
	afx_msg void OnModeAuto();
	afx_msg void OnModeManual();
	afx_msg void OnTestCamera();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileLoadpara();
	afx_msg void OnFileSavepara();
	afx_msg void OnTestProcimage();
};
//---------------------------------------------------------------------------
