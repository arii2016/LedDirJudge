//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "Rs232c.h"
//---------------------------------------------------------------------------
// ���͓_��
#define DF_IO_INPUT_NUM	4

typedef enum {
	// �X�^�[�g����
    EN_INPUT_START = 0,

	EN_INPUT_MAX
} EN_INPUT;

typedef enum {
	// READY�o��
    EN_OUTPUT_READY = 0,
	// EOT�o��
    EN_OUTPUT_EOT,
	// OK�o��
    EN_OUTPUT_OK,
	// NG�o��
    EN_OUTPUT_NG,

	EN_OUTPUT_MAX
} EN_OUTPUT;
//---------------------------------------------------------------------------
class CIo
{
private:
	// Rs232c�N���X
	CRs232c	m_clsRs232c;

	bool	m_bInput[DF_IO_INPUT_NUM];

protected:
	bool InData();
	bool OutData(EN_OUTPUT enOut, bool bOnOff);
	void SetOK(bool bOnOff);
	void SetNG(bool bOnOff);

public:
	CIo(void);
	~CIo(void);

	bool Init();
	void Close();

	void ClearOutPut();
	bool CheckStart();
	void SetReady(bool bOnOff);
	void SetEot(bool bOnOff);
	void SetJudge();

};
//---------------------------------------------------------------------------
extern CIo* g_pclIo;
//---------------------------------------------------------------------------

