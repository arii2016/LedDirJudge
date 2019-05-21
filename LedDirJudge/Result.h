//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "Define.h"
#include "D2DAutoCtrl.h"
//---------------------------------------------------------------------------
// ����
typedef enum {
	// ������
    EN_JUDGE_YET = 0,
    // �Ǖi
    EN_JUDGE_OK,
    // �s�Ǖi
    EN_JUDGE_NG,
    EN_JUDGE_MAX
} EN_JUDGE;
extern WCHAR* JudgeName[EN_JUDGE_MAX];
//---------------------------------------------------------------------------
// �p�^�[���}�b�`���O����
struct ST_RESULT_PATTERN {
	// ���֒l
	double			dCorrVal;
	// ���o�ʒu
	ST_LONG_POS		stDtectPos;
	// ���o�p�x
	double			dDtectAngle;

	// �A�t�B���p�x�p
	ST_AFFINE_ANGLE	stAffAngle;
	// �A�t�B���ʒu
	ST_LONG_POS		stAffPos;
	// �A�t�B���Z���^�[�ʒu
	ST_LONG_POS		stAffCenter;
};
// �P�̔��茋��
struct ST_RESULT_JUDGE_ONE {
	// �ʐϐ�
	long			lAreaNum;
	// ���茋��
	EN_JUDGE		enJudge;
};
// ���茋��
struct ST_RESULT_JUDGE {
	// �P�̔��茋��
	ST_RESULT_JUDGE_ONE	stOne[DF_JUDGE_NUM_MAX];

	// ���茋��
	EN_JUDGE		enJudge;
};
// ���ʍ\����
struct ST_RESULT {
	// �p�^�[���}�b�`���O����
	ST_RESULT_PATTERN	stPattern;
	// ���茋��
	ST_RESULT_JUDGE		stJudge;
	// �B������
	long				lGrabTime;
	// ��������
	long				lInspTime;
};
//---------------------------------------------------------------------------
class CResult
{
private:
	CStringW	m_strTime;

protected:

public:
	// ���ʍ\����
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
