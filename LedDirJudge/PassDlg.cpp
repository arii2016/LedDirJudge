// PassDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "LedDirJudge.h"
#include "PassDlg.h"
#include "afxdialogex.h"
#include "SysParam.h"

// CPassDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CPassDlg, CDialogEx)

CPassDlg::CPassDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPassDlg::IDD, pParent)
{
}

CPassDlg::~CPassDlg()
{
}

void CPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtPassword);
}


BEGIN_MESSAGE_MAP(CPassDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPassDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPassDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPassDlg ���b�Z�[�W �n���h���[
//---------------------------------------------------------------------------
BOOL CPassDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
//---------------------------------------------------------------------------
void CPassDlg::OnBnClickedOk()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CStringW	strBuf;
	m_edtPassword.GetWindowTextW(strBuf);

	if (wcsncmp(g_pclSysParam->stOhter.wcPassword, strBuf.GetBuffer(), DF_CHAR_MAX) == 0) {
		m_bPassOKFlg = true;
	}

	CDialogEx::OnOK();
}
//---------------------------------------------------------------------------
void CPassDlg::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialogEx::OnCancel();
}
//---------------------------------------------------------------------------
bool CPassDlg::ShowPassDlg()
{
	m_bPassOKFlg = false;

	if (this->DoModal() != IDOK) {
		return false;
	}
	if (m_bPassOKFlg == false) {
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------


