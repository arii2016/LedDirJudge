//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Rs232c.h"
#include "Common.h"
#include "Define.h"
//---------------------------------------------------------------------------
#define TX_BUF_SZ       4096
#define RX_BUF_SZ       4096
//---------------------------------------------------------------------------
CRs232c::CRs232c(void)
{
    hCom = INVALID_HANDLE_VALUE;

    //ﾃﾞﾘﾐﾀ設定
    delim = 'E';
}
//---------------------------------------------------------------------------
CRs232c::~CRs232c(void)
{
    Close();
}
//---------------------------------------------------------------------------
bool CRs232c::Open(WCHAR* wcPort, int bau, int bit, int parity, int stop)
{
	bool bRet = false;
	WCHAR	wcCom[DF_CHAR_MAX];

    Close();

	swprintf(wcCom, DF_CHAR_MAX, L"\\\\.\\%s", wcPort);
    hCom = CreateFile(wcCom,
        GENERIC_READ|GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hCom == INVALID_HANDLE_VALUE) {
		goto RETURN_NG;
    }
    if (!SetCommMask(hCom, EV_RXCHAR)) {
		goto RETURN_NG;
    }
    if (!SetupComm(hCom, RX_BUF_SZ, TX_BUF_SZ)) {
		goto RETURN_NG;
    }

    COMMTIMEOUTS CommTimeOuts;
    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
    CommTimeOuts.ReadTotalTimeoutConstant = 0;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
    CommTimeOuts.WriteTotalTimeoutConstant = 20000;
    if (!SetCommTimeouts(hCom,&CommTimeOuts)) {
		goto RETURN_NG;
    }

    DCB dcb;
    GetCommState(hCom,&dcb);
    dcb.DCBlength = sizeof(DCB);
    dcb.BaudRate = bau;
    dcb.fBinary  = TRUE;
    dcb.ByteSize = (BYTE)bit;
    dcb.Parity   = (BYTE)parity;
    dcb.StopBits = (BYTE)stop;
    dcb.fParity  = 2;
    if (!SetCommState(hCom,&dcb)) {
		goto RETURN_NG;
    }

	bRet = true;

RETURN_NG:
	if (bRet == false) {
	    Close();
	}

    return bRet;
}
//---------------------------------------------------------------------------
void CRs232c::Close(void)
{
    if (hCom == INVALID_HANDLE_VALUE) {
        return;
    }
    CloseHandle(hCom);
    hCom = INVALID_HANDLE_VALUE;
}
//---------------------------------------------------------------------------
// 送信
bool CRs232c::Putc(char c)
{
    if (hCom == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD n1, n2;
    n1 = 1;
    WriteFile(hCom, &c, n1, &n2, NULL);
    if (n1 == n2) {
        return true;
    }
    else {
        return false;
    }
}
//---------------------------------------------------------------------------
bool CRs232c::Puts(char *s)
{
    if (hCom == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD n1, n2;
    n1 = strlen(s);
    WriteFile(hCom, s, n1, &n2, NULL);
    if (n1 != n2) {
       return false;
   }
    Putc(delim);

    return true;
}
//---------------------------------------------------------------------------
char CRs232c::Getc(void)
{
    if (hCom == INVALID_HANDLE_VALUE) {
        return EOF;
    }

    DWORD readbytes;
    char c;

    ReadFile(hCom, &c, 1, &readbytes, NULL);

    if (readbytes == 0) {
        return EOF;
    }
    else {
        return c & 0x000000ff;
    }
}
//---------------------------------------------------------------------------
bool CRs232c::Gets(char* pcData, DWORD dwTimeOut)
{
	char c;
	int iCnt;

	// タイマースタート
	g_pclCommon->TimerStart(TIMER_RS232C);

	iCnt = 0;
	while (true) {
		c = Getc();
		if (c != EOF) {
			if (c == delim) {
				pcData[iCnt++] = '\0';
				break;
			}
			else {
				pcData[iCnt++] = c;
			}
		}
		// タイムアウト確認
		if (g_pclCommon->TimerLap(TIMER_RS232C) / 1000 > dwTimeOut) {
			return false;
		}
	}

	return true;
}
//---------------------------------------------------------------------------
void CRs232c::RxFlush(void)
{
    while (Getc() != EOF);
}
//---------------------------------------------------------------------------
