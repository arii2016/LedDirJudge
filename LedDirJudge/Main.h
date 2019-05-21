//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "Define.h"
#include "D2DAutoCtrl.h"
//---------------------------------------------------------------------------
class CMain
{
private:
	EN_SCREEN	m_enScreen;

	CD2DAutoCtrl*	m_pclAutoD2D;

protected:

public:
	CMain(void);
	~CMain(void);
	void Init();
	void Close();

	void SetScreen(EN_SCREEN enScreen);
	EN_SCREEN GetScreen(void);
	void SetAutoD2D(CD2DAutoCtrl* pclAutoD2D);

	bool ExecInspection();
	bool Grab();
	bool LoadImage(WCHAR* pwcFilePath);
	void ShowGrabImg();
	void ShowInspImg();

};
//---------------------------------------------------------------------------
extern CMain* g_pclMain;
//---------------------------------------------------------------------------
