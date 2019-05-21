//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "Rs232c.h"
//---------------------------------------------------------------------------
// 入力点数
#define DF_IO_INPUT_NUM	4

typedef enum {
	// スタート入力
    EN_INPUT_START = 0,

	EN_INPUT_MAX
} EN_INPUT;

typedef enum {
	// READY出力
    EN_OUTPUT_READY = 0,
	// EOT出力
    EN_OUTPUT_EOT,
	// OK出力
    EN_OUTPUT_OK,
	// NG出力
    EN_OUTPUT_NG,

	EN_OUTPUT_MAX
} EN_OUTPUT;
//---------------------------------------------------------------------------
class CIo
{
private:
	// Rs232cクラス
	CRs232c	m_clsRs232c;

	bool	m_bInput[DF_IO_INPUT_NUM];

protected:
	bool InData();
	bool OutData(EN_OUTPUT enOut, bool bOnOff);
	void SetOK(bool bOnOff);
	void SetNG(bool bOnOff);

public:
	CIo(void);
	~CIo(void);

	bool Init();
	void Close();

	void ClearOutPut();
	bool CheckStart();
	void SetReady(bool bOnOff);
	void SetEot(bool bOnOff);
	void SetJudge();

};
//---------------------------------------------------------------------------
extern CIo* g_pclIo;
//---------------------------------------------------------------------------

