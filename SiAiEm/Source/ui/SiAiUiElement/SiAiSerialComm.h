#pragma once

#define XON							0x11
#define XOFF						0x13
#define AI_SERVER_MAX_SERIAL_PORT		(100)

#define updater_hal_tick_get()			GetTickCount()
#define updater_device_address_get()	(0)

/* Function Definition-----------------------------------------------------------*/
// comm function
class SiAiSerialComm
{
public:
    // contruction and destruction
    SiAiSerialComm();
    virtual		~SiAiSerialComm();

    HANDLE GetHandle(void);
    void SetHandle(HANDLE handleValue);
    static BOOL_T SendChar(unsigned char data);
    static BOOL_T SendCharCompleted(unsigned char dummy);
    BOOL_T OpenSerialPort(LPCTSTR comPort,DWORD baud=9600, BYTE databit=8, BYTE parity=NOPARITY, BYTE stopbit=ONESTOPBIT);
    BOOL_T WriteSerialPort(BYTE * lpByte, DWORD dwBytesToWrite, DWORD timeout, DWORD *dwBytesFinish);
    DWORD ReadSerialPort(BYTE * lpByte, DWORD dwLength, DWORD timeout);
    void PurgeCommBuf(DWORD mode);
    void CloseCommPort();
    BOOL_T SetupCommPara(LPCTSTR comPort,DWORD baud,BYTE databit,BYTE parity,BYTE stopbit);
    BOOL_T  WriteCommBlock(BYTE * lpByte, DWORD dwBytesToWrite, DWORD timeout, DWORD *dwBytesFinish);
    DWORD Read_Wait(BYTE *lpByte, DWORD dwLength, DWORD timeout);

private:
    HANDLE hFile;
#ifdef AI_SERVER_SERIAL_COM_LOCK
    HANDLE hSemaphore;
#endif
    OVERLAPPED m_OverlappedRead;
    OVERLAPPED m_OverlappedWrite;

};

