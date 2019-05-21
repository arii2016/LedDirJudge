//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Main.h"
//---------------------------------------------------------------------------
#include "Common.h"
#include "Io.h"
#include "SysParam.h"
#include "Param.h"
#include "Cammera.h"
#include "ImgProc.h"
#include "ImgInsp.h"
#include "Result.h"
#include "MainThread.h"
//---------------------------------------------------------------------------
CMain* g_pclMain = NULL;
//---------------------------------------------------------------------------
CMain::CMain(void)
{
	g_pclCommon = new CCommon();
	g_pclSysParam = new CSysParam();
	g_pclParam = new CParam();
	g_pclResult = new CResult();
	g_pclIo = new CIo();
	g_pclCammera = new CCammera();
	g_pclImgProc = new CImgProc();
	g_pclImgInsp = new CImgInsp();
	g_pclMainThread = new CMainThread();
}
//---------------------------------------------------------------------------
CMain::~CMain(void)
{
	if (g_pclMainThread != NULL) {
		delete g_pclMainThread;
		g_pclMainThread = NULL;
	}
	if (g_pclImgInsp != NULL) {
		delete g_pclImgInsp;
		g_pclImgInsp = NULL;
	}
	if (g_pclImgProc != NULL) {
		delete g_pclImgProc;
		g_pclImgProc = NULL;
	}
	if (g_pclCammera != NULL) {
		delete g_pclCammera;
		g_pclCammera = NULL;
	}
	if (g_pclIo != NULL) {
		delete g_pclIo;
		g_pclIo = NULL;
	}
	if (g_pclResult != NULL) {
		delete g_pclResult;
		g_pclResult = NULL;
	}
	if (g_pclParam != NULL) {
		delete g_pclParam;
		g_pclParam = NULL;
	}
	if (g_pclSysParam != NULL) {
		delete g_pclSysParam;
		g_pclSysParam = NULL;
	}
	if (g_pclCommon != NULL) {
		delete g_pclCommon;
		g_pclCommon = NULL;
	}
}
//---------------------------------------------------------------------------
void CMain::Init()
{
	// IO������
	if (g_pclIo->Init() == false) {
		::MessageBox(NULL, L"IO�{�[�h�̏������Ɏ��s���܂����B", L"�������G���[", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}
	// �J����������
	if (g_pclCammera->Init() == false) {
		::MessageBox(NULL, L"�J�����̏������Ɏ��s���܂����B", L"�������G���[", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}
	// �I�����Ԑݒ�
	g_pclCammera->SetExposure(g_pclParam->stSet.stGrab.lExposure);

	// �摜�T�C�Y�ݒ�
	g_pclImgInsp->SetImgSize(g_pclCammera->ColorNum(), g_pclCammera->Width(), g_pclCammera->Height());

	// ���C���X���b�h�J�n
	g_pclMainThread->Resume();
}
//---------------------------------------------------------------------------
void CMain::Close()
{
	// ���C���X���b�h��~
	g_pclMainThread->WaitFor();

	// �J�������
	g_pclCammera->Close();
	// IO���
	g_pclIo->Close();
}
//---------------------------------------------------------------------------
void CMain::SetScreen(EN_SCREEN enScreen)
{
	m_enScreen = enScreen;
}
//---------------------------------------------------------------------------
EN_SCREEN CMain::GetScreen(void)
{
	return m_enScreen;
}
//---------------------------------------------------------------------------
void CMain::SetAutoD2D(CD2DAutoCtrl* pclAutoD2D)
{
	m_pclAutoD2D = pclAutoD2D;
	m_pclAutoD2D->SetImgSize(D2D1::SizeU(g_pclCammera->Width(), g_pclCammera->Height()));
}
//---------------------------------------------------------------------------
// �������s
bool CMain::ExecInspection()
{
	g_pclMainThread->m_bInspStart = true;

	return true;
}
//---------------------------------------------------------------------------
// �B�e
bool CMain::Grab()
{
	if (g_pclCammera->GrabStart() == false) {
		return false;
	}
	while (1) {
		if (g_pclCammera->CheckGrabEnd() == true) {
			break;
		}
		g_pclCommon->DoEvents();
	}

	// �摜�R�s�[
	g_pclImgInsp->SetImgCopy(g_pclCammera->GrabImg());

	return true;
}

//---------------------------------------------------------------------------
// �摜�ǂݍ���
bool CMain::LoadImage(WCHAR* pwcFilePath)
{
	// �ǂݍ���
	if (g_pclCammera->LoadImage(pwcFilePath) == false)
	{
		return false;
	}

	// �摜�R�s�[
	g_pclImgInsp->SetImgCopy(g_pclCammera->GrabImg());

	return true;
}
//---------------------------------------------------------------------------
// �B�e�摜�\��
void CMain::ShowGrabImg()
{
	// �摜�N���A
	m_pclAutoD2D->ClearDiagram();
	m_pclAutoD2D->ClearText();
	// �B���摜�ݒ�
	m_pclAutoD2D->SetImgCopy(g_pclCammera->GrabImg(), DF_COLOR_NUM_RGB);
	// ��ʍX�V
	m_pclAutoD2D->UpdateDraw();
}
//---------------------------------------------------------------------------
// �������ʉ摜�\��
void CMain::ShowInspImg()
{
	long lx;
	long ly;
	D2D1_POINT_2F stStart;
	D2D1_POINT_2F stEnd;
	D2D1_RECT_F	stRect;

	// �摜�N���A
	m_pclAutoD2D->ClearDiagram();
	m_pclAutoD2D->ClearText();
	// �B���摜�ݒ�
	m_pclAutoD2D->SetImgCopy(g_pclCammera->GrabImg(), DF_COLOR_NUM_RGB);
	// ���茋�ʕ\��
	stRect.top = 0.;
	stRect.left = 0.;
	stRect.bottom = 50.;
	stRect.right = 100.;
	m_pclAutoD2D->AddText(g_pclResult->GetJudgeStr(g_pclResult->stResult.stJudge.enJudge), g_pclResult->GetJudgeBrush(g_pclResult->stResult.stJudge.enJudge), EN_D2D_TEXT_FORMAT_48, stRect);
	// �B�����ԕ\��
	stRect.top = 50.;
	stRect.left = 0.;
	stRect.bottom = 100.;
	stRect.right = 250.;
	m_pclAutoD2D->AddText(g_pclResult->GetGrabTimeStr(), EN_D2D_BRUSH_WHITE, EN_D2D_TEXT_FORMAT_36, stRect);
	// �������ԕ\��
	stRect.top = 100.;
	stRect.left = 0.;
	stRect.bottom = 150.;
	stRect.right = 250.;
	m_pclAutoD2D->AddText(g_pclResult->GetInspTimeStr(), EN_D2D_BRUSH_WHITE, EN_D2D_TEXT_FORMAT_36, stRect);

	// �����G���A�\��
	for (int i = 0; i < g_pclParam->stSet.stJudge.lNum; i++) {
		g_pclImgProc->Affine(g_pclParam->stSet.stJudge.stOne[i].stArea.lx,
							 g_pclParam->stSet.stJudge.stOne[i].stArea.ly,
							 g_pclResult->stResult.stPattern.stAffAngle,
							 g_pclResult->stResult.stPattern.stAffPos,
							 g_pclResult->stResult.stPattern.stAffCenter,
							 &lx, &ly);
		stStart.x = (FLOAT)lx;
		stStart.y = (FLOAT)ly;
		g_pclImgProc->Affine(g_pclParam->stSet.stJudge.stOne[i].stArea.lx + g_pclParam->stSet.stJudge.stOne[i].stArea.lWidth,
							 g_pclParam->stSet.stJudge.stOne[i].stArea.ly,
							 g_pclResult->stResult.stPattern.stAffAngle,
							 g_pclResult->stResult.stPattern.stAffPos,
							 g_pclResult->stResult.stPattern.stAffCenter,
							 &lx, &ly);
		stEnd.x = (FLOAT)lx;
		stEnd.y = (FLOAT)ly;
		m_pclAutoD2D->AddLineDiagram(stStart, stEnd, g_pclResult->GetJudgeBrush(g_pclResult->stResult.stJudge.stOne[i].enJudge), 1.0);
		g_pclImgProc->Affine(g_pclParam->stSet.stJudge.stOne[i].stArea.lx + g_pclParam->stSet.stJudge.stOne[i].stArea.lWidth,
							 g_pclParam->stSet.stJudge.stOne[i].stArea.ly + g_pclParam->stSet.stJudge.stOne[i].stArea.lHeight,
							 g_pclResult->stResult.stPattern.stAffAngle,
							 g_pclResult->stResult.stPattern.stAffPos,
							 g_pclResult->stResult.stPattern.stAffCenter,
							 &lx, &ly);
		stStart.x = (FLOAT)lx;
		stStart.y = (FLOAT)ly;
		m_pclAutoD2D->AddLineDiagram(stEnd, stStart, g_pclResult->GetJudgeBrush(g_pclResult->stResult.stJudge.stOne[i].enJudge), 1.0);
		g_pclImgProc->Affine(g_pclParam->stSet.stJudge.stOne[i].stArea.lx,
							 g_pclParam->stSet.stJudge.stOne[i].stArea.ly + g_pclParam->stSet.stJudge.stOne[i].stArea.lHeight,
							 g_pclResult->stResult.stPattern.stAffAngle,
							 g_pclResult->stResult.stPattern.stAffPos,
							 g_pclResult->stResult.stPattern.stAffCenter,
							 &lx, &ly);
		stEnd.x = (FLOAT)lx;
		stEnd.y = (FLOAT)ly;
		m_pclAutoD2D->AddLineDiagram(stStart, stEnd, g_pclResult->GetJudgeBrush(g_pclResult->stResult.stJudge.stOne[i].enJudge), 1.0);
		g_pclImgProc->Affine(g_pclParam->stSet.stJudge.stOne[i].stArea.lx,
							 g_pclParam->stSet.stJudge.stOne[i].stArea.ly,
							 g_pclResult->stResult.stPattern.stAffAngle,
							 g_pclResult->stResult.stPattern.stAffPos,
							 g_pclResult->stResult.stPattern.stAffCenter,
							 &lx, &ly);
		stStart.x = (FLOAT)lx;
		stStart.y = (FLOAT)ly;
		m_pclAutoD2D->AddLineDiagram(stEnd, stStart, g_pclResult->GetJudgeBrush(g_pclResult->stResult.stJudge.stOne[i].enJudge), 1.0);
	}

	// ��ʍX�V
	m_pclAutoD2D->UpdateDraw();
}
//---------------------------------------------------------------------------
