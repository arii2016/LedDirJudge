//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "ImgInsp.h"
#include "ImgProc.h"
#include "Param.h"
#include "Result.h"
#include "Common.h"
//---------------------------------------------------------------------------
#define _USE_MATH_DEFINES
#include <math.h>
//---------------------------------------------------------------------------
CImgInsp* g_pclImgInsp = NULL;
//---------------------------------------------------------------------------
CImgInsp::CImgInsp(void)
{
	m_pucSrcImg = NULL;
	for (int h = DF_PATTERN_HIERARCHY_NUM - 1; h >= 0; h--) {
		m_pucInitImg[h] = NULL;
	}
	m_pucPatternImg = NULL;
	m_pucJudgeImg = NULL;

	m_pucTempImg = NULL;
}
//---------------------------------------------------------------------------
CImgInsp::~CImgInsp(void)
{
	DelImg();
}
//---------------------------------------------------------------------------
void CImgInsp::SetImgSize(long lColorNum, long lWidth, long lHeight)
{
	m_lColorNum = lColorNum;
	m_lWidth = lWidth;
	m_lHeight = lHeight;

	for (int h = DF_PATTERN_HIERARCHY_NUM - 1; h >= 0; h--) {
		double dCompRatio = 1.0 / pow(2, (double)h);

		m_lPatWidth[h] = (long)(lWidth * dCompRatio);
		m_lPatHeight[h] = (long)(lHeight * dCompRatio);
	}


	DelImg();
	m_pucSrcImg = new UCHAR[m_lColorNum * m_lWidth * m_lHeight];
	memset(m_pucSrcImg, 0, sizeof(UCHAR) * m_lColorNum * m_lWidth * m_lHeight);

	for (int h = DF_PATTERN_HIERARCHY_NUM - 1; h >= 0; h--) {
		m_pucInitImg[h] = new UCHAR[m_lPatWidth[h] * m_lPatHeight[h]];
		memset(m_pucInitImg[h], 0, sizeof(UCHAR) * m_lPatWidth[h] * m_lPatHeight[h]);
	}

	m_pucPatternImg = new UCHAR[m_lWidth * m_lHeight];
	memset(m_pucPatternImg, 0, sizeof(UCHAR) * m_lWidth * m_lHeight);

	m_pucJudgeImg = new UCHAR[m_lWidth * m_lHeight];
	memset(m_pucJudgeImg, 0, sizeof(UCHAR) * m_lWidth * m_lHeight);

	m_pucTempImg = new UCHAR[m_lWidth * m_lHeight];
	memset(m_pucTempImg, 0, sizeof(UCHAR) * m_lWidth * m_lHeight);


	g_pclImgProc->SetImgSize(lColorNum, lWidth, lHeight);
}
//---------------------------------------------------------------------------
void CImgInsp::DelImg()
{
	if (m_pucTempImg != NULL) {
		delete m_pucTempImg;
		m_pucTempImg = NULL;
	}

	if (m_pucJudgeImg != NULL) {
		delete m_pucJudgeImg;
		m_pucJudgeImg = NULL;
	}
	if (m_pucPatternImg != NULL) {
		delete m_pucPatternImg;
		m_pucPatternImg = NULL;
	}
	for (int h = DF_PATTERN_HIERARCHY_NUM - 1; h >= 0; h--) {
		if (m_pucInitImg[h] != NULL) {
			delete m_pucInitImg[h];
			m_pucInitImg[h] = NULL;
		}
	}
	if (m_pucSrcImg != NULL) {
		delete m_pucSrcImg;
		m_pucSrcImg = NULL;
	}
}
//---------------------------------------------------------------------------
void CImgInsp::SetImgCopy(UCHAR* pucImage)
{
	memcpy(m_pucSrcImg, pucImage, sizeof(UCHAR) * m_lColorNum * m_lWidth * m_lHeight);
}
//---------------------------------------------------------------------------
UCHAR* CImgInsp::PatternImg()
{
	return m_pucPatternImg;
}
//---------------------------------------------------------------------------
UCHAR* CImgInsp::JudgeImg()
{
	return m_pucJudgeImg;
}
//---------------------------------------------------------------------------
// 前処理
void CImgInsp::InitProc()
{
	// グレー処理
	g_pclImgProc->GrayProc(m_pucSrcImg, m_pucInitImg[0]);
	// 階層画像作成
	for (int h = DF_PATTERN_HIERARCHY_NUM - 1; h >= 1; h--) {
		double dCompRatio = 1.0 / pow(2, (double)h);

		for (int i = 0; i < m_lPatHeight[h]; i++) {
			for (int j = 0; j < m_lPatWidth[h]; j++) {
				m_pucInitImg[h][i * m_lPatWidth[h] + j] = m_pucInitImg[0][(int)(i / dCompRatio) * m_lPatWidth[0] + (int)(j / dCompRatio)];
			}
		}
	}
}
//---------------------------------------------------------------------------
// パターンマッチングのテンプレート用の前計算
void CImgInsp::CalcTemplate()
{
	// 前処理
	InitProc();

	for (int h = DF_PATTERN_HIERARCHY_NUM - 1; h >= 0; h--) {
		ST_SET_PATTERN_TEMPLATE* pstTemplate = &g_pclParam->stSet.stPattern.stTemplate[h];
		double dCompRatio = 1.0 / pow(2, (double)h);

		// 切り出した画像をテンプレート用構造体に設定
		pstTemplate->lWidth = (long)(g_pclParam->stSet.stPattern.stPatternArea.lWidth * dCompRatio);
		pstTemplate->lHeight = (long)(g_pclParam->stSet.stPattern.stPatternArea.lHeight * dCompRatio);

		memset(pstTemplate->ucImg, 0, sizeof(UCHAR) * DF_CAMMERA_WIDTH * DF_CAMMERA_HEIGHT);

		for (int i = 0; i < pstTemplate->lHeight; i++) {
			for (int j = 0; j < pstTemplate->lWidth; j++) {

				pstTemplate->ucImg[i * pstTemplate->lWidth + j] = m_pucInitImg[0][(int)(g_pclParam->stSet.stPattern.stPatternArea.ly + (i / dCompRatio)) * m_lWidth + (int)(g_pclParam->stSet.stPattern.stPatternArea.lx + (j / dCompRatio))];
			}
		}

		// 正規化相関計算時に使用する値
		pstTemplate->lN = pstTemplate->lWidth * pstTemplate->lHeight;

		pstTemplate->dT = 0.0;
		pstTemplate->dTT = 0.0;
		for (int i = 0; i < pstTemplate->lHeight; i++) {
			for (int j = 0; j < pstTemplate->lWidth; j++) {
				pstTemplate->dT += pstTemplate->ucImg[i * pstTemplate->lWidth + j];
				pstTemplate->dTT += pstTemplate->ucImg[i * pstTemplate->lWidth + j] * pstTemplate->ucImg[i * pstTemplate->lWidth + j];
			}
		}
		pstTemplate->dF2 = pstTemplate->lN * pstTemplate->dTT - pstTemplate->dT * pstTemplate->dT;
	}
}
//---------------------------------------------------------------------------
// パターンマッチング
bool CImgInsp::PatternMatching()
{
	ST_RESULT_PATTERN* pstPattern = &g_pclResult->stResult.stPattern;
	double dRevInterVal = DF_PATTERN_SET_REV_INTERVAL;

	for (int h = DF_PATTERN_HIERARCHY_NUM - 1; h >= 0; h--) {
		ST_SET_PATTERN_TEMPLATE* pstTemplate = &g_pclParam->stSet.stPattern.stTemplate[h];
		double dCompRatio = 1.0 / pow(2, (double)h);
		ST_LONG_RECT	stArea;

		if (h == DF_PATTERN_HIERARCHY_NUM - 1) {
			// 最初の階層は、飛ばし処理を行う
			stArea.lx = 0;
			stArea.ly = 0;
			stArea.lWidth = m_lPatWidth[h];
			stArea.lHeight = m_lPatHeight[h];

			if (FirstPatternMatching(h, stArea, &pstPattern->stDtectPos, &pstPattern->dDtectAngle, &pstPattern->dCorrVal, &dRevInterVal) == false) {
				return false;
			}
		}
		else {
			// 2回以降の階層は、飛ばさない
			double dRevStart;
			double dRevEnd;
			long lRevCnt;
			long lDtectAngle;

			// 次の位置設定
			stArea.lx = (long)(pstPattern->stDtectPos.lX * dCompRatio - 1);
			stArea.ly = (long)(pstPattern->stDtectPos.lY * dCompRatio - 1);
			stArea.lWidth = pstTemplate->lWidth + 3;
			stArea.lHeight = pstTemplate->lHeight + 3;
			if (stArea.lx < 0) {
				stArea.lx = 0;
			}
			if (stArea.ly < 0) {
				stArea.ly = 0;
			}
			if (stArea.lx + stArea.lWidth > m_lPatWidth[h]) {
				stArea.lWidth = m_lPatWidth[h] - stArea.lx;
			}
			if (stArea.ly + stArea.lHeight > m_lPatHeight[h]) {
				stArea.lHeight = m_lPatHeight[h] - stArea.ly;
			}
			// 次の角度設定
			if (h == 0) {
				// 最後は角度は検索しない
				dRevStart = pstPattern->dDtectAngle;
				lRevCnt = 1;
			}
			else {
				dRevInterVal = dRevInterVal / 2.0;
				dRevStart = pstPattern->dDtectAngle - dRevInterVal;
				if (dRevStart < g_pclParam->stSet.stPattern.stAngleRange.lLo) {
					dRevStart = g_pclParam->stSet.stPattern.stAngleRange.lLo;
				}
				dRevEnd = pstPattern->dDtectAngle + dRevInterVal;
				if (dRevEnd > g_pclParam->stSet.stPattern.stAngleRange.lHi) {
					dRevEnd = g_pclParam->stSet.stPattern.stAngleRange.lHi;
				}
				lRevCnt = (long)((dRevEnd - dRevStart) / dRevInterVal + 1);
			}

			// 回転位置計算
			CalcPatternAnglePos(h, dRevInterVal, dRevStart, lRevCnt);

			// パターンマッチング実行
			if (g_pclImgProc->ExecPatternMatching(m_pucInitImg[h],
													m_lPatWidth[h],
													m_lPatHeight[h],
													stArea,
													1,
													1,
													lRevCnt,
													m_stAnglePos,
													pstTemplate,
													&pstPattern->stDtectPos,
													&lDtectAngle,
													&pstPattern->dCorrVal
													) == false) {
				return false;
			}
			// 角度設定
			pstPattern->dDtectAngle = dRevStart + (dRevInterVal * lDtectAngle);
		}
		pstPattern->stDtectPos.lX = (long)(pstPattern->stDtectPos.lX / dCompRatio);
		pstPattern->stDtectPos.lY = (long)(pstPattern->stDtectPos.lY / dCompRatio);
	}

	// アフィン変換の角度を設定する
	double dTheta = g_pclImgProc->AngleToRadian(pstPattern->dDtectAngle);
	pstPattern->stAffAngle.dSin = sin(dTheta);
	pstPattern->stAffAngle.dCos = cos(dTheta);
	// アフィン変換の位置を設定
	pstPattern->stAffPos.lX = pstPattern->stDtectPos.lX;
	pstPattern->stAffPos.lY = pstPattern->stDtectPos.lY;
	// アフィン変換のセンター位置を設定
	pstPattern->stAffCenter.lX = g_pclParam->stSet.stPattern.stTemplate[0].lWidth / 2 + pstPattern->stAffPos.lX;
	pstPattern->stAffCenter.lY = g_pclParam->stSet.stPattern.stTemplate[0].lHeight / 2 + pstPattern->stAffPos.lY;

	// 相関値閾値確認
	if (g_pclParam->stSet.stPattern.dCorrThr / 100.0 > pstPattern->dCorrVal) {
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
// 最初のパターンマッチング
bool CImgInsp::FirstPatternMatching(int h, ST_LONG_RECT stArea, ST_LONG_POS* pstDtectPos, double* pdDtectAngle, double* pdCorrVa, double* pdRevInterVal)
{
	ST_SET_PATTERN_TEMPLATE* pstTemplate = &g_pclParam->stSet.stPattern.stTemplate[h];
	long lStep = 8;
	double dRevStart = g_pclParam->stSet.stPattern.stAngleRange.lLo;
	double dRevEnd = g_pclParam->stSet.stPattern.stAngleRange.lHi;
	long lRevCnt = (long)((g_pclParam->stSet.stPattern.stAngleRange.lHi - g_pclParam->stSet.stPattern.stAngleRange.lLo) / *pdRevInterVal + 1);

	while (true) {
		long lStepX = pstTemplate->lWidth / lStep;
		long lStepY = pstTemplate->lHeight / lStep;
		long lDtectAngle;

		if (lStepX <= 0) {
			lStepX = 1;
		}
		if (lStepY <= 0) {
			lStepY = 1;
		}
		// 回転位置計算
		CalcPatternAnglePos(h, *pdRevInterVal, dRevStart, lRevCnt);
		// パターンマッチング実行
		if (g_pclImgProc->ExecPatternMatching(m_pucInitImg[h],
											  m_lPatWidth[h],
											  m_lPatHeight[h],
											  stArea,
											  lStepX,
											  lStepY,
											  lRevCnt,
											  m_stAnglePos,
											  pstTemplate,
											  pstDtectPos,
											  &lDtectAngle,
											  pdCorrVa
											  ) == false) {
			return false;
		}
		// 角度設定
		*pdDtectAngle = dRevStart + (*pdRevInterVal * lDtectAngle);

		if (lStepX == 1 && lStepY == 1 && *pdRevInterVal < 1.0) {
			break;
		}

		// 次の位置設定
		stArea.lx = pstDtectPos->lX - lStepX;
		stArea.ly = pstDtectPos->lY - lStepY;
		stArea.lWidth = pstTemplate->lWidth + (lStepX * 2);
		stArea.lHeight = pstTemplate->lHeight + (lStepY * 2);
		if (stArea.lx < 0) {
			stArea.lx = 0;
		}
		if (stArea.ly < 0) {
			stArea.ly = 0;
		}
		if (stArea.lx + stArea.lWidth > m_lPatWidth[h]) {
			stArea.lWidth = m_lPatWidth[h] - stArea.lx;
		}
		if (stArea.ly + stArea.lHeight > m_lPatHeight[h]) {
			stArea.lHeight = m_lPatHeight[h] - stArea.ly;
		}
		lStep *= 2;
		// 次の角度設定
		*pdRevInterVal = *pdRevInterVal / 2.0;
		dRevStart = *pdDtectAngle - *pdRevInterVal;
		if (dRevStart < g_pclParam->stSet.stPattern.stAngleRange.lLo) {
			dRevStart = g_pclParam->stSet.stPattern.stAngleRange.lLo;
		}
		dRevEnd = *pdDtectAngle + *pdRevInterVal;
		if (dRevEnd > g_pclParam->stSet.stPattern.stAngleRange.lHi) {
			dRevEnd = g_pclParam->stSet.stPattern.stAngleRange.lHi;
		}
		lRevCnt = (long)((dRevEnd - dRevStart) / *pdRevInterVal + 1);
	}


	return true;
}
//---------------------------------------------------------------------------
void CImgInsp::CalcPatternAnglePos(int h, double dRevInterVal, double dRevStart, long lRevCnt)
{
	ST_SET_PATTERN_TEMPLATE* pstTemplate = &g_pclParam->stSet.stPattern.stTemplate[h];
	ST_AFFINE_ANGLE stAffAngle;
	ST_LONG_POS	stCenter;

	// 回転位置作成
	stCenter.lX = pstTemplate->lWidth / 2;
	stCenter.lY = pstTemplate->lHeight/ 2;

	for (int r = 0; r < lRevCnt; r++) {
		double dTheta = g_pclImgProc->AngleToRadian(dRevStart + dRevInterVal * r);

		stAffAngle.dSin = sin(dTheta);
		stAffAngle.dCos = cos(dTheta);

		for (int i = 0; i < pstTemplate->lHeight; i++) {
			for (int j = 0; j < pstTemplate->lWidth; j++) {
				m_stAnglePos[r][i * pstTemplate->lWidth + j].lX = j - ANGLE_AFFINE_X(j, i, stAffAngle, stCenter);
				m_stAnglePos[r][i * pstTemplate->lWidth + j].lY = i - ANGLE_AFFINE_Y(j, i, stAffAngle, stCenter);
			}
		}
	}
}
//---------------------------------------------------------------------------
// パターンマッチング用補正画像作成
void CImgInsp::MakePatternImg()
{
	long lXPos;
	long lYPos;
	ST_LONG_POS	stCenter;

	stCenter.lX = g_pclParam->stSet.stPattern.stTemplate[0].lWidth / 2 + g_pclResult->stResult.stPattern.stAffPos.lX;
	stCenter.lY = g_pclParam->stSet.stPattern.stTemplate[0].lHeight / 2 + g_pclResult->stResult.stPattern.stAffPos.lY;

	// 初期化
	memset(m_pucPatternImg, 0, sizeof(UCHAR) * m_lWidth * m_lHeight);

	for (int i = 0; i < m_lHeight; i++) {
        for (int j = 0; j < m_lWidth; j++) {
			lXPos = AFFINE_X(j, i, g_pclResult->stResult.stPattern.stAffAngle, g_pclResult->stResult.stPattern.stAffPos, stCenter);
			lYPos = AFFINE_Y(j, i, g_pclResult->stResult.stPattern.stAffAngle, g_pclResult->stResult.stPattern.stAffPos, stCenter);

			if (lXPos < 0 || lXPos >= m_lWidth || lYPos < 0 || lYPos >= m_lHeight) {
				continue;
			}

			m_pucPatternImg[i * m_lWidth + j] = m_pucInitImg[0][lYPos * m_lWidth + lXPos];
		}
	}
}
//---------------------------------------------------------------------------
// 判定
bool CImgInsp::Judgment()
{
	for (int i = 0; i < g_pclParam->stSet.stJudge.lNum; i++) {
		// エリア取得
		g_pclResult->stResult.stJudge.stOne[i].lAreaNum = g_pclImgProc->CountArea(m_pucInitImg[0],
																		 g_pclParam->stSet.stJudge.stOne[i].stThr,
																		 g_pclParam->stSet.stJudge.stOne[i].stArea,
																		 g_pclResult->stResult.stPattern.stAffAngle,
																		 g_pclResult->stResult.stPattern.stAffPos,
																		 g_pclResult->stResult.stPattern.stAffCenter);

		// 判定
		if (g_pclResult->stResult.stJudge.stOne[i].lAreaNum >= g_pclParam->stSet.stJudge.stOne[i].stAreaRange.lLo &&
			g_pclResult->stResult.stJudge.stOne[i].lAreaNum <= g_pclParam->stSet.stJudge.stOne[i].stAreaRange.lHi) {

				g_pclResult->stResult.stJudge.stOne[i].enJudge = EN_JUDGE_OK;
		}
		else {
				g_pclResult->stResult.stJudge.stOne[i].enJudge = EN_JUDGE_NG;
				g_pclResult->stResult.stJudge.enJudge = EN_JUDGE_NG;
		}
	}

	// 総合判定
	if (g_pclResult->stResult.stJudge.enJudge != EN_JUDGE_NG) {
		g_pclResult->stResult.stJudge.enJudge = EN_JUDGE_OK;
	}

	return true;
}
//---------------------------------------------------------------------------
// 判定画像作成
void CImgInsp::MakeJudgeImg()
{
	ST_AFFINE_ANGLE stAffAngle = g_pclResult->stResult.stPattern.stAffAngle;
	ST_LONG_POS stAffPos = g_pclResult->stResult.stPattern.stAffPos;
	ST_LONG_POS stAffCenter = g_pclResult->stResult.stPattern.stAffCenter;

	long lX;
	long lY;

	for (int i = 0; i < g_pclParam->stSet.stJudge.lNum; i++) {
		ST_LONG_RANGE stThr = g_pclParam->stSet.stJudge.stOne[i].stThr;
		ST_LONG_RECT stArea = g_pclParam->stSet.stJudge.stOne[i].stArea;

		for (int y = stArea.ly; y < stArea.ly + stArea.lHeight; y++) {
			for (int x = stArea.lx; x < stArea.lx + stArea.lWidth; x++) {
				lX = AFFINE_X(x, y, stAffAngle, stAffPos, stAffCenter);
				lY = AFFINE_Y(x, y, stAffAngle, stAffPos, stAffCenter);
				if (lX < 0 || lX >= m_lWidth || lY < 0 || lY >= m_lHeight) {
					continue;
				}
				if (m_pucInitImg[0][lY * m_lWidth + lX] >= stThr.lLo &&
					m_pucInitImg[0][lY * m_lWidth + lX] <= stThr.lHi) {

					m_pucJudgeImg[y * m_lWidth + x] = 255;
				}
				else {
					m_pucJudgeImg[y * m_lWidth + x] = 0;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
