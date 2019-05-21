//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "Define.h"
#include "Xml.h"
//---------------------------------------------------------------------------
// �񋓌^
//---------------------------------------------------------------------------
// �����񋓌^
typedef enum {
	// ��
    EN_SET_DIR_TOP = 0,
	// ��
    EN_SET_DIR_BOTTOM,
	// ��
    EN_SET_DIR_RIGHT,
	// ��
    EN_SET_DIR_LEFT,

	EN_SET_DIR_MAX
} EN_SET_DIR;
//---------------------------------------------------------------------------
// �ݒ�
//---------------------------------------------------------------------------
// �B���ݒ�
struct ST_SET_GRAB {
	// �I������
	long			lExposure;
};
// �p�^�[���}�b�`���O�̃e���v���[�g�̉�]�ʒu
struct ST_SET_PATTERN_TEMPLATE_REV_POS {
};
// �p�^�[���}�b�`���O�̃e���v���[�g�ݒ�
struct ST_SET_PATTERN_TEMPLATE {
	// �摜�T�C�Y
	long			lWidth;
	long			lHeight;
	// �摜�f�[�^
	UCHAR			ucImg[DF_CAMMERA_WIDTH * DF_CAMMERA_HEIGHT];
	// ���K�����֗p�v�Z���ʕۑ�
	long			lN;
	double			dT;
	double			dTT;
	double			dF2;
};
// �p�^�[���}�b�`���O�ݒ�
struct ST_SET_PATTERN {
	// �p�x�͈�
	ST_LONG_RANGE	stAngleRange;
	// ����臒l
	double			dCorrThr;
	// �p�^�[���G���A
	ST_LONG_RECT	stPatternArea;
	// �e���v���[�g�f�[�^(�K�w����)
	ST_SET_PATTERN_TEMPLATE	stTemplate[DF_PATTERN_HIERARCHY_NUM];
};
// �P�̔���ݒ�
struct ST_SET_JUDGE_ONE {
	// ��l��
	ST_LONG_RANGE	stThr;
	// ����G���A
	ST_LONG_RECT	stArea;
	// �G���A����臒l
	ST_LONG_RANGE	stAreaRange;
};
// ����ݒ�
struct ST_SET_JUDGE {
	// ���萔
	long	lNum;
	// �P�̔���ݒ�
	ST_SET_JUDGE_ONE	stOne[DF_JUDGE_NUM_MAX];
};
// �ݒ�\����
struct ST_SET {
	// �B���ݒ�
	ST_SET_GRAB		stGrab;
	// �p�^�[���}�b�`���O�ݒ�
	ST_SET_PATTERN	stPattern;
	// ����ݒ�
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
	// �ݒ�\����
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
