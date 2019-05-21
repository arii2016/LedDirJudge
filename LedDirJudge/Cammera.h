//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "Define.h"
#include "Inc/VTAPI.H"
//---------------------------------------------------------------------------
class CCammera
{
private:
	bool			m_bGrabEnd;

	HANDLE			m_hDevice;
	int				m_iRet;
	CapInfoStruct	m_CapInfo;

	UCHAR*		m_pucGrabImg;
	long		m_lColorNum;
	long		m_lWidth;
	long		m_lHeight;

protected:
	bool GetFileSerial(UCHAR* pucSerial);

public:

	CCammera(void);
	~CCammera(void);

	bool Init();
	void Close();

	bool SetExposure(long lExposure);
	bool GrabStart();
	void SetGrabEnd(UCHAR* pucImg);
	bool CheckGrabEnd();
	bool LoadImage(WCHAR* pwcFilePath);
	bool SaveImage(WCHAR* pwcFilePath);

	long Width();
	long Height();
	long ColorNum();
	UCHAR* GrabImg();
};
//---------------------------------------------------------------------------
extern CCammera* g_pclCammera;
//---------------------------------------------------------------------------
