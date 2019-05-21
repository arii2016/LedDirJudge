//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Xml.h"
//---------------------------------------------------------------------------
CXml::CXml(void)
{
}
//---------------------------------------------------------------------------
CXml::~CXml(void)
{
}
//---------------------------------------------------------------------------
bool CXml::OpenReadFile(WCHAR* pwcFile)
{
    CComPtr<IStream> pStream;

    if (FAILED(CreateXmlReader(__uuidof(IXmlReader), reinterpret_cast<void**>(&m_pReader), 0))) {
        return false;
    }
    // ファイルストリーム作成
    if (FAILED(SHCreateStreamOnFile(pwcFile, STGM_READ, &pStream))) {
        return false;
    }
	// ファイルストリームを関連付ける
    if (FAILED(m_pReader->SetInput(pStream))) {
        return false;
    }

	m_lNodeCnt = 0;

	return true;
}
//---------------------------------------------------------------------------
EN_RETURN CXml::ReadParaOne()
{
    XmlNodeType nodeType;
	LPCWSTR	pwszLocalName;
	LPCWSTR	pwszValue;

	while (S_OK == m_pReader->Read(&nodeType)) {
		HRESULT hr;

		switch (nodeType) {
		case XmlNodeType_Element:
			// 要素名を取得
			if (FAILED(m_pReader->GetLocalName(&pwszLocalName, NULL))) {
				return EN_RETURN_ERR;
			}
			wcscpy_s(m_wcNodeList[m_lNodeCnt], DF_CHAR_MAX, pwszLocalName);

			// 属性を取得
			hr = m_pReader->MoveToFirstAttribute();
			if (S_FALSE != hr && S_OK != hr) {
				return EN_RETURN_ERR;
			}
			if (S_OK == hr) {
				if (FAILED(m_pReader->GetValue(&pwszValue, NULL))) {
					return EN_RETURN_ERR;
				}
				m_lNodeAttList[m_lNodeCnt] = wcstol(pwszValue, NULL, 0);
			}
			else {
				m_lNodeAttList[m_lNodeCnt] = 0;
			}
			m_lNodeCnt++;
			break;
		case XmlNodeType_EndElement:
			if (FAILED(m_pReader->GetLocalName(&pwszLocalName, NULL))) {
				return EN_RETURN_ERR;
			}
			m_lNodeCnt--;
			break;	
		case XmlNodeType_Text:
            if (FAILED(m_pReader->GetValue(&pwszValue, NULL))) {
				return EN_RETURN_ERR;
            }
			wcscpy_s(m_wcValue, DF_CHAR_MAX, pwszValue);

			return EN_RETURN_SUCCESS;
		default:
			break;
		}
    }
	if (m_lNodeCnt != 0) {
		return EN_RETURN_ERR;
	}

	return EN_RETURN_END;
}
//---------------------------------------------------------------------------
void CXml::GetReadParaString(WCHAR* pwcValu, long lBufSize)
{
	wcscpy_s(pwcValu, lBufSize, m_wcValue);
}
//---------------------------------------------------------------------------
void CXml::GetReadParaLong(long* plValu)
{
	*plValu = wcstol(m_wcValue, NULL, 0);
}
//---------------------------------------------------------------------------
void CXml::GetReadParaDouble(double* pdValu)
{
	*pdValu = wcstod(m_wcValue, NULL);
}
//---------------------------------------------------------------------------
void CXml::GetReadParaBool(bool* pbValu)
{
	if (wcstol(m_wcValue, NULL, 0) == 0) {
		*pbValu = false;
	}
	else {
		*pbValu = true;
	}
}
//---------------------------------------------------------------------------
bool CXml::OpenWriteFile(WCHAR* pwcFile)
	{
    CComPtr<IStream> pStream;

	if (FAILED(CreateXmlWriter(__uuidof(IXmlWriter), reinterpret_cast<void**>(&m_pWriter), 0))) {
        return false;
	}

    // ファイルストリーム作成
    if (FAILED(SHCreateStreamOnFile(pwcFile, STGM_CREATE | STGM_WRITE, &pStream))) {
        return false;
    }

    if (FAILED(m_pWriter->SetOutput(pStream))) {
        return false;
    }

    // インデント有効化
    if (FAILED(m_pWriter->SetProperty(XmlWriterProperty_Indent, TRUE))) {
        return false;
    }

	// ヘッダを書き込む
	if (FAILED(m_pWriter->WriteStartDocument(XmlStandalone_Omit))) {
        return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool CXml::WriteParaStart(WCHAR* pwcNode, long lAtt)
{
	if (FAILED(m_pWriter->WriteStartElement(NULL, pwcNode, NULL))){
        return false;
	}
	if (lAtt != 0) {
		WCHAR	wcBuf[DF_CHAR_MAX];
		wsprintf(wcBuf, L"%ld", lAtt);

		if (FAILED(m_pWriter->WriteAttributeString(NULL, L"no", NULL, wcBuf))) {
	        return false;
		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool CXml::WriteParaString(WCHAR* pwcNode, long lAtt, WCHAR* pwcText)
{
	if (WriteParaStart(pwcNode, lAtt) == false){
        return false;
	}
	if (FAILED(m_pWriter->WriteString(pwcText))) {
        return false;
    }
	if (WriteParaEnd() == false){
        return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool CXml::WriteParaLong(WCHAR* pwcNode, long lAtt, long lText)
{
	WCHAR	wcBuf[DF_CHAR_MAX];

	if (WriteParaStart(pwcNode, lAtt) == false){
        return false;
	}
	wsprintf(wcBuf, L"%ld", lText);
	if (FAILED(m_pWriter->WriteString(wcBuf))) {
        return false;
    }
	if (WriteParaEnd() == false){
        return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool CXml::WriteParaDouble(WCHAR* pwcNode, long lAtt, double dText)
{
	WCHAR	wcBuf[DF_CHAR_MAX];

	if (WriteParaStart(pwcNode, lAtt) == false){
        return false;
	}
	swprintf_s(wcBuf, DF_CHAR_MAX, L"%f", dText);
	if (FAILED(m_pWriter->WriteString(wcBuf))) {
        return false;
    }
	if (WriteParaEnd() == false){
        return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool CXml::WriteParaBool(WCHAR* pwcNode, long lAtt, bool bText)
{
	WCHAR	wcBuf[DF_CHAR_MAX];

	if (WriteParaStart(pwcNode, lAtt) == false){
        return false;
	}
	if (bText == false) {
		wsprintf(wcBuf, L"%ld", 0);
	}
	else {
		wsprintf(wcBuf, L"%ld", 1);
	}
	if (FAILED(m_pWriter->WriteString(wcBuf))) {
        return false;
    }
	if (WriteParaEnd() == false){
        return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool CXml::WriteParaEnd()
{
	if (FAILED(m_pWriter->WriteFullEndElement())) {
        return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool CXml::SaveWriteFile()
{
	// すべての要素を閉じる
	if (FAILED(m_pWriter->WriteEndDocument())) {
        return false;
	}

	// 書き込む
    if (FAILED(m_pWriter->Flush())) {
        return false;
    }

	return true;
}
//---------------------------------------------------------------------------
