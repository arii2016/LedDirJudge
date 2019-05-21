// AutoPageDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "LedDirJudge.h"
#include "AutoPageDlg.h"
#include "afxdialogex.h"

#include "Main.h"

// CAutoPageDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CAutoPageDlg, CDialogEx)

CAutoPageDlg::CAutoPageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoPageDlg::IDD, pParent)
{

}

CAutoPageDlg::~CAutoPageDlg()
{
}

void CAutoPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_AUTO_D2D, m_clAutoD2D);
}


BEGIN_MESSAGE_MAP(CAutoPageDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAutoPageDlg ���b�Z�[�W �n���h���[


BOOL CAutoPageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������
	g_pclMain->SetAutoD2D(&m_clAutoD2D);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CAutoPageDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
}


void CAutoPageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	CWnd* pD2d;
	CRect rect = CRect(0, 0, cx, cy);

	pD2d = GetDlgItem(IDC_STATIC_AUTO_D2D);
	if (pD2d != NULL) {
		pD2d->MoveWindow(&rect, TRUE);
	}
}

