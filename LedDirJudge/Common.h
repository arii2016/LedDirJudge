//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
// タイマー定数
#define TIMER_INSP_WATCH	0
#define TIMER_OFF_EOT		1
#define TIMER_RS232C		2
#define TIMER_TEMP			255
//---------------------------------------------------------------------------
class CCommon
{
private:
	// システムタイマー
	LARGE_INTEGER   __SysFreqMicro;
	LARGE_INTEGER   __TmLap[256];
	long double     __SysFreqDiv;

protected:
	void TmInitial(void);

public:
	CCommon(void);
	~CCommon(void);

	void TimerStart(short idCnt);
	DWORD TimerLap(short idCnt);

	void DoEvents(void);

};
//---------------------------------------------------------------------------
extern CCommon* g_pclCommon;
//---------------------------------------------------------------------------
