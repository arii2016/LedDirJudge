// GrabPageDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LedDirJudge.h"
#include "GrabPageDlg.h"
#include "afxdialogex.h"
#include "Main.h"
#include "Cammera.h"
#include "Param.h"

// CGrabPageDlg ダイアログ

IMPLEMENT_DYNAMIC(CGrabPageDlg, CDialogEx)

CGrabPageDlg::CGrabPageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGrabPageDlg::IDD, pParent)
{
}

CGrabPageDlg::~CGrabPageDlg()
{
}

void CGrabPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GRAB_IMG, m_pcGrabImg);
	DDX_Control(pDX, IDC_EDIT_EXPOSURE, m_edtExposure);
}


BEGIN_MESSAGE_MAP(CGrabPageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GRAB, &CGrabPageDlg::OnBnClickedButtonGrab)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_IMG, &CGrabPageDlg::OnBnClickedButtonLoadImg)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_IMG, &CGrabPageDlg::OnBnClickedButtonSaveImg)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_EDIT_EXPOSURE, &CGrabPageDlg::OnEnKillfocusEditExposure)
	ON_BN_CLICKED(IDC_BUTTON_CONTIN_GRAB_START, &CGrabPageDlg::OnBnClickedButtonContinGrabStart)
	ON_BN_CLICKED(IDC_BUTTON_CONTIN_GRAB_END, &CGrabPageDlg::OnBnClickedButtonContinGrabEnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGrabPageDlg メッセージ ハンドラー
//---------------------------------------------------------------------------
BOOL CGrabPageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	
	// 画像サイズ取得
	m_lWidth = g_pclCammera->Width();
	m_lHeight = g_pclCammera->Height();

	// Picture Controlのデバイスコンテキスト取得
	m_clsPicDC = m_pcGrabImg.GetDC();

	// 画像サイズ設定
	CRect rect;
	rect.top = 5;
	rect.left = 5;
	rect.right = rect.left + m_lWidth;
	rect.bottom = rect.top + m_lHeight;
	m_pcGrabImg.MoveWindow(&rect, TRUE);

	// 画像表示用ビットマップの作成
	BITMAPINFO binfo;
    ZeroMemory(&binfo, sizeof(binfo));
    binfo.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
    binfo.bmiHeader.biBitCount = 32;
    binfo.bmiHeader.biPlanes   =  1;
    binfo.bmiHeader.biWidth    =  m_lWidth;
    binfo.bmiHeader.biHeight   =  -(m_lHeight);

	// m_hBmpに関連付けた CBitmapと、CDCを作成
	m_hBmp = CreateDIBSection( NULL, &binfo, DIB_RGB_COLORS, (void **)(&m_pucBmp), NULL, 0); 
	CBitmap* clsBmp = CBitmap::FromHandle(m_hBmp);
    m_clsBmpDC.CreateCompatibleDC(m_clsPicDC);
    m_clsOldBmp = m_clsBmpDC.SelectObject(clsBmp); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
//---------------------------------------------------------------------------
void CGrabPageDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_pcGrabImg.ReleaseDC(m_clsPicDC);
	m_clsBmpDC.SelectObject(m_clsOldBmp);
    DeleteDC(m_clsBmpDC);
    DeleteObject(m_hBmp);
}
//---------------------------------------------------------------------------
// 撮像
void CGrabPageDlg::OnBnClickedButtonGrab()
{
	// 撮影
	if (g_pclMain->Grab() == false) {
		::MessageBox(NULL, _T("撮影失敗"), _T("撮影失敗"), MB_OK | MB_ICONWARNING);
		return;
	}

	// 撮像画像表示
	GetGrabImg();
	ShowImg();
}
//---------------------------------------------------------------------------
// 連続撮像開始
void CGrabPageDlg::OnBnClickedButtonContinGrabStart()
{
	// 撮像と連続開始を非活性にする
	CButton *button;

	button = (CButton*)GetDlgItem(IDC_BUTTON_GRAB);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_BUTTON_CONTIN_GRAB_START);
	button->EnableWindow(FALSE);

	SetTimer(1, 10, NULL);
}
//---------------------------------------------------------------------------
// 連続撮像停止
void CGrabPageDlg::OnBnClickedButtonContinGrabEnd()
{
	KillTimer(1);

	// 撮像と連続開始を活性にする
	CButton *button;

	button = (CButton*)GetDlgItem(IDC_BUTTON_GRAB);
	button->EnableWindow(TRUE);
	button = (CButton*)GetDlgItem(IDC_BUTTON_CONTIN_GRAB_START);
	button->EnableWindow(TRUE);
}
//---------------------------------------------------------------------------
void CGrabPageDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	// 撮影
	if (g_pclMain->Grab() == false) {
		::MessageBox(NULL, _T("撮影失敗"), _T("撮影失敗"), MB_OK | MB_ICONWARNING);

		OnBnClickedButtonContinGrabEnd();
		return;
	}
	// 撮像画像表示
	GetGrabImg();
	ShowImg();

	CDialogEx::OnTimer(nIDEvent);
}
//---------------------------------------------------------------------------
// 画像読み込み
void CGrabPageDlg::OnBnClickedButtonLoadImg()
{
	// ファイルDlg表示
	CFileDialog clsFileDlg(TRUE, NULL, L"test.bmp", OFN_FILEMUSTEXIST  | OFN_HIDEREADONLY, L"BMPﾌｧｲﾙ | *.bmp|", this);
    if (clsFileDlg.DoModal() != IDOK ){
        return;
    }
	CStringW strFilePath = clsFileDlg.GetPathName();

	// 画像読み込み
	if (g_pclMain->LoadImage((WCHAR*)strFilePath.GetString()) == false) {
		::MessageBox(NULL, _T("読み込み失敗"), _T("読み込み失敗"), MB_OK | MB_ICONWARNING);
		return;
	}

	// 撮像画像表示
	GetGrabImg();
	ShowImg();
}
//---------------------------------------------------------------------------
// 画像保存
void CGrabPageDlg::OnBnClickedButtonSaveImg()
{

	// ファイルDlg表示
	CFileDialog clsFileDlg(FALSE, L"bmp", L"test.bmp", OFN_OVERWRITEPROMPT, L"BMPﾌｧｲﾙ | *.bmp|", this);
    if (clsFileDlg.DoModal() != IDOK ){
        return;
    }
	CStringW strFilePath = clsFileDlg.GetPathName();
	
	CImage image;
    CBitmap bmpForSave;
    CDC cdcForSave;

	bmpForSave.CreateCompatibleBitmap(m_clsPicDC, m_lWidth, m_lHeight);
    cdcForSave.CreateCompatibleDC(m_clsPicDC);
    cdcForSave.SelectObject(bmpForSave);
    cdcForSave.BitBlt(0, 0, m_lWidth, m_lHeight, m_clsPicDC, 0, 0, SRCCOPY);

	image.Attach((HBITMAP)bmpForSave);

	image.Save(strFilePath);
    image.Detach();
}
//---------------------------------------------------------------------------
// パラメーター設定
void CGrabPageDlg::ShowPara()
{
	CStringW	strBuf;

	// 露光時間
	strBuf.Format(L"%d", g_pclParam->stSet.stGrab.lExposure);
	m_edtExposure.SetWindowTextW(strBuf);
}
//---------------------------------------------------------------------------
// 露光時間
void CGrabPageDlg::OnEnKillfocusEditExposure()
{
	CStringW	strBuf;
	long		lBuf;

	// 値取得
	m_edtExposure.GetWindowTextW(strBuf);
	lBuf = _wtoi(strBuf.GetBuffer());
	if (lBuf < 1) {
		::MessageBox(NULL, _T("露光時間は1以上を設定してください。"), _T("設定エラー"), MB_OK | MB_ICONWARNING);
		m_edtExposure.SetFocus();
		return;		
	}
	g_pclParam->stSet.stGrab.lExposure = lBuf;
	// 表示
	strBuf.Format(L"%d", g_pclParam->stSet.stGrab.lExposure);
	m_edtExposure.SetWindowTextW(strBuf);
	// カメラに露光時間設定
	g_pclCammera->SetExposure(g_pclParam->stSet.stGrab.lExposure);
}
//---------------------------------------------------------------------------
// 撮像画像取得
void CGrabPageDlg::GetGrabImg()
{
	UCHAR* pcImg;

	// 画像取得
	pcImg = g_pclCammera->GrabImg();

	// 画像コピー
	for (int i = 0; i < m_lHeight; i++) {
		for (int j = 0; j < m_lWidth; j++) {
			m_pucBmp[4 * (i * m_lWidth + j) + 0] = pcImg[3 * (i * m_lWidth + j) + 0];
			m_pucBmp[4 * (i * m_lWidth + j) + 1] = pcImg[3 * (i * m_lWidth + j) + 1];
			m_pucBmp[4 * (i * m_lWidth + j) + 2] = pcImg[3 * (i * m_lWidth + j) + 2];
		}
	}
}
//---------------------------------------------------------------------------
// 画像表示処理
void CGrabPageDlg::ShowImg()
{
	// デバイスコンテキストにコピー
	m_clsPicDC->BitBlt(0, 0, m_lWidth, m_lHeight, &m_clsBmpDC, 0, 0, SRCCOPY);
}
//---------------------------------------------------------------------------






