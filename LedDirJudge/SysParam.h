//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "Define.h"
#include "Xml.h"
//---------------------------------------------------------------------------
// 構造体宣言
//---------------------------------------------------------------------------
// カメラ構造体
struct ST_SYS_CAMERA {
	// カメラ名
	WCHAR				wcName[DF_CHAR_MAX];
	// シリアルNo
	long				lSerialNo;

	// ゲイン
	long				lGain;
	// HBlank
	long				lHBlank;

};
// その他設定構造体
struct ST_SYS_OHTER {
	// パスワード
	WCHAR				wcPassword[DF_CHAR_MAX];

	// IOポート
	WCHAR				wcIoCom[DF_CHAR_MAX];
};
//---------------------------------------------------------------------------
class CSysParam
{
private:
	WCHAR m_wcFilePath[MAX_PATH];

protected:
	void Init(void);

public:
	WCHAR			wcParaFilePath[MAX_PATH];

	// カメラ構造体
	ST_SYS_CAMERA	stCamera;
	// その他設定構造体
	ST_SYS_OHTER	stOhter;



	CSysParam(void);
	~CSysParam(void);
	bool Read(WCHAR* pwcFile);
	bool Write(WCHAR* pwcFile);
};
//---------------------------------------------------------------------------
extern CSysParam* g_pclSysParam;
//---------------------------------------------------------------------------
