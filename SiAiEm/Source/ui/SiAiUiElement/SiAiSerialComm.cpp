#include "stdafx.h"
#include "SiAiGlobal.h"

#include <assert.h>

SiAiSerialComm::SiAiSerialComm()
{
    hFile = INVALID_HANDLE_VALUE;
#ifdef AI_SERVER_SERIAL_COM_LOCK
    hSemaphore = CreateSemaphore(NULL, 1, 1, "SiAiSerialCommSem");
#endif
}

//
// Delete dynamic memory
//
SiAiSerialComm::~SiAiSerialComm()
{
}

HANDLE SiAiSerialComm::GetHandle(void)
{
    return hFile;
}

void SiAiSerialComm::SetHandle(HANDLE handleValue)
{
    hFile = handleValue;
    return;
}

BOOL_T SiAiSerialComm::SendChar(unsigned char data)
{
    return TRUE;
}

BOOL_T SiAiSerialComm::SendCharCompleted(unsigned char dummy)
{
    return TRUE;
}

BOOL_T SiAiSerialComm::OpenSerialPort(LPCTSTR comPort,DWORD baud,BYTE databit,BYTE parity,BYTE stopbit)
{
    CString MsgStr;
    DCB			dcb;
    BOOL_T		fRetVal ;
    BYTE		i;
    DWORD		dwError;
    COMMTIMEOUTS  to;

    CloseCommPort();

#ifdef AI_SERVER_SERIAL_COM_LOCK
    WaitForSingleObject(hSemaphore, INFINITE);
#endif

    if ((hFile=CreateFile(comPort,
                          GENERIC_READ | GENERIC_WRITE,
                          0,                  // exclusive access
                          NULL,               // no security attributes
                          OPEN_EXISTING,
                          /* FILE_ATTRIBUTE_NORMAL | */ FILE_FLAG_OVERLAPPED,
                          NULL )) == INVALID_HANDLE_VALUE) {
#ifdef AI_SERVER_SERIAL_COM_LOCK
        ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
        return FALSE;
    }

    for(i=0; i<50; i++) {
        fRetVal = SetupComm(hFile, 8192, 8192);
        if (fRetVal)		break;
        Sleep(1);
    }

    // purge any information in the buffer
    PurgeComm(hFile, PURGE_TXABORT | PURGE_RXABORT |
              PURGE_TXCLEAR | PURGE_RXCLEAR );

    // set up for overlapped I/O
    GetCommTimeouts(hFile, &to);
    to.ReadIntervalTimeout = 0;
    to.ReadTotalTimeoutMultiplier = 0;
    to.ReadTotalTimeoutConstant = 2500;
    to.WriteTotalTimeoutMultiplier = 0;
    to.WriteTotalTimeoutConstant = 2500;
    SetCommTimeouts(hFile, &to) ;

    // set up DCB
    dcb.DCBlength = sizeof(DCB) ;

    for(i=0; i<50; i++) {
        fRetVal =GetCommState(hFile, &dcb);
        if (fRetVal)		break;
        Sleep(1);
    }
    if (fRetVal == 0) {
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
#ifdef AI_SERVER_SERIAL_COM_LOCK
        ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
        return FALSE;
    }

    dcb.BaudRate=baud;
    dcb.ByteSize=databit;
    dcb.Parity = parity;
    dcb.StopBits =stopbit;

    // because we have not hardware control and we send and receive binary data, so can not use XONXOFF, so must no flow control
#ifdef AI_SERVER_HARDWARE_FLOW_CONTROL
    BYTE bSet = TRUE;
    dcb.fOutxDsrFlow = bSet ;
    if (bSet)
        dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
    else
        dcb.fDtrControl = DTR_CONTROL_ENABLE;
#else
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
#endif

#ifdef AI_SERVER_HARDWARE_FLOW_CONTROL
    bSet = TRUE;
    dcb.fOutxCtsFlow = bSet;
    if (bSet)
        dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
    else
        dcb.fRtsControl = RTS_CONTROL_ENABLE;
    dcb.fDsrSensitivity = TRUE;
    dcb.fTXContinueOnXoff = FALSE;
#else
    dcb.fOutxCtsFlow = FALSE;
    dcb.fRtsControl = RTS_CONTROL_DISABLE;
    dcb.fDsrSensitivity = FALSE;
    dcb.fTXContinueOnXoff = FALSE;
#endif

#ifdef AI_SERVER_SOFTWARE_FLOW_CONTROL
    dcb.fInX = dcb.fOutX = TRUE ;
    dcb.XonLim = 100 ;
    dcb.XoffLim = 100 ;
    dcb.XonChar = XON;
    dcb.XoffChar = XOFF;
#else
    dcb.fInX = FALSE;
    dcb.fOutX = FALSE;
    dcb.XonLim = 0;
    dcb.XoffLim = 0;
    dcb.XonChar = XON;
    dcb.XoffChar = XOFF;
#endif

    // other various settings
    dcb.fBinary = TRUE;

    if (dcb.Parity == 0)
        dcb.fParity = FALSE;
    else
        dcb.fParity = TRUE;

    // set up DCB
    dcb.DCBlength = sizeof( DCB );

    for(i=0; i<50; i++) {
        fRetVal = SetCommState(hFile, &dcb);
        if (fRetVal)		break;
        Sleep(1);
    }
    if (fRetVal == 0) {
        dwError = GetLastError();
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
#ifdef AI_SERVER_SERIAL_COM_LOCK
        ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
        return FALSE;
    }

    GetCommState(hFile, &dcb ) ;

    memset(&m_OverlappedRead, 0, sizeof(OVERLAPPED));
    m_OverlappedRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    memset(&m_OverlappedWrite, 0, sizeof(OVERLAPPED));
    m_OverlappedWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    PurgeComm(hFile , PURGE_TXABORT | PURGE_RXABORT |
              PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
#ifdef AI_SERVER_SERIAL_COM_LOCK
    ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
    return TRUE;
}



BOOL_T SiAiSerialComm::WriteSerialPort(BYTE *lpByte, DWORD dwBytesToWrite, DWORD timeout, DWORD *dwBytesFinish)
{
    CString		MsgStr;
    BOOL_T        fWriteStat, fRetVal ;
    DWORD       dwBytesWritten ;
    DWORD       dwErrorFlags;
    DWORD   	dwError;
    DWORD       dwBytesSent=0;
    COMSTAT		ComStat;
    DWORD		dwBytesWrittenOneTime;
    DWORD       OldCount,Count;

#if (AI_SERVER_TICKET_TEST == 1)
	AiBaseTickTestAdd(_T("WriteSerialPort"));
#endif
#ifdef AI_SERVER_SERIAL_COM_LOCK
    WaitForSingleObject(hSemaphore, INFINITE);
#endif
    if (hFile == INVALID_HANDLE_VALUE) {
        *dwBytesFinish = 0;
#ifdef AI_SERVER_SERIAL_COM_LOCK
        ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
        return FALSE;
    }

    if(dwBytesToWrite==0) {
        *dwBytesFinish = 0;
#ifdef AI_SERVER_SERIAL_COM_LOCK
        ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
        return TRUE;
    }

    if (timeout < 1000) {
        timeout = 1000;
    }

    dwBytesWritten = 0;
    dwBytesWrittenOneTime = 0;
    OldCount=GetTickCount();
    while(dwBytesWritten != dwBytesToWrite) {
        fWriteStat = WriteFile(hFile, lpByte+dwBytesWritten, dwBytesToWrite-dwBytesWritten,
                               &dwBytesWrittenOneTime, &m_OverlappedWrite);
        if (!fWriteStat)  {
            dwError = GetLastError();
            while ((dwError == ERROR_IO_PENDING) || (dwError == ERROR_IO_INCOMPLETE)) {
//				WaitForSingleObject(m_OverlappedWrite.hEvent, timeout);
                GetOverlappedResult(hFile, &m_OverlappedWrite, &dwBytesWrittenOneTime, TRUE);
                dwBytesWritten = dwBytesWritten + dwBytesWrittenOneTime;
                dwError = GetLastError();
                if (dwBytesWritten != dwBytesToWrite) {
                    Count=GetTickCount();
                    if(Count-OldCount>=timeout) {
                        *dwBytesFinish = dwBytesWritten;
#ifdef AI_SERVER_SERIAL_COM_LOCK
                        ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
                        return FALSE;
                    }
                } else {
                    break;
                }
            }
            if ((dwError != ERROR_IO_PENDING) && (dwError != ERROR_IO_INCOMPLETE)) {
                // some other error occurred
                fRetVal = ClearCommError(hFile, &dwErrorFlags, &ComStat );
                *dwBytesFinish = dwBytesWritten;
#ifdef AI_SERVER_SERIAL_COM_LOCK
                ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
                return FALSE;
            }
            if (dwBytesWritten != dwBytesToWrite) {
                Count=GetTickCount();
                if(Count-OldCount>=timeout) {
                    *dwBytesFinish = dwBytesWritten;
#ifdef AI_SERVER_SERIAL_COM_LOCK
                    ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
                    return FALSE;
                }
            }
        }
    }

    *dwBytesFinish = dwBytesWritten;
#ifdef AI_SERVER_SERIAL_COM_LOCK
    ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
    return TRUE;
}


DWORD SiAiSerialComm::ReadSerialPort(BYTE * lpByte, DWORD dwLength, DWORD timeout)
{
    CString		MsgStr;
    BOOL_T        fReadStat;
    DWORD       OldCount;
    DWORD		Count;
    DWORD		dwReaded, dwError;
    DWORD       dwErrorFlags;
    COMSTAT		ComStat;
    DWORD		dwReadBytes=0;
    DWORD		dwResult;
	DWORD		dwWantReadNumber;

#ifdef AI_SERVER_SERIAL_COM_LOCK
    WaitForSingleObject(hSemaphore, INFINITE);
#endif
    if (hFile == INVALID_HANDLE_VALUE) {
#ifdef AI_SERVER_SERIAL_COM_LOCK
        ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
        return 0;
    }

    if(dwLength==0) {
#ifdef AI_SERVER_SERIAL_COM_LOCK
        ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
        return 0;
    }

    if (timeout < 1000) {
        timeout = 1000;
    }

    OldCount=GetTickCount();
    while (1) {
		// some other error occurred
        ClearCommError(hFile, &dwErrorFlags, &ComStat);
		if(dwErrorFlags>0)
		{
			PurgeComm(hFile,PURGE_RXABORT|PURGE_RXCLEAR);
#ifdef AI_SERVER_SERIAL_COM_LOCK
			ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
			return dwReadBytes;
		}
		dwWantReadNumber = ((DWORD)(((dwLength-dwReadBytes) < (ComStat.cbInQue))?(dwLength-dwReadBytes):ComStat.cbInQue));
		if (dwWantReadNumber == 0) {
#ifdef AI_SERVER_SERIAL_COM_LOCK
			ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
			return dwReadBytes;
		}
        fReadStat = ReadFile(hFile, lpByte+dwReadBytes, dwWantReadNumber,
                             &dwReaded, &m_OverlappedRead);

        if (!fReadStat) {
            dwError = GetLastError();
            if ((dwError == ERROR_IO_PENDING) || (dwError == ERROR_IO_INCOMPLETE)) {
                dwResult = WaitForSingleObject(m_OverlappedRead.hEvent, timeout);
                GetOverlappedResult(hFile, &m_OverlappedRead, &dwReaded, FALSE);
                dwReadBytes += dwReaded;
#ifdef AI_SERVER_SERIAL_COM_LOCK
                ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
                return dwReadBytes;
            } else {
                // some other error occurred
                ClearCommError(hFile, &dwErrorFlags, &ComStat);
                Sleep(1);
#ifdef AI_SERVER_SERIAL_COM_LOCK
                ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
                return dwReadBytes;
            }
        }

        dwReadBytes += dwReaded;
        if (dwReadBytes >= dwLength) {
#ifdef AI_SERVER_SERIAL_COM_LOCK
            ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
            return dwReadBytes;
        }

        Count=GetTickCount();
        if(Count-OldCount>=timeout) {
#ifdef AI_SERVER_SERIAL_COM_LOCK
            ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
            return dwReadBytes;
        }
        Count=GetTickCount();
    } //while
#ifdef AI_SERVER_SERIAL_COM_LOCK
    ReleaseSemaphore(hSemaphore, 1, NULL);
#endif
    return dwReadBytes;
}

void SiAiSerialComm::PurgeCommBuf(DWORD mode)
{
    PurgeComm(hFile, mode);
}

void SiAiSerialComm::CloseCommPort()
{
    int i, nRet;

#ifdef AI_SERVER_SERIAL_COM_LOCK
    WaitForSingleObject(hSemaphore, INFINITE);
#endif
    if (hFile !=INVALID_HANDLE_VALUE) {
        SetCommMask(hFile,NULL);
        SetEvent(m_OverlappedRead.hEvent);
        SetEvent(m_OverlappedWrite.hEvent);
        if( m_OverlappedRead.hEvent != NULL )
            CloseHandle( m_OverlappedRead.hEvent );
        if( m_OverlappedWrite.hEvent != NULL )
            CloseHandle( m_OverlappedWrite.hEvent );
        SetupComm(hFile, 1024, 1024);
        PurgeComm(hFile , PURGE_TXABORT | PURGE_RXABORT |
                  PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
        for(i=0; i<3; i++) {
            nRet =CloseHandle(hFile);
            if (nRet != FALSE)
                break;
        }
        hFile =INVALID_HANDLE_VALUE;
    }
#ifdef AI_SERVER_SERIAL_COM_LOCK
    ReleaseSemaphore(hSemaphore, 1, NULL);
#endif

    return;
}

BOOL_T SiAiSerialComm::SetupCommPara(LPCTSTR comPort,DWORD baud,BYTE databit,BYTE parity,BYTE stopbit)
{
    BOOL_T fRetVal;
    CString m_MsgStr;

    fRetVal =OpenSerialPort(comPort,baud,databit,parity,stopbit);
    return fRetVal;
}

BOOL_T  SiAiSerialComm::WriteCommBlock(BYTE * lpByte, DWORD dwBytesToWrite, DWORD timeout, DWORD *dwBytesFinish)
{
    BOOL_T fRetVal;

    fRetVal =WriteSerialPort(lpByte, dwBytesToWrite, timeout, dwBytesFinish);

    return fRetVal;
}

DWORD SiAiSerialComm::Read_Wait(BYTE *lpByte, DWORD dwLength, DWORD timeout)
{
    DWORD dwRet;

    dwRet =ReadSerialPort(lpByte, dwLength, timeout);

    return dwRet;
}



