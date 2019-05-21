//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "Define.h"
//---------------------------------------------------------------------------
// �u���V�񋓌^
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
// �e�L�X�g�t�H�[�}�b�g�񋓌^
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
// �}�`��ޗ񋓌^
typedef enum {
    EN_D2D_DIAGRAM_DOT = 0,		// �_
    EN_D2D_DIAGRAM_LINE,		// ��
    EN_D2D_DIAGRAM_RECT,		// ��`
    EN_D2D_DIAGRAM_ELLIPSE,		// �ȉ~
    EN_D2D_DIAGRAM_MAX
} EN_D2D_DIAGRAM;
// �}�`�\����
struct ST_D2D_DIAGRAM {
	// �L���E����
	bool				bUseFlg;
	// �}�`���
	EN_D2D_DIAGRAM		enDiagram;
	// �u���V
	EN_D2D_BRUSH		enBrush;
	// �T�C�Y
	float				fStrokeWidth;

	// �_�\����
	D2D1_POINT_2F 			Point;
	// ���\����
	D2D1_POINT_2F 			ptFrom;
	D2D1_POINT_2F 			ptTo;
	// ��`�\����
	D2D1_RECT_F				Rect;
	// �ȉ~�\����
	D2D1_ELLIPSE 			Ellipse;

};
//---------------------------------------------------------------------------
// ������\����
struct ST_D2D_TEXT {
	// �L���E����
	bool				bUseFlg;
	// �\��������
	WCHAR				wcBuf[DF_CHAR_MAX];
	// �u���V
	EN_D2D_BRUSH		enBrush;
	// �e�L�X�g�t�H�[�}�b�g
	EN_D2D_TEXT_FORMAT	enTextFormat;
	// �\���͈�
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

	// �Œ�}�`
	long			m_lConstDiagramCnt;
	ST_D2D_DIAGRAM*	m_pstConstDiagram;
	// �Œ蕶����
	long			m_lConstTextCnt;
	ST_D2D_TEXT*	m_pstConstText;


	CComQIPtr<ID2D1Bitmap>	m_bitmap;
	UCHAR*					m_pucImage;
	int						m_iPitch;

	// �}�`
	long			m_lDiagramCnt;
	ST_D2D_DIAGRAM*	m_pstDiagram;

	// ������
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
