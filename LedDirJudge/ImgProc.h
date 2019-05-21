//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "Param.h"
//---------------------------------------------------------------------------
// 定義
#define SOBEL_MASK_SIZE 3	// Sobelフィルターマスクサイズ
// ハフ変換検出角度精度
#define HOUGH_THETA_1	180				// 1度
#define HOUGH_THETA_01	1800			// 0.1度
#define HOUGH_THETA_MAX	HOUGH_THETA_01	// 最大精度
//---------------------------------------------------------------------------
// アフィンマクロ
#define AFFINE_X(lx, ly, Angle, Pos, Center)	((long)((lx + Pos.lX - Center.lX) * Angle.dCos - (ly + Pos.lY - Center.lY) * Angle.dSin + Center.lX))
#define AFFINE_Y(lx, ly, Angle, Pos, Center)	((long)((lx + Pos.lX - Center.lX) * Angle.dSin + (ly + Pos.lY - Center.lY) * Angle.dCos + Center.lY))

#define ANGLE_AFFINE_X(lx, ly, Angle, Center)	((long)((lx - Center.lX) * Angle.dCos - (ly - Center.lY) * Angle.dSin + Center.lX))
#define ANGLE_AFFINE_Y(lx, ly, Angle, Center)	((long)((lx - Center.lX) * Angle.dSin + (ly - Center.lY) * Angle.dCos + Center.lY))
//---------------------------------------------------------------------------
class CImgProc
{
private:
	long		m_lColorNum;
	long		m_lWidth;
	long		m_lHeight;
	ST_LONG_POS	m_stCenter;

	// Soleb
	long		m_lSobelMask[EN_SET_DIR_MAX][SOBEL_MASK_SIZE][SOBEL_MASK_SIZE];
	// ハフ変換
	double		m_dHoughSinTbl[HOUGH_THETA_MAX + 1];
	double		m_dHoughCosTbl[HOUGH_THETA_MAX + 1];
	long		m_lHoughRhoMax;
	long**		m_pplHoughCounter;
	// エッジ検出
	long*		m_plEdgeAvg;
	long*		m_plEdgeDiff;
	double*		m_pdEdgeHosei;

protected:

	void NewBuffer();
	void DeleteBuffer();

	__inline double CalcZncc(UCHAR* pucImg, long lImgWidth, long lImgHeight, long lXStart, long lYStart, ST_LONG_POS stAnglePos[], ST_SET_PATTERN_TEMPLATE* pstTemplate);
	int ThinningSingle(int delta, int width, int height, UCHAR* buf);
	long GetAvgConcen(UCHAR* pucSrc, ST_LONG_RECT stArea, EN_SET_DIR enDir, ST_AFFINE_ANGLE stAffAngle);
	void ThetaRhoToABC(double dRho, double dTheta, double* pdA, double* pdB, double* pdC);
	void ABCToThetaRho(double dA, double dB, double dC, double* pdRho, double* pdTheta);
	long PowerLong(long d, long a);
	double PowerDouble(double d, long a);

public:
	CImgProc(void);
	~CImgProc(void);

	void SetImgSize(long lColorNum, long lWidth, long lHeight);
	void GrayProc(UCHAR* pucSrc, UCHAR* pucDst);	
	bool ExecPatternMatching(UCHAR* pucImg, long lImgWidth, long lImgHeight,
								   ST_LONG_RECT stArea, long lStepX, long lStepY,
								   long lAngleCnt, ST_LONG_POS stAnglePos[][DF_CAMMERA_WIDTH * DF_CAMMERA_HEIGHT],
								   ST_SET_PATTERN_TEMPLATE* pstTemplate,
								   ST_LONG_POS* pstDtectPos, long* plDtectAngle, double* pdCorrVal);

								   void Threshold(UCHAR* pucSrc, UCHAR* pucDst, ST_LONG_RECT stArea, ST_LONG_RANGE stThr);
	void Sobel(UCHAR* pucSrc, UCHAR* pucDst, ST_LONG_RECT stArea, EN_SET_DIR enDir);
	bool HoughLines(UCHAR* pucSrc, ST_LONG_RECT stArea, ST_DOUBLE_RANGE stRadianRange, long lThetaPrec, double* pdTheta, long* plRho, long* plCounter);
	bool Thinning(UCHAR* pucSrc, UCHAR* pucDst, int iThiNum);
	bool EdgeDetect(UCHAR* pucSrc, ST_LONG_RECT stArea, EN_SET_DIR enDir, double dEdgeSens, ST_AFFINE_ANGLE stAffAngle, long* plPos);
	bool Affine(long lX, long lY, ST_AFFINE_ANGLE stAffAngle, ST_LONG_POS stAffPos, ST_LONG_POS stAffCenter, long* plX, long* plY);
	long CountArea(UCHAR* pucSrc, ST_LONG_RANGE stThr, ST_LONG_RECT stArea, ST_AFFINE_ANGLE stAffAngle, ST_LONG_POS stAffPos, ST_LONG_POS stAffCenter);

	void ThetaRhoToLine(double dRho, double dTheta, ST_LONG_POS* pstStart, ST_LONG_POS* pstEnd);
	double RadianToAngle(double dRadian);
	double AngleToRadian(double dAngle);
};
//---------------------------------------------------------------------------
extern CImgProc* g_pclImgProc;
//---------------------------------------------------------------------------
