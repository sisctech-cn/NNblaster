#pragma once

enum AiServerLogLevel
{
	AI_SERVER_LOG_OFF	= 0,// 关闭日志
	AI_SERVER_LOG_ERROR	= 1,// 错误
	AI_SERVER_LOG_INFO	= 2,// 消息
	AI_SERVER_LOG_DEBUG	= 3 // Debug
};


class SiAiLogFile
{
public:
	SiAiLogFile();   
	virtual ~SiAiLogFile();

public:
	AiServerLogLevel m_logLevel;
	CString m_strLogFolder;
	CString m_strLogFolderPacket;
	CString m_strLogFilePacket;
	CRITICAL_SECTION m_criticalSection;
	CMutex m_hMutex;
	FILE* m_pFile;
	CString m_strLogBuffer;

public:
	void SetLogLevel(AiServerLogLevel logLevel);
	CString GetModulePath(void);
	CString GetCurrentDateTime(void);
	CString GetDateTimeString(void);
	CString GetDate(void);
	CString GetCurrentDay(void);
	ERROR_T WriteLogFile(CString temp);
	ERROR_T PacketDataGet(UINT8_T *packet_buffer, UINT32_T packet_length, UINT32_T start, UINT32_T length, UINT64_T *value);
	ERROR_T LogSdkIndexDataInfo(UINT16_T index);
	ERROR_T LogSdkModbusIndexDataInfo(UINT16_T index);
    ERROR_T LogPacket(UINT8_T direction, UINT8_T *packet_buffer, UINT32_T packet_length);
	ERROR_T LogFp16(UINT8_T *str);
	ERROR_T LogFp32(UINT8_T *str);
	ERROR_T LogFp64(UINT8_T *str);
	ERROR_T LogLnetPacket(UINT8_T direction, UINT8_T *packet_buffer, UINT32_T packet_length);
	ERROR_T LogLnetModbus(UINT8_T direction, UINT8_T *packet_buffer, UINT32_T packet_length);
	ERROR_T LogRegister(AI_DAT_PTR ai_data, UINT32_T number);
};

#ifdef __cplusplus
extern "C" {
#endif
ERROR_T AiServerLogPacket(UINT8_T direction, UINT8_T *packet_buffer, UINT32_T packet_length);
ERROR_T AiServerLogFp16(UINT8_T *str);
ERROR_T AiServerLogFp32(UINT8_T *str);
ERROR_T AiServerLogFp64(UINT8_T *str);
#ifdef __cplusplus
}
#endif


