//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Cammera.h"
#include "Param.h"
#include "SysParam.h"
#include <stdio.h>
//---------------------------------------------------------------------------
#pragma comment(lib, "lib/VTCamAPIu.lib")
//---------------------------------------------------------------------------
CCammera* g_pclCammera = NULL;
//---------------------------------------------------------------------------
#define DF_SERIAL_BUF_NUM	16
//---------------------------------------------------------------------------
// コールバック関数
VOID CALLBACK FrameCallback(LPVOID pData, LPVOID lpReserve, LPVOID lpContext)
{
	((CCammera *)lpContext)->SetGrabEnd((UCHAR *)pData);
};
//---------------------------------------------------------------------------
CCammera::CCammera()
{
	m_hDevice = NULL;
	m_pucGrabImg = NULL;
	m_lColorNum = DF_COLOR_NUM_GRAY;
}
//---------------------------------------------------------------------------
CCammera::~CCammera(void)
{
}
//---------------------------------------------------------------------------
bool CCammera::Init()
{
	int nIndex = 1;
#ifndef DF_NO_SERIAL_CHECK
	UCHAR ucCamSerial[DF_SERIAL_BUF_NUM + 1];
	UCHAR ucFileSerial[DF_SERIAL_BUF_NUM + 1];
#endif

	Close();

	// 色数
	m_lColorNum = DF_COLOR_NUM_RGB;
	// 画像サイズ
	m_lWidth = DF_CAMMERA_WIDTH;
	m_lHeight = DF_CAMMERA_HEIGHT;

	// 画像バッファーを生成
	m_pucGrabImg = new UCHAR[m_lWidth * m_lHeight * m_lColorNum + 512];
	if (m_pucGrabImg == NULL) {
		goto RETURN_NG;
	}
	// 画像バッファー初期化
	memset(m_pucGrabImg, 0, sizeof(UCHAR) * m_lWidth * m_lHeight * m_lColorNum);

	// カメラ情報設定
	memset(&m_CapInfo, 0, sizeof(CapInfoStruct));
	m_CapInfo.Buffer = m_pucGrabImg;
	m_CapInfo.Width = m_lWidth;
	m_CapInfo.Height = m_lHeight;
	m_CapInfo.Exposure = 400;
	m_CapInfo.Gain[0] = 32;
	m_CapInfo.Gain[1] = 32;
	m_CapInfo.Gain[2] = 32;

	// カメラオープン
	m_iRet = VT_Initialize(L"LED", &nIndex, &m_CapInfo, &m_hDevice);
	if (m_iRet != ResSuccess) {
		goto RETURN_NG;
	}

#ifndef DF_NO_SERIAL_CHECK
	// シリアルNo取得
	memset(ucCamSerial, NULL, sizeof(ucCamSerial));
	m_iRet = VT_GetSerialNumber(m_hDevice, ucCamSerial);
	if (m_iRet != ResSuccess) {
		goto RETURN_NG;
	}
	// シリアルをファイルから取得
	if (GetFileSerial(ucFileSerial) == false) {
		goto RETURN_NG;
	}
	// 比較
	if (_mbscmp(ucCamSerial, ucFileSerial) != 0) {
		goto RETURN_NG;
	}
#endif

	// カメラ設定取得
	m_iRet =VT_GetCapInfo(m_hDevice, &m_CapInfo);
	if (m_iRet != ResSuccess) {
		goto RETURN_NG;
	}

	// 設定
	m_iRet = VT_SetParamColor(m_hDevice, COLOR_RGB24);
	if (m_iRet != ResSuccess) {
		goto RETURN_NG;
	}
	m_iRet = VT_SetContrastValue(m_hDevice, 16);
	if (m_iRet != ResSuccess) {
		goto RETURN_NG;
	}
	m_iRet = VT_SetGainR(m_hDevice, 574);
	if (m_iRet != ResSuccess) {
		goto RETURN_NG;
	}
	m_iRet = VT_SetGainG(m_hDevice, 500);
	if (m_iRet != ResSuccess) {
		goto RETURN_NG;
	}
	m_iRet = VT_SetGainB(m_hDevice, 762);
	if (m_iRet != ResSuccess) {
		goto RETURN_NG;
	}
	m_iRet = VT_SetHBlank(m_hDevice, g_pclSysParam->stCamera.lHBlank);
	if (m_iRet != ResSuccess) {
		goto RETURN_NG;
	}
	m_iRet = VT_SetVBlank(m_hDevice, 45);
	if (m_iRet != ResSuccess) {
		goto RETURN_NG;
	}

	// トリガーモード設定ON
	m_iRet = VT_SetTriggerMode(m_hDevice, TRUE);
	if (m_iRet != ResSuccess) {
		goto RETURN_NG;
	}

	// コールバック関数設定
	m_iRet = VT_SetFrameCallBack(m_hDevice, FrameCallback, LPVOID(this));
	if (m_iRet != ResSuccess) {
		goto RETURN_NG;
	}

	// カメラ転送開始
	m_iRet = VT_StartView(m_hDevice, L"", 0, 0, 0, 0, 0, NULL, NULL);
	if (m_iRet != ResSuccess) {
		goto RETURN_NG;
	}

	return true;

RETURN_NG:
	Close();
	return false;
}
//---------------------------------------------------------------------------
void CCammera::Close()
{
	if (m_pucGrabImg != NULL) {
		delete[] m_pucGrabImg;
		m_pucGrabImg = NULL;
	}
	if (m_hDevice != NULL) {
		VT_StopView(m_hDevice);
		VT_Uninitialize(m_hDevice);
		m_hDevice = NULL;
	}
}
//---------------------------------------------------------------------------
bool CCammera::SetExposure(long lExposure)
{
	// カメラ情報取得
	m_iRet =VT_GetCapInfo(m_hDevice, &m_CapInfo);
	if (m_iRet != ResSuccess) {
		return false;
	}
	// 露光時間設定
	m_CapInfo.Exposure = lExposure;

	// カメラ情報設定
	m_iRet =VT_SetCapInfo(m_hDevice, &m_CapInfo);
	if (m_iRet != ResSuccess) {
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool CCammera::GrabStart()
{
	m_bGrabEnd = false;

	m_iRet = VT_SoftTrigger(m_hDevice);
	if (m_iRet != ResSuccess) {
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
void CCammera::SetGrabEnd(UCHAR* pucImg)
{
	memcpy(m_pucGrabImg, pucImg, m_lWidth * m_lHeight * m_lColorNum);
	m_bGrabEnd = true;
}
//---------------------------------------------------------------------------
bool CCammera::CheckGrabEnd()
{
	return m_bGrabEnd;
}
//---------------------------------------------------------------------------
bool CCammera::LoadImage(WCHAR* pwcFilePath)
{
	CImage	clsImg;
	long	lImgWidth;
	long	lImgHeight;

	// 画像ファイルの読み込み
	if (FAILED(clsImg.Load(pwcFilePath))) {
		return false;
	}
	// 画像サイズ取得
	lImgWidth = clsImg.GetWidth();
	lImgHeight = clsImg.GetHeight();
	// 画像サイズ確認
	if (m_lWidth != lImgWidth || m_lHeight != lImgHeight) {
		return false;
	}
	// 画像データコピー
	for (int i = 0; i < m_lHeight; i++) {
		for (int j = 0; j < m_lWidth; j++) {
			COLORREF c = clsImg.GetPixel(j, i);

			m_pucGrabImg[3 * (i * m_lWidth + j) + 0] = GetBValue(c);
			m_pucGrabImg[3 * (i * m_lWidth + j) + 1] = GetGValue(c);
			m_pucGrabImg[3 * (i * m_lWidth + j) + 2] = GetRValue(c);			
		}
	}
	
	return true;
}
//---------------------------------------------------------------------------
bool CCammera::SaveImage(WCHAR* pwcFilePath)
{


	return true;
}
//---------------------------------------------------------------------------
bool CCammera::GetFileSerial(UCHAR* pucSerial)
{
	bool bRet = false;
    FILE *fp = NULL;
    char str[DF_SERIAL_BUF_NUM + 1];

    fopen_s(&fp, "C:\\Windows\\tecpho.dll", "r");
    if (fp == NULL) {
        goto RETURN_NG;
    }

	memset(str, NULL, sizeof(str));
    if (fgets(str, DF_SERIAL_BUF_NUM, fp) == NULL) {
		goto RETURN_NG;
	}
	strncpy_s((char*)pucSerial, DF_SERIAL_BUF_NUM, (const char*)str, DF_SERIAL_BUF_NUM);

	bRet = true;

RETURN_NG:
    if (fp != NULL) {
        fclose(fp);
    }

	return bRet;
}
//---------------------------------------------------------------------------
long CCammera::Width()
{
	return m_lWidth;
}
//---------------------------------------------------------------------------
long CCammera::Height()
{
	return m_lHeight;
}
//---------------------------------------------------------------------------
long CCammera::ColorNum()
{
	return m_lColorNum;
}
//---------------------------------------------------------------------------
UCHAR* CCammera::GrabImg()
{
	return m_pucGrabImg;
}
//---------------------------------------------------------------------------
