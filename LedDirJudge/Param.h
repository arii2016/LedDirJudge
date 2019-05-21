//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "Define.h"
#include "Xml.h"
//---------------------------------------------------------------------------
// 列挙型
//---------------------------------------------------------------------------
// 方向列挙型
typedef enum {
	// ↓
    EN_SET_DIR_TOP = 0,
	// ↑
    EN_SET_DIR_BOTTOM,
	// →
    EN_SET_DIR_RIGHT,
	// ←
    EN_SET_DIR_LEFT,

	EN_SET_DIR_MAX
} EN_SET_DIR;
//---------------------------------------------------------------------------
// 設定
//---------------------------------------------------------------------------
// 撮像設定
struct ST_SET_GRAB {
	// 露光時間
	long			lExposure;
};
// パターンマッチングのテンプレートの回転位置
struct ST_SET_PATTERN_TEMPLATE_REV_POS {
};
// パターンマッチングのテンプレート設定
struct ST_SET_PATTERN_TEMPLATE {
	// 画像サイズ
	long			lWidth;
	long			lHeight;
	// 画像データ
	UCHAR			ucImg[DF_CAMMERA_WIDTH * DF_CAMMERA_HEIGHT];
	// 正規化相関用計算結果保存
	long			lN;
	double			dT;
	double			dTT;
	double			dF2;
};
// パターンマッチング設定
struct ST_SET_PATTERN {
	// 角度範囲
	ST_LONG_RANGE	stAngleRange;
	// 相関閾値
	double			dCorrThr;
	// パターンエリア
	ST_LONG_RECT	stPatternArea;
	// テンプレートデータ(階層ごと)
	ST_SET_PATTERN_TEMPLATE	stTemplate[DF_PATTERN_HIERARCHY_NUM];
};
// 単体判定設定
struct ST_SET_JUDGE_ONE {
	// 二値化
	ST_LONG_RANGE	stThr;
	// 判定エリア
	ST_LONG_RECT	stArea;
	// エリア判定閾値
	ST_LONG_RANGE	stAreaRange;
};
// 判定設定
struct ST_SET_JUDGE {
	// 判定数
	long	lNum;
	// 単体判定設定
	ST_SET_JUDGE_ONE	stOne[DF_JUDGE_NUM_MAX];
};
// 設定構造体
struct ST_SET {
	// 撮像設定
	ST_SET_GRAB		stGrab;
	// パターンマッチング設定
	ST_SET_PATTERN	stPattern;
	// 判定設定
	ST_SET_JUDGE	stJudge;
};
//---------------------------------------------------------------------------
class CParam
{
private:

protected:
	void Init(void);
	void InitGrab(ST_SET_GRAB* pstGrab);
	void InitPattern(ST_SET_PATTERN* pstPattern);
	void InitJudge(ST_SET_JUDGE* pstJudge);

public:
	// 設定構造体
	ST_SET	stSet;

	CParam();
	~CParam(void);

	bool Read(WCHAR* pwcFile);
	void ReadGrab(CXml* pcXml, ST_SET_GRAB* pstGrab);
	void ReadPattern(CXml* pcXml, ST_SET_PATTERN* pstPattern);
	void ReadJudg(CXml* pcXml, ST_SET_JUDGE* pstJudg);
	bool ReadTemplate(WCHAR* pwcFile, ST_SET_PATTERN_TEMPLATE stTemplate[]);

	bool Write(WCHAR* pwcFile);
	void WriteGrab(CXml* pcXml, ST_SET_GRAB* pstGrab);
	void WritePattern(CXml* pcXml, ST_SET_PATTERN* pstPattern);
	void WriteJudg(CXml* pcXml, ST_SET_JUDGE* pstJudg);
	bool WriteTemplate(WCHAR* pwcFile, ST_SET_PATTERN_TEMPLATE stTemplate[]);
};
//---------------------------------------------------------------------------
extern CParam* g_pclParam;
//---------------------------------------------------------------------------
