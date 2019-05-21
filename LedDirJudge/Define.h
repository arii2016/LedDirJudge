//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
// �o�[�W��������
// v1.00  �V�K�쐬

#define DF_NO_SERIAL_CHECK	// �J�����V���A���F�؂��s��Ȃ�


//---------------------------------------------------------------------------
// ���ʒ�`
//---------------------------------------------------------------------------
// �ő啶����
#define DF_CHAR_MAX		1024


//---------------------------------------------------------------------------
// �摜������`
//---------------------------------------------------------------------------
// �p�^�[���}�b�`���O�K�w�T����
#define DF_PATTERN_HIERARCHY_NUM	4
// �p�^�[���}�b�`���O�ݒ��]�Ԋu
#define DF_PATTERN_SET_REV_INTERVAL	20
// �p�^�[���}�b�`���O�ݒ��]�͈�
#define DF_PATTERN_SET_REV_RANGE	180
// �p�^�[���}�b�`���O�ő��]��
#define DF_PATTERN_REV_MAX			18

// �ő唻�萔
#define DF_JUDGE_NUM_MAX			5

//---------------------------------------------------------------------------
// Direct2D��`
//---------------------------------------------------------------------------
// �ő啶����
#define DF_D2D_TEXT_MAX		128
// �ő�}�`��
#define DF_DIAGRAM_MAX		128
//---------------------------------------------------------------------------
// �J�����F�萔
#define DF_COLOR_NUM_GRAY	1
#define DF_COLOR_NUM_RGB	3
// �J�����𑜓x
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
// �̈�
//---------------------------------------------------------------------------
// ��`�\����
struct ST_LONG_RECT {
    long lx;
    long ly;
    long lWidth;
    long lHeight;
};
// �A�t�B���p�x�\����
struct ST_AFFINE_ANGLE {
	// SIN
	double	dSin;
	// COS
	double	dCos;
};


//---------------------------------------------------------------------------
// �߂�l
typedef enum {
    EN_RETURN_ERR = 0,
    EN_RETURN_END,
    EN_RETURN_SUCCESS,
	EN_RETURN_MAX
} EN_RETURN;
//---------------------------------------------------------------------------
// ���[�h
typedef enum {
    EN_SCREEN_AUTO = 0,
    EN_SCREEN_MANUAL,
    EN_SCREEN_MAX
} EN_SCREEN;
//---------------------------------------------------------------------------
