#pragma once

class SiAiAlgorithm
{
public:
	SiAiAlgorithm();   
	virtual ~SiAiAlgorithm();

public:
    void HexToAscii(unsigned char ucData, unsigned char *pucDataOut);
    unsigned char AsciiToHex(unsigned char *ucBuffer);
    void HexToAsciiString(unsigned char *ucData, unsigned long ulLength, unsigned char *pucDataOut);
    void AsciiToHexString(unsigned char *ucBuffer, unsigned long ulLength, unsigned char *pucDataOut);
    CString HexToCString(unsigned char *ucBuffer, unsigned long ulLength);
    CString AsciiToCString(unsigned char *ucBuffer, unsigned long ulLength);
	void PackUint16Data(VOID_T *address, CString value);
	unsigned long long StrToUnsignedLongLong(CString string, int base);
	long long StrToSignedLongLong(CString string, int base);
	CString UnsignedLongLongToStr(unsigned long long value);
	CString SignedLongLongToStr(long long value);
	void CalculateCaliSlopeIntercept(INT64_T yData1, INT64_T xData1, INT64_T yData2, INT64_T xData2, INT32_T *slope, INT32_T *intercept);  
};

#ifdef __cplusplus
extern "C" {
#endif
ERROR_T AiServerSleep(UINT32_T timeout);
UINT64_T AiServerTickGet(void);
#ifdef __cplusplus
}
#endif
