//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "afxwin.h"
//---------------------------------------------------------------------------
// CPatternPageDlg ダイアログ
class CPatternPageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPatternPageDlg)

public:
	CPatternPageDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CPatternPageDlg();

// ダイアログ データ
	enum { IDD = IDD_PATTERN_PAGEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

	bool			m_bMouseOn;

	void SetGrabImg();
	void ShowImg();
	void DrawPatternArea();
	void DrawDtectArea();
	void SetTemplateImg();
	void ShowTemplateImg();
	void DeletTemplateBmp();

public:
	long		m_lWidth;
	long		m_lHeight;

	CStatic		m_pcPatternImg;
	CDC*		m_clsPicDC;
    UCHAR*		m_pucBmp;
	HBITMAP		m_hBmp;
	CDC			m_clsBmpDC;
	CBitmap*	m_clsOldBmp;
	CPen		m_clsPatternRectPen;
	CPen		m_clsDtectRectPen;

	CStatic		m_pcTemplateImg;
	CDC*		m_clsTemplatePicDC;
    UCHAR*		m_pucTemplateBmp;
	HBITMAP		m_hTemplateBmp;
	CDC*		m_pclsTemplateBmpDC;
	CBitmap*	m_clsTemplateOldBmp;

	CComboBox m_cbAngleRangeLo;
	CComboBox m_cbAngleRangeHi;
	CEdit m_edtCorrThr;

	CEdit m_edtDtectPosX;
	CEdit m_edtDtectPosY;
	CEdit m_edtDtectAngle;
	CEdit m_edtCorrVal;

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonShowOrgImg();
	afx_msg void OnBnClickedButtonPatternRegist();
	afx_msg void OnBnClickedButtonPattern();
	afx_msg void OnCbnSelchangeComboAngleRangeLo();
	afx_msg void OnCbnSelchangeComboAngleRangeHi();
	afx_msg void OnEnKillfocusEditCorrThr();

	void ShowPara();
};
//---------------------------------------------------------------------------
