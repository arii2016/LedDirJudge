// ManualPageDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LedDirJudge.h"
#include "ManualPageDlg.h"
#include "afxdialogex.h"


// CManualPageDlg ダイアログ

IMPLEMENT_DYNAMIC(CManualPageDlg, CDialogEx)

CManualPageDlg::CManualPageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualPageDlg::IDD, pParent)
{

}

CManualPageDlg::~CManualPageDlg()
{
}

void CManualPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MANUAL, m_clsTab);
}


BEGIN_MESSAGE_MAP(CManualPageDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MANUAL, &CManualPageDlg::OnTcnSelchangeTabManual)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CManualPageDlg メッセージ ハンドラー
//---------------------------------------------------------------------------
BOOL CManualPageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください

    CRect rect;
        
    // 子ダイアログ作成
    m_dlgGrab.Create(CGrabPageDlg::IDD, this);
    m_dlgPattern.Create(CPatternPageDlg::IDD, this);
    m_dlgJudge.Create(CJudgePageDlg::IDD, this);

    // タブ作成
    m_clsTab.InsertItem(MANUAL_GRAB_DLG, _T("撮像"));
    m_clsTab.InsertItem(MANUAL_PATTERN_DLG, _T("パターン登録"));
    m_clsTab.InsertItem(MANUAL_JUDGE_DLG, _T("判定"));


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
//---------------------------------------------------------------------------
void CManualPageDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
}
//---------------------------------------------------------------------------
void CManualPageDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if (bShow == TRUE) {
		// 初期表示
		OnTcnSelchangeTabManual(0, 0);
		// 設定表示
		ShowPara();
	}
}
//---------------------------------------------------------------------------
void CManualPageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	CWnd* pWnd;
	CRect rect = CRect(0, 0, cx, cy);

	pWnd = GetDlgItem(IDC_TAB_MANUAL);
	if (pWnd != NULL) {
		pWnd->MoveWindow(&rect, TRUE);

		rect.top += 21;
		rect.bottom -= 3;
		rect.right -=3;
		rect.left +=3;
		m_dlgGrab.MoveWindow(&rect, FALSE);
		m_dlgPattern.MoveWindow(&rect, FALSE);
		m_dlgJudge.MoveWindow(&rect, FALSE);
	}
}
//---------------------------------------------------------------------------
void CManualPageDlg::OnTcnSelchangeTabManual(NMHDR *pNMHDR, LRESULT *pResult)
{
    int        sel;
    
    sel = m_clsTab.GetCurSel();
    m_dlgGrab.ShowWindow(sel == MANUAL_GRAB_DLG ? SW_SHOW : SW_HIDE);
    m_dlgPattern.ShowWindow(sel == MANUAL_PATTERN_DLG ? SW_SHOW : SW_HIDE);
    m_dlgJudge.ShowWindow(sel == MANUAL_JUDGE_DLG ? SW_SHOW : SW_HIDE);

	// 連続撮像を停止する
	StopContinGrab();

    if (pResult) *pResult = 0;
}
//---------------------------------------------------------------------------
void CManualPageDlg::ShowPara()
{
	m_dlgGrab.ShowPara();
	m_dlgPattern.ShowPara();
	m_dlgJudge.ShowPara();
}
//---------------------------------------------------------------------------
void CManualPageDlg::StopContinGrab()
{
	// 連続撮像を停止する
	m_dlgGrab.OnBnClickedButtonContinGrabEnd();
}
//---------------------------------------------------------------------------

