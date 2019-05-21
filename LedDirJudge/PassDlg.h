//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "afxwin.h"
//---------------------------------------------------------------------------
// CPassDlg �_�C�A���O
class CPassDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPassDlg)

public:
	CPassDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CPassDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PASSWORD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

private:
	bool		m_bPassOKFlg;

public:
	CEdit	m_edtPassword;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	bool ShowPassDlg();
};
//---------------------------------------------------------------------------
