// SiAiAlgorithm.cpp : 实现文件
//

#include "stdafx.h"
#include "SiAiGlobal.h"
#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// Global Variable Declaration

////////////////////////////////////////////////////////////////////////////
// Extern Variable Declaration

////////////////////////////////////////////////////////////////////////////
// Function Declaration
SiAiAlgorithm::SiAiAlgorithm()
{
}

SiAiAlgorithm::~SiAiAlgorithm()
{
}

void SiAiAlgorithm::HexToAscii(unsigned char ucData, unsigned char *pucDataOut)
{
    unsigned char Temp[3];
    unsigned char ucRet = 0;

    Temp[0] = (ucData & 0xF0) >> 4;
    Temp[1] = ucData & 0x0F;

    if(Temp[0] <= 0x09) {
        Temp[0] = Temp[0] + 0x30;
    } else if((Temp[0] >= 0x0A)  && (Temp[0] <= 0x0F)) {
        Temp[0] = (Temp[0] - 0x0A) + 'A';
    }

    if(Temp[1] <= 0x09) {
        Temp[1] = Temp[1] + 0x30;
    } else if((Temp[1] >= 0x0A)  && (Temp[1] <= 0x0F)) {
        Temp[1] = (Temp[1] - 0x0A) + 'A';
    }

    memcpy(pucDataOut, Temp, 2);
    return;
}

unsigned char SiAiAlgorithm::AsciiToHex(unsigned char *ucBuffer)
{
    unsigned char Temp[2];
    unsigned char ucTemp = 0x00;

    if((ucBuffer[0] >= '0') && (ucBuffer[0] <= '9')) {
        Temp[0] = ucBuffer[0] - 0x30;
    } else if((ucBuffer[0] >= 'a') && (ucBuffer[0] <= 'z')) {
        Temp[0] = ucBuffer[0] - 0x61 + 0x0a;
    } else if((ucBuffer[0] >= 'A') && (ucBuffer[0] <= 'Z')) {
        Temp[0] = ucBuffer[0] - 0x41 + 0x0a;
    }

    if((ucBuffer[1] >= '0') && (ucBuffer[1] <= '9')) {
        Temp[1] = ucBuffer[1] - 0x30;
    } else if((ucBuffer[1] >= 'a') && (ucBuffer[1] <= 'z')) {
        Temp[1] = ucBuffer[1] - 0x61 + 0x0a;
    } else if((ucBuffer[1] >= 'A') && (ucBuffer[1] <= 'Z')) {
        Temp[1] = ucBuffer[1] - 0x41 + 0x0a;
    }

    ucTemp = (Temp[0] << 4) + Temp[1];
    return (ucTemp);
}

void SiAiAlgorithm::HexToAsciiString(unsigned char *ucData, unsigned long ulLength, unsigned char *pucDataOut)
{
    unsigned long i;

    i=0;
    while(i<ulLength) {
        HexToAscii(*(ucData+i), (pucDataOut+(i*2)));
        i = i + 1;
    }
    return;
}

void SiAiAlgorithm::AsciiToHexString(unsigned char *ucBuffer, unsigned long ulLength, unsigned char *pucDataOut)
{
    unsigned long i;
	unsigned long length;

	length = (ulLength / 2);
	length = length * 2;
    i=0;
    while(i<length) {
        *(pucDataOut+(i/2)) = AsciiToHex((ucBuffer+i));
        i = i + 2;
    }
    return;
}

CString SiAiAlgorithm::HexToCString(unsigned char *ucBuffer, unsigned long ulLength)
{
    unsigned long i;
    CString tempString;

    tempString = _T("");
    i=0;
    while(i<ulLength) {
		CString oneCharString;
        oneCharString.Format(_T("%02x"), ucBuffer[i]);
        tempString = tempString + oneCharString;
        i = i + 1;
    }
    return tempString;
}

CString SiAiAlgorithm::AsciiToCString(unsigned char *ucBuffer, unsigned long ulLength)
{
    unsigned long i;
    CString tempString;
    CString oneCharString;

    tempString = _T("");
    i=0;
    while(i<ulLength) {
        oneCharString.Format(_T("%c"), ucBuffer[i]);
        tempString = tempString + oneCharString;
        i = i + 1;
    }
    return tempString;
}

void SiAiAlgorithm::PackUint16Data(VOID_T *address, CString value) 
{
	unsigned char *buffer;
	int value_t;

	buffer = (unsigned char *)(*address);
	if (value.Compare(_T("")) == 0) {
		value_t = -1;
	} else {
		value_t = atoi(value);
	}
	buffer[0] = (UINT8_T)(((value_t)		) & 0x00FF);
	buffer[1] = (UINT8_T)(((value_t)  >> 8 ) & 0x00FF);
	*address = *address + 2;
	return;
}

unsigned long long SiAiAlgorithm::StrToUnsignedLongLong(CString string, int base) 
{
	unsigned long long value = 0;
	int index;
	unsigned char data;
	unsigned char data_index;

	for (index = 0; index < string.GetLength(); index++) {
		data_index = string.GetAt(index);
		if((data_index >= '0') && (data_index <= '9')) {
				data = data_index - 0x30;
		} else if((data_index >= 'a') && (data_index <= 'z')) {
				data = data_index - 0x61 + 0x0a;
		} else if((data_index >= 'A') && (data_index <= 'Z')) {
				data = data_index - 0x41 + 0x0a;
		} else {
				data = 0;
		}
		value = (value * base) + data;
	}

	return value;
}

long long SiAiAlgorithm::StrToSignedLongLong(CString string, int base) 
{
	long long value = 0;
	long long nagative = 1;
	int index;
	unsigned char data;
	unsigned char data_index;


	for (index = 0; index < string.GetLength(); index++) {
		data_index = string.GetAt(index);
		if (index == 0) {
			if (data_index == '-') {
				nagative = -1;
			}
		}
		if((data_index >= '0') && (data_index <= '9')) {
				data = data_index - 0x30;
		} else if((data_index >= 'a') && (data_index <= 'z')) {
				data = data_index - 0x61 + 0x0a;
		} else if((data_index >= 'A') && (data_index <= 'Z')) {
				data = data_index - 0x41 + 0x0a;
		} else {
				data = 0;
		}
		value = (value * base) + data;
	}

	return (value*nagative);
}

CString SiAiAlgorithm::UnsignedLongLongToStr(unsigned long long value)
{
	char buffer[64];
	CString temp;

	_ui64toa(value,buffer,10);
	temp = buffer;

	return temp;
}

CString SiAiAlgorithm::SignedLongLongToStr(long long value)
{
	char buffer[64];
	CString temp;

	_i64toa(value,buffer,10);
	temp = buffer;

	return temp;
}

void SiAiAlgorithm::CalculateCaliSlopeIntercept(INT64_T yData1, INT64_T xData1, INT64_T yData2, INT64_T xData2, INT32_T *slope, INT32_T *intercept)
{
	INT32_T slope_temp = 0;
	INT32_T intercept_temp = 0;
	INT64_T temp;
	INT64_T temp1;

	if (xData1 == xData2)
		xData2 = xData1 - 2;
	if (((xData1 - xData2) / 2) == 0) 
		xData2 = xData1 - 3;

	temp = (yData1 - yData2) * 100000;
	temp = (temp / (xData1 - xData2)) + ((temp % (xData1 - xData2)) / (((xData1 - xData2) /2)));
	*slope = (INT32_T)temp;

	temp1 = (100000 * yData2) - (temp * xData2);
	temp1 = (temp1 / 10000) + ((temp1 % 10000) / 5000);
	*intercept = (INT32_T)temp1;

	return;
}


ERROR_T AiServerSleep(UINT32_T timeout)
{
	Sleep(timeout);

	return AI_ERROR_SUCCESS;
}

UINT64_T AiServerTickGet(void) {
	return GetTickCount();
}