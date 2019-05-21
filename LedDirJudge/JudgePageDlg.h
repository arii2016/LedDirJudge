//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "afxwin.h"
#include "Define.h"
//---------------------------------------------------------------------------
// CJudgePageDlg ダイアログ
class CJudgePageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CJudgePageDlg)

public:
	CJudgePageDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CJudgePageDlg();

// ダイアログ データ
	enum { IDD = IDD_JUDGE_PAGEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

	bool		m_bMouseOn;
	long		m_lSelectNo;

	void SetPatternImg();
	void SetJudgeImg();
	void ShowImg();
	void DrawJudgeArea();

public:
	long		m_lWidth;
	long		m_lHeight;

	CStatic		m_pcJudgeImg;
	CDC*		m_clsPicDC;
    UCHAR*		m_pucBmp;
	HBITMAP		m_hBmp;
	CDC			m_clsBmpDC;
	CBitmap*	m_clsOldBmp;
	CPen		m_clsRectPen;

	CComboBox	m_cbNum;
	CComboBox	m_cbSelectNo;
	CEdit		m_edtLumiVal;

	CEdit m_edtThrLo1;
	CEdit m_edtThrLo2;
	CEdit m_edtThrLo3;
	CEdit m_edtThrLo4;
	CEdit m_edtThrLo5;
	CEdit* m_pedtThrLo[DF_JUDGE_NUM_MAX];

	CEdit m_edtThrHi1;
	CEdit m_edtThrHi2;
	CEdit m_edtThrHi3;
	CEdit m_edtThrHi4;
	CEdit m_edtThrHi5;
	CEdit* m_pedtThrHi[DF_JUDGE_NUM_MAX];

	CEdit m_edtAreaRangeLo1;
	CEdit m_edtAreaRangeLo2;
	CEdit m_edtAreaRangeLo3;
	CEdit m_edtAreaRangeLo4;
	CEdit m_edtAreaRangeLo5;
	CEdit* m_pedtAreaRangeLo[DF_JUDGE_NUM_MAX];

	CEdit m_edtAreaRangeHi1;
	CEdit m_edtAreaRangeHi2;
	CEdit m_edtAreaRangeHi3;
	CEdit m_edtAreaRangeHi4;
	CEdit m_edtAreaRangeHi5;
	CEdit* m_pedtAreaRangeHi[DF_JUDGE_NUM_MAX];

	CEdit m_edtAreaNumResult1;
	CEdit m_edtAreaNumResult2;
	CEdit m_edtAreaNumResult3;
	CEdit m_edtAreaNumResult4;
	CEdit m_edtAreaNumResult5;
	CEdit* m_pedtAreaNumResult[DF_JUDGE_NUM_MAX];

	CEdit m_edtJudgeResult1;
	CEdit m_edtJudgeResult2;
	CEdit m_edtJudgeResult3;
	CEdit m_edtJudgeResult4;
	CEdit m_edtJudgeResult5;
	CEdit* m_pedtJudgeResult[DF_JUDGE_NUM_MAX];

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonShowOrgImg();
	afx_msg void OnBnClickedButtonJudge();
	afx_msg void OnCbnSelchangeComboNum();
	afx_msg void OnCbnSelchangeComboSelectNo();

	afx_msg void OnEnKillfocusEditThrLo1();
	afx_msg void OnEnKillfocusEditThrLo2();
	afx_msg void OnEnKillfocusEditThrLo3();
	afx_msg void OnEnKillfocusEditThrLo4();
	afx_msg void OnEnKillfocusEditThrLo5();
	void OnEnKillfocusEditThrLo(int iNo);

	afx_msg void OnEnKillfocusEditThrHi1();
	afx_msg void OnEnKillfocusEditThrHi2();
	afx_msg void OnEnKillfocusEditThrHi3();
	afx_msg void OnEnKillfocusEditThrHi4();
	afx_msg void OnEnKillfocusEditThrHi5();
	void OnEnKillfocusEditThrHi(int iNo);

	afx_msg void OnEnKillfocusEditAreaRangeLo1();
	afx_msg void OnEnKillfocusEditAreaRangeLo2();
	afx_msg void OnEnKillfocusEditAreaRangeLo3();
	afx_msg void OnEnKillfocusEditAreaRangeLo4();
	afx_msg void OnEnKillfocusEditAreaRangeLo5();
	void OnEnKillfocusEditAreaRangeLo(int iNo);

	afx_msg void OnEnKillfocusEditAreaRangeHi1();
	afx_msg void OnEnKillfocusEditAreaRangeHi2();
	afx_msg void OnEnKillfocusEditAreaRangeHi3();
	afx_msg void OnEnKillfocusEditAreaRangeHi4();
	afx_msg void OnEnKillfocusEditAreaRangeHi5();
	void OnEnKillfocusEditAreaRangeHi(int iNo);

	void ShowPara();


};
//---------------------------------------------------------------------------
