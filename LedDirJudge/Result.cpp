//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Result.h"
//---------------------------------------------------------------------------
CResult* g_pclResult = NULL;
//---------------------------------------------------------------------------
WCHAR* JudgeName[EN_JUDGE_MAX] = {
        L"---",
        L"OK",
        L"NG"
};
//---------------------------------------------------------------------------
CResult::CResult(void)
{
	InitResult();
}
//---------------------------------------------------------------------------
CResult::~CResult(void)
{

}
//---------------------------------------------------------------------------
void CResult::InitResult()
{
	// åãâ èâä˙âª
	stResult.stPattern.dCorrVal = 0.0;
	stResult.stPattern.stDtectPos.lX = 0;
	stResult.stPattern.stDtectPos.lY = 0;
	stResult.stPattern.dDtectAngle = 0.0;
	stResult.stPattern.stAffAngle.dSin = 0.0;
	stResult.stPattern.stAffAngle.dCos = 1.0;
	stResult.stPattern.stAffPos.lX = 0;
	stResult.stPattern.stAffPos.lY = 0;
	stResult.stPattern.stAffCenter.lX = 0;
	stResult.stPattern.stAffCenter.lY = 0;

	for (int i = 0; i < DF_JUDGE_NUM_MAX; i++) {
		stResult.stJudge.stOne[i].lAreaNum = 0;
		stResult.stJudge.stOne[i].enJudge = EN_JUDGE_YET;
	}
	stResult.stJudge.enJudge = EN_JUDGE_YET;

	stResult.lGrabTime = 0;
	stResult.lInspTime = 0;
}
//---------------------------------------------------------------------------
WCHAR* CResult::GetJudgeStr(EN_JUDGE enJudge)
{
	return JudgeName[enJudge];
}
//---------------------------------------------------------------------------
EN_D2D_BRUSH CResult::GetJudgeBrush(EN_JUDGE enJudge)
{
	switch (enJudge) {
		case EN_JUDGE_OK:
			return EN_D2D_BRUSH_GREEN;
		case EN_JUDGE_NG:
			return EN_D2D_BRUSH_RED;
		default:
			return EN_D2D_BRUSH_WHITE;
	}
}
//---------------------------------------------------------------------------
WCHAR* CResult::GetGrabTimeStr()
{
	m_strTime.Format(L"éBëú%dmsec", stResult.lGrabTime);
	return m_strTime.GetBuffer();
}
//---------------------------------------------------------------------------
WCHAR* CResult::GetInspTimeStr()
{
	m_strTime.Format(L"çáåv%dmsec", stResult.lInspTime);
	return m_strTime.GetBuffer();
}
//---------------------------------------------------------------------------
