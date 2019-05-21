// PassDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LedDirJudge.h"
#include "PassDlg.h"
#include "afxdialogex.h"
#include "SysParam.h"

// CPassDlg ダイアログ

IMPLEMENT_DYNAMIC(CPassDlg, CDialogEx)

CPassDlg::CPassDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPassDlg::IDD, pParent)
{
}

CPassDlg::~CPassDlg()
{
}

void CPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtPassword);
}


BEGIN_MESSAGE_MAP(CPassDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPassDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPassDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPassDlg メッセージ ハンドラー
//---------------------------------------------------------------------------
BOOL CPassDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
//---------------------------------------------------------------------------
void CPassDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CStringW	strBuf;
	m_edtPassword.GetWindowTextW(strBuf);

	if (wcsncmp(g_pclSysParam->stOhter.wcPassword, strBuf.GetBuffer(), DF_CHAR_MAX) == 0) {
		m_bPassOKFlg = true;
	}

	CDialogEx::OnOK();
}
//---------------------------------------------------------------------------
void CPassDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}
//---------------------------------------------------------------------------
bool CPassDlg::ShowPassDlg()
{
	m_bPassOKFlg = false;

	if (this->DoModal() != IDOK) {
		return false;
	}
	if (m_bPassOKFlg == false) {
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------


