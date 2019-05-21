//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Param.h"
#include "SysParam.h"
#include <math.h>
//---------------------------------------------------------------------------
CParam* g_pclParam = NULL;
//---------------------------------------------------------------------------
CParam::CParam()
{
	if (Read(g_pclSysParam->wcParaFilePath) == false) {
		::MessageBox(NULL, L"PARA FILE READ ERROR!!!", L"ERROR", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}
}
//---------------------------------------------------------------------------
CParam::~CParam(void)
{
	if (Write(g_pclSysParam->wcParaFilePath) == false) {
		::MessageBox(NULL, L"PARA FILE WRITE ERROR!!!", L"ERROR", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}
}
//---------------------------------------------------------------------------
void CParam::Init(void)
{
	InitGrab(&stSet.stGrab);
	InitPattern(&stSet.stPattern);
	InitJudge(&stSet.stJudge);
}
//---------------------------------------------------------------------------
void CParam::InitGrab(ST_SET_GRAB* pstGrab)
{
	pstGrab->lExposure = 10;
}
//---------------------------------------------------------------------------
void CParam::InitPattern(ST_SET_PATTERN* pstPattern)
{
	pstPattern->stAngleRange.lLo = -(DF_PATTERN_SET_REV_INTERVAL * 4);
	pstPattern->stAngleRange.lHi = DF_PATTERN_SET_REV_INTERVAL * 4;

	pstPattern->dCorrThr = 85.0;

	pstPattern->stPatternArea.lx = 100;
	pstPattern->stPatternArea.ly = 100;
	pstPattern->stPatternArea.lWidth = 200;
	pstPattern->stPatternArea.lHeight = 200;

	for (int i = DF_PATTERN_HIERARCHY_NUM - 1; i >= 0; i--) {
		pstPattern->stTemplate[i].lWidth = (long)(DF_CAMMERA_WIDTH / 2);
		pstPattern->stTemplate[i].lHeight = (long)(DF_CAMMERA_HEIGHT / 2);
		memset(pstPattern->stTemplate[i].ucImg, 0, sizeof(UCHAR) * DF_CAMMERA_WIDTH * DF_CAMMERA_HEIGHT);

		pstPattern->stTemplate[i].lN = pstPattern->stTemplate[i].lWidth * pstPattern->stTemplate[i].lHeight;
		pstPattern->stTemplate[i].dT = 0.0;
		pstPattern->stTemplate[i].dTT = 0.0;
		pstPattern->stTemplate[i].dF2 = 0.0;
	}
}
//---------------------------------------------------------------------------
void CParam::InitJudge(ST_SET_JUDGE* pstJudge)
{
	
	pstJudge->lNum = DF_JUDGE_NUM_MAX;

	for (int i = 0; i < DF_JUDGE_NUM_MAX; i++) {
		pstJudge->stOne[i].stThr.lLo = 100;
		pstJudge->stOne[i].stThr.lHi = 255;

		pstJudge->stOne[i].stArea.lx = 300;
		pstJudge->stOne[i].stArea.ly = 300;
		pstJudge->stOne[i].stArea.lWidth = 20;
		pstJudge->stOne[i].stArea.lHeight = 20;

		pstJudge->stOne[i].stAreaRange.lLo = 1;
		pstJudge->stOne[i].stAreaRange.lHi = 1000;
	}
}
//---------------------------------------------------------------------------
bool CParam::Read(WCHAR* pwcFile)
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
		if (wcscmp(cXml.m_wcNodeList[1], L"stSet") == 0) {

			if (wcscmp(cXml.m_wcNodeList[2], L"stGrab") == 0) {
				ReadGrab(&cXml, &stSet.stGrab);
				continue;
			}
			if (wcscmp(cXml.m_wcNodeList[2], L"stPattern") == 0) {
				ReadPattern(&cXml, &stSet.stPattern);
				continue;
			}
			if (wcscmp(cXml.m_wcNodeList[2], L"stJudge") == 0) {
				ReadJudg(&cXml, &stSet.stJudge);
				continue;
			}
		}
	}

	// テンプレートデータ読み込み
	if (ReadTemplate(pwcFile, stSet.stPattern.stTemplate) == false) {
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
void CParam::ReadGrab(CXml* pcXml, ST_SET_GRAB* pstGrab)
{
	if (wcscmp(pcXml->m_wcNodeList[3], L"lExposure") == 0) {
		pcXml->GetReadParaLong(&pstGrab->lExposure);
		return;
	}
}
//---------------------------------------------------------------------------
void CParam::ReadPattern(CXml* pcXml, ST_SET_PATTERN* pstPattern)
{
	if (wcscmp(pcXml->m_wcNodeList[3], L"stAngleRange") == 0) {
		if (wcscmp(pcXml->m_wcNodeList[4], L"lLo") == 0) {
			pcXml->GetReadParaLong(&pstPattern->stAngleRange.lLo);
			return;
		}
		if (wcscmp(pcXml->m_wcNodeList[4], L"lHi") == 0) {
			pcXml->GetReadParaLong(&pstPattern->stAngleRange.lHi);
			return;
		}
	}
	if (wcscmp(pcXml->m_wcNodeList[3], L"dCorrThr") == 0) {
		pcXml->GetReadParaDouble(&pstPattern->dCorrThr);
		return;
	}
	if (wcscmp(pcXml->m_wcNodeList[3], L"stPatternArea") == 0) {
		if (wcscmp(pcXml->m_wcNodeList[4], L"lx") == 0) {
			pcXml->GetReadParaLong(&pstPattern->stPatternArea.lx);
			return;
		}
		if (wcscmp(pcXml->m_wcNodeList[4], L"ly") == 0) {
			pcXml->GetReadParaLong(&pstPattern->stPatternArea.ly);
			return;
		}
		if (wcscmp(pcXml->m_wcNodeList[4], L"lWidth") == 0) {
			pcXml->GetReadParaLong(&pstPattern->stPatternArea.lWidth);
			return;
		}
		if (wcscmp(pcXml->m_wcNodeList[4], L"lHeight") == 0) {
			pcXml->GetReadParaLong(&pstPattern->stPatternArea.lHeight);
			return;
		}
	}
}
//---------------------------------------------------------------------------
void CParam::ReadJudg(CXml* pcXml, ST_SET_JUDGE* pstJudg)
{
	int		i1;

	if (wcscmp(pcXml->m_wcNodeList[3], L"lNum") == 0) {
		pcXml->GetReadParaLong(&pstJudg->lNum);
		return;
	}

	if (wcscmp(pcXml->m_wcNodeList[3], L"stOne") == 0) {
		i1 = pcXml->m_lNodeAttList[3] - 1;

		if (wcscmp(pcXml->m_wcNodeList[4], L"stThr") == 0) {
			if (wcscmp(pcXml->m_wcNodeList[5], L"lLo") == 0) {
				pcXml->GetReadParaLong(&pstJudg->stOne[i1].stThr.lLo);
				return;
			}
			if (wcscmp(pcXml->m_wcNodeList[5], L"lHi") == 0) {
				pcXml->GetReadParaLong(&pstJudg->stOne[i1].stThr.lHi);
				return;
			}
		}
		if (wcscmp(pcXml->m_wcNodeList[4], L"stArea") == 0) {
			if (wcscmp(pcXml->m_wcNodeList[5], L"lx") == 0) {
				pcXml->GetReadParaLong(&pstJudg->stOne[i1].stArea.lx);
				return;
			}
			if (wcscmp(pcXml->m_wcNodeList[5], L"ly") == 0) {
				pcXml->GetReadParaLong(&pstJudg->stOne[i1].stArea.ly);
				return;
			}
			if (wcscmp(pcXml->m_wcNodeList[5], L"lWidth") == 0) {
				pcXml->GetReadParaLong(&pstJudg->stOne[i1].stArea.lWidth);
				return;
			}
			if (wcscmp(pcXml->m_wcNodeList[5], L"lHeight") == 0) {
				pcXml->GetReadParaLong(&pstJudg->stOne[i1].stArea.lHeight);
				return;
			}
		}
		if (wcscmp(pcXml->m_wcNodeList[4], L"stAreaRange") == 0) {
			if (wcscmp(pcXml->m_wcNodeList[5], L"lLo") == 0) {
				pcXml->GetReadParaLong(&pstJudg->stOne[i1].stAreaRange.lLo);
				return;
			}
			if (wcscmp(pcXml->m_wcNodeList[5], L"lHi") == 0) {
				pcXml->GetReadParaLong(&pstJudg->stOne[i1].stAreaRange.lHi);
				return;
			}
		}
	}
}
//---------------------------------------------------------------------------
bool CParam::ReadTemplate(WCHAR* pwcFile, ST_SET_PATTERN_TEMPLATE stTemplate[])
{
	// テンプレートファイルパス作成
	WCHAR			wcDrive[_MAX_DRIVE];
	WCHAR			wcDir[_MAX_DIR];
	WCHAR			wcFName[_MAX_FNAME];
	WCHAR			wcExt[_MAX_EXT];
	WCHAR			wcFilePath[MAX_PATH];
	_wsplitpath_s(pwcFile, wcDrive, _MAX_DRIVE, wcDir, _MAX_DIR, wcFName, _MAX_FNAME, wcExt, _MAX_EXT);
	_wmakepath_s(wcFilePath, MAX_PATH, wcDrive, wcDir, wcFName, L"tmp");

	// ファイルから読み込み
	bool bRet = false;
	FILE* fp = NULL;

	if (_wfopen_s(&fp, wcFilePath, L"rb") != 0) {
		goto REUTNR_NG;
	}

	if (fread(stTemplate, sizeof(ST_SET_PATTERN_TEMPLATE), DF_PATTERN_HIERARCHY_NUM, fp) != DF_PATTERN_HIERARCHY_NUM) {
		goto REUTNR_NG;
	}

	bRet = true;

REUTNR_NG:
	if (fp != NULL) {
		fclose(fp);
	}

	return bRet;
}
//---------------------------------------------------------------------------
bool CParam::Write(WCHAR* pwcFile)
{
	CXml	cXml;

	if (cXml.OpenWriteFile(pwcFile) == false) {
		return false;
	}

	cXml.WriteParaStart(L"Param", 0);

		cXml.WriteParaStart(L"stSet", 0);

			WriteGrab(&cXml, &stSet.stGrab);
			WritePattern(&cXml, &stSet.stPattern);
			WriteJudg(&cXml, &stSet.stJudge);

		cXml.WriteParaEnd();	// stSet

	cXml.WriteParaEnd();	// Param

	if (WriteTemplate(pwcFile, stSet.stPattern.stTemplate) == false) {
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
void CParam::WriteGrab(CXml* pcXml, ST_SET_GRAB* pstGrab)
{
	pcXml->WriteParaStart(L"stGrab", 0);

		pcXml->WriteParaLong(L"lExposure", 0, pstGrab->lExposure);

	pcXml->WriteParaEnd();	// stGrab
}
//---------------------------------------------------------------------------
void CParam::WritePattern(CXml* pcXml, ST_SET_PATTERN* pstPattern)
{
	pcXml->WriteParaStart(L"stPattern", 0);

		pcXml->WriteParaStart(L"stAngleRange", 0);

			pcXml->WriteParaLong(L"lLo", 0, pstPattern->stAngleRange.lLo);
			pcXml->WriteParaLong(L"lHi", 0, pstPattern->stAngleRange.lHi);

		pcXml->WriteParaEnd();	// stAngleRange

		pcXml->WriteParaDouble(L"dCorrThr", 0, pstPattern->dCorrThr);

		pcXml->WriteParaStart(L"stPatternArea", 0);

			pcXml->WriteParaLong(L"lx", 0, pstPattern->stPatternArea.lx);
			pcXml->WriteParaLong(L"ly", 0, pstPattern->stPatternArea.ly);
			pcXml->WriteParaLong(L"lWidth", 0, pstPattern->stPatternArea.lWidth);
			pcXml->WriteParaLong(L"lHeight", 0, pstPattern->stPatternArea.lHeight);

		pcXml->WriteParaEnd();	// stPatternArea


	pcXml->WriteParaEnd();	// stPattern
}
//---------------------------------------------------------------------------
void CParam::WriteJudg(CXml* pcXml, ST_SET_JUDGE* pstJudg)
{
	pcXml->WriteParaStart(L"stJudge", 0);

		pcXml->WriteParaLong(L"lNum", 0, pstJudg->lNum);

		for (int i = 0; i < DF_JUDGE_NUM_MAX; i++) {
			pcXml->WriteParaStart(L"stOne", i + 1);

				pcXml->WriteParaStart(L"stThr", 0);

					pcXml->WriteParaLong(L"lLo", 0, pstJudg->stOne[i].stThr.lLo);
					pcXml->WriteParaLong(L"lHi", 0, pstJudg->stOne[i].stThr.lHi);

				pcXml->WriteParaEnd();	// stThr

				pcXml->WriteParaStart(L"stArea", 0);

					pcXml->WriteParaLong(L"lx", 0, pstJudg->stOne[i].stArea.lx);
					pcXml->WriteParaLong(L"ly", 0, pstJudg->stOne[i].stArea.ly);
					pcXml->WriteParaLong(L"lWidth", 0, pstJudg->stOne[i].stArea.lWidth);
					pcXml->WriteParaLong(L"lHeight", 0, pstJudg->stOne[i].stArea.lHeight);

				pcXml->WriteParaEnd();	// stArea

				pcXml->WriteParaStart(L"stAreaRange", 0);

					pcXml->WriteParaLong(L"lLo", 0, pstJudg->stOne[i].stAreaRange.lLo);
					pcXml->WriteParaLong(L"lHi", 0, pstJudg->stOne[i].stAreaRange.lHi);

				pcXml->WriteParaEnd();	// stAreaRange

			pcXml->WriteParaEnd();	// stOne
		}

	pcXml->WriteParaEnd();	// stJudge
}
//---------------------------------------------------------------------------
bool CParam::WriteTemplate(WCHAR* pwcFile, ST_SET_PATTERN_TEMPLATE stTemplate[])
{
	// テンプレートファイルパス作成
	WCHAR			wcDrive[_MAX_DRIVE];
	WCHAR			wcDir[_MAX_DIR];
	WCHAR			wcFName[_MAX_FNAME];
	WCHAR			wcExt[_MAX_EXT];
	WCHAR			wcFilePath[MAX_PATH];
	_wsplitpath_s(pwcFile, wcDrive, _MAX_DRIVE, wcDir, _MAX_DIR, wcFName, _MAX_FNAME, wcExt, _MAX_EXT);
	_wmakepath_s(wcFilePath, MAX_PATH, wcDrive, wcDir, wcFName, L"tmp");

	// ファイルへ書き込み
	bool bRet = false;
	FILE* fp = NULL;

	if (_wfopen_s(&fp, wcFilePath, L"wb") != 0) {
		goto REUTNR_NG;
	}

	if (fwrite(stTemplate, sizeof(ST_SET_PATTERN_TEMPLATE), DF_PATTERN_HIERARCHY_NUM, fp) != DF_PATTERN_HIERARCHY_NUM) {
		goto REUTNR_NG;
	}

	bRet = true;

REUTNR_NG:
	if (fp != NULL) {
		fclose(fp);
	}

	return bRet;
}
//---------------------------------------------------------------------------
