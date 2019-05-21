//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "Define.h"
#include "D2DAutoCtrl.h"
//---------------------------------------------------------------------------
// 判定
typedef enum {
	// 未判定
    EN_JUDGE_YET = 0,
    // 良品
    EN_JUDGE_OK,
    // 不良品
    EN_JUDGE_NG,
    EN_JUDGE_MAX
} EN_JUDGE;
extern WCHAR* JudgeName[EN_JUDGE_MAX];
//---------------------------------------------------------------------------
// パターンマッチング結果
struct ST_RESULT_PATTERN {
	// 相関値
	double			dCorrVal;
	// 検出位置
	ST_LONG_POS		stDtectPos;
	// 検出角度
	double			dDtectAngle;

	// アフィン角度用
	ST_AFFINE_ANGLE	stAffAngle;
	// アフィン位置
	ST_LONG_POS		stAffPos;
	// アフィンセンター位置
	ST_LONG_POS		stAffCenter;
};
// 単体判定結果
struct ST_RESULT_JUDGE_ONE {
	// 面積数
	long			lAreaNum;
	// 判定結果
	EN_JUDGE		enJudge;
};
// 判定結果
struct ST_RESULT_JUDGE {
	// 単体判定結果
	ST_RESULT_JUDGE_ONE	stOne[DF_JUDGE_NUM_MAX];

	// 判定結果
	EN_JUDGE		enJudge;
};
// 結果構造体
struct ST_RESULT {
	// パターンマッチング結果
	ST_RESULT_PATTERN	stPattern;
	// 判定結果
	ST_RESULT_JUDGE		stJudge;
	// 撮像時間
	long				lGrabTime;
	// 検査時間
	long				lInspTime;
};
//---------------------------------------------------------------------------
class CResult
{
private:
	CStringW	m_strTime;

protected:

public:
	// 結果構造体
	ST_RESULT	stResult;

	CResult(void);
	~CResult(void);

	void InitResult();
	WCHAR* GetJudgeStr(EN_JUDGE enJudge);
	EN_D2D_BRUSH GetJudgeBrush(EN_JUDGE enJudge);
	WCHAR* GetGrabTimeStr();
	WCHAR* GetInspTimeStr();

};
//---------------------------------------------------------------------------
extern CResult* g_pclResult;
//---------------------------------------------------------------------------
