//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include <atlbase.h>

#include <xmllite.h>
#pragma comment(lib, "xmllite.lib")
//---------------------------------------------------------------------------
#include "Define.h"
//---------------------------------------------------------------------------
// ç≈ëÂÉmÅ[Éhêî
#define DF_NODE_MAX		100
//---------------------------------------------------------------------------
class CXml
{
private:
    CComPtr<IXmlReader> m_pReader;
    CComPtr<IXmlWriter> m_pWriter;

protected:

public:
	long	m_lNodeCnt;
	WCHAR	m_wcNodeList[DF_NODE_MAX][DF_CHAR_MAX];
	long	m_lNodeAttList[DF_NODE_MAX];
	WCHAR	m_wcValue[DF_CHAR_MAX];

	CXml(void);
	~CXml(void);

	bool OpenReadFile(WCHAR* pwcFile);
	EN_RETURN ReadParaOne();
	void GetReadParaString(WCHAR* pwcValu, long lBufSize);
	void GetReadParaLong(long* plValu);
	void GetReadParaDouble(double* pdValu);
	void GetReadParaBool(bool* pbValu);

	bool OpenWriteFile(WCHAR* pwcFile);
	bool WriteParaStart(WCHAR* pwcNode, long lAtt);
	bool WriteParaString(WCHAR* pwcNode, long lAtt, WCHAR* pwcText);
	bool WriteParaLong(WCHAR* pwcNode, long lAtt, long lText);
	bool WriteParaDouble(WCHAR* pwcNode, long lAtt, double dText);
	bool WriteParaBool(WCHAR* pwcNode, long lAtt, bool bText);
	bool WriteParaEnd();
	bool SaveWriteFile();

};
//---------------------------------------------------------------------------
