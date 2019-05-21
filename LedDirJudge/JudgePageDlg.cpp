// JudgePageDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LedDirJudge.h"
#include "JudgePageDlg.h"
#include "afxdialogex.h"
#include "ImgInsp.h"
#include "Cammera.h"
#include "Param.h"
#include "ImgProc.h"
#include "Result.h"


// CJudgePageDlg ダイアログ

IMPLEMENT_DYNAMIC(CJudgePageDlg, CDialogEx)

CJudgePageDlg::CJudgePageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CJudgePageDlg::IDD, pParent)
{
	// メンバ変数初期化
	m_bMouseOn = false;
	m_lSelectNo = 0;
}

CJudgePageDlg::~CJudgePageDlg()
{
}

void CJudgePageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_JUDGE_IMG, m_pcJudgeImg);
	DDX_Control(pDX, IDC_EDIT_LUMI_VAL, m_edtLumiVal);
	DDX_Control(pDX, IDC_COMBO_NUM, m_cbNum);
	DDX_Control(pDX, IDC_EDIT_THR_LO1, m_edtThrLo1);
	DDX_Control(pDX, IDC_EDIT_THR_LO2, m_edtThrLo2);
	DDX_Control(pDX, IDC_EDIT_THR_LO3, m_edtThrLo3);
	DDX_Control(pDX, IDC_EDIT_THR_LO4, m_edtThrLo4);
	DDX_Control(pDX, IDC_EDIT_THR_LO5, m_edtThrLo5);
	DDX_Control(pDX, IDC_EDIT_THR_HI1, m_edtThrHi1);
	DDX_Control(pDX, IDC_EDIT_THR_HI2, m_edtThrHi2);
	DDX_Control(pDX, IDC_EDIT_THR_HI3, m_edtThrHi3);
	DDX_Control(pDX, IDC_EDIT_THR_HI4, m_edtThrHi4);
	DDX_Control(pDX, IDC_EDIT_THR_HI5, m_edtThrHi5);
	DDX_Control(pDX, IDC_EDIT_AREA_RANGE_LO1, m_edtAreaRangeLo1);
	DDX_Control(pDX, IDC_EDIT_AREA_RANGE_LO2, m_edtAreaRangeLo2);
	DDX_Control(pDX, IDC_EDIT_AREA_RANGE_LO3, m_edtAreaRangeLo3);
	DDX_Control(pDX, IDC_EDIT_AREA_RANGE_LO4, m_edtAreaRangeLo4);
	DDX_Control(pDX, IDC_EDIT_AREA_RANGE_LO5, m_edtAreaRangeLo5);
	DDX_Control(pDX, IDC_EDIT_AREA_RANGE_HI1, m_edtAreaRangeHi1);
	DDX_Control(pDX, IDC_EDIT_AREA_RANGE_HI2, m_edtAreaRangeHi2);
	DDX_Control(pDX, IDC_EDIT_AREA_RANGE_HI3, m_edtAreaRangeHi3);
	DDX_Control(pDX, IDC_EDIT_AREA_RANGE_HI4, m_edtAreaRangeHi4);
	DDX_Control(pDX, IDC_EDIT_AREA_RANGE_HI5, m_edtAreaRangeHi5);
	DDX_Control(pDX, IDC_EDIT_AREA_NUM_RESULT1, m_edtAreaNumResult1);
	DDX_Control(pDX, IDC_EDIT_AREA_NUM_RESULT2, m_edtAreaNumResult2);
	DDX_Control(pDX, IDC_EDIT_AREA_NUM_RESULT3, m_edtAreaNumResult3);
	DDX_Control(pDX, IDC_EDIT_AREA_NUM_RESULT4, m_edtAreaNumResult4);
	DDX_Control(pDX, IDC_EDIT_AREA_NUM_RESULT5, m_edtAreaNumResult5);
	DDX_Control(pDX, IDC_EDIT_JUDGE_RESULT1, m_edtJudgeResult1);
	DDX_Control(pDX, IDC_EDIT_JUDGE_RESULT2, m_edtJudgeResult2);
	DDX_Control(pDX, IDC_EDIT_JUDGE_RESULT3, m_edtJudgeResult3);
	DDX_Control(pDX, IDC_EDIT_JUDGE_RESULT4, m_edtJudgeResult4);
	DDX_Control(pDX, IDC_EDIT_JUDGE_RESULT5, m_edtJudgeResult5);
	DDX_Control(pDX, IDC_COMBO_SELECT_NO, m_cbSelectNo);
}


BEGIN_MESSAGE_MAP(CJudgePageDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SHOW_ORG_IMG, &CJudgePageDlg::OnBnClickedButtonShowOrgImg)
	ON_BN_CLICKED(IDC_BUTTON_JUDGE, &CJudgePageDlg::OnBnClickedButtonJudge)
	ON_CBN_SELCHANGE(IDC_COMBO_NUM, &CJudgePageDlg::OnCbnSelchangeComboNum)
	ON_EN_KILLFOCUS(IDC_EDIT_THR_LO1, &CJudgePageDlg::OnEnKillfocusEditThrLo1)
	ON_EN_KILLFOCUS(IDC_EDIT_THR_LO2, &CJudgePageDlg::OnEnKillfocusEditThrLo2)
	ON_EN_KILLFOCUS(IDC_EDIT_THR_LO3, &CJudgePageDlg::OnEnKillfocusEditThrLo3)
	ON_EN_KILLFOCUS(IDC_EDIT_THR_LO4, &CJudgePageDlg::OnEnKillfocusEditThrLo4)
	ON_EN_KILLFOCUS(IDC_EDIT_THR_LO5, &CJudgePageDlg::OnEnKillfocusEditThrLo5)
	ON_EN_KILLFOCUS(IDC_EDIT_THR_HI1, &CJudgePageDlg::OnEnKillfocusEditThrHi1)
	ON_EN_KILLFOCUS(IDC_EDIT_THR_HI2, &CJudgePageDlg::OnEnKillfocusEditThrHi2)
	ON_EN_KILLFOCUS(IDC_EDIT_THR_HI3, &CJudgePageDlg::OnEnKillfocusEditThrHi3)
	ON_EN_KILLFOCUS(IDC_EDIT_THR_HI4, &CJudgePageDlg::OnEnKillfocusEditThrHi4)
	ON_EN_KILLFOCUS(IDC_EDIT_THR_HI5, &CJudgePageDlg::OnEnKillfocusEditThrHi5)
	ON_EN_KILLFOCUS(IDC_EDIT_AREA_RANGE_LO1, &CJudgePageDlg::OnEnKillfocusEditAreaRangeLo1)
	ON_EN_KILLFOCUS(IDC_EDIT_AREA_RANGE_LO2, &CJudgePageDlg::OnEnKillfocusEditAreaRangeLo2)
	ON_EN_KILLFOCUS(IDC_EDIT_AREA_RANGE_LO3, &CJudgePageDlg::OnEnKillfocusEditAreaRangeLo3)
	ON_EN_KILLFOCUS(IDC_EDIT_AREA_RANGE_LO4, &CJudgePageDlg::OnEnKillfocusEditAreaRangeLo4)
	ON_EN_KILLFOCUS(IDC_EDIT_AREA_RANGE_LO5, &CJudgePageDlg::OnEnKillfocusEditAreaRangeLo5)
	ON_EN_KILLFOCUS(IDC_EDIT_AREA_RANGE_HI1, &CJudgePageDlg::OnEnKillfocusEditAreaRangeHi1)
	ON_EN_KILLFOCUS(IDC_EDIT_AREA_RANGE_HI2, &CJudgePageDlg::OnEnKillfocusEditAreaRangeHi2)
	ON_EN_KILLFOCUS(IDC_EDIT_AREA_RANGE_HI3, &CJudgePageDlg::OnEnKillfocusEditAreaRangeHi3)
	ON_EN_KILLFOCUS(IDC_EDIT_AREA_RANGE_HI4, &CJudgePageDlg::OnEnKillfocusEditAreaRangeHi4)
	ON_EN_KILLFOCUS(IDC_EDIT_AREA_RANGE_HI5, &CJudgePageDlg::OnEnKillfocusEditAreaRangeHi5)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECT_NO, &CJudgePageDlg::OnCbnSelchangeComboSelectNo)
END_MESSAGE_MAP()


// CJudgePageDlg メッセージ ハンドラー
//---------------------------------------------------------------------------
BOOL CJudgePageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	// コントロールを配列に設定
	m_pedtThrLo[0] = &m_edtThrLo1;
	m_pedtThrLo[1] = &m_edtThrLo2;
	m_pedtThrLo[2] = &m_edtThrLo3;
	m_pedtThrLo[3] = &m_edtThrLo4;
	m_pedtThrLo[4] = &m_edtThrLo5;

	m_pedtThrHi[0] = &m_edtThrHi1;
	m_pedtThrHi[1] = &m_edtThrHi2;
	m_pedtThrHi[2] = &m_edtThrHi3;
	m_pedtThrHi[3] = &m_edtThrHi4;
	m_pedtThrHi[4] = &m_edtThrHi5;

	m_pedtAreaRangeLo[0] = &m_edtAreaRangeLo1;
	m_pedtAreaRangeLo[1] = &m_edtAreaRangeLo2;
	m_pedtAreaRangeLo[2] = &m_edtAreaRangeLo3;
	m_pedtAreaRangeLo[3] = &m_edtAreaRangeLo4;
	m_pedtAreaRangeLo[4] = &m_edtAreaRangeLo5;

	m_pedtAreaRangeHi[0] = &m_edtAreaRangeHi1;
	m_pedtAreaRangeHi[1] = &m_edtAreaRangeHi2;
	m_pedtAreaRangeHi[2] = &m_edtAreaRangeHi3;
	m_pedtAreaRangeHi[3] = &m_edtAreaRangeHi4;
	m_pedtAreaRangeHi[4] = &m_edtAreaRangeHi5;

	m_pedtAreaNumResult[0] = &m_edtAreaNumResult1;
	m_pedtAreaNumResult[1] = &m_edtAreaNumResult2;
	m_pedtAreaNumResult[2] = &m_edtAreaNumResult3;
	m_pedtAreaNumResult[3] = &m_edtAreaNumResult4;
	m_pedtAreaNumResult[4] = &m_edtAreaNumResult5;

	m_pedtJudgeResult[0] = &m_edtJudgeResult1;
	m_pedtJudgeResult[1] = &m_edtJudgeResult2;
	m_pedtJudgeResult[2] = &m_edtJudgeResult3;
	m_pedtJudgeResult[3] = &m_edtJudgeResult4;
	m_pedtJudgeResult[4] = &m_edtJudgeResult5;

	// ピクチャーボックスのイベント通知を有効にする
	m_pcJudgeImg.ModifyStyle(0, SS_NOTIFY);

	// 画像サイズ取得
	m_lWidth = g_pclCammera->Width();
	m_lHeight = g_pclCammera->Height();

	// Picture Controlのデバイスコンテキスト取得
	m_clsPicDC = m_pcJudgeImg.GetDC();

	// 画像サイズ設定
	CRect rect;
	rect.top = 5;
	rect.left = 5;
	rect.right = rect.left + m_lWidth;
	rect.bottom = rect.top + m_lHeight;
	m_pcJudgeImg.MoveWindow(&rect, TRUE);

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
	m_clsRectPen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));

	// コンボボックスの初期化
	for (int i = 0; i < DF_JUDGE_NUM_MAX; i++) {
		CStringW	strBuf;

		strBuf.Format(L"%d", i + 1);
		m_cbNum.InsertString(-1, strBuf);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_pcJudgeImg.ReleaseDC(m_clsPicDC);
	m_clsBmpDC.SelectObject(m_clsOldBmp);
    DeleteDC(m_clsBmpDC);
    DeleteObject(m_hBmp);
}
//---------------------------------------------------------------------------
BOOL CJudgePageDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
    if (pMsg->hwnd == m_pcJudgeImg.m_hWnd) {
		int x = GET_X_LPARAM(pMsg->lParam);
		int y = GET_Y_LPARAM(pMsg->lParam);

		switch(pMsg->message){
        case WM_LBUTTONDOWN: 
			if (m_bMouseOn == false) {
				m_bMouseOn = true;
				g_pclParam->stSet.stJudge.stOne[m_lSelectNo].stArea.lx = x;
				g_pclParam->stSet.stJudge.stOne[m_lSelectNo].stArea.ly = y;
			}

			break;
        case WM_MOUSEMOVE: 
			if (m_bMouseOn == true) {
				g_pclParam->stSet.stJudge.stOne[m_lSelectNo].stArea.lWidth = abs(x - g_pclParam->stSet.stJudge.stOne[m_lSelectNo].stArea.lx);
				if (g_pclParam->stSet.stJudge.stOne[m_lSelectNo].stArea.lx > x) {
					g_pclParam->stSet.stJudge.stOne[m_lSelectNo].stArea.lx = x;
				}
				g_pclParam->stSet.stJudge.stOne[m_lSelectNo].stArea.lHeight = abs(y - g_pclParam->stSet.stJudge.stOne[m_lSelectNo].stArea.ly);
				if (g_pclParam->stSet.stJudge.stOne[m_lSelectNo].stArea.ly > y) {
					g_pclParam->stSet.stJudge.stOne[m_lSelectNo].stArea.ly = y;
				}
				// エリア再描画
				ShowImg();
				DrawJudgeArea();
			}
			else {
				// ポインタの輝度値表示
				CStringW	strBuf;

				strBuf.Format(L"%d", m_pucBmp[4 * (y * m_lWidth + x) + 1] );
				m_edtLumiVal.SetWindowTextW(strBuf);
				
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
void CJudgePageDlg::OnBnClickedButtonShowOrgImg()
{
	// パターンマッチング位置角度補正処理画像描画
	SetPatternImg();
	ShowImg();
	DrawJudgeArea();
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnBnClickedButtonJudge()
{
	// 判定
	if (g_pclImgInsp->Judgment() == false) {
		::MessageBox(NULL, _T("判定に失敗しました。"), _T("画像処理エラー"), MB_OK | MB_ICONWARNING);
		return;
	}

	// 処理画像を作成
	g_pclImgInsp->MakeJudgeImg();

	// エリア数、判定を表示
	CStringW	strBuf;

	for (int i = 0; i < g_pclParam->stSet.stJudge.lNum; i++) {
		strBuf.Format(L"%d", g_pclResult->stResult.stJudge.stOne[i].lAreaNum);
		m_pedtAreaNumResult[i]->SetWindowTextW(strBuf);

		m_pedtJudgeResult[i]->SetWindowTextW(JudgeName[g_pclResult->stResult.stJudge.stOne[i].enJudge]);
	}

	// 画像を表示
	SetJudgeImg();
	ShowImg();
	DrawJudgeArea();
}
//---------------------------------------------------------------------------
void CJudgePageDlg::ShowPara()
{
	CStringW	strBuf;

	// エリア数
	m_cbNum.SetCurSel(g_pclParam->stSet.stJudge.lNum - 1);
	// 選択No
	m_cbSelectNo.ResetContent();
	for (int i = 0; i < g_pclParam->stSet.stJudge.lNum; i++) {
		strBuf.Format(L"%d", i + 1);
		m_cbSelectNo.InsertString(-1, strBuf);
	}
	m_cbSelectNo.SetCurSel(m_lSelectNo);


	for (int i = 0; i < g_pclParam->stSet.stJudge.lNum; i++) {
		// 二値化下限
		strBuf.Format(L"%d", g_pclParam->stSet.stJudge.stOne[i].stThr.lLo);
		m_pedtThrLo[i]->SetWindowTextW(strBuf);
		// 二値化上限
		strBuf.Format(L"%d", g_pclParam->stSet.stJudge.stOne[i].stThr.lHi);
		m_pedtThrHi[i]->SetWindowTextW(strBuf);
		// エリア判定閾値下限
		strBuf.Format(L"%d", g_pclParam->stSet.stJudge.stOne[i].stAreaRange.lLo);
		m_pedtAreaRangeLo[i]->SetWindowTextW(strBuf);
		// エリア判定閾値上限
		strBuf.Format(L"%d", g_pclParam->stSet.stJudge.stOne[i].stAreaRange.lHi);
		m_pedtAreaRangeHi[i]->SetWindowTextW(strBuf);
	}
}
//---------------------------------------------------------------------------
// エリア数
void CJudgePageDlg::OnCbnSelchangeComboNum()
{
	g_pclParam->stSet.stJudge.lNum = m_cbNum.GetCurSel() + 1;

	// 選択No設定
	m_cbSelectNo.ResetContent();
	for (int i = 0; i < g_pclParam->stSet.stJudge.lNum; i++) {
		CStringW	strBuf;

		strBuf.Format(L"%d", i + 1);
		m_cbSelectNo.InsertString(-1, strBuf);
	}
	if (m_lSelectNo >= g_pclParam->stSet.stJudge.lNum) {
		m_lSelectNo = g_pclParam->stSet.stJudge.lNum - 1;
	}
	m_cbSelectNo.SetCurSel(m_lSelectNo);
}
//---------------------------------------------------------------------------
// 選択No
void CJudgePageDlg::OnCbnSelchangeComboSelectNo()
{
	m_lSelectNo = m_cbSelectNo.GetCurSel();
}
//---------------------------------------------------------------------------
// 二値化下限
void CJudgePageDlg::OnEnKillfocusEditThrLo1()
{
	OnEnKillfocusEditThrLo(0);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditThrLo2()
{
	OnEnKillfocusEditThrLo(1);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditThrLo3()
{
	OnEnKillfocusEditThrLo(2);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditThrLo4()
{
	OnEnKillfocusEditThrLo(3);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditThrLo5()
{
	OnEnKillfocusEditThrLo(4);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditThrLo(int iNo)
{
	CStringW	strBuf;
	long		lBuf;

	// 値取得
	m_pedtThrLo[iNo]->GetWindowTextW(strBuf);
	lBuf = _wtoi(strBuf.GetBuffer());
	if (lBuf < 0 || lBuf > 255) {
		::MessageBox(NULL, _T("二値化は0～255を設定してください。"), _T("設定エラー"), MB_OK | MB_ICONWARNING);
		m_pedtThrLo[iNo]->SetFocus();
		return;		
	}
	if (lBuf >= g_pclParam->stSet.stJudge.stOne[iNo].stThr.lHi) {
		::MessageBox(NULL, _T("二値化上限より小さい値を設定してください。"), _T("設定エラー"), MB_OK | MB_ICONWARNING);
		m_pedtThrLo[iNo]->SetFocus();
		return;		
	}
	g_pclParam->stSet.stJudge.stOne[iNo].stThr.lLo = lBuf;
	// 表示
	strBuf.Format(L"%d", g_pclParam->stSet.stJudge.stOne[iNo].stThr.lLo);
	m_pedtThrLo[iNo]->SetWindowTextW(strBuf);
}
//---------------------------------------------------------------------------
// 二値化上限
void CJudgePageDlg::OnEnKillfocusEditThrHi1()
{
	OnEnKillfocusEditThrHi(0);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditThrHi2()
{
	OnEnKillfocusEditThrHi(1);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditThrHi3()
{
	OnEnKillfocusEditThrHi(2);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditThrHi4()
{
	OnEnKillfocusEditThrHi(3);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditThrHi5()
{
	OnEnKillfocusEditThrHi(4);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditThrHi(int iNo)
{
	CStringW	strBuf;
	long		lBuf;

	// 値取得
	m_pedtThrHi[iNo]->GetWindowTextW(strBuf);
	lBuf = _wtoi(strBuf.GetBuffer());
	if (lBuf < 0 || lBuf > 255) {
		::MessageBox(NULL, _T("二値化は0～255を設定してください。"), _T("設定エラー"), MB_OK | MB_ICONWARNING);
		m_pedtThrHi[iNo]->SetFocus();
		return;		
	}
	if (lBuf <= g_pclParam->stSet.stJudge.stOne[iNo].stThr.lLo) {
		::MessageBox(NULL, _T("二値化下限より大きい値を設定してください。"), _T("設定エラー"), MB_OK | MB_ICONWARNING);
		m_pedtThrHi[iNo]->SetFocus();
		return;		
	}
	g_pclParam->stSet.stJudge.stOne[iNo].stThr.lHi = lBuf;
	// 表示
	strBuf.Format(L"%d", g_pclParam->stSet.stJudge.stOne[iNo].stThr.lHi);
	m_pedtThrHi[iNo]->SetWindowTextW(strBuf);
}
//---------------------------------------------------------------------------
// エリア判定閾値下限
void CJudgePageDlg::OnEnKillfocusEditAreaRangeLo1()
{
	OnEnKillfocusEditAreaRangeLo(0);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditAreaRangeLo2()
{
	OnEnKillfocusEditAreaRangeLo(1);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditAreaRangeLo3()
{
	OnEnKillfocusEditAreaRangeLo(2);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditAreaRangeLo4()
{
	OnEnKillfocusEditAreaRangeLo(3);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditAreaRangeLo5()
{
	OnEnKillfocusEditAreaRangeLo(4);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditAreaRangeLo(int iNo)
{
	CStringW	strBuf;
	long		lBuf;

	// 値取得
	m_pedtAreaRangeLo[iNo]->GetWindowTextW(strBuf);
	lBuf = _wtoi(strBuf.GetBuffer());
	if (lBuf < 0) {
		::MessageBox(NULL, _T("エリア判定閾値は0以上を設定してください。"), _T("設定エラー"), MB_OK | MB_ICONWARNING);
		m_pedtAreaRangeLo[iNo]->SetFocus();
		return;		
	}
	if (lBuf >= g_pclParam->stSet.stJudge.stOne[iNo].stAreaRange.lHi) {
		::MessageBox(NULL, _T("エリア判定閾値上限より小さい値を設定してください。"), _T("設定エラー"), MB_OK | MB_ICONWARNING);
		m_pedtAreaRangeLo[iNo]->SetFocus();
		return;		
	}
	g_pclParam->stSet.stJudge.stOne[iNo].stAreaRange.lLo = lBuf;
	// 表示
	strBuf.Format(L"%d", g_pclParam->stSet.stJudge.stOne[iNo].stAreaRange.lLo);
	m_pedtAreaRangeLo[iNo]->SetWindowTextW(strBuf);
}
//---------------------------------------------------------------------------
// エリア判定閾値上限
void CJudgePageDlg::OnEnKillfocusEditAreaRangeHi1()
{
	OnEnKillfocusEditAreaRangeHi(0);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditAreaRangeHi2()
{
	OnEnKillfocusEditAreaRangeHi(1);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditAreaRangeHi3()
{
	OnEnKillfocusEditAreaRangeHi(2);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditAreaRangeHi4()
{
	OnEnKillfocusEditAreaRangeHi(3);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditAreaRangeHi5()
{
	OnEnKillfocusEditAreaRangeHi(4);
}
//---------------------------------------------------------------------------
void CJudgePageDlg::OnEnKillfocusEditAreaRangeHi(int iNo)
{
	CStringW	strBuf;
	long		lBuf;

	// 値取得
	m_pedtAreaRangeHi[iNo]->GetWindowTextW(strBuf);
	lBuf = _wtoi(strBuf.GetBuffer());
	if (lBuf < 0) {
		::MessageBox(NULL, _T("エリア判定閾値は0以上を設定してください。"), _T("設定エラー"), MB_OK | MB_ICONWARNING);
		m_pedtAreaRangeHi[iNo]->SetFocus();
		return;		
	}
	if (lBuf <= g_pclParam->stSet.stJudge.stOne[iNo].stAreaRange.lLo) {
		::MessageBox(NULL, _T("エリア判定閾値下限より大きい値を設定してください。"), _T("設定エラー"), MB_OK | MB_ICONWARNING);
		m_pedtAreaRangeHi[iNo]->SetFocus();
		return;		
	}
	g_pclParam->stSet.stJudge.stOne[iNo].stAreaRange.lHi = lBuf;
	// 表示
	strBuf.Format(L"%d", g_pclParam->stSet.stJudge.stOne[iNo].stAreaRange.lHi);
	m_pedtAreaRangeHi[iNo]->SetWindowTextW(strBuf);
}
//---------------------------------------------------------------------------
// 位置処理画像設定
void CJudgePageDlg::SetPatternImg()
{
	UCHAR* pcImg;

	// 画像取得
	pcImg = g_pclImgInsp->PatternImg();

	// 画像コピー
	for (int i = 0; i < m_lHeight; i++) {
		for (int j = 0; j < m_lWidth; j++) {
			m_pucBmp[4 * (i * m_lWidth + j) + 0] = pcImg[i * m_lWidth + j];
			m_pucBmp[4 * (i * m_lWidth + j) + 1] = pcImg[i * m_lWidth + j];
			m_pucBmp[4 * (i * m_lWidth + j) + 2] = pcImg[i * m_lWidth + j];
		}
	}
}
//---------------------------------------------------------------------------
// 画像処理画像取得
void CJudgePageDlg::SetJudgeImg()
{
	// パターンマッチング位置角度補正処理画像描画
	SetPatternImg();

	// 画像取得
	UCHAR* pcImg = g_pclImgInsp->JudgeImg();

	for (int a = 0; a < g_pclParam->stSet.stJudge.lNum; a++) {
		// 描画範囲
		ST_LONG_RECT stArea = g_pclParam->stSet.stJudge.stOne[a].stArea;

		// 画像コピー
		for (int i = stArea.ly; i < stArea.ly + stArea.lHeight; i++) {
			for (int j = stArea.lx; j < stArea.lx + stArea.lWidth; j++) {
				m_pucBmp[4 * (i * m_lWidth + j) + 0] = pcImg[i * m_lWidth + j];
				m_pucBmp[4 * (i * m_lWidth + j) + 1] = pcImg[i * m_lWidth + j];
				m_pucBmp[4 * (i * m_lWidth + j) + 2] = pcImg[i * m_lWidth + j];
			}
		}
	}

}
//---------------------------------------------------------------------------
// 画像表示処理
void CJudgePageDlg::ShowImg()
{
	// デバイスコンテキストにコピー
	m_clsPicDC->BitBlt(0, 0, m_lWidth, m_lHeight, &m_clsBmpDC, 0, 0, SRCCOPY);
}
//---------------------------------------------------------------------------
// 判定エリア表示
void CJudgePageDlg::DrawJudgeArea()
{
	// ペンを設定
	m_clsPicDC->SelectObject(&m_clsRectPen);

	for (int a = 0; a < g_pclParam->stSet.stJudge.lNum; a++) {
		// エリア表示
		m_clsPicDC->MoveTo(g_pclParam->stSet.stJudge.stOne[a].stArea.lx, g_pclParam->stSet.stJudge.stOne[a].stArea.ly);
		m_clsPicDC->LineTo(g_pclParam->stSet.stJudge.stOne[a].stArea.lx + g_pclParam->stSet.stJudge.stOne[a].stArea.lWidth, g_pclParam->stSet.stJudge.stOne[a].stArea.ly);
		m_clsPicDC->LineTo(g_pclParam->stSet.stJudge.stOne[a].stArea.lx + g_pclParam->stSet.stJudge.stOne[a].stArea.lWidth, g_pclParam->stSet.stJudge.stOne[a].stArea.ly + g_pclParam->stSet.stJudge.stOne[a].stArea.lHeight);
		m_clsPicDC->LineTo(g_pclParam->stSet.stJudge.stOne[a].stArea.lx, g_pclParam->stSet.stJudge.stOne[a].stArea.ly + g_pclParam->stSet.stJudge.stOne[a].stArea.lHeight);
		m_clsPicDC->LineTo(g_pclParam->stSet.stJudge.stOne[a].stArea.lx, g_pclParam->stSet.stJudge.stOne[a].stArea.ly);
	}
}
//---------------------------------------------------------------------------


