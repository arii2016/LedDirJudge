//---------------------------------------------------------------------------
// LedDirJudgeDlg.h : �w�b�_�[ �t�@�C��
//
//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "autopagedlg.h"
#include "manualpagedlg.h"
#include "PassDlg.h"
#include "Define.h"
//---------------------------------------------------------------------------
// CLedDirJudgeDlg �_�C�A���O
class CLedDirJudgeDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CLedDirJudgeDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[
	~CLedDirJudgeDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_LEDDIRJUDGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CAutoPageDlg	m_clAutoPage;
	CManualPageDlg	m_clManualPage;
	CPassDlg		m_clPassDlg;

	afx_msg void OnFileExit();
	afx_msg void OnModeAuto();
	afx_msg void OnModeManual();
	afx_msg void OnTestCamera();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileLoadpara();
	afx_msg void OnFileSavepara();
	afx_msg void OnTestProcimage();
};
//---------------------------------------------------------------------------
