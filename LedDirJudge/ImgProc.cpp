//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "ImgProc.h"
#include "Common.h"
#include "Result.h"
//---------------------------------------------------------------------------
#define _USE_MATH_DEFINES
#include <math.h>
//---------------------------------------------------------------------------
CImgProc* g_pclImgProc = NULL;
//---------------------------------------------------------------------------
CImgProc::CImgProc(void)
{
	// Sobel�}�X�N���쐬
	m_lSobelMask[EN_SET_DIR_TOP][0][0] = -1;
	m_lSobelMask[EN_SET_DIR_TOP][0][1] = -2;
	m_lSobelMask[EN_SET_DIR_TOP][0][2] = -1;
	m_lSobelMask[EN_SET_DIR_TOP][1][0] = 0;
	m_lSobelMask[EN_SET_DIR_TOP][1][1] = 0;
	m_lSobelMask[EN_SET_DIR_TOP][1][2] = 0;
	m_lSobelMask[EN_SET_DIR_TOP][2][0] = 1;
	m_lSobelMask[EN_SET_DIR_TOP][2][1] = 2;
	m_lSobelMask[EN_SET_DIR_TOP][2][2] = 1;

	m_lSobelMask[EN_SET_DIR_BOTTOM][0][0] = 1;
	m_lSobelMask[EN_SET_DIR_BOTTOM][0][1] = 2;
	m_lSobelMask[EN_SET_DIR_BOTTOM][0][2] = 1;
	m_lSobelMask[EN_SET_DIR_BOTTOM][1][0] = 0;
	m_lSobelMask[EN_SET_DIR_BOTTOM][1][1] = 0;
	m_lSobelMask[EN_SET_DIR_BOTTOM][1][2] = 0;
	m_lSobelMask[EN_SET_DIR_BOTTOM][2][0] = -1;
	m_lSobelMask[EN_SET_DIR_BOTTOM][2][1] = -2;
	m_lSobelMask[EN_SET_DIR_BOTTOM][2][2] = -1;

	m_lSobelMask[EN_SET_DIR_RIGHT][0][0] = -1;
	m_lSobelMask[EN_SET_DIR_RIGHT][0][1] = 0;
	m_lSobelMask[EN_SET_DIR_RIGHT][0][2] = 1;
	m_lSobelMask[EN_SET_DIR_RIGHT][1][0] = -2;
	m_lSobelMask[EN_SET_DIR_RIGHT][1][1] = 0;
	m_lSobelMask[EN_SET_DIR_RIGHT][1][2] = 2;
	m_lSobelMask[EN_SET_DIR_RIGHT][2][0] = -1;
	m_lSobelMask[EN_SET_DIR_RIGHT][2][1] = 0;
	m_lSobelMask[EN_SET_DIR_RIGHT][2][2] = 1;

	m_lSobelMask[EN_SET_DIR_LEFT][0][0] = 1;
	m_lSobelMask[EN_SET_DIR_LEFT][0][1] = 0;
	m_lSobelMask[EN_SET_DIR_LEFT][0][2] = -1;
	m_lSobelMask[EN_SET_DIR_LEFT][1][0] = 2;
	m_lSobelMask[EN_SET_DIR_LEFT][1][1] = 0;
	m_lSobelMask[EN_SET_DIR_LEFT][1][2] = -2;
	m_lSobelMask[EN_SET_DIR_LEFT][2][0] = 1;
	m_lSobelMask[EN_SET_DIR_LEFT][2][1] = 0;
	m_lSobelMask[EN_SET_DIR_LEFT][2][2] = -1;

	// �n�t�ϊ��p�O�p�֐��e�[�u�����쐬
	for (int i = 0; i <= HOUGH_THETA_MAX; i++) {
		m_dHoughSinTbl[i] = sin((M_PI / (double)HOUGH_THETA_MAX) * (double)i);
		m_dHoughCosTbl[i] = cos((M_PI / (double)HOUGH_THETA_MAX) * (double)i);
	}

	// ������
	m_pplHoughCounter = NULL;
	m_plEdgeAvg = NULL;
	m_plEdgeDiff = NULL;
	m_pdEdgeHosei = NULL;

}
//---------------------------------------------------------------------------
CImgProc::~CImgProc(void)
{
	DeleteBuffer();
}
//---------------------------------------------------------------------------
void CImgProc::SetImgSize(long lColorNum, long lWidth, long lHeight)
{
	m_lColorNum = lColorNum;
	m_lWidth = lWidth;
	m_lHeight = lHeight;
	m_stCenter.lX = m_lWidth / 2;
	m_stCenter.lY = m_lHeight/ 2;

	NewBuffer();
}
//---------------------------------------------------------------------------
void CImgProc::NewBuffer()
{
	// �z�񂪐�������Ă���ꍇ�͍폜
	DeleteBuffer();

	// �n�t�p�z��
	// �Ίp���̒���
	m_lHoughRhoMax = (long)sqrt((double)(PowerLong(m_lWidth, 2) + PowerLong(m_lHeight, 2)));
	// �񎟌��z�񐶐�
	m_pplHoughCounter = new long*[HOUGH_THETA_MAX + 1];
	for (int i = 0; i <= HOUGH_THETA_MAX; i++) {
		m_pplHoughCounter[i] = new long[m_lHoughRhoMax * 2];
	}

	// �G�b�W���o�p�z��
	m_plEdgeAvg = new long[(m_lWidth > m_lHeight) ? m_lWidth : m_lHeight];
	m_plEdgeDiff = new long[(m_lWidth > m_lHeight) ? m_lWidth : m_lHeight];
	m_pdEdgeHosei = new double[(m_lWidth > m_lHeight) ? m_lWidth : m_lHeight];
}
//---------------------------------------------------------------------------
void CImgProc::DeleteBuffer()
{
	// �G�b�W���o�p�z��
	if (m_plEdgeAvg != NULL) {
		delete[] m_plEdgeAvg;
		m_plEdgeAvg = NULL;
	}
	if (m_plEdgeDiff != NULL) {
		delete[] m_plEdgeDiff;
		m_plEdgeDiff = NULL;
	}
	if (m_pdEdgeHosei != NULL) {
		delete[] m_pdEdgeHosei;
		m_pdEdgeHosei = NULL;
	}

	// �n�t�p�z��
	if (m_pplHoughCounter != NULL) {
		for (int i = 0; i <= HOUGH_THETA_MAX; i++) {
			delete[] m_pplHoughCounter[i];
		}
		delete[] m_pplHoughCounter;
		m_pplHoughCounter = NULL;
	}

}
//---------------------------------------------------------------------------
// �O���[����
void CImgProc::GrayProc(UCHAR* pucSrc, UCHAR* pucDst)
{
	for (int i = 0; i < m_lHeight; i++) {
		for (int j = 0; j < m_lWidth; j++) {
			long lTemp = pucSrc[3 * (i * m_lWidth + j) + 0] +
						 pucSrc[3 * (i * m_lWidth + j) + 1] + 
						 pucSrc[3 * (i * m_lWidth + j) + 2];

			pucDst[i * m_lWidth + j] = (UCHAR)(lTemp / 3);
		}
	}

}
//---------------------------------------------------------------------------
// �p�^�[���}�b�`���O
bool CImgProc::ExecPatternMatching(UCHAR* pucImg, long lImgWidth, long lImgHeight,
								   ST_LONG_RECT stArea, long lStepX, long lStepY,
								   long lAngleCnt, ST_LONG_POS stAnglePos[][DF_CAMMERA_WIDTH * DF_CAMMERA_HEIGHT],
								   ST_SET_PATTERN_TEMPLATE* pstTemplate,
								   ST_LONG_POS* pstDtectPos, long* plDtectAngle, double* pdCorrVal)
{
	double dVal;

	*pdCorrVal = -1;

	for (int i = stArea.ly; i < stArea.ly + stArea.lHeight - pstTemplate->lHeight; i+=lStepY) {
		for (int j = stArea.lx; j < stArea.lx + stArea.lWidth - pstTemplate->lWidth; j+=lStepX) {
			for (int k = 0; k < lAngleCnt; k++) {
				dVal = CalcZncc(pucImg, lImgWidth, lImgHeight, j, i, stAnglePos[k], pstTemplate);
				if (dVal > *pdCorrVal) {
					*pdCorrVal = dVal;
					pstDtectPos->lX = j;
					pstDtectPos->lY = i;
					*plDtectAngle = k;
				}
			}
		}
	}

	return true;
}
//---------------------------------------------------------------------------
// ���K�����ݑ��֌v�Z
__inline double CImgProc::CalcZncc(UCHAR* pucImg, long lImgWidth, long lImgHeight, long lXStart, long lYStart, ST_LONG_POS stAnglePos[], ST_SET_PATTERN_TEMPLATE* pstTemplate)
{
	long lTempImgCnt = 0;
	int iXPos;
	int iYPos;
	double dTI = 0.0;
	double dI  = 0.0;
	double dII = 0.0;

	for (int i = lYStart; i < lYStart + pstTemplate->lHeight; i++) {
		for (int j = lXStart; j < lXStart + pstTemplate->lWidth; j++, lTempImgCnt++) {
			iXPos = j - stAnglePos[lTempImgCnt].lX;
			iYPos = i - stAnglePos[lTempImgCnt].lY;
			if (iXPos < 0 || iXPos >= lImgWidth || iYPos < 0 || iYPos >= lImgHeight) {
				continue;
			}
			dTI += pstTemplate->ucImg[lTempImgCnt] * pucImg[iYPos * lImgWidth + iXPos];
			dI  += pucImg[iYPos * lImgWidth + iXPos];
			dII += pucImg[iYPos * lImgWidth + iXPos] * pucImg[iYPos * lImgWidth + iXPos];
		}
	}

	double dF = (pstTemplate->lN * dII - dI * dI) * pstTemplate->dF2;
	if (dF == 0.0) {
		return 0.0;
	}

	return (pstTemplate->lN * dTI - dI * pstTemplate->dT) / sqrt(dF);
}
//---------------------------------------------------------------------------
// ��l��
void CImgProc::Threshold(UCHAR* pucSrc, UCHAR* pucDst, ST_LONG_RECT stArea, ST_LONG_RANGE stThr)
{
	for (int i = stArea.ly; i < stArea.ly + stArea.lHeight; i++) {
		for (int j = stArea.lx; j < stArea.lx + stArea.lWidth; j++) {
			if (pucSrc[i * m_lWidth + j] >= stThr.lLo && pucSrc[i * m_lWidth + j] <= stThr.lHi) {
				pucDst[i * m_lWidth + j] = 255;
			}
			else {
				pucDst[i * m_lWidth + j] = 0;
			}

		}
	}
}
//---------------------------------------------------------------------------
// sobel�t�B���^�[
void CImgProc::Sobel(UCHAR* pucSrc, UCHAR* pucDst, ST_LONG_RECT stArea, EN_SET_DIR enDir)
{
	int sum;
	int x;
	int y;
	long lAreaWidth = stArea.lx + stArea.lWidth;
	long lAreaHeight = stArea.ly + stArea.lHeight;

#ifdef _OPENMP
#pragma omp parallel for
#endif
	for (int i = stArea.ly; i < lAreaHeight; i++) {
        for (int j = stArea.lx; j < lAreaWidth; j++) {

            sum = 0;
            for (int k = -SOBEL_MASK_SIZE / 2; k <= SOBEL_MASK_SIZE / 2; k++) {
                for (int n = -SOBEL_MASK_SIZE / 2; n <= SOBEL_MASK_SIZE / 2; n++) {
					x = j + n;
					y = i + k;
                    if (x < stArea.lx || x >= lAreaWidth || y < stArea.ly || y >= lAreaHeight) {
						// �͈͊O�ɐڂ���_��0�ɂ���
						sum = 0;
						goto OUT_SIDE;
					}
                    sum = sum + pucSrc[m_lWidth * y + x] * m_lSobelMask[enDir][k + SOBEL_MASK_SIZE / 2][n + SOBEL_MASK_SIZE / 2];
                }
            }
		OUT_SIDE:
            pucDst[m_lWidth * i + j] = (UCHAR)sum;
		}
    }
}
//---------------------------------------------------------------------------
/*!
 @brief �n�t�ϊ��ɂ�钼�����o

 @param[in]     pucSrc          ���͉摜
 @param[in]     stArea		    ���o�G���A�͈�
 @param[in]     stRadianRange   ���o�p�x�͈�
 @param[in]     iThetaPrec		���o�p�x���x
 @param[out]    pdTheta         �_(0, 0)��������܂ł̋���
 @param[out]    pdRho           x���Ɛ����̖@���������p�x
 @param[out]    plCounter       ���o�J�E���g��

 @retval        true            ����
 @retval        false           ���s
*/
bool CImgProc::HoughLines(UCHAR* pucSrc, ST_LONG_RECT stArea, ST_DOUBLE_RANGE stRadianRange, long lThetaPrec, double* pdTheta, long* plRho, long* plCounter)
{
	// �X�e�b�v
	long lStep = HOUGH_THETA_MAX / lThetaPrec;
	// ��No
	long lThetaNo;
	// �p�x�͈͌v�Z
	long lStartTheta = (long)(stRadianRange.dLo / (M_PI / lThetaPrec) + 0.5);
	long lEndTheta = (long)(stRadianRange.dHi / (M_PI / lThetaPrec) + 0.5);
	if (lStartTheta < -(lThetaPrec)) {
		lStartTheta = -(lThetaPrec);
	}
	if (lEndTheta > lThetaPrec) {
		lEndTheta = lThetaPrec;
	}
	// Rho�͈�
	long lStartRho = m_lHoughRhoMax * 2;
	long lEndRho = 0;

	// �J�E���^������
	for (int i = 0; i <= lThetaPrec; i++) {
		memset(m_pplHoughCounter[i], 0, sizeof(long) * m_lHoughRhoMax * 2);
	}

	// �n�t�ϊ�
	for (int i = stArea.ly; i < stArea.ly + stArea.lHeight; i++) {
        for (int j = stArea.lx; j < stArea.lx + stArea.lWidth; j++) {
			if (pucSrc[i * m_lWidth + j] == 0) {
				continue;
			}
			for (int iTheta = lStartTheta; iTheta <= lEndTheta; iTheta++) {
				lThetaNo = iTheta;
				if (lThetaNo < 0) {
					lThetaNo = lThetaPrec + lThetaNo;
				}
				long lRho = (long)(j *m_dHoughCosTbl[lThetaNo * lStep] + i * m_dHoughSinTbl[lThetaNo * lStep] + 0.5);
				m_pplHoughCounter[lThetaNo][m_lHoughRhoMax + lRho]++;
				// Rho�͈͐ݒ�
				if (m_lHoughRhoMax + lRho < lStartRho) {
					lStartRho = m_lHoughRhoMax + lRho;
				}
				if (m_lHoughRhoMax + lRho > lEndRho) {
					lEndRho = m_lHoughRhoMax + lRho;
				}
			}
		}
	}

	// �J�E���^�̍ő�l���擾
	long lCounterMax = 0;
	long lThetaMax;
	long lRhoMax;
	for (int iTheta = lStartTheta; iTheta <= lEndTheta; iTheta++) {
		lThetaNo = iTheta;
		if (lThetaNo < 0) {
			lThetaNo = lThetaPrec + lThetaNo;
		}
		for (int iRho = lStartRho; iRho <= lEndRho; iRho++) {
			if (m_pplHoughCounter[lThetaNo][iRho] < lCounterMax) {
				continue;
			}
			lCounterMax = m_pplHoughCounter[lThetaNo][iRho];
			lThetaMax = lThetaNo;
			lRhoMax = iRho;
		}
	}
	if (lCounterMax < 2) {
		return false;
	}
	// �ƁA�ρA�J�E���g��ݒ�
	*pdTheta = lThetaMax * M_PI / lThetaPrec;
	*plRho = lRhoMax - m_lHoughRhoMax;
	*plCounter = lCounterMax;

	return true;
}
//---------------------------------------------------------------------------
// �א���
bool CImgProc::Thinning(UCHAR* pucSrc, UCHAR* pucDst, int iThiNum)
{
    unsigned char* pcBuf;

    pcBuf = new unsigned char[m_lWidth * (m_lHeight + 2)];
    pcBuf += m_lWidth;
    for (int i = 0; i < m_lHeight; i++) {
        memcpy(&pcBuf[m_lWidth * i], (UCHAR *)&pucSrc[m_lWidth * i], m_lWidth * sizeof(UCHAR));
    }

    int iRet[2];
    for (int i = 0; i < iThiNum; i++) {
        iRet[0] = ThinningSingle(1 , m_lWidth, m_lHeight, pcBuf);
        iRet[1] = ThinningSingle(-1, m_lWidth, m_lHeight, pcBuf);
        if (iRet[0] == 0 && iRet[1] == 0) {
            break;
        }
    }

    for (int i = 0; i < m_lHeight; i++) {
        memcpy((UCHAR *)&pucDst[m_lWidth * i], &pcBuf[m_lWidth * i], m_lWidth * sizeof(UCHAR));
    }
    pcBuf -= m_lWidth;
    delete(pcBuf);

    return true;
}
//---------------------------------------------------------------------------
int CImgProc::ThinningSingle(int delta, int width, int height, UCHAR* buf)
{
    int jsw = 0;
    int i_s, i_e, j_s, j_e, first;
    int a1, a2, a3, a4, a5, a6, a7, a8;
    UCHAR *buf_cur, *buf_end_i, *buf_end_j;

    if (delta == 1) {
        i_s = 0;            i_e = height;       j_s = 1;            j_e = width - 1;
    }
    else {
        i_s = height - 1;   i_e = -1;           j_s = width - 2;    j_e = 0;
    }

    buf_cur   = buf + i_s * width + j_s;
    buf_end_i = buf + i_e * width + j_e;
    buf_end_j = buf + i_s * width + j_e;
    memcpy(buf - width, buf + (height - 1) * width, width * sizeof(UCHAR));
    first = true;

    do {
        do {
            if (!(*buf_cur & 0x40)) {

            }
            else if (*buf_cur & 0x80) {
                if (!(*(buf_cur - width) & 0x40) || !(*(buf_cur - 1) & 0x40) || !(*(buf_cur + width) & 0x40) || !(*(buf_cur + 1) & 0x40)) {

                    a4 = *(buf_cur - width - 1) & 0x80; a3 = *(buf_cur - width) & 0x80;     a2 = *(buf_cur - width + 1) & 0x80;
                    a5 = *(buf_cur         - 1) & 0x80;                                     a1 = *(buf_cur         + 1) & 0x80;
                    a6 = *(buf_cur + width - 1) & 0x80; a7 = *(buf_cur + width) & 0x80;     a8 = *(buf_cur + width + 1) & 0x80;

                    int k;
                    k = !a1 + !a3 + !a5 + !a7 - (!a1 && !a2 && !a3) - (!a3 && !a4 && !a5) - (!a5 && !a6 && !a7) - (!a7 && !a8 && !a1);
                    if (k == 1) {
                        *buf_cur >>= 1;
                        jsw = 1;
                    }
                }
            }
            else {
                *buf_cur &= 0x20;
            }
            buf_cur += delta;
        } while (buf_cur != buf_end_j);
        if (first) {
            memcpy(buf + i_e * width, buf + i_s * width, width * sizeof(UCHAR));
            first = false;
        }
        buf_cur += delta * 2;
        buf_end_j += delta * width;
    } while(buf_end_j != buf_end_i);

    return jsw;
}
//---------------------------------------------------------------------------
/*!
 @brief �G�b�W���o

 @param[in]     pucSrc          ���͉摜
 @param[in]     stArea		    ���o�G���A
 @param[in]     enDir   		���o����
 @param[in]     dEdgeSens  		�G�b�W���x
 @param[in]     stAffAngle 		�A�t�B���p�x�ϊ����
 @param[out]    plPos           ���o�ʒu

 @retval        true            ����
 @retval        false           ���s
*/
bool CImgProc::EdgeDetect(UCHAR* pucSrc, ST_LONG_RECT stArea, EN_SET_DIR enDir, double dEdgeSens, ST_AFFINE_ANGLE stAffAngle, long* plPos)
{
	long lCnt;
	long lMax;
	long lPos;

	// ���ϔZ�x�擾
	lCnt = GetAvgConcen(pucSrc, stArea, enDir, stAffAngle);
	// ����
	m_plEdgeDiff[0] = 0;
	for (int i = 1; i < lCnt; i++) {
		m_plEdgeDiff[i] = m_plEdgeAvg[i] - m_plEdgeAvg[i - 1];
		if (m_plEdgeDiff[i] < 0) {
			// �}�C�i�X��0�ɂ���
			m_plEdgeDiff[i] = 0;
		}
	}
	// �G�b�W���x�̍ő�l��100%�ɂȂ�悤�ɕ␳
	lMax = 0;
	for (int i = 0; i < lCnt; i++) {
		if (m_plEdgeDiff[i] > lMax) {
			lMax = abs(m_plEdgeDiff[i]);
		}
	}
	if (lMax == 0) {
		return false;
	}
	for (int i = 0; i < lCnt; i++) {
		m_pdEdgeHosei[i] = ((double)m_plEdgeDiff[i]) / lMax * 100.0;
	}
	// �ʒu�����߂�
	bool bFlg = false;
	for (int i = 0; i < lCnt; i++) {
		if (m_pdEdgeHosei[i] > dEdgeSens) {
			lPos = i;
			bFlg = true;
			break;
		}
	}
	if (bFlg == false) {
		return false;
	}

	switch (enDir) {
		case EN_SET_DIR_RIGHT:
			// ������
			*plPos = lPos;
			break;
		case EN_SET_DIR_LEFT:
			// ������
			*plPos = stArea.lWidth - lPos;
			break;
		case EN_SET_DIR_TOP:
			// ������
			*plPos = lPos;
			break;
		case EN_SET_DIR_BOTTOM:
			// ������
			*plPos = stArea.lHeight - lPos;
			break;
	}

	return true;
}
//---------------------------------------------------------------------------
// ���ϔZ�x
long CImgProc::GetAvgConcen(UCHAR* pucSrc, ST_LONG_RECT stArea, EN_SET_DIR enDir, ST_AFFINE_ANGLE stAffAngle)
{
	ST_LONG_POS stAffPos;
	long lX;
	long lY;
	long lCnt;
	long lAvgCnt;

	stAffPos.lX = 0;
	stAffPos.lY = 0;

	switch (enDir) {
		case EN_SET_DIR_RIGHT:
			// ������
			lCnt = 0;
			for (int x = stArea.lx; x < stArea.lx + stArea.lWidth; x++) {
				m_plEdgeAvg[lCnt] = 0;

				lAvgCnt = 0;
				for (int y = stArea.ly; y < stArea.ly + stArea.lHeight; y++) {
					//if (Affine(x, y, stAffAngle, stAffPos, &lX, &lY) == false) {
					//	continue;
					//}
					// �A�t�B���}�N���ō�����
					lX = AFFINE_X(x, y, stAffAngle, stAffPos, m_stCenter);
					lY = AFFINE_Y(x, y, stAffAngle, stAffPos, m_stCenter);
					if (lX < 0 || lX >= m_lWidth || lY < 0 || lY >= m_lHeight) {
						continue;
					}

					m_plEdgeAvg[lCnt] +=  pucSrc[lY * m_lWidth + lX];
					lAvgCnt++;
				}
				if (lAvgCnt != 0) {
					m_plEdgeAvg[lCnt] /= lAvgCnt;
				}
				lCnt++;
			}
			break;
		case EN_SET_DIR_LEFT:
			// ������
			lCnt = 0;
			for (int x = stArea.lx + stArea.lWidth - 1; x >= stArea.lx; x--) {
				m_plEdgeAvg[lCnt] = 0;

				lAvgCnt = 0;
				for (int y = stArea.ly; y < stArea.ly + stArea.lHeight; y++) {
					//if (Affine(x, y, stAffAngle, stAffPos, &lX, &lY) == false) {
					//	continue;
					//}
					// �A�t�B���}�N���ō�����
					lX = AFFINE_X(x, y, stAffAngle, stAffPos, m_stCenter);
					lY = AFFINE_Y(x, y, stAffAngle, stAffPos, m_stCenter);
					if (lX < 0 || lX >= m_lWidth || lY < 0 || lY >= m_lHeight) {
						continue;
					}

					m_plEdgeAvg[lCnt] +=  pucSrc[lY * m_lWidth + lX];
					lAvgCnt++;
				}
				if (lAvgCnt != 0) {
					m_plEdgeAvg[lCnt] /= lAvgCnt;
				}
				lCnt++;
			}
			break;
		case EN_SET_DIR_TOP:
			// ������
			lCnt = 0;
			for (int y = stArea.ly; y < stArea.ly + stArea.lHeight; y++) {
				m_plEdgeAvg[lCnt] = 0;

				lAvgCnt = 0;
				for (int x = stArea.lx; x < stArea.lx + stArea.lWidth; x++) {
					//if (Affine(x, y, stAffAngle, stAffPos, &lX, &lY) == false) {
					//	continue;
					//}
					// �A�t�B���}�N���ō�����
					lX = AFFINE_X(x, y, stAffAngle, stAffPos, m_stCenter);
					lY = AFFINE_Y(x, y, stAffAngle, stAffPos, m_stCenter);
					if (lX < 0 || lX >= m_lWidth || lY < 0 || lY >= m_lHeight) {
						continue;
					}

					m_plEdgeAvg[lCnt] +=  pucSrc[lY * m_lWidth + lX];
					lAvgCnt++;
				}
				if (lAvgCnt != 0) {
					m_plEdgeAvg[lCnt] /= lAvgCnt;
				}
				lCnt++;
			}
			break;
		case EN_SET_DIR_BOTTOM:
			// ������
			lCnt = 0;
			for (int y = stArea.ly + stArea.lHeight - 1; y >= stArea.ly; y--) {
				m_plEdgeAvg[lCnt] = 0;

				lAvgCnt = 0;
				for (int x = stArea.lx; x < stArea.lx + stArea.lWidth; x++) {
					//if (Affine(x, y, stAffAngle, stAffPos, &lX, &lY) == false) {
					//	continue;
					//}
					// �A�t�B���}�N���ō�����
					lX = AFFINE_X(x, y, stAffAngle, stAffPos, m_stCenter);
					lY = AFFINE_Y(x, y, stAffAngle, stAffPos, m_stCenter);
					if (lX < 0 || lX >= m_lWidth || lY < 0 || lY >= m_lHeight) {
						continue;
					}

					m_plEdgeAvg[lCnt] +=  pucSrc[lY * m_lWidth + lX];
					lAvgCnt++;
				}
				if (lAvgCnt != 0) {
					m_plEdgeAvg[lCnt] /= lAvgCnt;
				}
				lCnt++;
			}
			break;
	}

	return lCnt;
}
//---------------------------------------------------------------------------
// �A�t�B���ϊ�
bool CImgProc::Affine(long lX, long lY, ST_AFFINE_ANGLE stAffAngle, ST_LONG_POS stAffPos, ST_LONG_POS stAffCenter, long* plX, long* plY)
{
	long lPosX = lX + stAffPos.lX;
	long lPosY = lY + stAffPos.lY;

	*plX = (long)((lPosX - stAffCenter.lX) * stAffAngle.dCos - (lPosY - stAffCenter.lY) * stAffAngle.dSin + stAffCenter.lX);
	*plY = (long)((lPosX - stAffCenter.lX) * stAffAngle.dSin + (lPosY - stAffCenter.lY) * stAffAngle.dCos + stAffCenter.lY);

	if (*plX < 0 || *plX >= m_lWidth || *plY < 0 || *plY >= m_lHeight) {
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
// �G���A�J�E���g
long CImgProc::CountArea(UCHAR* pucSrc, ST_LONG_RANGE stThr, ST_LONG_RECT stArea, ST_AFFINE_ANGLE stAffAngle, ST_LONG_POS stAffPos, ST_LONG_POS stAffCenter)
{
	long lX;
	long lY;
	long lAreaCnt = 0;

	for (int y = stArea.ly; y < stArea.ly + stArea.lHeight; y++) {
		for (int x = stArea.lx; x < stArea.lx + stArea.lWidth; x++) {
			//if (Affine(x, y, stAffAngle, stAffPos, &lX, &lY) == false) {
			//	continue;
			//}
			// �A�t�B���}�N���ō�����
			lX = AFFINE_X(x, y, stAffAngle, stAffPos, stAffCenter);
			lY = AFFINE_Y(x, y, stAffAngle, stAffPos, stAffCenter);
			if (lX < 0 || lX >= m_lWidth || lY < 0 || lY >= m_lHeight) {
				continue;
			}
			if (pucSrc[lY * m_lWidth + lX] < stThr.lLo || pucSrc[lY * m_lWidth + lX] > stThr.lHi) {
				continue;
			}
			lAreaCnt++;
		}
	}

	return lAreaCnt;
}
//---------------------------------------------------------------------------
// �u�ƁA�ρv����uAx + By + C = 0�v�ɕϊ�
void CImgProc::ThetaRhoToABC(double dRho, double dTheta, double* pdA, double* pdB, double* pdC)
{
    *pdA = cos(dTheta);
    *pdB = sin(dTheta);
    *pdC = -(dRho);
}
//---------------------------------------------------------------------------
// �uAx + By + C = 0�v����u�ƁA�ρv�ɕϊ�
void CImgProc::ABCToThetaRho(double dA, double dB, double dC, double* pdRho, double* pdTheta)
{
    if (dA == 0. && dB == 0.) {
        *pdTheta = 0.;
    }
    else {
        *pdTheta = atan2(dB, dA);
    }
    *pdRho = -(dC);
}
//---------------------------------------------------------------------------
// �ƁA�ς��璼���擾
void CImgProc::ThetaRhoToLine(double dRho, double dTheta, ST_LONG_POS* pstStart, ST_LONG_POS* pstEnd)
{
	pstStart->lX = 0;
	pstStart->lY = (long)((dRho + pstStart->lX * cos(dTheta)) / sin(dTheta));

	pstEnd->lX = m_lWidth - 1;
	pstEnd->lY = (long)((dRho - pstEnd->lX * cos(dTheta)) / sin(dTheta));

	if (pstStart->lY >= 0 && pstStart->lY < m_lHeight && pstEnd->lY >= 0 && pstEnd->lY < m_lHeight) {
		return;
	}

	pstStart->lY = 0;
	pstStart->lX = (long)((dRho - pstStart->lY * sin(dTheta)) / cos(dTheta));
	pstEnd->lY = m_lHeight - 1;
	pstEnd->lX = (long)((dRho - pstEnd->lY * sin(dTheta)) / cos(dTheta));
}
//---------------------------------------------------------------------------
// �u���W�A���v����u�x�v�ɕϊ�
double CImgProc::RadianToAngle(double dRadian)
{
    return dRadian * 180. / M_PI;
}
//---------------------------------------------------------------------------
// �u�x�v����u���W�A���v�ɕϊ�
double CImgProc::AngleToRadian(double dAngle)
{
    return dAngle * M_PI / 180.;
}
//---------------------------------------------------------------------------
// d^a �̍�����
long CImgProc::PowerLong(long d, long a)
{
    long lData = 1;

    for (int i = 0; i < a; i++) {
        lData *= d;
    }

    return lData;
}
//---------------------------------------------------------------------------
// d^a �̍�����
double CImgProc::PowerDouble(double d, long a)
{
    double dData = 1;

    for (int i = 0; i < a; i++) {
        dData *= d;
    }

    return dData;
}
//---------------------------------------------------------------------------
