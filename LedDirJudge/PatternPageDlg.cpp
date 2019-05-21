// PatternPageDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "LedDirJudge.h"
#include "PatternPageDlg.h"
#include "afxdialogex.h"
#include "ImgInsp.h"
#include "Cammera.h"
#include "Param.h"
#include "ImgProc.h"
#include "Result.h"
#include "Common.h"


// CPatternPageDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CPatternPageDlg, CDialogEx)

CPatternPageDlg::CPatternPageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPatternPageDlg::IDD, pParent)
{
	// �����o�ϐ�������
	m_bMouseOn = false;
	m_clsTemplatePicDC = NULL;
}

CPatternPageDlg::~CPatternPageDlg()
{
}

void CPatternPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PATTERN_IMG, m_pcPatternImg);
	DDX_Control(pDX, IDC_STATIC_TEMPLATE_IMG, m_pcTemplateImg);
	DDX_Control(pDX, IDC_EDIT_CORR_THR, m_edtCorrThr);
	DDX_Control(pDX, IDC_EDIT_DTECT_ANGLE, m_edtDtectAngle);
	DDX_Control(pDX, IDC_EDIT_DTECT_POS_X, m_edtDtectPosX);
	DDX_Control(pDX, IDC_EDIT_DTECT_POS_Y, m_edtDtectPosY);
	DDX_Control(pDX, IDC_EDIT_CORR_VAL, m_edtCorrVal);
	DDX_Control(pDX, IDC_COMBO_ANGLE_RANGE_LO, m_cbAngleRangeLo);
	DDX_Control(pDX, IDC_COMBO_ANGLE_RANGE_HI, m_cbAngleRangeHi);
}


BEGIN_MESSAGE_MAP(CPatternPageDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SHOW_ORG_IMG, &CPatternPageDlg::OnBnClickedButtonShowOrgImg)
	ON_BN_CLICKED(IDC_BUTTON_PATTERN, &CPatternPageDlg::OnBnClickedButtonPattern)
	ON_EN_KILLFOCUS(IDC_EDIT_CORR_THR, &CPatternPageDlg::OnEnKillfocusEditCorrThr)
	ON_BN_CLICKED(IDC_BUTTON_PATTERN_REGIST, &CPatternPageDlg::OnBnClickedButtonPatternRegist)
	ON_CBN_SELCHANGE(IDC_COMBO_ANGLE_RANGE_LO, &CPatternPageDlg::OnCbnSelchangeComboAngleRangeLo)
	ON_CBN_SELCHANGE(IDC_COMBO_ANGLE_RANGE_HI, &CPatternPageDlg::OnCbnSelchangeComboAngleRangeHi)
END_MESSAGE_MAP()


// CPatternPageDlg ���b�Z�[�W �n���h���[
//---------------------------------------------------------------------------
BOOL CPatternPageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������
	// �s�N�`���[�{�b�N�X�̃C�x���g�ʒm��L���ɂ���
	m_pcPatternImg.ModifyStyle(0, SS_NOTIFY);

	// �摜�T�C�Y�擾
	m_lWidth = g_pclCammera->Width();
	m_lHeight = g_pclCammera->Height();

	// Picture Control�̃f�o�C�X�R���e�L�X�g�擾
	m_clsPicDC = m_pcPatternImg.GetDC();

	// �摜�T�C�Y�ݒ�
	CRect rect;
	rect.top = 5;
	rect.left = 5;
	rect.right = rect.left + m_lWidth;
	rect.bottom = rect.top + m_lHeight;
	m_pcPatternImg.MoveWindow(&rect, TRUE);

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

	// �`��p�y��
	m_clsPatternRectPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	m_clsDtectRectPen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));

	// �R���{�{�b�N�X�̏�����
	for (int i = 0; i <= DF_PATTERN_SET_REV_RANGE; i += DF_PATTERN_SET_REV_INTERVAL) {
		CStringW	strBuf;

		strBuf.Format(L"%d", -i);
		m_cbAngleRangeLo.InsertString(-1, strBuf);
		strBuf.Format(L"%d", i);
		m_cbAngleRangeHi.InsertString(-1, strBuf);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
//---------------------------------------------------------------------------
void CPatternPageDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	m_pcPatternImg.ReleaseDC(m_clsPicDC);
	m_clsBmpDC.SelectObject(m_clsOldBmp);
    DeleteDC(m_clsBmpDC);
    DeleteObject(m_hBmp);

	DeletTemplateBmp();
}
//---------------------------------------------------------------------------
BOOL CPatternPageDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
    if (pMsg->hwnd == m_pcPatternImg.m_hWnd) {
		int x = GET_X_LPARAM(pMsg->lParam);
		int y = GET_Y_LPARAM(pMsg->lParam);

		switch(pMsg->message){
        case WM_LBUTTONDOWN: 
			if (m_bMouseOn == false) {
				m_bMouseOn = true;
				g_pclParam->stSet.stPattern.stPatternArea.lx = x;
				g_pclParam->stSet.stPattern.stPatternArea.ly = y;
			}

			break;
        case WM_MOUSEMOVE: 
			if (m_bMouseOn == true) {
				g_pclParam->stSet.stPattern.stPatternArea.lWidth = abs(x - g_pclParam->stSet.stPattern.stPatternArea.lx);
				if (g_pclParam->stSet.stPattern.stPatternArea.lx > x) {
					g_pclParam->stSet.stPattern.stPatternArea.lx = x;
				}
				g_pclParam->stSet.stPattern.stPatternArea.lHeight = abs(y - g_pclParam->stSet.stPattern.stPatternArea.ly);
				if (g_pclParam->stSet.stPattern.stPatternArea.ly > y) {
					g_pclParam->stSet.stPattern.stPatternArea.ly = y;
				}
				// �G���A�ĕ`��
				ShowImg();
				DrawPatternArea();
			}
			break;
        case WM_LBUTTONUP: 
			m_bMouseOn = false;

			break;
        }
    }

	return CDialogEx::PreTranslateMessage(pMsg);
}
//---------------------------------------------------------------------------
void CPatternPageDlg::OnBnClickedButtonShowOrgImg()
{
	// �B���摜�\��
	SetGrabImg();
	ShowImg();
	DrawPatternArea();
	// �e���v���[�g�摜�\��
	SetTemplateImg();
	ShowTemplateImg();
}
//---------------------------------------------------------------------------
void CPatternPageDlg::OnBnClickedButtonPatternRegist()
{
	// �e���v���[�g�̌v�Z���s��
	g_pclImgInsp->CalcTemplate();

	// �e���v���[�g�摜�\��
	SetTemplateImg();
	ShowTemplateImg();
}
//---------------------------------------------------------------------------
void CPatternPageDlg::OnBnClickedButtonPattern()
{
	CStringW	strBuf;

	// �O����
	g_pclImgInsp->InitProc();
	
// �������ԃ^�C�}�[�X�^�[�g
g_pclCommon->TimerStart(TIMER_TEMP);

	// �p�^�[���}�b�`���O
	if (g_pclImgInsp->PatternMatching() == false) {
		::MessageBox(NULL, _T("�p�^�[���}�b�`���O�Ɏ��s���܂����B"), _T("�摜�����G���["), MB_OK | MB_ICONWARNING);
		return;
	}
strBuf.Format(L"%dmsec", g_pclCommon->TimerLap(TIMER_TEMP) / 1000);
::MessageBox(NULL, strBuf, _T("����"), MB_OK | MB_ICONWARNING);

	// �p�^�[���}�b�`���O�p��]�E�ʒu�␳�����摜���쐬
	g_pclImgInsp->MakePatternImg();

	// ���o�ʒu�\��
	strBuf.Format(L"%d", g_pclResult->stResult.stPattern.stDtectPos.lX);
	m_edtDtectPosX.SetWindowTextW(strBuf);
	strBuf.Format(L"%d", g_pclResult->stResult.stPattern.stDtectPos.lY);
	m_edtDtectPosY.SetWindowTextW(strBuf);

	// ���o�p�x�\��
	strBuf.Format(L"%.1f��", g_pclResult->stResult.stPattern.dDtectAngle);
	m_edtDtectAngle.SetWindowTextW(strBuf);

	// ���֒l
	strBuf.Format(L"%.1f", g_pclResult->stResult.stPattern.dCorrVal * 100.0);
	m_edtCorrVal.SetWindowTextW(strBuf);

	// �摜�\��
	SetGrabImg();
	ShowImg();
	DrawDtectArea();
}
//---------------------------------------------------------------------------
// �p�����[�^�[�ݒ�
void CPatternPageDlg::ShowPara()
{
	CStringW	strBuf;

	// �p�x�͈͉���
	m_cbAngleRangeLo.SetCurSel(abs(g_pclParam->stSet.stPattern.stAngleRange.lLo / DF_PATTERN_SET_REV_INTERVAL));
	// �p�x�͈͏��
	m_cbAngleRangeHi.SetCurSel(g_pclParam->stSet.stPattern.stAngleRange.lHi / DF_PATTERN_SET_REV_INTERVAL);

	// ����臒l
	strBuf.Format(L"%.1f", g_pclParam->stSet.stPattern.dCorrThr);
	m_edtCorrThr.SetWindowTextW(strBuf);

}
//---------------------------------------------------------------------------
void CPatternPageDlg::OnCbnSelchangeComboAngleRangeLo()
{
	g_pclParam->stSet.stPattern.stAngleRange.lLo = -(m_cbAngleRangeLo.GetCurSel() * DF_PATTERN_SET_REV_INTERVAL);
}
//---------------------------------------------------------------------------
void CPatternPageDlg::OnCbnSelchangeComboAngleRangeHi()
{
	g_pclParam->stSet.stPattern.stAngleRange.lHi = m_cbAngleRangeHi.GetCurSel() * DF_PATTERN_SET_REV_INTERVAL;
}
//---------------------------------------------------------------------------
void CPatternPageDlg::OnEnKillfocusEditCorrThr()
{
	CStringW	strBuf;
	double		dBuf;

	// �l�擾
	m_edtCorrThr.GetWindowTextW(strBuf);
	dBuf = _wtof(strBuf.GetBuffer());
	if (dBuf < 0.0 || dBuf > 100.0) {
		::MessageBox(NULL, _T("����臒l��0.0�`100.0��ݒ肵�Ă��������B"), _T("�ݒ�G���["), MB_OK | MB_ICONWARNING);
		m_edtCorrThr.SetFocus();
		return;		
	}
	g_pclParam->stSet.stPattern.dCorrThr = dBuf;
	// �\��
	strBuf.Format(L"%.1f", g_pclParam->stSet.stPattern.dCorrThr);
	m_edtCorrThr.SetWindowTextW(strBuf);
}
//---------------------------------------------------------------------------
// �B���摜�擾
void CPatternPageDlg::SetGrabImg()
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
void CPatternPageDlg::ShowImg()
{
	// �f�o�C�X�R���e�L�X�g�ɃR�s�[
	m_clsPicDC->BitBlt(0, 0, m_lWidth, m_lHeight, &m_clsBmpDC, 0, 0, SRCCOPY);
}
//---------------------------------------------------------------------------
// �p�^�[���G���A�`��
void CPatternPageDlg::DrawPatternArea()
{
	// �y����ݒ�
	m_clsPicDC->SelectObject(&m_clsPatternRectPen);

	// �G���A�\��
	m_clsPicDC->MoveTo(g_pclParam->stSet.stPattern.stPatternArea.lx, g_pclParam->stSet.stPattern.stPatternArea.ly);
	m_clsPicDC->LineTo(g_pclParam->stSet.stPattern.stPatternArea.lx + g_pclParam->stSet.stPattern.stPatternArea.lWidth, g_pclParam->stSet.stPattern.stPatternArea.ly);
	m_clsPicDC->LineTo(g_pclParam->stSet.stPattern.stPatternArea.lx + g_pclParam->stSet.stPattern.stPatternArea.lWidth, g_pclParam->stSet.stPattern.stPatternArea.ly + g_pclParam->stSet.stPattern.stPatternArea.lHeight);
	m_clsPicDC->LineTo(g_pclParam->stSet.stPattern.stPatternArea.lx, g_pclParam->stSet.stPattern.stPatternArea.ly + g_pclParam->stSet.stPattern.stPatternArea.lHeight);
	m_clsPicDC->LineTo(g_pclParam->stSet.stPattern.stPatternArea.lx, g_pclParam->stSet.stPattern.stPatternArea.ly);
}
//---------------------------------------------------------------------------
// ���o�G���A�`��
void CPatternPageDlg::DrawDtectArea()
{
	ST_LONG_POS	stStartPos;
	ST_LONG_POS	stPos;

	ST_LONG_POS	stAffPos;
	ST_LONG_POS	stCenter;

	stAffPos.lX = 0;
	stAffPos.lY = 0;

	stCenter.lX = g_pclParam->stSet.stPattern.stTemplate[0].lWidth / 2;
	stCenter.lY = g_pclParam->stSet.stPattern.stTemplate[0].lHeight / 2;

	// �y����ݒ�
	m_clsPicDC->SelectObject(&m_clsDtectRectPen);

	// �G���A�\��
	stStartPos.lX = AFFINE_X(0, 0, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stStartPos.lY = AFFINE_Y(0, 0, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stStartPos.lX += g_pclResult->stResult.stPattern.stAffPos.lX;
	stStartPos.lY += g_pclResult->stResult.stPattern.stAffPos.lY;
	m_clsPicDC->MoveTo(stStartPos.lX, stStartPos.lY);


	stPos.lX = AFFINE_X(g_pclParam->stSet.stPattern.stTemplate[0].lWidth, 0, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stPos.lY = AFFINE_Y(g_pclParam->stSet.stPattern.stTemplate[0].lWidth, 0, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stPos.lX += g_pclResult->stResult.stPattern.stAffPos.lX;
	stPos.lY += g_pclResult->stResult.stPattern.stAffPos.lY;
	m_clsPicDC->LineTo(stPos.lX, stPos.lY);

	stPos.lX = AFFINE_X(g_pclParam->stSet.stPattern.stTemplate[0].lWidth, g_pclParam->stSet.stPattern.stTemplate[0].lHeight, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stPos.lY = AFFINE_Y(g_pclParam->stSet.stPattern.stTemplate[0].lWidth, g_pclParam->stSet.stPattern.stTemplate[0].lHeight, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stPos.lX += g_pclResult->stResult.stPattern.stAffPos.lX;
	stPos.lY += g_pclResult->stResult.stPattern.stAffPos.lY;
	m_clsPicDC->LineTo(stPos.lX, stPos.lY);

	stPos.lX = AFFINE_X(0, g_pclParam->stSet.stPattern.stTemplate[0].lHeight, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stPos.lY = AFFINE_Y(0, g_pclParam->stSet.stPattern.stTemplate[0].lHeight, g_pclResult->stResult.stPattern.stAffAngle, stAffPos, stCenter);
	stPos.lX += g_pclResult->stResult.stPattern.stAffPos.lX;
	stPos.lY += g_pclResult->stResult.stPattern.stAffPos.lY;
	m_clsPicDC->LineTo(stPos.lX, stPos.lY);

	m_clsPicDC->LineTo(stStartPos.lX, stStartPos.lY);

}
//---------------------------------------------------------------------------
void CPatternPageDlg::SetTemplateImg()
{
	DeletTemplateBmp();

	// �e���v���[�g�pPicture Control�̃f�o�C�X�R���e�L�X�g�擾
	m_clsTemplatePicDC = m_pcTemplateImg.GetDC();

	// �摜�T�C�Y�ݒ�
	m_pcTemplateImg.MoveWindow(290, 490, g_pclParam->stSet.stPattern.stTemplate[0].lWidth, g_pclParam->stSet.stPattern.stTemplate[0].lHeight, TRUE);

	// �e���v���[�g�p�r�b�g�}�b�v�̍쐬
	BITMAPINFO binfo;
    ZeroMemory(&binfo, sizeof(binfo));
    binfo.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
    binfo.bmiHeader.biBitCount = 32;
    binfo.bmiHeader.biPlanes   =  1;
    binfo.bmiHeader.biWidth    =  g_pclParam->stSet.stPattern.stTemplate[0].lWidth;
    binfo.bmiHeader.biHeight   =  -(g_pclParam->stSet.stPattern.stTemplate[0].lHeight);

	// m_hTemplateBmp�Ɋ֘A�t���� CBitmap�ƁACDC���쐬
	m_hTemplateBmp = CreateDIBSection( NULL, &binfo, DIB_RGB_COLORS, (void **)(&m_pucTemplateBmp), NULL, 0); 
	CBitmap* clsTemplateBmp = CBitmap::FromHandle(m_hTemplateBmp);
	m_pclsTemplateBmpDC = new CDC();
    m_pclsTemplateBmpDC->CreateCompatibleDC(m_clsTemplatePicDC);
    m_clsTemplateOldBmp = m_pclsTemplateBmpDC->SelectObject(clsTemplateBmp); 

	// �摜�R�s�[
	for (int i = 0; i < g_pclParam->stSet.stPattern.stTemplate[0].lHeight; i++) {
		for (int j = 0; j < g_pclParam->stSet.stPattern.stTemplate[0].lWidth; j++) {
			m_pucTemplateBmp[4 * (i * g_pclParam->stSet.stPattern.stTemplate[0].lWidth + j) + 0] = g_pclParam->stSet.stPattern.stTemplate[0].ucImg[i * g_pclParam->stSet.stPattern.stTemplate[0].lWidth + j];
			m_pucTemplateBmp[4 * (i * g_pclParam->stSet.stPattern.stTemplate[0].lWidth + j) + 1] = g_pclParam->stSet.stPattern.stTemplate[0].ucImg[i * g_pclParam->stSet.stPattern.stTemplate[0].lWidth + j];
			m_pucTemplateBmp[4 * (i * g_pclParam->stSet.stPattern.stTemplate[0].lWidth + j) + 2] = g_pclParam->stSet.stPattern.stTemplate[0].ucImg[i * g_pclParam->stSet.stPattern.stTemplate[0].lWidth + j];
		}
	}
}
//---------------------------------------------------------------------------
void CPatternPageDlg::ShowTemplateImg()
{
	// �f�o�C�X�R���e�L�X�g�ɃR�s�[
	m_clsTemplatePicDC->BitBlt(0, 0, g_pclParam->stSet.stPattern.stTemplate[0].lWidth, g_pclParam->stSet.stPattern.stTemplate[0].lHeight, m_pclsTemplateBmpDC, 0, 0, SRCCOPY);
}
//---------------------------------------------------------------------------
void CPatternPageDlg::DeletTemplateBmp()
{
	if (m_clsTemplatePicDC == NULL) {
		return;
	}
	m_pcTemplateImg.ReleaseDC(m_clsTemplatePicDC);
	m_pclsTemplateBmpDC->SelectObject(m_clsTemplateOldBmp);
    DeleteDC(*m_pclsTemplateBmpDC);
	delete m_pclsTemplateBmpDC;
    DeleteObject(m_hTemplateBmp);

	m_clsTemplatePicDC = NULL;
}
//---------------------------------------------------------------------------
