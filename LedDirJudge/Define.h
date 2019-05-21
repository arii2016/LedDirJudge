//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
// バージョン履歴
// v1.00  新規作成

#define DF_NO_SERIAL_CHECK	// カメラシリアル認証を行わない


//---------------------------------------------------------------------------
// 共通定義
//---------------------------------------------------------------------------
// 最大文字数
#define DF_CHAR_MAX		1024


//---------------------------------------------------------------------------
// 画像処理定義
//---------------------------------------------------------------------------
// パターンマッチング階層探索数
#define DF_PATTERN_HIERARCHY_NUM	4
// パターンマッチング設定回転間隔
#define DF_PATTERN_SET_REV_INTERVAL	20
// パターンマッチング設定回転範囲
#define DF_PATTERN_SET_REV_RANGE	180
// パターンマッチング最大回転数
#define DF_PATTERN_REV_MAX			18

// 最大判定数
#define DF_JUDGE_NUM_MAX			5

//---------------------------------------------------------------------------
// Direct2D定義
//---------------------------------------------------------------------------
// 最大文字列数
#define DF_D2D_TEXT_MAX		128
// 最大図形数
#define DF_DIAGRAM_MAX		128
//---------------------------------------------------------------------------
// カメラ色定数
#define DF_COLOR_NUM_GRAY	1
#define DF_COLOR_NUM_RGB	3
// カメラ解像度
#define DF_CAMMERA_WIDTH	752
#define DF_CAMMERA_HEIGHT	480

//---------------------------------------------------------------------------
struct ST_LONG_SIZE {
    long lW;
    long lH;
};
struct ST_DOUBLE_SIZE {
    double dW;
    double dH;
};
struct ST_LONG_POS {
    long lX;
    long lY;
};
struct ST_DOUBLE_POS {
    double dX;
    double dY;
};
struct ST_LONG_RANGE {
    long lLo;
    long lHi;
};
struct ST_DOUBLE_RANGE {
    double dLo;
    double dHi;
};
//---------------------------------------------------------------------------
// 領域
//---------------------------------------------------------------------------
// 矩形構造体
struct ST_LONG_RECT {
    long lx;
    long ly;
    long lWidth;
    long lHeight;
};
// アフィン角度構造体
struct ST_AFFINE_ANGLE {
	// SIN
	double	dSin;
	// COS
	double	dCos;
};


//---------------------------------------------------------------------------
// 戻り値
typedef enum {
    EN_RETURN_ERR = 0,
    EN_RETURN_END,
    EN_RETURN_SUCCESS,
	EN_RETURN_MAX
} EN_RETURN;
//---------------------------------------------------------------------------
// モード
typedef enum {
    EN_SCREEN_AUTO = 0,
    EN_SCREEN_MANUAL,
    EN_SCREEN_MAX
} EN_SCREEN;
//---------------------------------------------------------------------------
