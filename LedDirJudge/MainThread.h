//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
typedef enum {
	ST_MAIN_INIT,
    ST_MAIN_WAIT,
	ST_MAIN_START,
    ST_MAIN_GRAB_START,
    ST_MAIN_GRAB_WAIT,
    ST_MAIN_INSP_INIT,
    ST_MAIN_PATTERN,
    ST_MAIN_JUDGMENT,
	ST_MAIN_END,
    ST_MAIN_SHOW_RESULT,
    ST_MAIN_OFF_EOT,
    ST_MAIN_MANUAL
} STEP_MAIN;
//---------------------------------------------------------------------------
class CMainThread
{
private:
    HANDLE				m_hdlThread;
	CRITICAL_SECTION	m_CritiSec;
	bool				m_bThreadEndFlg;
	STEP_MAIN			m_StateMain;

protected:
	void MainRoop();

public:
	CMainThread(void);
	~CMainThread(void);

    void Resume(void);
    void WaitFor(void);
    void ExecThread(void);

	bool m_bInspStart;
};
//---------------------------------------------------------------------------
typedef unsigned int (__stdcall *FuncPointer)(void *);
void Execute(void* pPara);
//---------------------------------------------------------------------------
extern CMainThread* g_pclMainThread;
//---------------------------------------------------------------------------

