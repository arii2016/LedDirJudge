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
	// IO初期化
	if (g_pclIo->Init() == false) {
		::MessageBox(NULL, L"IOボードの初期化に失敗しました。", L"初期化エラー", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}
	// カメラ初期化
	if (g_pclCammera->Init() == false) {
		::MessageBox(NULL, L"カメラの初期化に失敗しました。", L"初期化エラー", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}
	// 露光時間設定
	g_pclCammera->SetExposure(g_pclParam->stSet.stGrab.lExposure);

	// 画像サイズ設定
	g_pclImgInsp->SetImgSize(g_pclCammera->ColorNum(), g_pclCammera->Width(), g_pclCammera->Height());

	// メインスレッド開始
	g_pclMainThread->Resume();
}
//---------------------------------------------------------------------------
void CMain::Close()
{
	// メインスレッド停止
	g_pclMainThread->WaitFor();

	// カメラ解放
	g_pclCammera->Close();
	// IO解放
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
// 検査実行
bool CMain::ExecInspection()
{
	g_pclMainThread->m_bInspStart = true;

	return true;
}
//---------------------------------------------------------------------------
// 撮影
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

	// 画像コピー
	g_pclImgInsp->SetImgCopy(g_pclCammera->GrabImg());

	return true;
}

//---------------------------------------------------------------------------
// 画像読み込み
bool CMain::LoadImage(WCHAR* pwcFilePath)
{
	// 読み込み
	if (g_pclCammera->LoadImage(pwcFilePath) == false)
	{
		return false;
	}

	// 画像コピー
	g_pclImgInsp->SetImgCopy(g_pclCammera->GrabImg());

	return true;
}
//---------------------------------------------------------------------------
// 撮影画像表示
void CMain::ShowGrabImg()
{
	// 画像クリア
	m_pclAutoD2D->ClearDiagram();
	m_pclAutoD2D->ClearText();
	// 撮像画像設定
	m_pclAutoD2D->SetImgCopy(g_pclCammera->GrabImg(), DF_COLOR_NUM_RGB);
	// 画面更新
	m_pclAutoD2D->UpdateDraw();
}
//---------------------------------------------------------------------------
// 検査結果画像表示
void CMain::ShowInspImg()
{
	long lx;
	long ly;
	D2D1_POINT_2F stStart;
	D2D1_POINT_2F stEnd;
	D2D1_RECT_F	stRect;

	// 画像クリア
	m_pclAutoD2D->ClearDiagram();
	m_pclAutoD2D->ClearText();
	// 撮像画像設定
	m_pclAutoD2D->SetImgCopy(g_pclCammera->GrabImg(), DF_COLOR_NUM_RGB);
	// 判定結果表示
	stRect.top = 0.;
	stRect.left = 0.;
	stRect.bottom = 50.;
	stRect.right = 100.;
	m_pclAutoD2D->AddText(g_pclResult->GetJudgeStr(g_pclResult->stResult.stJudge.enJudge), g_pclResult->GetJudgeBrush(g_pclResult->stResult.stJudge.enJudge), EN_D2D_TEXT_FORMAT_48, stRect);
	// 撮像時間表示
	stRect.top = 50.;
	stRect.left = 0.;
	stRect.bottom = 100.;
	stRect.right = 250.;
	m_pclAutoD2D->AddText(g_pclResult->GetGrabTimeStr(), EN_D2D_BRUSH_WHITE, EN_D2D_TEXT_FORMAT_36, stRect);
	// 検査時間表示
	stRect.top = 100.;
	stRect.left = 0.;
	stRect.bottom = 150.;
	stRect.right = 250.;
	m_pclAutoD2D->AddText(g_pclResult->GetInspTimeStr(), EN_D2D_BRUSH_WHITE, EN_D2D_TEXT_FORMAT_36, stRect);

	// 検査エリア表示
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

	// 画面更新
	m_pclAutoD2D->UpdateDraw();
}
//---------------------------------------------------------------------------
