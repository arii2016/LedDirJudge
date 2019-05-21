//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include "Define.h"
#include "Xml.h"
//---------------------------------------------------------------------------
// �\���̐錾
//---------------------------------------------------------------------------
// �J�����\����
struct ST_SYS_CAMERA {
	// �J������
	WCHAR				wcName[DF_CHAR_MAX];
	// �V���A��No
	long				lSerialNo;

	// �Q�C��
	long				lGain;
	// HBlank
	long				lHBlank;

};
// ���̑��ݒ�\����
struct ST_SYS_OHTER {
	// �p�X���[�h
	WCHAR				wcPassword[DF_CHAR_MAX];

	// IO�|�[�g
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

	// �J�����\����
	ST_SYS_CAMERA	stCamera;
	// ���̑��ݒ�\����
	ST_SYS_OHTER	stOhter;



	CSysParam(void);
	~CSysParam(void);
	bool Read(WCHAR* pwcFile);
	bool Write(WCHAR* pwcFile);
};
//---------------------------------------------------------------------------
extern CSysParam* g_pclSysParam;
//---------------------------------------------------------------------------
