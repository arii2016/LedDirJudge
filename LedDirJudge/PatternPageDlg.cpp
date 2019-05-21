// PatternPageDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LedDirJudge.h"
#include "PatternPageDlg.h"
#include "afxdialogex.h"
#include "ImgInsp.h"
#include "Cammera.h"
#include "Param.h"
#include "ImgProc.h"
#include "Result.h"
#include "Common.h"


// CPatternPageDlg ダイアログ

IMPLEMENT_DYNAMIC(CPatternPageDlg, CDialogEx)

CPatternPageDlg::CPatternPageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPatternPageDlg::IDD, pParent)
{
	// メンバ変数初期化
	m_bMouseOn = false;
	m_clsTemplatePicDC = NULL;
}

CPatternPageDlg::~CPatternPageDlg()
{
}

void CPatternPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PATTERN_IMG, m_pcPatternImg);
	DDX_Control(pDX, IDC_STATIC_TEMPLATE_IMG, m_pcTemplateImg);
	DDX_Control(pDX, IDC_EDIT_CORR_THR, m_edtCorrThr);
	DDX_Control(pDX, IDC_EDIT_DTECT_ANGLE, m_edtDtectAngle);
	DDX_Control(pDX, IDC_EDIT_DTECT_POS_X, m_edtDtectPosX);
	DDX_Control(pDX, IDC_EDIT_DTECT_POS_Y, m_edtDtectPosY);
	DDX_Control(pDX, IDC_EDIT_CORR_VAL, m_edtCorrVal);
	DDX_Control(pDX, IDC_COMBO_ANGLE_RANGE_LO, m_cbAngleRangeLo);
	DDX_Control(pDX, IDC_COMBO_ANGLE_RANGE_HI, m_cbAngleRangeHi);
}


BEGIN_MESSAGE_MAP(CPatternPageDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SHOW_ORG_IMG, &CPatternPageDlg::OnBnClickedButtonShowOrgImg)
	ON_BN_CLICKED(IDC_BUTTON_PATTERN, &CPatternPageDlg::OnBnClickedButtonPattern)
	ON_EN_KILLFOCUS(IDC_EDIT_CORR_THR, &CPatternPageDlg::OnEnKillfocusEditCorrThr)
	ON_BN_CLICKED(IDC_BUTTON_PATTERN_REGIST, &CPatternPageDlg::OnBnClickedButtonPatternRegist)
	ON_CBN_SELCHANGE(IDC_COMBO_ANGLE_RANGE_LO, &CPatternPageDlg::OnCbnSelchangeComboAngleRangeLo)
	ON_CBN_SELCHANGE(IDC_COMBO_ANGLE_RANGE_HI, &CPatternPageDlg::OnCbnSelchangeComboAngleRangeHi)
END_MESSAGE_MAP()


// CPatternPageDlg メッセージ ハンドラー
//---------------------------------------------------------------------------
BOOL CPatternPageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	// ピクチャーボックスのイベント通知を有効にする
	m_pcPatternImg.ModifyStyle(0, SS_NOTIFY);

	// 画像サイズ取得
	m_lWidth = g_pclCammera->Width();
	m_lHeight = g_pclCammera->Height();

	// Picture Controlのデバイスコンテキスト取得
	m_clsPicDC = m_pcPatternImg.GetDC();

	// 画像サイズ設定
	CRect rect;
	rect.top = 5;
	rect.left = 5;
	rect.right = rect.left + m_lWidth;
	rect.bottom = rect.top + m_lHeight;
	m_pcPatternImg.MoveWindow(&rect, TRUE);

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

	// 描画用ペン
	m_clsPatternRectPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	m_clsDtectRectPen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));

	// コンボボックスの初期化
	for (int i = 0; i <= DF_PATTERN_SET_REV_RANGE; i += DF_PATTERN_SET_REV_INTERVAL) {
		CStringW	strBuf;

		strBuf.Format(L"%d", -i);
		m_cbAngleRangeLo.InsertString(-1, strBuf);
		strBuf.Format(L"%d", i);
		m_cbAngleRangeHi.InsertString(-1, strBuf);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
//---------------------------------------------------------------------------
void CPatternPageDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_pcPatternImg.ReleaseDC(m_clsPicDC);
	m_clsBmpDC.SelectObject(m_clsOldBmp);
    DeleteDC(m_clsBmpDC);
    DeleteObject(m_hBmp);

	DeletTemplateBmp();
}
//---------------------------------------------------------------------------
BOOL CPatternPageDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
    if (pMsg->hwnd == m_pcPatternImg.m_hWnd) {
		int x = GET_X_LPARAM(pMsg->lParam);
		int y = GET_Y_LPARAM(pMsg->lParam);

		switch(pMsg->message){
        case WM_LBUTTONDOWN: 
			if (m_bMouseOn == false) {
				m_bMouseOn = true;
				g_pclParam->stSet.stPattern.stPatternArea.lx = x;
				g_pclParam->stSet.stPattern.stPatternArea.ly = y;
			}

			break;
        case WM_MOUSEMOVE: 
			if (m_bMouseOn == true) {
				g_pclParam->stSet.stPattern.stPatternArea.lWidth = abs(x - g_pclParam->stSet.stPattern.stPatternArea.lx);
				if (g_pclParam->stSet.stPattern.stPatternArea.lx > x) {
					g_pclParam->stSet.stPattern.stPatternArea.lx = x;
				}
				g_pclParam->stSet.stPattern.stPatternArea.lHeight = abs(y - g_pclParam->stSet.stPattern.stPatternArea.ly);
				if (g_pclParam->stSet.stPattern.stPatternArea.ly > y) {
					g_pclParam->stSet.stPattern.stPatternArea.ly = y;
				}
				// エリア再描画
				ShowImg();
				DrawPatternArea();
			}
			break;
        case WM_LBUTTONUP: 
			m_bMouseOn = false;

			break;
        }
    }

	return CDialogEx::PreTranslateMessage(pMsg);
}
//---------------------------------------------------------------------------
void CPatternPageDlg::OnBnClickedButtonShowOrgImg()
{
	// 撮像画像表示
	SetGrabImg();
	ShowImg();
	DrawPatternArea();
	// テンプレート画像表示
	SetTemplateImg();
	ShowTemplateImg();
}
//---------------------------------------------------------------------------
void CPatternPageDlg::OnBnClickedButtonPatternRegist()
{
	// テンプレートの計算を行う
	g_pclImgInsp->CalcTemplate();

	// テンプレート画像表示
	SetTemplateImg();
	ShowTemplateImg();
}
//---------------------------------------------------------------------------
void CPatternPageDlg::OnBnClickedButtonPattern()
{
	CStringW	strBuf;

	// 前処理
	g_pclImgInsp->InitProc();
	
// 検査時間タイマースタート
g_pclCommon->TimerStart(TIMER_TEMP);

	// パターンマッチング
	if (g_pclImgInsp->PatternMatching() == false) {
		::MessageBox(NULL, _T("パターンマッチングに失敗しました。"), _T("画像処理エラー"), MB_OK | MB_ICONWARNING);
		return;
	}
strBuf.Format(L"%dmsec", g_pclCommon->TimerLap(TIMER_TEMP) / 1000);
::MessageBox(NULL, strBuf, _T("時間"), MB_OK | MB_ICONWARNING);

	// パターンマッチング用回転・位置補正した画像を作成
	g_pclImgInsp->MakePatternImg();

	// 検出位置表示
	strBuf.Format(L"%d", g_pclResult->stResult.stPattern.stDtectPos.lX);
	m_edtDtectPosX.SetWindowTextW(strBuf);
	strBuf.Format(L"%d", g_pclResult->stResult.stPattern.stDtectPos.lY);
	m_edtDtectPosY.SetWindowTextW(strBuf);

	// 検出角度表示
	strBuf.Format(L"%.1f°", g_pclResult->stResult.stPattern.dDtectAngle);
	m_edtDtectAngle.SetWindowTextW(strBuf);

	// 相関値
	strBuf.Format(L"%.1f", g_pclResult->stResult.stPattern.dCorrVal * 100.0);
	m_edtCorrVal.SetWindowTextW(strBuf);

	// 画像表示
	SetGrabImg();
	ShowImg();
	DrawDtectArea();
}
//---------------------------------------------------------------------------
// パラメーター設定
void CPatternPageDlg::ShowPara()
{
	CStringW	strBuf;

	// 角度範囲下限
	m_cbAngleRangeLo.SetCurSel(abs(g_pclParam->stSet.stPattern.stAngleRange.lLo / DF_PATTERN_SET_REV_INTERVAL));
	// 角度範囲上限
	m_cbAngleRangeHi.SetCurSel(g_pclParam->stSet.stPattern.stAngleRange.lHi / DF_PATTERN_SET_REV_INTERVAL);

	// 相関閾値
	strBuf.Format(L"%.1f", g_pclParam->stSet.stPattern.dCorrThr);
	m_edtCorrThr.SetWindowTextW(strBuf);

}
//---------------------------------------------------------------------------
void CPatternPageDlg::OnCbnSelchangeComboAngleRangeLo()
{
	g_pclParam->stSet.stPattern.stAngleRange.lLo = -(m_cbAngleRangeLo.GetCurSel() * DF_PATTERN_SET_REV_INTERVAL);
}
//---------------------------------------------------------------------------
void CPatternPageDlg::OnCbnSelchangeComboAngleRangeHi()
{
	g_pclParam->stSet.stPattern.stAngleRange.lHi = m_cbAngleRangeHi.GetCurSel() * DF_PATTERN_SET_REV_INTERVAL;
}
//---------------------------------------------------------------------------
void CPatternPageDlg::OnEnKillfocusEditCorrThr()
{
	CStringW	strBuf;
	double		dBuf;

	// 値取得
	m_edtCorrThr.GetWindowTextW(strBuf);
	dBuf = _wtof(strBuf.GetBuffer());
	if (dBuf < 0.0 || dBuf > 100.0) {
		::MessageBox(NULL, _T("相関閾値は0.0～100.0を設定してください。"), _T("設定エラー"), MB_OK | MB_ICONWARNING);
		m_edtCorrThr.SetFocus();
		return;		
	}
	g_pclParam->stSet.stPattern.dCorrThr = dBuf;
	// 表示
	strBuf.Format(L"%.1f", g_pclParam->stSet.stPattern.dCorrThr);
	m_edtCorrThr.SetWindowTextW(strBuf);
}
//---------------------------------------------------------------------------
// 撮像画像取得
void CPatternPageDlg::SetGrabImg()
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
void CPatternPageDlg::ShowImg()
{
	// デバイスコンテキストにコピー
	m_clsPicDC->BitBlt(0, 0, m_lWidth, m_lHeight, &m_clsBmpDC, 0, 0, SRCCOPY);
}
//---------------------------------------------------------------------------
// パターンエリア描画
void CPatternPageDlg::DrawPatternArea()
{
	// ペンを設定
	m_clsPicDC->SelectObject(&m_clsPatternRectPen);

	// エリア表示
	m_clsPicDC->MoveTo(g_pclParam->stSet.stPattern.stPatternArea.lx, g_pclParam->stSet.stPattern.stPatternArea.ly);
	m_clsPicDC->LineTo(g_pclParam->stSet.stPattern.stPatternArea.lx + g_pclParam->stSet.stPattern.stPatternArea.lWidth, g_pclParam->stSet.stPattern.stPatternArea.ly);
	m_clsPicDC->LineTo(g_pclParam->stSet.stPattern.stPatternArea.lx + g_pclParam->stSet.stPattern.stPatternArea.lWidth, g_pclParam->stSet.stPattern.stPatternArea.ly + g_pclParam->stSet.stPattern.stPatternArea.lHeight);
	m_clsPicDC->LineTo(g_pclParam->stSet.stPattern.stPatternArea.lx, g_pclParam->stSet.stPattern.stPatternArea.ly + g_pclParam->stSet.stPattern.stPatternArea.lHeight);
	m_clsPicDC->LineTo(g_pclParam->stSet.stPattern.stPatternArea.lx, g_pclParam->stSet.stPattern.stPatternArea.ly);
}
//---------------------------------------------------------------------------
// 検出エリア描画
void CPatternPageDlg::DrawDtectArea()
{
	ST_LONG_POS	stStartPos;
	ST_LONG_POS	stPos;

	ST_LONG_POS	stAffPos;
	ST_LONG_POS	stCenter;

	stAffPos.lX = 0;
	stAffPos.lY = 0;

	stCenter.lX = g_pclParam->stSet.stPattern.stTemplate[0].lWidth / 2;
	stCenter.lY = g_pclParam->stSet.stPattern.stTemplate[0].lHeight / 2;

	// ペンを設定
	m_clsPicDC->SelectObject(&m_clsDtectRectPen);

	// エリア表示
	stStartPos.lX = AFFINE_X(0, 0, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stStartPos.lY = AFFINE_Y(0, 0, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stStartPos.lX += g_pclResult->stResult.stPattern.stAffPos.lX;
	stStartPos.lY += g_pclResult->stResult.stPattern.stAffPos.lY;
	m_clsPicDC->MoveTo(stStartPos.lX, stStartPos.lY);


	stPos.lX = AFFINE_X(g_pclParam->stSet.stPattern.stTemplate[0].lWidth, 0, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stPos.lY = AFFINE_Y(g_pclParam->stSet.stPattern.stTemplate[0].lWidth, 0, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stPos.lX += g_pclResult->stResult.stPattern.stAffPos.lX;
	stPos.lY += g_pclResult->stResult.stPattern.stAffPos.lY;
	m_clsPicDC->LineTo(stPos.lX, stPos.lY);

	stPos.lX = AFFINE_X(g_pclParam->stSet.stPattern.stTemplate[0].lWidth, g_pclParam->stSet.stPattern.stTemplate[0].lHeight, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stPos.lY = AFFINE_Y(g_pclParam->stSet.stPattern.stTemplate[0].lWidth, g_pclParam->stSet.stPattern.stTemplate[0].lHeight, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stPos.lX += g_pclResult->stResult.stPattern.stAffPos.lX;
	stPos.lY += g_pclResult->stResult.stPattern.stAffPos.lY;
	m_clsPicDC->LineTo(stPos.lX, stPos.lY);

	stPos.lX = AFFINE_X(0, g_pclParam->stSet.stPattern.stTemplate[0].lHeight, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stPos.lY = AFFINE_Y(0, g_pclParam->stSet.stPattern.stTemplate[0].lHeight, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stPos.lX += g_pclResult->stResult.stPattern.stAffPos.lX;
	stPos.lY += g_pclResult->stResult.stPattern.stAffPos.lY;
	m_clsPicDC->LineTo(stPos.lX, stPos.lY);

	m_clsPicDC->LineTo(stStartPos.lX, stStartPos.lY);

}
//---------------------------------------------------------------------------
void CPatternPageDlg::SetTemplateImg()
{
	DeletTemplateBmp();

	// テンプレート用Picture Controlのデバイスコンテキスト取得
	m_clsTemplatePicDC = m_pcTemplateImg.GetDC();

	// 画像サイズ設定
	m_pcTemplateImg.MoveWindow(290, 490, g_pclParam->stSet.stPattern.stTemplate[0].lWidth, g_pclParam->stSet.stPattern.stTemplate[0].lHeight, TRUE);

	// テンプレート用ビットマップの作成
	BITMAPINFO binfo;
    ZeroMemory(&binfo, sizeof(binfo));
    binfo.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
    binfo.bmiHeader.biBitCount = 32;
    binfo.bmiHeader.biPlanes   =  1;
    binfo.bmiHeader.biWidth    =  g_pclParam->stSet.stPattern.stTemplate[0].lWidth;
    binfo.bmiHeader.biHeight   =  -(g_pclParam->stSet.stPattern.stTemplate[0].lHeight);

	// m_hTemplateBmpに関連付けた CBitmapと、CDCを作成
	m_hTemplateBmp = CreateDIBSection( NULL, &binfo, DIB_RGB_COLORS, (void **)(&m_pucTemplateBmp), NULL, 0); 
	CBitmap* clsTemplateBmp = CBitmap::FromHandle(m_hTemplateBmp);
	m_pclsTemplateBmpDC = new CDC();
    m_pclsTemplateBmpDC->CreateCompatibleDC(m_clsTemplatePicDC);
    m_clsTemplateOldBmp = m_pclsTemplateBmpDC->SelectObject(clsTemplateBmp); 

	// 画像コピー
	for (int i = 0; i < g_pclParam->stSet.stPattern.stTemplate[0].lHeight; i++) {
		for (int j = 0; j < g_pclParam->stSet.stPattern.stTemplate[0].lWidth; j++) {
			m_pucTemplateBmp[4 * (i * g_pclParam->stSet.stPattern.stTemplate[0].lWidth + j) + 0] = g_pclParam->stSet.stPattern.stTemplate[0].ucImg[i * g_pclParam->stSet.stPattern.stTemplate[0].lWidth + j];
			m_pucTemplateBmp[4 * (i * g_pclParam->stSet.stPattern.stTemplate[0].lWidth + j) + 1] = g_pclParam->stSet.stPattern.stTemplate[0].ucImg[i * g_pclParam->stSet.stPattern.stTemplate[0].lWidth + j];
			m_pucTemplateBmp[4 * (i * g_pclParam->stSet.stPattern.stTemplate[0].lWidth + j) + 2] = g_pclParam->stSet.stPattern.stTemplate[0].ucImg[i * g_pclParam->stSet.stPattern.stTemplate[0].lWidth + j];
		}
	}
}
//---------------------------------------------------------------------------
void CPatternPageDlg::ShowTemplateImg()
{
	// デバイスコンテキストにコピー
	m_clsTemplatePicDC->BitBlt(0, 0, g_pclParam->stSet.stPattern.stTemplate[0].lWidth, g_pclParam->stSet.stPattern.stTemplate[0].lHeight, m_pclsTemplateBmpDC, 0, 0, SRCCOPY);
}
//---------------------------------------------------------------------------
void CPatternPageDlg::DeletTemplateBmp()
{
	if (m_clsTemplatePicDC == NULL) {
		return;
	}
	m_pcTemplateImg.ReleaseDC(m_clsTemplatePicDC);
	m_pclsTemplateBmpDC->SelectObject(m_clsTemplateOldBmp);
    DeleteDC(*m_pclsTemplateBmpDC);
	delete m_pclsTemplateBmpDC;
    DeleteObject(m_hTemplateBmp);

	m_clsTemplatePicDC = NULL;
}
//---------------------------------------------------------------------------
