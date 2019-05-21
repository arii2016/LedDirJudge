//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "MainThread.h"
#include "Main.h"
#include "ImgInsp.h"
#include "Cammera.h"
#include "Common.h"
#include "Result.h"
#include "Io.h"
//---------------------------------------------------------------------------
CMainThread* g_pclMainThread = NULL;
//---------------------------------------------------------------------------
CMainThread::CMainThread(void)
{
    m_hdlThread = NULL;
	m_bThreadEndFlg = false;
	m_StateMain = ST_MAIN_INIT;
	m_bInspStart = false;

	// クリティカルセクションを初期化する
    InitializeCriticalSection(&m_CritiSec);
}
//---------------------------------------------------------------------------
CMainThread::~CMainThread(void)
{
    // クリティカルセクションを破棄する
    DeleteCriticalSection(&m_CritiSec);

	if (m_hdlThread != NULL) {
        CloseHandle(m_hdlThread);
        m_hdlThread = NULL;
    }
}
//---------------------------------------------------------------------------
// スレッド開始
void CMainThread::Resume(void)
{
    if (m_hdlThread != NULL) {
        return;
    }
    UINT unThreadID;
    m_hdlThread = (HANDLE)_beginthreadex(NULL, 0, (FuncPointer)Execute,
                                       (void *)this, CREATE_SUSPENDED,
                                       &unThreadID);
    SetThreadPriority(m_hdlThread, THREAD_PRIORITY_NORMAL);
    //SetThreadPriority(m_hdlThread, THREAD_PRIORITY_ABOVE_NORMAL);
    //SetThreadPriority(m_hdlThread, THREAD_PRIORITY_HIGHEST);
    //SetThreadPriority(m_hdlThread, THREAD_PRIORITY_TIME_CRITICAL);

	ResumeThread(m_hdlThread);
}
//---------------------------------------------------------------------------
// スレッド終了処理
void CMainThread::WaitFor(void)
{
    DWORD dwExCode;

	// スレッド終了FlgをON
    EnterCriticalSection(&m_CritiSec);   // スレッド終了フラグの書込み排他開始
	m_bThreadEndFlg = true;
    LeaveCriticalSection(&m_CritiSec);   // スレッド終了フラグの書込み排他終了

	while (1) {
        GetExitCodeThread(m_hdlThread, &dwExCode);
        if (dwExCode != STILL_ACTIVE) {
            break;
        }
    }
}
//---------------------------------------------------------------------------
void Execute(void* pPara)
{
    CMainThread* pclsMainThread;

    pclsMainThread = (CMainThread *)pPara;

    pclsMainThread->ExecThread();
}
//---------------------------------------------------------------------------
void CMainThread::ExecThread(void)
{
	m_bThreadEndFlg = false;
	m_StateMain = ST_MAIN_INIT;
	
    while (m_bThreadEndFlg == false) {
		//SleepEx(1, FALSE);
		MainRoop();	
	}
    // 出力クリア
	g_pclIo->ClearOutPut();
}
//---------------------------------------------------------------------------
// メインシーケンス
void CMainThread::MainRoop()
{
    switch (m_StateMain) {
    //-----------------------------------------------------------------------
    case ST_MAIN_INIT:        
		// READY ON
		g_pclIo->SetReady(true);
		// EOT OFF
		g_pclIo->SetEot(false);

		m_StateMain = ST_MAIN_WAIT;
        break;

	//-----------------------------------------------------------------------
    case ST_MAIN_WAIT:
        if (g_pclMain->GetScreen() == EN_SCREEN_MANUAL) {
            m_StateMain = ST_MAIN_MANUAL;
            break;
        }
		// 検査開始確認
		if (g_pclIo->CheckStart() == true || m_bInspStart == true) {
			m_bInspStart = false;
	        m_StateMain = ST_MAIN_START;
            break;
        }

        break;

    //-----------------------------------------------------------------------
    case ST_MAIN_START:
        // READY OFF
		g_pclIo->SetReady(false);
		// 結果初期化
		g_pclResult->InitResult();
		// 検査時間タイマースタート
		g_pclCommon->TimerStart(TIMER_INSP_WATCH);

		m_StateMain = ST_MAIN_GRAB_START;
		break;

	//-----------------------------------------------------------------------
    case ST_MAIN_GRAB_START:
		// 撮像開始
		if (g_pclCammera->GrabStart() == false) {
			m_StateMain = ST_MAIN_END;
			break;
		}

		m_StateMain = ST_MAIN_GRAB_WAIT;
        break;

    //-----------------------------------------------------------------------
    case ST_MAIN_GRAB_WAIT:
		if (g_pclCammera->CheckGrabEnd() == true) {
			// 撮像時間設定
			g_pclResult->stResult.lGrabTime = g_pclCommon->TimerLap(TIMER_INSP_WATCH) / 1000;
	        m_StateMain = ST_MAIN_INSP_INIT;
		}

		break;

    //-----------------------------------------------------------------------
    case ST_MAIN_INSP_INIT:
		// 画像コピー
		g_pclImgInsp->SetImgCopy(g_pclCammera->GrabImg());
		// 画像前処理
		g_pclImgInsp->InitProc();

		m_StateMain = ST_MAIN_PATTERN;
        break;

    //-----------------------------------------------------------------------
    case ST_MAIN_PATTERN:
		// パターンマッチング
		if (g_pclImgInsp->PatternMatching() == false) {
			m_StateMain = ST_MAIN_END;
			break;
		}

		m_StateMain = ST_MAIN_JUDGMENT;
        break;


	//-----------------------------------------------------------------------
    case ST_MAIN_JUDGMENT:
		// エリア判定
		if (g_pclImgInsp->Judgment() == false) {
			m_StateMain = ST_MAIN_END;
			break;
		}

		m_StateMain = ST_MAIN_END;
        break;

	//-----------------------------------------------------------------------
    case ST_MAIN_END:
		// 検査時間設定
		g_pclResult->stResult.lInspTime = g_pclCommon->TimerLap(TIMER_INSP_WATCH) / 1000;
		// 判定IO出力
		g_pclIo->SetJudge();
		// EOTをON
		g_pclIo->SetEot(true);
		// EOTのOFFタイマー開始
		g_pclCommon->TimerStart(TIMER_OFF_EOT);

		m_StateMain = ST_MAIN_SHOW_RESULT;
        break;

	//-----------------------------------------------------------------------
    case ST_MAIN_SHOW_RESULT:
		// 結果表示
		g_pclMain->ShowInspImg();

		m_StateMain = ST_MAIN_OFF_EOT;
        break;

	//-----------------------------------------------------------------------
    case ST_MAIN_OFF_EOT:
		if (g_pclCommon->TimerLap(TIMER_OFF_EOT) < 5000) {
			break;
		}
		// EOTをOFF
		g_pclIo->SetEot(false);

		m_StateMain = ST_MAIN_INIT;
        break;

	//-----------------------------------------------------------------------
    case ST_MAIN_MANUAL:
        if (g_pclMain->GetScreen() == EN_SCREEN_AUTO) {
            m_StateMain = ST_MAIN_WAIT;
        }
        break;

    }
}
//---------------------------------------------------------------------------

