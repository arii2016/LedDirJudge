//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "Define.h"
//---------------------------------------------------------------------------
class CImgInsp
{
private:
	long		m_lColorNum;
	long		m_lWidth;
	long		m_lHeight;
	long		m_lPatWidth[DF_PATTERN_HIERARCHY_NUM];
	long		m_lPatHeight[DF_PATTERN_HIERARCHY_NUM];

	UCHAR*		m_pucSrcImg;
	UCHAR*		m_pucInitImg[DF_PATTERN_HIERARCHY_NUM];
	UCHAR*		m_pucPatternImg;
	UCHAR*		m_pucJudgeImg;

	UCHAR*		m_pucTempImg;

	ST_LONG_POS m_stAnglePos[DF_PATTERN_REV_MAX + 1][DF_CAMMERA_WIDTH * DF_CAMMERA_HEIGHT];

protected:

public:
	CImgInsp(void);
	~CImgInsp(void);

	void SetImgSize(long lColorNum, long lWidth, long lHeight);
	void DelImg();
	void SetImgCopy(UCHAR* pucImage);
	UCHAR* PatternImg();
	UCHAR* JudgeImg();
	void MakePatternImg();
	void MakeJudgeImg();

	void InitProc();
	void CalcTemplate();
	bool PatternMatching();
	bool FirstPatternMatching(int h, ST_LONG_RECT stArea, ST_LONG_POS* pstDtectPos, double* pdDtectAngle, double* pdCorrVa, double* pdRevInterVal);
	void CalcPatternAnglePos(int h, double dRevInterVal, double dRevStart, long lRevCnt);
	bool Judgment();
};
//---------------------------------------------------------------------------
extern CImgInsp* g_pclImgInsp;
//---------------------------------------------------------------------------

