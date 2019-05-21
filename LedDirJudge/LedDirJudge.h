
// LedDirJudge.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CLedDirJudgeApp:
// このクラスの実装については、LedDirJudge.cpp を参照してください。
//

class CLedDirJudgeApp : public CWinApp
{
public:
	CLedDirJudgeApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CLedDirJudgeApp theApp;