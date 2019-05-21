//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "afxwin.h"
//---------------------------------------------------------------------------
// CPassDlg ダイアログ
class CPassDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPassDlg)

public:
	CPassDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CPassDlg();

// ダイアログ データ
	enum { IDD = IDD_PASSWORD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	bool		m_bPassOKFlg;

public:
	CEdit	m_edtPassword;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	bool ShowPassDlg();
};
//---------------------------------------------------------------------------
