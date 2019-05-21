
// LedDirJudgeDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LedDirJudge.h"
#include "LedDirJudgeDlg.h"
#include "afxdialogex.h"

#include "Main.h"
#include "ImgInsp.h"
#include "Param.h"
#include "SysParam.h"
#include "Cammera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLedDirJudgeDlg ダイアログ




CLedDirJudgeDlg::CLedDirJudgeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLedDirJudgeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// メインクラス生成
	g_pclMain = new CMain();
}

CLedDirJudgeDlg::~CLedDirJudgeDlg()
{
	if (g_pclMain != NULL) {
		delete g_pclMain;
		g_pclMain = NULL;
	}
}

void CLedDirJudgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLedDirJudgeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_EXIT, &CLedDirJudgeDlg::OnFileExit)
	ON_COMMAND(ID_MODE_AUTO, &CLedDirJudgeDlg::OnModeAuto)
	ON_COMMAND(ID_MODE_MANUAL, &CLedDirJudgeDlg::OnModeManual)
	ON_COMMAND(ID_TEST_CAMERA, &CLedDirJudgeDlg::OnTestCamera)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_LOADPARA, &CLedDirJudgeDlg::OnFileLoadpara)
	ON_COMMAND(ID_FILE_SAVEPARA, &CLedDirJudgeDlg::OnFileSavepara)
	ON_COMMAND(ID_TEST_PROCIMAGE, &CLedDirJudgeDlg::OnTestProcimage)
END_MESSAGE_MAP()


// CLedDirJudgeDlg メッセージ ハンドラー
//---------------------------------------------------------------------------
BOOL CLedDirJudgeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	g_pclMain->Init();

	// 自動画面ページ、マニュアル画面ページを作成
	m_clAutoPage.Create(IDD_AUTO_PAGE_DIALOG, this);
	m_clManualPage.Create(IDD_MANUAL_PAGEDIALOG, this);

	CRect    rect;
	GetClientRect(&rect);
	m_clAutoPage.MoveWindow(&rect, FALSE);
	m_clManualPage.MoveWindow(&rect, FALSE);
	OnModeAuto();

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。

	g_pclMain->Close();
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。

	// ダイアログのサイズに合わせて、画像表示を変更する
	CRect rect = CRect(0, 0, cx, cy);
	if (m_clAutoPage.m_hWnd != NULL) {
		m_clAutoPage.MoveWindow(&rect, TRUE);
	}
	if (m_clManualPage.m_hWnd != NULL) {
		m_clManualPage.MoveWindow(&rect, TRUE);
	}
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
//---------------------------------------------------------------------------
// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CLedDirJudgeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnFileLoadpara()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	CFileDialog clsFileDlg(TRUE, NULL, L"test.xml", OFN_FILEMUSTEXIST  | OFN_HIDEREADONLY, L"XMLﾌｧｲﾙ | *.xml|", this);
    if (clsFileDlg.DoModal() != IDOK ){
        return;
    }
	CStringW strFilePath = clsFileDlg.GetPathName();
	if (g_pclParam->Read((WCHAR*)strFilePath.GetString()) == false)
	{
		::MessageBox(NULL, L"PARA FILE READ ERROR!!!", L"ERROR", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		return;
	}
	wcscpy_s(g_pclSysParam->wcParaFilePath, MAX_PATH, strFilePath.GetString());	

	// 露光時間設定
	g_pclCammera->SetExposure(g_pclParam->stSet.stGrab.lExposure);

	// 設定画面更新
	m_clManualPage.ShowPara();
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnFileSavepara()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	CFileDialog clsFileDlg(FALSE, NULL, L"test.xml", OFN_FILEMUSTEXIST  | OFN_HIDEREADONLY, L"XMLﾌｧｲﾙ | *.xml|", this);
    if (clsFileDlg.DoModal() != IDOK ){
        return;
    }
	CStringW strFilePath = clsFileDlg.GetPathName();
	if (g_pclParam->Write((WCHAR*)strFilePath.GetString()) == false)
	{
		::MessageBox(NULL, L"PARA FILE WRITE ERROR!!!", L"ERROR", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		return;
	}
	wcscpy_s(g_pclSysParam->wcParaFilePath, MAX_PATH, strFilePath.GetString());
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnFileExit()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OnOK();
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnModeAuto()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	// 自動画面を表示
	m_clAutoPage.ShowWindow(SW_SHOW);
	m_clManualPage.ShowWindow(SW_HIDE);
	g_pclMain->SetScreen(EN_SCREEN_AUTO);
	// メニューを変更
	CWnd* pWnd = AfxGetMainWnd();
	CMenu* pMenu = pWnd->GetMenu();
	pMenu->EnableMenuItem(ID_MODE_MANUAL, MF_ENABLED);
	pMenu->EnableMenuItem(ID_MODE_AUTO, MF_DISABLED);
	pMenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
	pWnd->DrawMenuBar();
	// 連続撮像を行っていた場合は停止する
	m_clManualPage.StopContinGrab();
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnModeManual()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	if (m_clPassDlg.ShowPassDlg() == false) {
		return;
	}
	// マニュアル画面を表示
	m_clAutoPage.ShowWindow(SW_HIDE);
	m_clManualPage.ShowWindow(SW_SHOW);
	g_pclMain->SetScreen(EN_SCREEN_MANUAL);
	// メニューを変更
	CWnd* pWnd = AfxGetMainWnd();
	CMenu* pMenu = pWnd->GetMenu();
	pMenu->EnableMenuItem(ID_MODE_AUTO, MF_ENABLED);
	pMenu->EnableMenuItem(ID_MODE_MANUAL, MF_DISABLED);
	pMenu->EnableMenuItem(2, MF_BYPOSITION | MF_GRAYED);
	pWnd->DrawMenuBar();
	// 最初は撮像画面表示
	m_clManualPage.m_clsTab.SetCurSel(MANUAL_GRAB_DLG);
	m_clManualPage.OnTcnSelchangeTabManual(0, 0);
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnTestCamera()
{
	// 撮影
	if (g_pclMain->Grab() == false) {
		::MessageBox(NULL, _T("撮影失敗"), _T("撮影失敗"), MB_OK | MB_ICONWARNING);
		return;
	}
	// 撮像画像表示
	g_pclMain->ShowGrabImg();
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnTestProcimage()
{
	// 検査実行
	g_pclMain->ExecInspection();
}
//---------------------------------------------------------------------------
