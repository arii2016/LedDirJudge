
// LedDirJudgeDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "LedDirJudge.h"
#include "LedDirJudgeDlg.h"
#include "afxdialogex.h"

#include "Main.h"
#include "ImgInsp.h"
#include "Param.h"
#include "SysParam.h"
#include "Cammera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLedDirJudgeDlg �_�C�A���O




CLedDirJudgeDlg::CLedDirJudgeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLedDirJudgeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// ���C���N���X����
	g_pclMain = new CMain();
}

CLedDirJudgeDlg::~CLedDirJudgeDlg()
{
	if (g_pclMain != NULL) {
		delete g_pclMain;
		g_pclMain = NULL;
	}
}

void CLedDirJudgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLedDirJudgeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_EXIT, &CLedDirJudgeDlg::OnFileExit)
	ON_COMMAND(ID_MODE_AUTO, &CLedDirJudgeDlg::OnModeAuto)
	ON_COMMAND(ID_MODE_MANUAL, &CLedDirJudgeDlg::OnModeManual)
	ON_COMMAND(ID_TEST_CAMERA, &CLedDirJudgeDlg::OnTestCamera)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_LOADPARA, &CLedDirJudgeDlg::OnFileLoadpara)
	ON_COMMAND(ID_FILE_SAVEPARA, &CLedDirJudgeDlg::OnFileSavepara)
	ON_COMMAND(ID_TEST_PROCIMAGE, &CLedDirJudgeDlg::OnTestProcimage)
END_MESSAGE_MAP()


// CLedDirJudgeDlg ���b�Z�[�W �n���h���[
//---------------------------------------------------------------------------
BOOL CLedDirJudgeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	g_pclMain->Init();

	// ������ʃy�[�W�A�}�j���A����ʃy�[�W���쐬
	m_clAutoPage.Create(IDD_AUTO_PAGE_DIALOG, this);
	m_clManualPage.Create(IDD_MANUAL_PAGEDIALOG, this);

	CRect    rect;
	GetClientRect(&rect);
	m_clAutoPage.MoveWindow(&rect, FALSE);
	m_clManualPage.MoveWindow(&rect, FALSE);
	OnModeAuto();

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B

	g_pclMain->Close();
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B

	// �_�C�A���O�̃T�C�Y�ɍ��킹�āA�摜�\����ύX����
	CRect rect = CRect(0, 0, cx, cy);
	if (m_clAutoPage.m_hWnd != NULL) {
		m_clAutoPage.MoveWindow(&rect, TRUE);
	}
	if (m_clManualPage.m_hWnd != NULL) {
		m_clManualPage.MoveWindow(&rect, TRUE);
	}
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
//---------------------------------------------------------------------------
// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CLedDirJudgeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnFileLoadpara()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
	CFileDialog clsFileDlg(TRUE, NULL, L"test.xml", OFN_FILEMUSTEXIST  | OFN_HIDEREADONLY, L"XMĻ�� | *.xml|", this);
    if (clsFileDlg.DoModal() != IDOK ){
        return;
    }
	CStringW strFilePath = clsFileDlg.GetPathName();
	if (g_pclParam->Read((WCHAR*)strFilePath.GetString()) == false)
	{
		::MessageBox(NULL, L"PARA FILE READ ERROR!!!", L"ERROR", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		return;
	}
	wcscpy_s(g_pclSysParam->wcParaFilePath, MAX_PATH, strFilePath.GetString());	

	// �I�����Ԑݒ�
	g_pclCammera->SetExposure(g_pclParam->stSet.stGrab.lExposure);

	// �ݒ��ʍX�V
	m_clManualPage.ShowPara();
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnFileSavepara()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
	CFileDialog clsFileDlg(FALSE, NULL, L"test.xml", OFN_FILEMUSTEXIST  | OFN_HIDEREADONLY, L"XMĻ�� | *.xml|", this);
    if (clsFileDlg.DoModal() != IDOK ){
        return;
    }
	CStringW strFilePath = clsFileDlg.GetPathName();
	if (g_pclParam->Write((WCHAR*)strFilePath.GetString()) == false)
	{
		::MessageBox(NULL, L"PARA FILE WRITE ERROR!!!", L"ERROR", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		return;
	}
	wcscpy_s(g_pclSysParam->wcParaFilePath, MAX_PATH, strFilePath.GetString());
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnFileExit()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
	OnOK();
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnModeAuto()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
	// ������ʂ�\��
	m_clAutoPage.ShowWindow(SW_SHOW);
	m_clManualPage.ShowWindow(SW_HIDE);
	g_pclMain->SetScreen(EN_SCREEN_AUTO);
	// ���j���[��ύX
	CWnd* pWnd = AfxGetMainWnd();
	CMenu* pMenu = pWnd->GetMenu();
	pMenu->EnableMenuItem(ID_MODE_MANUAL, MF_ENABLED);
	pMenu->EnableMenuItem(ID_MODE_AUTO, MF_DISABLED);
	pMenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
	pWnd->DrawMenuBar();
	// �A���B�����s���Ă����ꍇ�͒�~����
	m_clManualPage.StopContinGrab();
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnModeManual()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
	if (m_clPassDlg.ShowPassDlg() == false) {
		return;
	}
	// �}�j���A����ʂ�\��
	m_clAutoPage.ShowWindow(SW_HIDE);
	m_clManualPage.ShowWindow(SW_SHOW);
	g_pclMain->SetScreen(EN_SCREEN_MANUAL);
	// ���j���[��ύX
	CWnd* pWnd = AfxGetMainWnd();
	CMenu* pMenu = pWnd->GetMenu();
	pMenu->EnableMenuItem(ID_MODE_AUTO, MF_ENABLED);
	pMenu->EnableMenuItem(ID_MODE_MANUAL, MF_DISABLED);
	pMenu->EnableMenuItem(2, MF_BYPOSITION | MF_GRAYED);
	pWnd->DrawMenuBar();
	// �ŏ��͎B����ʕ\��
	m_clManualPage.m_clsTab.SetCurSel(MANUAL_GRAB_DLG);
	m_clManualPage.OnTcnSelchangeTabManual(0, 0);
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnTestCamera()
{
	// �B�e
	if (g_pclMain->Grab() == false) {
		::MessageBox(NULL, _T("�B�e���s"), _T("�B�e���s"), MB_OK | MB_ICONWARNING);
		return;
	}
	// �B���摜�\��
	g_pclMain->ShowGrabImg();
}
//---------------------------------------------------------------------------
void CLedDirJudgeDlg::OnTestProcimage()
{
	// �������s
	g_pclMain->ExecInspection();
}
//---------------------------------------------------------------------------
