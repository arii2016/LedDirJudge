//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "afxwin.h"
//---------------------------------------------------------------------------
// CGrabPageDlg �_�C�A���O
class CGrabPageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGrabPageDlg)

public:
	CGrabPageDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CGrabPageDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_GRAB_PAGEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

	void GetGrabImg();
	void ShowImg();

public:
	long		m_lWidth;
	long		m_lHeight;

	CStatic		m_pcGrabImg;
	CDC*		m_clsPicDC;
    UCHAR*		m_pucBmp;
	HBITMAP		m_hBmp;
	CDC			m_clsBmpDC;
	CBitmap*	m_clsOldBmp;

	CEdit		m_edtExposure;

	afx_msg void OnBnClickedButtonGrab();
	afx_msg void OnBnClickedButtonLoadImg();
	afx_msg void OnBnClickedButtonSaveImg();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnEnKillfocusEditExposure();

	void ShowPara();
	afx_msg void OnBnClickedButtonContinGrabStart();
	afx_msg void OnBnClickedButtonContinGrabEnd();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
//---------------------------------------------------------------------------
