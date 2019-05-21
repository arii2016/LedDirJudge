//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
class CRs232c
{
private:
    HANDLE hCom;
    char delim;

public:

    CRs232c(void);
    ~CRs232c(void);

    bool Open(WCHAR* wcPort, int bau, int bit, int parity, int stop);
    void Close(void);
    bool  Putc(char c);
    bool Puts(char *s);
    char Getc(void);
	bool Gets(char* pcData, DWORD dwTimeOut);
    void RxFlush(void);
};
//---------------------------------------------------------------------------
