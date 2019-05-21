//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "Define.h"
//---------------------------------------------------------------------------
// ブラシ列挙型
typedef enum {
    EN_D2D_BRUSH_WHITE = 0,
    EN_D2D_BRUSH_GRAY,
    EN_D2D_BRUSH_BLACK,
    EN_D2D_BRUSH_LIME,
    EN_D2D_BRUSH_GREEN,
    EN_D2D_BRUSH_AQUA,
    EN_D2D_BRUSH_BLUE,
    EN_D2D_BRUSH_PINK,
    EN_D2D_BRUSH_PURPLE,
    EN_D2D_BRUSH_ORANGE,
    EN_D2D_BRUSH_RED,
    EN_D2D_BRUSH_BROWN,
    EN_D2D_BRUSH_YELLOW,
    EN_D2D_BRUSH_MAX
} EN_D2D_BRUSH;
// テキストフォーマット列挙型
typedef enum {
    EN_D2D_TEXT_FORMAT_9 = 0,
    EN_D2D_TEXT_FORMAT_12,
    EN_D2D_TEXT_FORMAT_16,
    EN_D2D_TEXT_FORMAT_24,
    EN_D2D_TEXT_FORMAT_36,
    EN_D2D_TEXT_FORMAT_48,
    EN_D2D_TEXT_FORMAT_72,
    EN_D2D_TEXT_FORMAT_MAX
} EN_D2D_TEXT_FORMAT;
//---------------------------------------------------------------------------
// 図形種類列挙型
typedef enum {
    EN_D2D_DIAGRAM_DOT = 0,		// 点
    EN_D2D_DIAGRAM_LINE,		// 線
    EN_D2D_DIAGRAM_RECT,		// 矩形
    EN_D2D_DIAGRAM_ELLIPSE,		// 楕円
    EN_D2D_DIAGRAM_MAX
} EN_D2D_DIAGRAM;
// 図形構造体
struct ST_D2D_DIAGRAM {
	// 有効・無効
	bool				bUseFlg;
	// 図形種類
	EN_D2D_DIAGRAM		enDiagram;
	// ブラシ
	EN_D2D_BRUSH		enBrush;
	// サイズ
	float				fStrokeWidth;

	// 点構造体
	D2D1_POINT_2F 			Point;
	// 線構造体
	D2D1_POINT_2F 			ptFrom;
	D2D1_POINT_2F 			ptTo;
	// 矩形構造体
	D2D1_RECT_F				Rect;
	// 楕円構造体
	D2D1_ELLIPSE 			Ellipse;

};
//---------------------------------------------------------------------------
// 文字列構造体
struct ST_D2D_TEXT {
	// 有効・無効
	bool				bUseFlg;
	// 表示文字列
	WCHAR				wcBuf[DF_CHAR_MAX];
	// ブラシ
	EN_D2D_BRUSH		enBrush;
	// テキストフォーマット
	EN_D2D_TEXT_FORMAT	enTextFormat;
	// 表示範囲
	D2D1_RECT_F			Rect;
};
// CD2DAutoCtrl
//---------------------------------------------------------------------------
class CD2DAutoCtrl : public CStatic
{
	DECLARE_DYNAMIC(CD2DAutoCtrl)

public:
	CD2DAutoCtrl();
	virtual ~CD2DAutoCtrl();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	afx_msg LRESULT OnDraw2D(WPARAM wParam, LPARAM lParam);

private:
	CD2DSolidColorBrush*	m_pBrush[EN_D2D_BRUSH_MAX];
	CD2DTextFormat*			m_pTextFormat[EN_D2D_TEXT_FORMAT_MAX];

	// 固定図形
	long			m_lConstDiagramCnt;
	ST_D2D_DIAGRAM*	m_pstConstDiagram;
	// 固定文字列
	long			m_lConstTextCnt;
	ST_D2D_TEXT*	m_pstConstText;


	CComQIPtr<ID2D1Bitmap>	m_bitmap;
	UCHAR*					m_pucImage;
	int						m_iPitch;

	// 図形
	long			m_lDiagramCnt;
	ST_D2D_DIAGRAM*	m_pstDiagram;

	// 文字列
	long			m_lTextCnt;
	ST_D2D_TEXT*	m_pstText;

public:
	void UpdateDraw();
	bool SetImgSize(D2D1_SIZE_U uSize);
	void DelImg();
	bool SetImgCopy(UCHAR* pucImage, long lColorNum);
	void ClearConstDiagram();
	void ClearDiagram();
	void ClearConstText();
	void ClearText();
	void AddDotDiagram(D2D1_POINT_2F point, EN_D2D_BRUSH en_brush, float f_strokeWidth);
	void AddLineDiagram(D2D1_POINT_2F pt_from, D2D1_POINT_2F pt_to, EN_D2D_BRUSH en_brush, float f_strokeWidth);
	void AddRectDiagram(D2D1_RECT_F rect, EN_D2D_BRUSH en_brush, float f_strokeWidth);
	void AddEllipseDiagram(D2D1_ELLIPSE ellipse, EN_D2D_BRUSH en_brush, float f_strokeWidth);
	void AddText(WCHAR* pwcText, EN_D2D_BRUSH en_brush, EN_D2D_TEXT_FORMAT en_text_format, D2D1_RECT_F rect);
};
//---------------------------------------------------------------------------
