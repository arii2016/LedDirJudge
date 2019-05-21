// AutoPageDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LedDirJudge.h"
#include "AutoPageDlg.h"
#include "afxdialogex.h"

#include "Main.h"

// CAutoPageDlg ダイアログ

IMPLEMENT_DYNAMIC(CAutoPageDlg, CDialogEx)

CAutoPageDlg::CAutoPageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoPageDlg::IDD, pParent)
{

}

CAutoPageDlg::~CAutoPageDlg()
{
}

void CAutoPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_AUTO_D2D, m_clAutoD2D);
}


BEGIN_MESSAGE_MAP(CAutoPageDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAutoPageDlg メッセージ ハンドラー


BOOL CAutoPageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	g_pclMain->SetAutoD2D(&m_clAutoD2D);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CAutoPageDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
}


void CAutoPageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	CWnd* pD2d;
	CRect rect = CRect(0, 0, cx, cy);

	pD2d = GetDlgItem(IDC_STATIC_AUTO_D2D);
	if (pD2d != NULL) {
		pD2d->MoveWindow(&rect, TRUE);
	}
}

