//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "SysParam.h"
//---------------------------------------------------------------------------
CSysParam* g_pclSysParam = NULL;
//---------------------------------------------------------------------------
CSysParam::CSysParam(void)
{
	WCHAR wcDrive[_MAX_DRIVE];
	WCHAR wcDir[_MAX_DIR];
	WCHAR wcFName[_MAX_FNAME];
	WCHAR wcExt[_MAX_EXT];

	// システムファイルを読み込む
	GetModuleFileName(NULL, m_wcFilePath, MAX_PATH);
	_wsplitpath_s(m_wcFilePath, wcDrive, _MAX_DRIVE, wcDir, _MAX_DIR, wcFName, _MAX_FNAME, wcExt, _MAX_EXT);
	_wmakepath_s(m_wcFilePath, MAX_PATH, wcDrive, wcDir, wcFName, L"XML");

	if (Read(m_wcFilePath) == false) {
		::MessageBox(NULL, L"SYSTEM FILE READ ERROR!!!", L"ERROR", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}
}
//---------------------------------------------------------------------------
CSysParam::~CSysParam(void)
{
	if (Write(m_wcFilePath) == false) {
		::MessageBox(NULL, L"SYSTEM FILE WRITE ERROR!!!", L"ERROR", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}
}
//---------------------------------------------------------------------------
void CSysParam::Init(void)
{
	wcscpy_s(wcParaFilePath, MAX_PATH, L"");
	wcscpy_s(stCamera.wcName, DF_CHAR_MAX, L"");
	stCamera.lSerialNo = 0;
	stCamera.lGain = 100;
	stCamera.lHBlank = 250;

	wcscpy_s(stOhter.wcPassword, DF_CHAR_MAX, L"1234");
	wcscpy_s(stOhter.wcIoCom, DF_CHAR_MAX, L"COM1");
}
//---------------------------------------------------------------------------
bool CSysParam::Read(WCHAR* pwcFile)
{
	EN_RETURN	enRet;
	CXml	cXml;

	Init();

	if (cXml.OpenReadFile(pwcFile) == false) {
		return false;
	}

	while (true) {
		enRet = cXml.ReadParaOne();
		if (enRet == EN_RETURN_ERR) {
			return false;
		}
		else if (enRet == EN_RETURN_END) {
			break;
		}
		if (wcscmp(cXml.m_wcNodeList[1], L"wcParaFilePath") == 0) {
			cXml.GetReadParaString(wcParaFilePath, MAX_PATH);
			continue;
		}
		if (wcscmp(cXml.m_wcNodeList[1], L"stCamera") == 0) {

			if (wcscmp(cXml.m_wcNodeList[2], L"wcName") == 0) {
				cXml.GetReadParaString(stCamera.wcName, DF_CHAR_MAX);
				continue;
			}
			if (wcscmp(cXml.m_wcNodeList[2], L"lSerialNo") == 0) {
				cXml.GetReadParaLong(&stCamera.lSerialNo);
				continue;
			}
			if (wcscmp(cXml.m_wcNodeList[2], L"lGain") == 0) {
				cXml.GetReadParaLong(&stCamera.lGain);
				continue;
			}
			if (wcscmp(cXml.m_wcNodeList[2], L"lHBlank") == 0) {
				cXml.GetReadParaLong(&stCamera.lHBlank);
				continue;
			}
		}
		if (wcscmp(cXml.m_wcNodeList[1], L"stOhter") == 0) {
			if (wcscmp(cXml.m_wcNodeList[2], L"wcPassword") == 0) {
				cXml.GetReadParaString(stOhter.wcPassword, DF_CHAR_MAX);
				continue;
			}
			if (wcscmp(cXml.m_wcNodeList[2], L"wcIoCom") == 0) {
				cXml.GetReadParaString(stOhter.wcIoCom, DF_CHAR_MAX);
				continue;
			}
		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool CSysParam::Write(WCHAR* pwcFile)
{
	CXml	cXml;

	if (cXml.OpenWriteFile(pwcFile) == false) {
		return false;
	}

	cXml.WriteParaStart(L"SysParam", 0);

		cXml.WriteParaString(L"wcParaFilePath", 0, wcParaFilePath);

		cXml.WriteParaStart(L"stCamera", 0);

			cXml.WriteParaString(L"wcName", 0, stCamera.wcName);
			cXml.WriteParaLong(L"lSerialNo", 0, stCamera.lSerialNo);
			cXml.WriteParaLong(L"lGain", 0, stCamera.lGain);
			cXml.WriteParaLong(L"lHBlank", 0, stCamera.lHBlank);

		cXml.WriteParaEnd();	// stCamera

		cXml.WriteParaStart(L"stOhter", 0);

			cXml.WriteParaString(L"wcPassword", 0, stOhter.wcPassword);
			cXml.WriteParaString(L"wcIoCom", 0, stOhter.wcIoCom);

		cXml.WriteParaEnd();	// stOhter

	cXml.WriteParaEnd();	// SysParam

	return true;
}
//---------------------------------------------------------------------------
