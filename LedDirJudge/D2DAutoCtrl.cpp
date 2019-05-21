// D2DAutoCtrl.cpp : 実装ファイル
//
#include "stdafx.h"
#include "D2DAutoCtrl.h"
//---------------------------------------------------------------------------
// CD2DAutoCtrl
//---------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CD2DAutoCtrl, CStatic)
//---------------------------------------------------------------------------
CD2DAutoCtrl::CD2DAutoCtrl()
{
	m_pstConstDiagram = new ST_D2D_DIAGRAM[DF_DIAGRAM_MAX];
	m_pstConstText = new ST_D2D_TEXT[DF_D2D_TEXT_MAX];
	ClearConstDiagram();
	ClearConstText();


	m_pucImage = NULL;

	m_pstDiagram = new ST_D2D_DIAGRAM[DF_DIAGRAM_MAX];
	m_pstText = new ST_D2D_TEXT[DF_D2D_TEXT_MAX];

	ClearDiagram();
	ClearText();

}
//---------------------------------------------------------------------------
CD2DAutoCtrl::~CD2DAutoCtrl()
{
	DelImg();

	delete m_pstDiagram;
	delete m_pstText;

	delete m_pstConstDiagram;
	delete m_pstConstText;
}
//---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CD2DAutoCtrl, CStatic)
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CD2DAutoCtrl::OnDraw2D)
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CD2DAutoCtrl メッセージ ハンドラー
//---------------------------------------------------------------------------
void CD2DAutoCtrl::PreSubclassWindow()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	// Enable D2D support for this window:
	EnableD2DSupport();

	// ブラシ作成
	m_pBrush[EN_D2D_BRUSH_WHITE] = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::White));
	m_pBrush[EN_D2D_BRUSH_GRAY] = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Gray));
	m_pBrush[EN_D2D_BRUSH_BLACK] = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Black));
	m_pBrush[EN_D2D_BRUSH_LIME] = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Lime));
	m_pBrush[EN_D2D_BRUSH_GREEN] = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Green));
	m_pBrush[EN_D2D_BRUSH_AQUA] = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Aqua));
	m_pBrush[EN_D2D_BRUSH_BLUE] = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Blue));
	m_pBrush[EN_D2D_BRUSH_PINK] = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Pink));
	m_pBrush[EN_D2D_BRUSH_PURPLE] = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Purple));
	m_pBrush[EN_D2D_BRUSH_ORANGE] = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Orange));
	m_pBrush[EN_D2D_BRUSH_RED] = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Red));
	m_pBrush[EN_D2D_BRUSH_BROWN] = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Brown));
	m_pBrush[EN_D2D_BRUSH_YELLOW] = new CD2DSolidColorBrush(GetRenderTarget(), D2D1::ColorF(D2D1::ColorF::Yellow));

	// テキストフォーマット
	m_pTextFormat[EN_D2D_TEXT_FORMAT_9] = new CD2DTextFormat(GetRenderTarget(), _T("Verdana"), 9);
	m_pTextFormat[EN_D2D_TEXT_FORMAT_12] = new CD2DTextFormat(GetRenderTarget(), _T("Verdana"), 12);
	m_pTextFormat[EN_D2D_TEXT_FORMAT_16] = new CD2DTextFormat(GetRenderTarget(), _T("Verdana"), 16);
	m_pTextFormat[EN_D2D_TEXT_FORMAT_24] = new CD2DTextFormat(GetRenderTarget(), _T("Verdana"), 24);
	m_pTextFormat[EN_D2D_TEXT_FORMAT_36] = new CD2DTextFormat(GetRenderTarget(), _T("Verdana"), 36);
	m_pTextFormat[EN_D2D_TEXT_FORMAT_48] = new CD2DTextFormat(GetRenderTarget(), _T("Verdana"), 48);
	m_pTextFormat[EN_D2D_TEXT_FORMAT_72] = new CD2DTextFormat(GetRenderTarget(), _T("Verdana"), 72);
	//--------------------------------------------------------------------------------------------------------------------------------------------------------

	CStatic::PreSubclassWindow();
}
//---------------------------------------------------------------------------
void CD2DAutoCtrl::OnDestroy()
{
	CStatic::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
}
//---------------------------------------------------------------------------
afx_msg LRESULT CD2DAutoCtrl::OnDraw2D(WPARAM wParam, LPARAM lParam)
{
    CHwndRenderTarget* pRenderTarget = (CHwndRenderTarget*)lParam;
    ASSERT_VALID(pRenderTarget);

	// 画像表示
	pRenderTarget->GetHwndRenderTarget()->DrawBitmap(m_bitmap, D2D1::RectF(0.0, 0.0, m_bitmap->GetSize().width, m_bitmap->GetSize().height));

	// 固定図形表示
	for (int i = 0; i < m_lConstDiagramCnt; i++) {
		if (m_pstConstDiagram[i].bUseFlg == false) {
			continue;
		}
		switch (m_pstConstDiagram[i].enDiagram) {
		case EN_D2D_DIAGRAM_DOT:
			pRenderTarget->FillEllipse(D2D1::Ellipse(m_pstConstDiagram[i].Point, m_pstConstDiagram[i].fStrokeWidth, m_pstConstDiagram[i].fStrokeWidth), m_pBrush[m_pstConstDiagram[i].enBrush]);
			break;
		case EN_D2D_DIAGRAM_LINE:
			pRenderTarget->DrawLine(m_pstConstDiagram[i].ptFrom, m_pstConstDiagram[i].ptTo, m_pBrush[m_pstConstDiagram[i].enBrush], m_pstConstDiagram[i].fStrokeWidth);
			break;
		case EN_D2D_DIAGRAM_RECT:
			pRenderTarget->DrawRectangle(m_pstConstDiagram[i].Rect, m_pBrush[m_pstConstDiagram[i].enBrush], m_pstConstDiagram[i].fStrokeWidth);
			break;
		case EN_D2D_DIAGRAM_ELLIPSE:
			pRenderTarget->DrawEllipse(m_pstConstDiagram[i].Ellipse, m_pBrush[m_pstConstDiagram[i].enBrush], m_pstConstDiagram[i].fStrokeWidth);
			break;
		}
	}
	// 図形表示
	for (int i = 0; i < m_lDiagramCnt; i++) {
		if (m_pstDiagram[i].bUseFlg == false) {
			continue;
		}
		switch (m_pstDiagram[i].enDiagram) {
		case EN_D2D_DIAGRAM_DOT:
			pRenderTarget->FillEllipse(D2D1::Ellipse(m_pstDiagram[i].Point, m_pstDiagram[i].fStrokeWidth, m_pstDiagram[i].fStrokeWidth), m_pBrush[m_pstDiagram[i].enBrush]);
			break;
		case EN_D2D_DIAGRAM_LINE:
			pRenderTarget->DrawLine(m_pstDiagram[i].ptFrom, m_pstDiagram[i].ptTo, m_pBrush[m_pstDiagram[i].enBrush], m_pstDiagram[i].fStrokeWidth);
			break;
		case EN_D2D_DIAGRAM_RECT:
			pRenderTarget->DrawRectangle(m_pstDiagram[i].Rect, m_pBrush[m_pstDiagram[i].enBrush], m_pstDiagram[i].fStrokeWidth);
			break;
		case EN_D2D_DIAGRAM_ELLIPSE:
			pRenderTarget->DrawEllipse(m_pstDiagram[i].Ellipse, m_pBrush[m_pstDiagram[i].enBrush], m_pstDiagram[i].fStrokeWidth);
			break;
		}
	}

	// 固定文字列表示
	for (int i = 0; i < m_lConstTextCnt; i++) {
		if (m_pstConstText[i].bUseFlg == false) {
			continue;
		}
		pRenderTarget->DrawText(m_pstConstText[i].wcBuf, m_pstConstText[i].Rect, m_pBrush[m_pstConstText[i].enBrush], m_pTextFormat[m_pstConstText[i].enTextFormat]);
	}
	// 文字列表示
	for (int i = 0; i < m_lTextCnt; i++) {
		if (m_pstText[i].bUseFlg == false) {
			continue;
		}
		pRenderTarget->DrawText(m_pstText[i].wcBuf, m_pstText[i].Rect, m_pBrush[m_pstText[i].enBrush], m_pTextFormat[m_pstText[i].enTextFormat]);
	}

	// 画面サイズに拡大・縮小する
    CRect CliRect;
    GetClientRect(CliRect);
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale((float)CliRect.Width() / (float)m_bitmap->GetSize().width, (float)CliRect.Height() / (float)m_bitmap->GetSize().height));

	return TRUE;
}
//---------------------------------------------------------------------------
void CD2DAutoCtrl::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	UpdateDraw();
}
//---------------------------------------------------------------------------
// 描画
void CD2DAutoCtrl::UpdateDraw()
{
    Invalidate(FALSE);
}
//---------------------------------------------------------------------------
// 画像サイズ設定
bool CD2DAutoCtrl::SetImgSize(D2D1_SIZE_U uSize)
{
	if (FAILED(GetRenderTarget()->GetHwndRenderTarget()->CreateBitmap(uSize, D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)), &m_bitmap))) {
		return false;
	}

	DelImg();
	m_pucImage = new UCHAR[uSize.width * uSize.height * 4];
	m_iPitch = uSize.width * 4;

	return true;
}
//---------------------------------------------------------------------------
void CD2DAutoCtrl::DelImg()
{
	if (m_pucImage != NULL) {
		delete m_pucImage;
		m_pucImage = NULL;
	}
}
//---------------------------------------------------------------------------
// 画像設定
bool CD2DAutoCtrl::SetImgCopy(UCHAR* pucImage, long lColorNum)
{
	int iW = (int)m_bitmap->GetSize().width;
	int iH = (int)m_bitmap->GetSize().height;

	switch (lColorNum) {
	case DF_COLOR_NUM_GRAY:
#ifdef _OPENMP
#pragma omp parallel for
#endif
		for (int i = 0; i < iH; i++) {
			for (int j = 0; j < iW; j++) {
				m_pucImage[4 * (i * iW + j) + 0] = pucImage[i * iW + j];
				m_pucImage[4 * (i * iW + j) + 1] = pucImage[i * iW + j];
				m_pucImage[4 * (i * iW + j) + 2] = pucImage[i * iW + j];
				m_pucImage[4 * (i * iW + j) + 3] = 0;
			}
		}
		break;
	case DF_COLOR_NUM_RGB:
#ifdef _OPENMP
#pragma omp parallel for
#endif
		for (int i = 0; i < iH; i++) {
			for (int j = 0; j < iW; j++) {
				m_pucImage[4 * (i * iW + j) + 0] = pucImage[3 * (i * iW + j) + 0];
				m_pucImage[4 * (i * iW + j) + 1] = pucImage[3 * (i * iW + j) + 1];
				m_pucImage[4 * (i * iW + j) + 2] = pucImage[3 * (i * iW + j) + 2];
				m_pucImage[4 * (i * iW + j) + 3] = 0;
			}
		}
		break;
	}

	if (FAILED(m_bitmap->CopyFromMemory(&D2D1::RectU(0, 0, (UINT32)m_bitmap->GetSize().width, (UINT32)m_bitmap->GetSize().height), m_pucImage, m_iPitch))) {
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
void CD2DAutoCtrl::ClearConstDiagram()
{
	m_lConstDiagramCnt = 0;
	for (int i = 0 ; i < DF_DIAGRAM_MAX; i++) {
		m_pstConstDiagram[i].bUseFlg = false;
	}
}
//---------------------------------------------------------------------------
void CD2DAutoCtrl::ClearDiagram()
{
	m_lDiagramCnt = 0;
	for (int i = 0 ; i < DF_DIAGRAM_MAX; i++) {
		m_pstDiagram[i].bUseFlg = false;
	}
}
//---------------------------------------------------------------------------
void CD2DAutoCtrl::ClearConstText()
{
	m_lConstTextCnt = 0;
	for (int i = 0 ; i < DF_D2D_TEXT_MAX; i++) {
		m_pstConstText[i].bUseFlg = false;
	}
}
//---------------------------------------------------------------------------
void CD2DAutoCtrl::ClearText()
{
	m_lTextCnt = 0;
	for (int i = 0 ; i < DF_D2D_TEXT_MAX; i++) {
		m_pstText[i].bUseFlg = false;
	}
}
//---------------------------------------------------------------------------
void CD2DAutoCtrl::AddDotDiagram( D2D1_POINT_2F point, EN_D2D_BRUSH en_brush, float f_strokeWidth)
{
	m_pstDiagram[m_lDiagramCnt].bUseFlg = true;
	m_pstDiagram[m_lDiagramCnt].enDiagram = EN_D2D_DIAGRAM_DOT;
	m_pstDiagram[m_lDiagramCnt].Point = point;
	m_pstDiagram[m_lDiagramCnt].enBrush = en_brush;
	m_pstDiagram[m_lDiagramCnt].fStrokeWidth = f_strokeWidth;
	m_lDiagramCnt++;
}
//---------------------------------------------------------------------------
void CD2DAutoCtrl::AddLineDiagram(D2D1_POINT_2F pt_from, D2D1_POINT_2F pt_to, EN_D2D_BRUSH en_brush, float f_strokeWidth)
{
	m_pstDiagram[m_lDiagramCnt].bUseFlg = true;
	m_pstDiagram[m_lDiagramCnt].enDiagram = EN_D2D_DIAGRAM_LINE;
	m_pstDiagram[m_lDiagramCnt].ptFrom = pt_from;
	m_pstDiagram[m_lDiagramCnt].ptTo = pt_to;
	m_pstDiagram[m_lDiagramCnt].enBrush = en_brush;
	m_pstDiagram[m_lDiagramCnt].fStrokeWidth = f_strokeWidth;
	m_lDiagramCnt++;
}
//---------------------------------------------------------------------------
void CD2DAutoCtrl::AddRectDiagram(D2D1_RECT_F rect, EN_D2D_BRUSH en_brush, float f_strokeWidth)
{
	m_pstDiagram[m_lDiagramCnt].bUseFlg = true;
	m_pstDiagram[m_lDiagramCnt].enDiagram = EN_D2D_DIAGRAM_RECT;
	m_pstDiagram[m_lDiagramCnt].Rect = rect;
	m_pstDiagram[m_lDiagramCnt].enBrush = en_brush;
	m_pstDiagram[m_lDiagramCnt].fStrokeWidth = f_strokeWidth;
	m_lDiagramCnt++;
}
//---------------------------------------------------------------------------
void CD2DAutoCtrl::AddEllipseDiagram(D2D1_ELLIPSE ellipse, EN_D2D_BRUSH en_brush, float f_strokeWidth)
{
	m_pstDiagram[m_lDiagramCnt].bUseFlg = true;
	m_pstDiagram[m_lDiagramCnt].enDiagram = EN_D2D_DIAGRAM_ELLIPSE;
	m_pstDiagram[m_lDiagramCnt].Ellipse = ellipse;
	m_pstDiagram[m_lDiagramCnt].enBrush = en_brush;
	m_pstDiagram[m_lDiagramCnt].fStrokeWidth = f_strokeWidth;
	m_lDiagramCnt++;
}
//---------------------------------------------------------------------------
void CD2DAutoCtrl::AddText(WCHAR* pwcText, EN_D2D_BRUSH en_brush, EN_D2D_TEXT_FORMAT en_text_format, D2D1_RECT_F rect)
{
	m_pstText[m_lTextCnt].bUseFlg = true;
	wcscpy_s(m_pstText[m_lTextCnt].wcBuf, DF_CHAR_MAX, pwcText);
	m_pstText[m_lTextCnt].enBrush = en_brush;
	m_pstText[m_lTextCnt].enTextFormat = en_text_format;
	m_pstText[m_lTextCnt].Rect = rect;
	m_lTextCnt++;
}
//---------------------------------------------------------------------------
