//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Common.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
//---------------------------------------------------------------------------
CCommon* g_pclCommon = NULL;
//---------------------------------------------------------------------------
CCommon::CCommon(void)
{
    // 分解能を1ミリ秒に設定
    timeBeginPeriod(1);

	TmInitial();
}
//---------------------------------------------------------------------------
CCommon::~CCommon(void)
{
    // 分解能をクリア
    timeEndPeriod(1);
}
//---------------------------------------------------------------------------
void CCommon::TmInitial(void)
{
	int     i;
	for( i=0; i<256; i++ ){
		__TmLap[i].QuadPart = 0;
	}
	QueryPerformanceFrequency( &__SysFreqMicro );
	__SysFreqDiv = (long double)__SysFreqMicro.QuadPart / 1000000.0;
}
//---------------------------------------------------------------------------
void CCommon::TimerStart(short idCnt)
{
	QueryPerformanceCounter( &__TmLap[idCnt] );
	QueryPerformanceFrequency( &__SysFreqMicro );
	__SysFreqDiv = (long double)__SysFreqMicro.QuadPart / 1000000.0;
}
//---------------------------------------------------------------------------
DWORD CCommon::TimerLap(short idCnt)
{
	LARGE_INTEGER   lap;
	long double     tmp;

	QueryPerformanceCounter( &lap );

	if( __TmLap[idCnt].QuadPart <= lap.QuadPart ){
		tmp = (long double)( lap.QuadPart - __TmLap[idCnt].QuadPart) / __SysFreqDiv;
	}
	else{
		tmp = (long double)( lap.QuadPart + (_I64_MAX - __TmLap[idCnt].QuadPart)) / __SysFreqDiv;
	}

	return (DWORD)tmp;
}
//---------------------------------------------------------------------------
void CCommon::DoEvents(void)
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE)) {
		if (!AfxGetApp()->PumpMessage())
		return;
	}
}
//---------------------------------------------------------------------------
