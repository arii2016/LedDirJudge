//---------------------------------------------------------------------------
#pragma once
#include "D2DAutoCtrl.h"
#include "Define.h"
//---------------------------------------------------------------------------
// CAutoPageDlg ダイアログ
class CAutoPageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoPageDlg)

public:
	CAutoPageDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CAutoPageDlg();

// ダイアログ データ
	enum { IDD = IDD_AUTO_PAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();

	CD2DAutoCtrl m_clAutoD2D;
};
//---------------------------------------------------------------------------
