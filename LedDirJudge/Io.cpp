//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Io.h"
#include "Result.h"
#include "SysParam.h"
#include <stdio.h>
//---------------------------------------------------------------------------
CIo* g_pclIo = NULL;
//---------------------------------------------------------------------------
CIo::CIo(void)
{
	// 初期化
	for (int i = 0; i < DF_IO_INPUT_NUM; i++) {
		m_bInput[i] = false;
	}
}
//---------------------------------------------------------------------------
CIo::~CIo(void)
{
}
//---------------------------------------------------------------------------
bool CIo::Init()
{
	// COMをオープン
	if (m_clsRs232c.Open(g_pclSysParam->stOhter.wcIoCom, 9600, 8, NOPARITY, ONESTOPBIT) == false) {
		return false;
	}

	// アドレス設定
	if (m_clsRs232c.Puts("OADDR(00)") == false) {
		return false;
	}
			
	// 出力をクリア
	ClearOutPut();
	
	return true;
}
//---------------------------------------------------------------------------
void CIo::Close()
{
	// 出力をクリア
	ClearOutPut();

	m_clsRs232c.Close();
}
//---------------------------------------------------------------------------
void CIo::ClearOutPut()
{
	m_clsRs232c.Puts("O(00,09,0)");
}
//---------------------------------------------------------------------------
bool CIo::InData()
{
	char cData[DF_CHAR_MAX];

	// 入力取得コマンド
	if (m_clsRs232c.Puts("O(00,10,1)") == false) {
		return false;
	}
	// データ取得
	if (m_clsRs232c.Gets(cData, 100) == false) {
		return false;
	}
	for (int i = 0; i < DF_IO_INPUT_NUM; i++) {
		if (cData[i + 8] == '0') {
			m_bInput[i] = false;
		}
		else {
			m_bInput[i] = true;
		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool CIo::OutData(EN_OUTPUT enOut, bool bOnOff)
{
	char cData[DF_CHAR_MAX];

	if (bOnOff == true) {
		sprintf_s(cData, DF_CHAR_MAX, "O(00,%02d,1)", ((int)enOut + 1));
	}
	else {
		sprintf_s(cData, DF_CHAR_MAX, "O(00,%02d,0)", ((int)enOut + 1));
	}

	if (m_clsRs232c.Puts(cData) == false) {
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool CIo::CheckStart()
{
	if (InData() == false) {
		return false;
	}
	if (m_bInput[(int)EN_INPUT_START] == false) {
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
void CIo::SetReady(bool bOnOff)
{
	OutData(EN_OUTPUT_READY, bOnOff);
}
//---------------------------------------------------------------------------
void CIo::SetEot(bool bOnOff)
{
	OutData(EN_OUTPUT_EOT, bOnOff);
}
//---------------------------------------------------------------------------
void CIo::SetJudge()
{
	if (g_pclResult->stResult.stJudge.enJudge == EN_JUDGE_OK) {
		SetOK(true);
		SetNG(false);
	}
	else {
		SetOK(false);
		SetNG(true);
	}
}
//---------------------------------------------------------------------------
void CIo::SetOK(bool bOnOff)
{
	OutData(EN_OUTPUT_OK, bOnOff);
}
//---------------------------------------------------------------------------
void CIo::SetNG(bool bOnOff)
{
	OutData(EN_OUTPUT_NG, bOnOff);
}
//---------------------------------------------------------------------------
