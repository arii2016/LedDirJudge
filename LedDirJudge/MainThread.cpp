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

	// �N���e�B�J���Z�N�V����������������
    InitializeCriticalSection(&m_CritiSec);
}
//---------------------------------------------------------------------------
CMainThread::~CMainThread(void)
{
    // �N���e�B�J���Z�N�V������j������
    DeleteCriticalSection(&m_CritiSec);

	if (m_hdlThread != NULL) {
        CloseHandle(m_hdlThread);
        m_hdlThread = NULL;
    }
}
//---------------------------------------------------------------------------
// �X���b�h�J�n
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
// �X���b�h�I������
void CMainThread::WaitFor(void)
{
    DWORD dwExCode;

	// �X���b�h�I��Flg��ON
    EnterCriticalSection(&m_CritiSec);   // �X���b�h�I���t���O�̏����ݔr���J�n
	m_bThreadEndFlg = true;
    LeaveCriticalSection(&m_CritiSec);   // �X���b�h�I���t���O�̏����ݔr���I��

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
    // �o�̓N���A
	g_pclIo->ClearOutPut();
}
//---------------------------------------------------------------------------
// ���C���V�[�P���X
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
		// �����J�n�m�F
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
		// ���ʏ�����
		g_pclResult->InitResult();
		// �������ԃ^�C�}�[�X�^�[�g
		g_pclCommon->TimerStart(TIMER_INSP_WATCH);

		m_StateMain = ST_MAIN_GRAB_START;
		break;

	//-----------------------------------------------------------------------
    case ST_MAIN_GRAB_START:
		// �B���J�n
		if (g_pclCammera->GrabStart() == false) {
			m_StateMain = ST_MAIN_END;
			break;
		}

		m_StateMain = ST_MAIN_GRAB_WAIT;
        break;

    //-----------------------------------------------------------------------
    case ST_MAIN_GRAB_WAIT:
		if (g_pclCammera->CheckGrabEnd() == true) {
			// �B�����Ԑݒ�
			g_pclResult->stResult.lGrabTime = g_pclCommon->TimerLap(TIMER_INSP_WATCH) / 1000;
	        m_StateMain = ST_MAIN_INSP_INIT;
		}

		break;

    //-----------------------------------------------------------------------
    case ST_MAIN_INSP_INIT:
		// �摜�R�s�[
		g_pclImgInsp->SetImgCopy(g_pclCammera->GrabImg());
		// �摜�O����
		g_pclImgInsp->InitProc();

		m_StateMain = ST_MAIN_PATTERN;
        break;

    //-----------------------------------------------------------------------
    case ST_MAIN_PATTERN:
		// �p�^�[���}�b�`���O
		if (g_pclImgInsp->PatternMatching() == false) {
			m_StateMain = ST_MAIN_END;
			break;
		}

		m_StateMain = ST_MAIN_JUDGMENT;
        break;


	//-----------------------------------------------------------------------
    case ST_MAIN_JUDGMENT:
		// �G���A����
		if (g_pclImgInsp->Judgment() == false) {
			m_StateMain = ST_MAIN_END;
			break;
		}

		m_StateMain = ST_MAIN_END;
        break;

	//-----------------------------------------------------------------------
    case ST_MAIN_END:
		// �������Ԑݒ�
		g_pclResult->stResult.lInspTime = g_pclCommon->TimerLap(TIMER_INSP_WATCH) / 1000;
		// ����IO�o��
		g_pclIo->SetJudge();
		// EOT��ON
		g_pclIo->SetEot(true);
		// EOT��OFF�^�C�}�[�J�n
		g_pclCommon->TimerStart(TIMER_OFF_EOT);

		m_StateMain = ST_MAIN_SHOW_RESULT;
        break;

	//-----------------------------------------------------------------------
    case ST_MAIN_SHOW_RESULT:
		// ���ʕ\��
		g_pclMain->ShowInspImg();

		m_StateMain = ST_MAIN_OFF_EOT;
        break;

	//-----------------------------------------------------------------------
    case ST_MAIN_OFF_EOT:
		if (g_pclCommon->TimerLap(TIMER_OFF_EOT) < 5000) {
			break;
		}
		// EOT��OFF
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

