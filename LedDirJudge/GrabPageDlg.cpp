// GrabPageDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "LedDirJudge.h"
#include "GrabPageDlg.h"
#include "afxdialogex.h"
#include "Main.h"
#include "Cammera.h"
#include "Param.h"

// CGrabPageDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CGrabPageDlg, CDialogEx)

CGrabPageDlg::CGrabPageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGrabPageDlg::IDD, pParent)
{
}

CGrabPageDlg::~CGrabPageDlg()
{
}

void CGrabPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GRAB_IMG, m_pcGrabImg);
	DDX_Control(pDX, IDC_EDIT_EXPOSURE, m_edtExposure);
}


BEGIN_MESSAGE_MAP(CGrabPageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GRAB, &CGrabPageDlg::OnBnClickedButtonGrab)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_IMG, &CGrabPageDlg::OnBnClickedButtonLoadImg)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_IMG, &CGrabPageDlg::OnBnClickedButtonSaveImg)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_EDIT_EXPOSURE, &CGrabPageDlg::OnEnKillfocusEditExposure)
	ON_BN_CLICKED(IDC_BUTTON_CONTIN_GRAB_START, &CGrabPageDlg::OnBnClickedButtonContinGrabStart)
	ON_BN_CLICKED(IDC_BUTTON_CONTIN_GRAB_END, &CGrabPageDlg::OnBnClickedButtonContinGrabEnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGrabPageDlg ���b�Z�[�W �n���h���[
//---------------------------------------------------------------------------
BOOL CGrabPageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������
	
	// �摜�T�C�Y�擾
	m_lWidth = g_pclCammera->Width();
	m_lHeight = g_pclCammera->Height();

	// Picture Control�̃f�o�C�X�R���e�L�X�g�擾
	m_clsPicDC = m_pcGrabImg.GetDC();

	// �摜�T�C�Y�ݒ�
	CRect rect;
	rect.top = 5;
	rect.left = 5;
	rect.right = rect.left + m_lWidth;
	rect.bottom = rect.top + m_lHeight;
	m_pcGrabImg.MoveWindow(&rect, TRUE);

	// �摜�\���p�r�b�g�}�b�v�̍쐬
	BITMAPINFO binfo;
    ZeroMemory(&binfo, sizeof(binfo));
    binfo.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
    binfo.bmiHeader.biBitCount = 32;
    binfo.bmiHeader.biPlanes   =  1;
    binfo.bmiHeader.biWidth    =  m_lWidth;
    binfo.bmiHeader.biHeight   =  -(m_lHeight);

	// m_hBmp�Ɋ֘A�t���� CBitmap�ƁACDC���쐬
	m_hBmp = CreateDIBSection( NULL, &binfo, DIB_RGB_COLORS, (void **)(&m_pucBmp), NULL, 0); 
	CBitmap* clsBmp = CBitmap::FromHandle(m_hBmp);
    m_clsBmpDC.CreateCompatibleDC(m_clsPicDC);
    m_clsOldBmp = m_clsBmpDC.SelectObject(clsBmp); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
//---------------------------------------------------------------------------
void CGrabPageDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	m_pcGrabImg.ReleaseDC(m_clsPicDC);
	m_clsBmpDC.SelectObject(m_clsOldBmp);
    DeleteDC(m_clsBmpDC);
    DeleteObject(m_hBmp);
}
//---------------------------------------------------------------------------
// �B��
void CGrabPageDlg::OnBnClickedButtonGrab()
{
	// �B�e
	if (g_pclMain->Grab() == false) {
		::MessageBox(NULL, _T("�B�e���s"), _T("�B�e���s"), MB_OK | MB_ICONWARNING);
		return;
	}

	// �B���摜�\��
	GetGrabImg();
	ShowImg();
}
//---------------------------------------------------------------------------
// �A���B���J�n
void CGrabPageDlg::OnBnClickedButtonContinGrabStart()
{
	// �B���ƘA���J�n��񊈐��ɂ���
	CButton *button;

	button = (CButton*)GetDlgItem(IDC_BUTTON_GRAB);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_BUTTON_CONTIN_GRAB_START);
	button->EnableWindow(FALSE);

	SetTimer(1, 10, NULL);
}
//---------------------------------------------------------------------------
// �A���B����~
void CGrabPageDlg::OnBnClickedButtonContinGrabEnd()
{
	KillTimer(1);

	// �B���ƘA���J�n�������ɂ���
	CButton *button;

	button = (CButton*)GetDlgItem(IDC_BUTTON_GRAB);
	button->EnableWindow(TRUE);
	button = (CButton*)GetDlgItem(IDC_BUTTON_CONTIN_GRAB_START);
	button->EnableWindow(TRUE);
}
//---------------------------------------------------------------------------
void CGrabPageDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	// �B�e
	if (g_pclMain->Grab() == false) {
		::MessageBox(NULL, _T("�B�e���s"), _T("�B�e���s"), MB_OK | MB_ICONWARNING);

		OnBnClickedButtonContinGrabEnd();
		return;
	}
	// �B���摜�\��
	GetGrabImg();
	ShowImg();

	CDialogEx::OnTimer(nIDEvent);
}
//---------------------------------------------------------------------------
// �摜�ǂݍ���
void CGrabPageDlg::OnBnClickedButtonLoadImg()
{
	// �t�@�C��Dlg�\��
	CFileDialog clsFileDlg(TRUE, NULL, L"test.bmp", OFN_FILEMUSTEXIST  | OFN_HIDEREADONLY, L"BMP̧�� | *.bmp|", this);
    if (clsFileDlg.DoModal() != IDOK ){
        return;
    }
	CStringW strFilePath = clsFileDlg.GetPathName();

	// �摜�ǂݍ���
	if (g_pclMain->LoadImage((WCHAR*)strFilePath.GetString()) == false) {
		::MessageBox(NULL, _T("�ǂݍ��ݎ��s"), _T("�ǂݍ��ݎ��s"), MB_OK | MB_ICONWARNING);
		return;
	}

	// �B���摜�\��
	GetGrabImg();
	ShowImg();
}
//---------------------------------------------------------------------------
// �摜�ۑ�
void CGrabPageDlg::OnBnClickedButtonSaveImg()
{

	// �t�@�C��Dlg�\��
	CFileDialog clsFileDlg(FALSE, L"bmp", L"test.bmp", OFN_OVERWRITEPROMPT, L"BMP̧�� | *.bmp|", this);
    if (clsFileDlg.DoModal() != IDOK ){
        return;
    }
	CStringW strFilePath = clsFileDlg.GetPathName();
	
	CImage image;
    CBitmap bmpForSave;
    CDC cdcForSave;

	bmpForSave.CreateCompatibleBitmap(m_clsPicDC, m_lWidth, m_lHeight);
    cdcForSave.CreateCompatibleDC(m_clsPicDC);
    cdcForSave.SelectObject(bmpForSave);
    cdcForSave.BitBlt(0, 0, m_lWidth, m_lHeight, m_clsPicDC, 0, 0, SRCCOPY);

	image.Attach((HBITMAP)bmpForSave);

	image.Save(strFilePath);
    image.Detach();
}
//---------------------------------------------------------------------------
// �p�����[�^�[�ݒ�
void CGrabPageDlg::ShowPara()
{
	CStringW	strBuf;

	// �I������
	strBuf.Format(L"%d", g_pclParam->stSet.stGrab.lExposure);
	m_edtExposure.SetWindowTextW(strBuf);
}
//---------------------------------------------------------------------------
// �I������
void CGrabPageDlg::OnEnKillfocusEditExposure()
{
	CStringW	strBuf;
	long		lBuf;

	// �l�擾
	m_edtExposure.GetWindowTextW(strBuf);
	lBuf = _wtoi(strBuf.GetBuffer());
	if (lBuf < 1) {
		::MessageBox(NULL, _T("�I�����Ԃ�1�ȏ��ݒ肵�Ă��������B"), _T("�ݒ�G���["), MB_OK | MB_ICONWARNING);
		m_edtExposure.SetFocus();
		return;		
	}
	g_pclParam->stSet.stGrab.lExposure = lBuf;
	// �\��
	strBuf.Format(L"%d", g_pclParam->stSet.stGrab.lExposure);
	m_edtExposure.SetWindowTextW(strBuf);
	// �J�����ɘI�����Ԑݒ�
	g_pclCammera->SetExposure(g_pclParam->stSet.stGrab.lExposure);
}
//---------------------------------------------------------------------------
// �B���摜�擾
void CGrabPageDlg::GetGrabImg()
{
	UCHAR* pcImg;

	// �摜�擾
	pcImg = g_pclCammera->GrabImg();

	// �摜�R�s�[
	for (int i = 0; i < m_lHeight; i++) {
		for (int j = 0; j < m_lWidth; j++) {
			m_pucBmp[4 * (i * m_lWidth + j) + 0] = pcImg[3 * (i * m_lWidth + j) + 0];
			m_pucBmp[4 * (i * m_lWidth + j) + 1] = pcImg[3 * (i * m_lWidth + j) + 1];
			m_pucBmp[4 * (i * m_lWidth + j) + 2] = pcImg[3 * (i * m_lWidth + j) + 2];
		}
	}
}
//---------------------------------------------------------------------------
// �摜�\������
void CGrabPageDlg::ShowImg()
{
	// �f�o�C�X�R���e�L�X�g�ɃR�s�[
	m_clsPicDC->BitBlt(0, 0, m_lWidth, m_lHeight, &m_clsBmpDC, 0, 0, SRCCOPY);
}
//---------------------------------------------------------------------------






