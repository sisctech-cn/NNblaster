// SiAiLogFile.cpp : 实现文件
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
SiAiLogFile *g_SiAiLogFile;

////////////////////////////////////////////////////////////////////////////
// Extern Variable Declaration

////////////////////////////////////////////////////////////////////////////
// Function Declaration
SiAiLogFile::SiAiLogFile()
{
	m_logLevel = AI_SERVER_LOG_OFF;
	m_strLogFolder.Format("%sLog\\", GetModulePath());
	CreateDirectory(m_strLogFolder, NULL);
	m_strLogFolderPacket.Format("%sLog\\Packet\\", GetModulePath());
	CreateDirectory(m_strLogFolderPacket, NULL);
	m_strLogFilePacket.Format("%s%s%s.%s", m_strLogFolderPacket, _T("Packet"), GetDateTimeString(), _T("log"));
	m_strLogBuffer.Format(_T(""));
	::InitializeCriticalSection(&m_criticalSection);

	g_SiAiLogFile = this;
}

SiAiLogFile::~SiAiLogFile()
{
	::DeleteCriticalSection(&m_criticalSection);
}

void SiAiLogFile::SetLogLevel(AiServerLogLevel logLevel)
{
	m_logLevel = logLevel;
	return;
}

CString SiAiLogFile::GetModulePath(void) 
{
	CString strPath;

	char cFile[2048];
	GetModuleFileName(NULL, cFile, sizeof(cFile)); 

	CString strTemp = cFile;
	int nPos = strTemp.ReverseFind('\\');
	strPath = strTemp.Left(nPos+1);

	return strPath;
}

CString SiAiLogFile::GetCurrentDateTime(void)
{
	CString strTime;
	SYSTEMTIME CurTime;

	GetLocalTime(&CurTime);
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", CurTime.wYear, CurTime.wMonth, CurTime.wDay, CurTime.wHour, CurTime.wMinute, CurTime.wSecond, CurTime.wMilliseconds);

	return strTime;
}

CString SiAiLogFile::GetDate(void)
{
	CString strDate;

	COleDateTime currentTime = COleDateTime::GetCurrentTime();
	strDate = currentTime.Format("%Y%m%d");

	return strDate;
}

CString SiAiLogFile::GetDateTimeString(void)
{
	CString strDate;

	COleDateTime currentTime = COleDateTime::GetCurrentTime();
	strDate = currentTime.Format("%Y%m%d%H%M%S");

	return strDate;
}

CString SiAiLogFile::GetCurrentDay(void)
{
	CString strDay;

	COleDateTime currentTime = COleDateTime::GetCurrentTime();
	strDay = currentTime.Format("%Y-%m-%d");

	return strDay;
}

ERROR_T SiAiLogFile::WriteLogFile(CString temp) 
{
	m_strLogBuffer.AppendFormat(_T("%s"),temp);
	if (m_strLogBuffer.GetLength() < 1/*(1024*16)*/) {
		return AI_ERROR_SUCCESS;
	}

	m_hMutex.Lock();
	::EnterCriticalSection(&m_criticalSection);
	m_pFile = fopen(m_strLogFilePacket,"a+");
	if(m_pFile)
	{
		fputs(m_strLogBuffer.GetBuffer(), m_pFile);
		m_strLogBuffer.ReleaseBuffer();
		fflush(m_pFile);
		fclose(m_pFile);
	}
	m_strLogBuffer.Format(_T(""));
	::LeaveCriticalSection(&m_criticalSection);
	m_hMutex.Unlock();


	return AI_ERROR_SUCCESS;
}

ERROR_T SiAiLogFile::PacketDataGet(UINT8_T *packet_buffer, UINT32_T packet_length, UINT32_T start, UINT32_T length, UINT64_T *value)
{
    UINT32_T index;
    UINT32_T buffer_index;
    UINT64_T temp = 0;

    buffer_index = start;
    for (index = 0; index < length; index++) {
        if (buffer_index >= (packet_length))
            return AI_ERROR_GENERAL;
        temp = (temp << 8) + packet_buffer[buffer_index];
        buffer_index = (buffer_index + 1);
    }

    *value = temp;
    return AI_ERROR_SUCCESS;
}

ERROR_T SiAiLogFile::LogSdkIndexDataInfo(UINT16_T index) {
    UINT8_T *name;
    UINT8_T *description;
    UINT8_T *unit;
    UINT8_T *comment;
    UINT64_T sub_index;
    UINT8_T sub_data_type;
    UINT64_T value_min;
    UINT64_T value_max;
    UINT8_T catagory;
    UINT8_T operation;
	CString strTemp;

    if (AiSdkIndexDataTypeGet(index, &sub_index, &sub_data_type, &value_min, &value_max, &catagory, &operation) != AI_ERROR_SUCCESS)
    	return AI_ERROR_GENERAL;
    if (AiSdkIndexDataTypeMenuGet(index, &name, &description, &unit, &comment) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;
	strTemp.Format(_T("..............................................................\n"));
	WriteLogFile(strTemp);
    strTemp.Format(_T("index: %d\n"), (index));
	WriteLogFile(strTemp);
    strTemp.Format(_T("sub max: %d\n"), sub_index);
	WriteLogFile(strTemp);
    strTemp.Format(_T("data type: %d\n"), sub_data_type);
	WriteLogFile(strTemp);
    strTemp.Format(_T("value min: %d\n"), value_min);
	WriteLogFile(strTemp);
    strTemp.Format(_T("value max: %d\n"), value_max);
	WriteLogFile(strTemp);
    strTemp.Format(_T("class: %d\n"), catagory);
	WriteLogFile(strTemp);
    strTemp.Format(_T("operation: %d\n"), operation);
	WriteLogFile(strTemp);
    strTemp.Format(_T("name: %s\n"), name);
	WriteLogFile(strTemp);
    if (description != NULL) {
    	strTemp.Format(_T("description: %s\n"), description);
		WriteLogFile(strTemp);
    }
    strTemp.Format(_T("unit: %s\n"), unit);
	WriteLogFile(strTemp);
#if 0
    if (comment != NULL) {
    	strTemp.Format(_T("comment: %s\n"), comment);
		WriteLogFile(strTemp);
    }
#endif
	strTemp.Format(_T("..............................................................\n"));
	WriteLogFile(strTemp);

	return AI_ERROR_SUCCESS;
}

ERROR_T SiAiLogFile::LogSdkModbusIndexDataInfo(UINT16_T index) {
	return AI_ERROR_SUCCESS;
}

ERROR_T SiAiLogFile::LogPacket(UINT8_T direction, UINT8_T *packet_buffer, UINT32_T packet_length)
{
	ERROR_T result = AI_ERROR_SUCCESS;

#if (AI_PRODUCT_OPTION_LNET_PACKET == 1)
	result = LogLnetPacket(direction, packet_buffer, packet_length);
#endif
#if (AI_PRODUCT_OPTION_LNET_MODBUS == 1)
	result = LogLnetModbus(direction, packet_buffer, packet_length);
#endif
	return result;
}

ERROR_T SiAiLogFile::LogFp16(UINT8_T *str)
{
	CString strTemp;
#if 0
	CString strDayTime;
	
	/* get date time */
	strDayTime = GetCurrentDateTime();
	strTemp.Format(_T("[%s]: [Level:DEBUG][Module:FP16]:%s"), strDayTime, str);
#endif
#if 1
	strTemp.Format(_T("%s"), str);
#endif
	WriteLogFile(strTemp);

	return AI_ERROR_SUCCESS;
}

ERROR_T SiAiLogFile::LogFp32(UINT8_T *str)
{
	CString strTemp;
#if 0
	CString strDayTime;
	
	/* get date time */
	strDayTime = GetCurrentDateTime();
	strTemp.Format(_T("[%s]: [Level:DEBUG][Module:FP16]:%s"), strDayTime, str);
#endif
#if 1
	strTemp.Format(_T("%s"), str);
#endif
	WriteLogFile(strTemp);

	return AI_ERROR_SUCCESS;
}

ERROR_T SiAiLogFile::LogFp64(UINT8_T *str)
{
	CString strTemp;
#if 0
	CString strDayTime;
	
	/* get date time */
	strDayTime = GetCurrentDateTime();
	strTemp.Format(_T("[%s]: [Level:DEBUG][Module:FP16]:%s"), strDayTime, str);
#endif
#if 1
	strTemp.Format(_T("%s"), str);
#endif
	WriteLogFile(strTemp);

	return AI_ERROR_SUCCESS;
}

ERROR_T SiAiLogFile::LogLnetPacket(UINT8_T direction, UINT8_T *packet_buffer, UINT32_T packet_length)
{
#if (AI_PRODUCT_OPTION_LNET_PACKET == 1)
	CString strTemp;
	CString strDayTime;
	UINT32_T index_packet;
	/* decode data */
	UINT32_T decode_start;
    UINT32_T decode_end;
    UINT64_T decode_data;
	UINT8_T decode_command;
    UINT32_T decode_field_index;
    UINT8_T decode_field_map;
    UINT8_T decode_index_length;
    UINT16_T decode_index;
    UINT8_T decode_sub_index_valid;
    UINT8_T decode_sub_index_length;
    UINT64_T decode_sub_index;
    UINT8_T decode_data_type_size_valid;
    UINT8_T decode_data_type_size;
    UINT8_T decode_data_number_valid;
    UINT8_T decode_data_number_length;
    UINT32_T decode_data_number;
    UINT8_T decode_data_value_valid;
    UINT32_T decode_data_value_length;
    UINT64_T decode_data_value;
	/* link data */
	UINT32_T decode_link_length;
	UINT64_T decode_packet_length;
	UINT64_T decode_packet_parity;
	UINT16_T decode_calculate_parity;
	
	strTemp.Format(_T("\n*****************************************************************\n"));
	WriteLogFile(strTemp);
	/* get date time */
	strDayTime = GetCurrentDateTime();
	/* description */
	if (direction == 0) {
		strTemp.Format(_T("[%s]: [Level:DEBUG][Module:PACKET][Driection:IN][Length:%d]\n"), strDayTime, packet_length);
	} else {
		strTemp.Format(_T("[%s]: [Level:DEBUG][Module:PACKET][Driection:OUT][Length:%d]\n"), strDayTime, packet_length);
	}
	WriteLogFile(strTemp);

	/* content */
	strTemp.Format(_T("[Content]:"));
	for(index_packet = 0; index_packet < packet_length; index_packet++) {
		strTemp.AppendFormat(_T("[%04d:0x%02X] "), index_packet, packet_buffer[index_packet]);
	}
	strTemp.AppendFormat(_T("\n"));
	WriteLogFile(strTemp);

	/* set packet */
	decode_start = 0;
	decode_end = decode_start + packet_length;

    /* check the stx of packet */
    while (1) {
        if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_STX_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
			strTemp.Format(_T("[RESULT]: Decode packet stx error!\n"));
			WriteLogFile(strTemp);
            return AI_ERROR_GENERAL;
        }
        if (decode_data == AI_BASE_LNET_PACKET_STX)
            break;
        decode_start = (decode_start + AI_BASE_LNET_PACKET_STX_LENGTH);
    }
	strTemp.Format(_T("[STX]:[%04d:0x%02X]\n"), decode_start, packet_buffer[decode_start]);
	WriteLogFile(strTemp);

    /* get and check the link length */
    decode_link_length = (decode_end - decode_start);
    if (decode_link_length < (AI_BASE_LNET_PACKET_LINK_LENGTH_MIN-1)) {
		strTemp.Format(_T("[RESULT]: Decode link length error[%d<%d]!\n"), decode_link_length, AI_BASE_LNET_PACKET_LINK_LENGTH_MIN);
		WriteLogFile(strTemp);
        return AI_ERROR_GENERAL;
	}

    /* get the packete length */
    if (PacketDataGet(packet_buffer, packet_length, (decode_start + AI_BASE_LNET_PACKET_STX_LENGTH), AI_BASE_LNET_PACKET_LENGTH_LENGTH, 
			&decode_packet_length) != AI_ERROR_SUCCESS) {
		strTemp.Format(_T("[RESULT]: Decode packet length error!\n"));
		WriteLogFile(strTemp);
        return AI_ERROR_GENERAL;
    }
	strTemp.Format(_T("[LENGTH]:[%04d:0x%X:"), (decode_start + AI_BASE_LNET_PACKET_STX_LENGTH), decode_packet_length);
	strTemp.AppendFormat(_T("%d]\n"), decode_packet_length);
	WriteLogFile(strTemp);

    /* check the packet length value */
    if ((decode_packet_length < AI_BASE_LNET_PACKET_LENGTH_MIN) || (decode_packet_length > AI_BASE_LNET_PACKET_LENGTH_MAX)) {
        strTemp.Format(_T("[RESULT]: Decode packet length error[%d<%d][%d>%d]!\n"), 
			decode_packet_length, AI_BASE_LNET_PACKET_LENGTH_MIN, decode_packet_length, AI_BASE_LNET_PACKET_LENGTH_MAX);
		WriteLogFile(strTemp);
        return AI_ERROR_GENERAL;
    }
    if (decode_link_length < (decode_packet_length + AI_BASE_LNET_PACKET_HEAD_TAIL_LENGTH - 1)) {
		strTemp.Format(_T("[RESULT]: Decode packet length error[%d<%d+"), 
				decode_link_length, decode_packet_length);
		strTemp.AppendFormat(_T("%d]!\n"), 
				AI_BASE_LNET_PACKET_HEAD_TAIL_LENGTH);
		WriteLogFile(strTemp);
        return AI_ERROR_GENERAL;
    }

    /* get the parity */
    if (PacketDataGet(packet_buffer, packet_length, (decode_start + AI_BASE_LNET_PACKET_HEAD_LENGTH + (UINT32_T)decode_packet_length), 
			AI_BASE_LNET_PACKET_CHECKSUM_LENGTH, &decode_packet_parity) != AI_ERROR_SUCCESS) {
		strTemp.Format(_T("[RESULT]: Decode packet parity error!\n"));
		WriteLogFile(strTemp);
        return AI_ERROR_GENERAL;
	}
	strTemp.Format(_T("[CHECKSUM]:[%04d:0x%X:"), (decode_start + AI_BASE_LNET_PACKET_HEAD_LENGTH + (UINT32_T)decode_packet_length), decode_packet_parity);
	strTemp.AppendFormat(_T("%d]\n"), decode_packet_parity);
	WriteLogFile(strTemp);

    /* calculate the parity */
    decode_calculate_parity = 0;
    for (index_packet = 0; index_packet < decode_packet_length; index_packet++) {
        if (PacketDataGet(packet_buffer, packet_length, (decode_start + AI_BASE_LNET_PACKET_HEAD_LENGTH + index_packet), 1, &decode_data) != AI_ERROR_SUCCESS) {
			strTemp.Format(_T("[RESULT]: Decode packet parity error!\n"));
			WriteLogFile(strTemp);
            return AI_ERROR_GENERAL;
		}
        decode_calculate_parity = decode_calculate_parity + (UINT16_T)decode_data;
    }
    /* check the parity */
    if (decode_calculate_parity != decode_packet_parity) {
		strTemp.Format(_T("[RESULT]: Decode packet parity error[%d != %d]!\n"), decode_calculate_parity, decode_packet_parity);
		WriteLogFile(strTemp);
        return AI_ERROR_GENERAL;
    }

	/* get sequence */
	decode_start = (decode_start + AI_BASE_LNET_PACKET_HEAD_LENGTH);
	decode_end = (decode_start + (UINT32_T)decode_packet_length);
	if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_SEQUENCE_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
		strTemp.Format(_T("[RESULT]: Decode packet sequence error!\n"));
		WriteLogFile(strTemp);
		return AI_ERROR_GENERAL;
	}
	strTemp.Format(_T("[SEQUENCE]:[%04d:0x%X:"), decode_start, decode_data);
	strTemp.AppendFormat(_T("%d]\n"), decode_data);
	WriteLogFile(strTemp);
    decode_start = (decode_start + AI_BASE_LNET_PACKET_SEQUENCE_LENGTH);

    /* get source address */
    if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_SOURCE_ADDR_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
		strTemp.Format(_T("[RESULT]: Decode packet source address error!\n"));
		WriteLogFile(strTemp);
        return AI_ERROR_GENERAL;
    }
	strTemp.Format(_T("[SOURCE ADDRESS]:[%04d:0x%X:"), decode_start, decode_data);
	strTemp.AppendFormat(_T("%d]\n"), decode_data);
	WriteLogFile(strTemp);
    decode_start = (decode_start + AI_BASE_LNET_PACKET_SOURCE_ADDR_LENGTH);

    /* get dest address */
    if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
		strTemp.Format(_T("[RESULT]: Decode packet destination address error!\n"));
		WriteLogFile(strTemp);
        return AI_ERROR_GENERAL;
    }
	strTemp.Format(_T("[DESTINATION ADDRESS]:[%04d:0x%X:"), decode_start, decode_data);
	strTemp.AppendFormat(_T("%d]\n"), decode_data);
	WriteLogFile(strTemp);
    decode_start = (decode_start + AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH);

    /* get command */
    if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_COMMAND_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
		strTemp.Format(_T("[RESULT]: Decode packet command error!\n"));
		WriteLogFile(strTemp);
        return AI_ERROR_GENERAL;
    }
	strTemp.Format(_T("[COMMAND]:[%04d:0x%X:"), decode_start, decode_data);
	strTemp.AppendFormat(_T("%d]\n"), decode_data);
	WriteLogFile(strTemp);
    decode_start = (decode_start + AI_BASE_LNET_PACKET_COMMAND_LENGTH);

	decode_command = (UINT8_T)decode_data;
	switch (decode_command & 0x0F) {
	case AI_BASE_LNET_PACKET_COMMAND_READ_REQUEST:
		strTemp.Format(_T("[COMMAND]:[COMMAND_READ_REQUEST]\n"));
		WriteLogFile(strTemp);
		break;
	case AI_BASE_LNET_PACKET_COMMAND_READ_RESPONSE:
		strTemp.Format(_T("[COMMAND]:[COMMAND_READ_RESPONSE]\n"));
		WriteLogFile(strTemp);
		break;
	case AI_BASE_LNET_PACKET_COMMAND_WRITE_REQUEST:
		strTemp.Format(_T("[COMMAND]:[COMMAND_WRITE_REQUEST]\n"));
		WriteLogFile(strTemp);
		break;
	case AI_BASE_LNET_PACKET_COMMAND_WRITE_RESPONSE:
		strTemp.Format(_T("[COMMAND]:[COMMAND_WRITE_RESPONSE]\n"));
		WriteLogFile(strTemp);
		break;
	default:
		strTemp.Format(_T("[COMMAND]:[COMMAND_UNKNOWN]\n"));
		WriteLogFile(strTemp);
		break;
	}
	strTemp.Format(_T("[COMMAND]:[RETCODE=%d]\n"), ((decode_command & AI_BASE_LNET_PACKET_COMMAND_RETCODE_MASK) >> AI_BASE_LNET_PACKET_COMMAND_RETCODE_SHIFT));
	WriteLogFile(strTemp);


    for (decode_field_index = 0; decode_field_index < AI_BASE_LNET_PACKET_FIELD_MAX; decode_field_index++) {
        if (decode_start >= decode_end)
            break;

		strTemp.Format(_T("-------------------------------------------------------------\n"));
		WriteLogFile(strTemp);

        /* get field map */
        if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_FIELD_MAP_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
			strTemp.Format(_T("[RESULT]: Decode packet field map error!\n"));
			WriteLogFile(strTemp);
            return AI_ERROR_PACKET;
        }
		strTemp.Format(_T("[FIELD MAP]:[%04d:0x%X:"), decode_start, decode_data);
		strTemp.AppendFormat(_T("%d]\n"), decode_data);
		WriteLogFile(strTemp);
        decode_field_map = (UINT8_T)decode_data;
        decode_start = (decode_start + AI_BASE_LNET_PACKET_FIELD_MAP_LENGTH);

        /* get index_length */
#if (AI_BASE_LNET_PACKET_INDEX_LENGTH_MAP == 1)
        decode_index_length = (((decode_field_map & AI_BASE_LNET_PACKET_INDEX_LENGTH_MASK) >> AI_BASE_LNET_PACKET_INDEX_LENGTH_SHIFT) + AI_BASE_LNET_PACKET_INDEX_LENGTH_ADD);
#else
        decode_index_length = AI_BASE_LNET_PACKET_INDEX_LENGTH_BYTE;
#endif
		strTemp.Format(_T("[FIELD MAP:INDEX LENGTH]:[0x%X:%d]\n"), decode_index_length, decode_index_length);
		WriteLogFile(strTemp);

        /* get index */
        if (PacketDataGet(packet_buffer, packet_length, decode_start, decode_index_length, &decode_data) != AI_ERROR_SUCCESS) {
			strTemp.Format(_T("[RESULT]: Decode packet field index error!\n"));
			WriteLogFile(strTemp);
            return AI_ERROR_PACKET;
        }
		strTemp.Format(_T("[FIELD INDEX]:[%04d:0x%X:"), decode_start, decode_data);
		strTemp.AppendFormat(_T("%d]\n"), decode_data);
		WriteLogFile(strTemp);
        decode_index = (UINT16_T)decode_data;
        decode_start = (decode_start + decode_index_length);
		LogSdkIndexDataInfo(decode_index);

        /* get sub_index_valid */
        decode_sub_index_valid = (((decode_field_map & AI_BASE_LNET_PACKET_SUB_INDEX_VALID_MASK) >> AI_BASE_LNET_PACKET_SUB_INDEX_VALID_SHIFT) + AI_BASE_LNET_PACKET_SUB_INDEX_VALID_ADD);
		strTemp.Format(_T("[FIELD MAP:SUB INDEX VALID]:[0x%X:%d]\n"), decode_sub_index_valid, decode_sub_index_valid);
		WriteLogFile(strTemp);

        /* get sub_index_length */
        if (decode_sub_index_valid == AI_BASE_LNET_PACKET_SUB_INDEX_VALID) {
            decode_sub_index_length = (((decode_field_map & AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_MASK) >> AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_SHIFT));
            decode_sub_index_length = ((decode_sub_index_length == AI_BASE_LNET_PACKET_SUB_INDEX_MAP_BYTE)?AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_BYTE:
                                ((decode_sub_index_length == AI_BASE_LNET_PACKET_SUB_INDEX_MAP_SHORT)?AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_SHORT:
                                 ((decode_sub_index_length == AI_BASE_LNET_PACKET_SUB_INDEX_MAP_THREE)?AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_WORD:
                                  ((decode_sub_index_length <= AI_BASE_LNET_PACKET_SUB_INDEX_MAP_WORD)?AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_WORD:
                                   AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_DWORD))));
			strTemp.Format(_T("[FIELD MAP:SUB INDEX LENGTH]:[0x%X:%d]\n"), decode_sub_index_length, decode_sub_index_length);
			WriteLogFile(strTemp);
        }
        /* get sub_index */
        if (decode_sub_index_valid == AI_BASE_LNET_PACKET_SUB_INDEX_INVALID) {
            decode_sub_index = AI_BASE_LNET_PACKET_SUB_INDEX_DEFAULT;
        } else {
            /* get sub_index */
            if (PacketDataGet(packet_buffer, packet_length, decode_start, decode_sub_index_length, &decode_data) != AI_ERROR_SUCCESS) {
				strTemp.Format(_T("[RESULT]: Decode packet field sub index error!\n"));
				WriteLogFile(strTemp);
                return AI_ERROR_PACKET;
            }
			strTemp.Format(_T("[SUB INDEX]:[%04d:0x%X:"), decode_start, decode_data);
			strTemp.AppendFormat(_T("%d]\n"), decode_data);
			WriteLogFile(strTemp);
            decode_sub_index = decode_data;
            decode_start = (decode_start + decode_sub_index_length);
        }
		strTemp.Format(_T("[SUB INDEX]:[0x%X:"), decode_sub_index);
		strTemp.AppendFormat(_T("%d]\n"), decode_sub_index);
		WriteLogFile(strTemp);

        /* get data_type_size_valid */
        decode_data_type_size_valid = (((decode_field_map & AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_VALID_MASK) >> AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_VALID_SHIFT) + AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_VALID_ADD);
		strTemp.Format(_T("[FIELD MAP:DATA TYPE SIZE VALID]:[0x%X:%d]\n"), decode_data_type_size_valid, decode_data_type_size_valid);
		WriteLogFile(strTemp);
		/* get dat_type_size */
        if (decode_data_type_size_valid == AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_INVALID) {
            decode_data_type_size = AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_DEFAULT;
        } else {
            /* get data_type_size */
            if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
				strTemp.Format(_T("[RESULT]: Decode packet field data type size error!\n"));
				WriteLogFile(strTemp);
                return AI_ERROR_PACKET;
            }
			strTemp.Format(_T("[DATA TYPE SIZE]:[%04d:0x%X:"), decode_start, decode_data, decode_data);
			strTemp.AppendFormat(_T("%d]\n"), decode_sub_index);
			WriteLogFile(strTemp);
            decode_data_type_size = (UINT8_T)decode_data;
            decode_start = (decode_start + AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_LENGTH);
        }
		strTemp.Format(_T("[DATA TYPE SIZE]:[0x%X:%d]\n"), decode_data_type_size, decode_data_type_size);
		WriteLogFile(strTemp);

        /* get data_number_valid */
        decode_data_number_valid = (((decode_field_map & AI_BASE_LNET_PACKET_DATA_NUMBER_VALID_MASK) >> AI_BASE_LNET_PACKET_DATA_NUMBER_VALID_SHIFT) + AI_BASE_LNET_PACKET_DATA_NUMBER_VALID_ADD);
		strTemp.Format(_T("[FIELD MAP:DATA NUMBER VALID]:[0x%X:%d]\n"), decode_data_number_valid, decode_data_number_valid);
		WriteLogFile(strTemp);
        /* get data_number_length */
        decode_data_number_length = (((decode_field_map & AI_BASE_LNET_PACKET_DATA_NUMBER_LENGTH_MASK) >> AI_BASE_LNET_PACKET_DATA_NUMBER_LENGTH_SHIFT) + AI_BASE_LNET_PACKET_DATA_NUMBER_LENGTH_ADD);
		strTemp.Format(_T("[FIELD MAP:DATA NUMBER LENGTH]:[0x%X:%d]\n"), decode_data_number_length, decode_data_number_length);
		WriteLogFile(strTemp);
        /* get data_number */
        if (decode_data_number_valid == AI_BASE_LNET_PACKET_DATA_NUMBER_INVALID) {
            decode_data_number = AI_BASE_LNET_PACKET_DATA_NUMBER_DEFAULT;
        } else {
            /* get data_number */
            if (PacketDataGet(packet_buffer, packet_length, decode_start, decode_data_number_length, &decode_data) != AI_ERROR_SUCCESS) {
				strTemp.Format(_T("[RESULT]: Decode packet field data number error!\n"));
				WriteLogFile(strTemp);
                return AI_ERROR_PACKET;
            }
			strTemp.Format(_T("[DATA NUMBER]:[%04d:0x%X:"), decode_start, decode_data, decode_data);
			strTemp.AppendFormat(_T("%d]\n"), decode_data);
			WriteLogFile(strTemp);
            decode_data_number = (UINT32_T)decode_data;
            decode_start = (decode_start + decode_data_number_length);
        }
		strTemp.Format(_T("[DATA NUMBER]:[0x%X:%d]\n"), decode_data_number, decode_data_number);
		WriteLogFile(strTemp);

        /* get data_number_valid */
        decode_data_value_valid = (((decode_field_map & AI_BASE_LNET_PACKET_DATA_VALUE_VALID_MASK) >> AI_BASE_LNET_PACKET_DATA_VALUE_VALID_SHIFT) + AI_BASE_LNET_PACKET_DATA_VALUE_VALID_ADD);
		strTemp.Format(_T("[FIELD MAP:DATA VALUE VALID]:[0x%X:%d]\n"), decode_data_value_valid, decode_data_value_valid);
		WriteLogFile(strTemp);
        /* get value */
        if (decode_data_value_valid == AI_BASE_LNET_PACKET_DATA_VALUE_INVALID) {
            decode_data_value_length = (decode_data_type_size * decode_data_number);
            decode_data_value = 0;
			strTemp.Format(_T("[DATA VALUE LENGTH]:[0x%X:%d]\n"), decode_data_value_length, decode_data_value_length);
			WriteLogFile(strTemp);
			strTemp.Format(_T("[DATA VALUE]:[NULL]\n"));
			WriteLogFile(strTemp);
        } else {
            /* get value */
            decode_data_value_length = (decode_data_type_size * decode_data_number);
            strTemp.Format(_T("[DATA VALUE LENGTH]:[0x%X:%d]\n"), decode_data_value_length, decode_data_value_length);
			WriteLogFile(strTemp);
			if (decode_data_value_length > AI_BASE_LNET_PACKET_DATA_LENGTH_AS_ADDR) {
				strTemp.Format(_T("[DATA VALUE]:"));
				for(index_packet = 0; index_packet < decode_data_value_length; index_packet++) {
					strTemp.AppendFormat(_T("[%04d:%02X] "), decode_start+index_packet, packet_buffer[decode_start+index_packet]);
				}
				strTemp.AppendFormat(_T("\n"));
				WriteLogFile(strTemp);

				decode_data_value = (UINT64_T) (packet_buffer + decode_start);
                decode_start = (decode_start + decode_data_value_length);
            } else {
                /* get data_number */
                if (PacketDataGet(packet_buffer, packet_length, decode_start, decode_data_value_length, &decode_data) != AI_ERROR_SUCCESS) {
					strTemp.Format(_T("[RESULT]: Decode packet field data value error!\n"));
					WriteLogFile(strTemp);
                    return AI_ERROR_PACKET;
                }

				strTemp.Format(_T("[DATA VALUE]:"));
				for(index_packet = 0; index_packet < decode_data_value_length; index_packet++) {
					strTemp.AppendFormat(_T("[%04d:%02X] "), decode_start+index_packet, packet_buffer[decode_start+index_packet]);
				}
				strTemp.AppendFormat(_T("\n"));
				WriteLogFile(strTemp);

                decode_data_value = decode_data;
                decode_start = (decode_start + decode_data_value_length);
            }
        }
    }
#endif
	return AI_ERROR_SUCCESS;
}

ERROR_T SiAiLogFile::LogLnetModbus(UINT8_T direction, UINT8_T *packet_buffer, UINT32_T packet_length)
{
#if (AI_PRODUCT_OPTION_LNET_MODBUS == 1)
	CString strTemp;
	CString strDayTime;
	UINT32_T index_packet;
	/* decode data */
	UINT32_T decode_start;
    UINT32_T decode_end;
    UINT64_T decode_data;
	UINT8_T decode_device_address;
	UINT8_T decode_function_code;
	UINT8_T decode_function_code_real;
	UINT8_T decode_error_code;
	UINT16_T decode_reg_start_address;
	UINT16_T decode_reg_number;
	UINT8_T deocde_reg_value_length;
	UINT32_T field_index;
	UINT16_T decode_reg_value;

	strTemp.Format(_T("\n*****************************************************************\n"));
	WriteLogFile(strTemp);
	/* get date time */
	strDayTime = GetCurrentDateTime();
	/* description */
	if (direction == 0) {
		strTemp.Format(_T("[%s]: [Level:DEBUG][Module:PACKET][Driection:IN][Length:%d]\n"), strDayTime, packet_length);
	} else {
		strTemp.Format(_T("[%s]: [Level:DEBUG][Module:PACKET][Driection:OUT][Length:%d]\n"), strDayTime, packet_length);
	}
	WriteLogFile(strTemp);

	/* content */
	strTemp.Format(_T("[Content]:"));
	for(index_packet = 0; index_packet < packet_length; index_packet++) {
		strTemp.AppendFormat(_T("[%04d:0x%02X] "), index_packet, packet_buffer[index_packet]);
	}
	strTemp.AppendFormat(_T("\n"));
	WriteLogFile(strTemp);

	if (direction == 1) {
		/* set packet */
		decode_start = 0;
		decode_end = decode_start + packet_length;

		/* get dest address */
		if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
			strTemp.Format(_T("[RESULT]: Decode packet device address error!\n"));
			WriteLogFile(strTemp);
            return AI_ERROR_GENERAL;
        }
		decode_device_address = (UINT8_T)decode_data;
		strTemp.Format(_T("[ADDRESS]:[%04d:0x%02X]\n"), decode_start, decode_device_address);
		WriteLogFile(strTemp);
		decode_start = decode_start + AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH;

		/* get function code */
		if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_FCODE_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
			strTemp.Format(_T("[RESULT]: Decode packet function code error!\n"));
			WriteLogFile(strTemp);
			return AI_ERROR_GENERAL;
		}
		decode_function_code = (UINT8_T)(decode_data);
		strTemp.Format(_T("[FUNCTION CODE]:[%04d:0x%02X]\n"), decode_start, decode_function_code);
		WriteLogFile(strTemp);
		decode_start = decode_start + AI_BASE_LNET_PACKET_FCODE_LENGTH;

		/* check command */
		decode_function_code_real = ((decode_function_code & AI_BASE_LNET_PACKET_COMMAND_MASK) >> AI_BASE_LNET_PACKET_COMMAND_SHIFT);
		if ((decode_function_code_real != AI_BASE_LNET_PACKET_FCODE_READ_HOLD_REGS) && (decode_function_code_real != AI_BASE_LNET_PACKET_FCODE_READ_INPUT_REGS) &&
			(decode_function_code_real != AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REG) && (decode_function_code_real != AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REGS)) {
			strTemp.Format(_T("[RESULT]: Decode packet function code error!\n"));
			WriteLogFile(strTemp);
			return AI_ERROR_GENERAL;
		}
		/* error code */
		if (decode_function_code > AI_BASE_LNET_PACKET_FCODE_ERROR_MIN) {
			/* check the error code */
			if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_ECODE_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
				strTemp.Format(_T("[RESULT]: Decode packet function error code error!\n"));
				WriteLogFile(strTemp);
				return AI_ERROR_GENERAL;
			}
			decode_error_code = (UINT8_T)(decode_data);
			strTemp.Format(_T("[ERROR CODE]:[%04d:0x%02X]\n"), decode_start, decode_error_code);
			WriteLogFile(strTemp);
			return AI_ERROR_SUCCESS;
		}

		/* write hold regs */
		if (decode_function_code == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REGS) {
			/* get the reg_start_address */
			if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
				strTemp.Format(_T("[RESULT]: Decode packet register start address error!\n"));
				WriteLogFile(strTemp);
				return AI_ERROR_GENERAL;
			}
			decode_reg_start_address = (UINT16_T)decode_data;
			strTemp.Format(_T("[REGISTER START ADDRESS]:[%04d:0x%02X]\n"), decode_start, decode_reg_start_address);
			WriteLogFile(strTemp);
			decode_start = (decode_start + AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH);
			
			/* get the reg_number */
			if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_REG_NUMBER_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
				strTemp.Format(_T("[RESULT]: Decode packet register number error!\n"));
				WriteLogFile(strTemp);
				return AI_ERROR_GENERAL;
			}
			decode_reg_number = (UINT16_T)decode_data;
			strTemp.Format(_T("[REGISTER NUMBER]:[%04d:0x%02X]\n"), decode_start, decode_reg_number);
			WriteLogFile(strTemp);
			decode_start = (decode_start + AI_BASE_LNET_PACKET_REG_NUMBER_LENGTH);

			/* get the packete length */
			if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_LENGTH_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
				strTemp.Format(_T("[RESULT]: Decode packet packet length error!\n"));
				WriteLogFile(strTemp);
				return AI_ERROR_GENERAL;
			}
			deocde_reg_value_length = (UINT8_T)decode_data;
			strTemp.Format(_T("[REGISTER VALUE LENGTH]:[%04d:0x%02X]\n"), decode_start, deocde_reg_value_length);
			WriteLogFile(strTemp);
			if ((deocde_reg_value_length % 2) != 0) {
				strTemp.Format(_T("[RESULT]: Decode packet packet length error!\n"));
				WriteLogFile(strTemp);
				return AI_ERROR_GENERAL;
			}
			decode_reg_number = (deocde_reg_value_length / 2);
			decode_start = (decode_start + AI_BASE_LNET_PACKET_LENGTH_LENGTH);
			
		}

		/* write hold reg */
		if (decode_function_code == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REG) {
			/* get the reg_start_address */
			if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
				strTemp.Format(_T("[RESULT]: Decode packet register start address error!\n"));
				WriteLogFile(strTemp);
				return AI_ERROR_GENERAL;
			}
			decode_reg_start_address = (UINT16_T)decode_data;
			strTemp.Format(_T("[REGISTER START ADDRESS]:[%04d:0x%02X]\n"), decode_start, decode_reg_start_address);
			WriteLogFile(strTemp);
			decode_start = (decode_start + AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH);
			deocde_reg_value_length = 2;
			decode_reg_number = 1;
		}

		if ((decode_function_code == AI_BASE_LNET_PACKET_FCODE_READ_HOLD_REGS) ||
			(decode_function_code == AI_BASE_LNET_PACKET_FCODE_READ_INPUT_REGS)) {
			/* get the reg_start_address */
			if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
				strTemp.Format(_T("[RESULT]: Decode packet register start address error!\n"));
				WriteLogFile(strTemp);
				return AI_ERROR_GENERAL;
			}
			decode_reg_start_address = (UINT16_T)decode_data;
			strTemp.Format(_T("[REGISTER START ADDRESS]:[%04d:0x%02X]\n"), decode_start, decode_reg_start_address);
			WriteLogFile(strTemp);
			decode_start = (decode_start + AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH);

			/* get the reg_number */
			if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_REG_NUMBER_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
				strTemp.Format(_T("[RESULT]: Decode packet register number error!\n"));
				WriteLogFile(strTemp);
				return AI_ERROR_GENERAL;
			}
			decode_reg_number = (UINT16_T)decode_data;
			strTemp.Format(_T("[REGISTER NUMBER]:[%04d:0x%02X]\n"), decode_start, decode_reg_number);
			WriteLogFile(strTemp);
			decode_start = (decode_start + AI_BASE_LNET_PACKET_REG_NUMBER_LENGTH);
		}

		for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
			if (field_index >= decode_reg_number)
				break;

			/* get reg value */
			if ((decode_function_code == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REG) ||
				(decode_function_code == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REGS)) {
				if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_REG_VALUE_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
					strTemp.Format(_T("[RESULT]: Decode packet value error!\n"));
					WriteLogFile(strTemp);
					return AI_ERROR_PACKET;
				}
				decode_reg_value = (UINT16_T)decode_data;
				strTemp.Format(_T("[REGISTER ADDRESS]:[%04d:0x%02X]\n"), decode_start, (decode_reg_start_address + field_index));
				WriteLogFile(strTemp);
				strTemp.Format(_T("[REGISTER VALUE]:[%04d:0x%02X]\n"), decode_start, (decode_reg_value));
				WriteLogFile(strTemp);
				decode_start = (decode_start + AI_BASE_LNET_PACKET_REG_VALUE_LENGTH);
				LogSdkModbusIndexDataInfo(decode_reg_start_address + field_index);
			} else {
				LogSdkModbusIndexDataInfo(decode_reg_start_address + field_index);
			}
		}
	}
	if (direction == 0) {
		/* set packet */
		decode_start = 0;
		decode_end = decode_start + packet_length;

		/* get dest address */
		if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
			strTemp.Format(_T("[RESULT]: Decode packet device address error!\n"));
			WriteLogFile(strTemp);
            return AI_ERROR_GENERAL;
        }
		decode_device_address = (UINT8_T)decode_data;
		strTemp.Format(_T("[ADDRESS]:[%04d:0x%02X]\n"), decode_start, decode_device_address);
		WriteLogFile(strTemp);
		decode_start = decode_start + AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH;

		/* get function code */
		if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_FCODE_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
			strTemp.Format(_T("[RESULT]: Decode packet function code error!\n"));
			WriteLogFile(strTemp);
			return AI_ERROR_GENERAL;
		}
		decode_function_code = (UINT8_T)(decode_data);
		strTemp.Format(_T("[FUNCTION CODE]:[%04d:0x%02X]\n"), decode_start, decode_function_code);
		WriteLogFile(strTemp);
		decode_start = decode_start + AI_BASE_LNET_PACKET_FCODE_LENGTH;

		/* check command */
		decode_function_code_real = ((decode_function_code & AI_BASE_LNET_PACKET_COMMAND_MASK) >> AI_BASE_LNET_PACKET_COMMAND_SHIFT);
		if ((decode_function_code_real != AI_BASE_LNET_PACKET_FCODE_READ_HOLD_REGS) && (decode_function_code_real != AI_BASE_LNET_PACKET_FCODE_READ_INPUT_REGS) &&
			(decode_function_code_real != AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REG) && (decode_function_code_real != AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REGS)) {
			strTemp.Format(_T("[RESULT]: Decode packet function code error!\n"));
			WriteLogFile(strTemp);
			return AI_ERROR_GENERAL;
		}

		/* error code */
		if (decode_function_code > AI_BASE_LNET_PACKET_FCODE_ERROR_MIN) {
			/* check the error code */
			if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_ECODE_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
				strTemp.Format(_T("[RESULT]: Decode packet function error code error!\n"));
				WriteLogFile(strTemp);
				return AI_ERROR_GENERAL;
			}
			decode_error_code = (UINT8_T)(decode_data);
			strTemp.Format(_T("[ERROR CODE]:[%04d:0x%02X]\n"), decode_start, decode_error_code);
			WriteLogFile(strTemp);
			return AI_ERROR_SUCCESS;
		}

		/* write hold regs */
		if (decode_function_code == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REGS) {
			/* get the reg_start_address */
			if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
				strTemp.Format(_T("[RESULT]: Decode packet register start address error!\n"));
				WriteLogFile(strTemp);
				return AI_ERROR_GENERAL;
			}
			decode_reg_start_address = (UINT16_T)decode_data;
			strTemp.Format(_T("[REGISTER START ADDRESS]:[%04d:0x%02X]\n"), decode_start, decode_reg_start_address);
			WriteLogFile(strTemp);
			decode_start = (decode_start + AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH);

			/* get the reg_number */
			if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_REG_NUMBER_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
				strTemp.Format(_T("[RESULT]: Decode packet register number error!\n"));
				WriteLogFile(strTemp);
				return AI_ERROR_GENERAL;
			}
			decode_reg_number = (UINT16_T)decode_data;
			strTemp.Format(_T("[REGISTER NUMBER]:[%04d:0x%02X]\n"), decode_start, decode_reg_number);
			WriteLogFile(strTemp);
			decode_start = (decode_start + AI_BASE_LNET_PACKET_REG_NUMBER_LENGTH);
		}

		/* write hold reg */
		if (decode_function_code == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REG) {
			/* get the reg_start_address */
			if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
				strTemp.Format(_T("[RESULT]: Decode packet register start address error!\n"));
				WriteLogFile(strTemp);
				return AI_ERROR_GENERAL;
			}
			decode_reg_start_address = (UINT16_T)decode_data;
			strTemp.Format(_T("[REGISTER START ADDRESS]:[%04d:0x%02X]\n"), decode_start, decode_reg_start_address);
			WriteLogFile(strTemp);
			decode_start = (decode_start + AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH);
			deocde_reg_value_length = 2;
			decode_reg_number = 1;
		}

		if ((decode_function_code == AI_BASE_LNET_PACKET_FCODE_READ_HOLD_REGS) ||
			(decode_function_code == AI_BASE_LNET_PACKET_FCODE_READ_INPUT_REGS)) {
			/* get the packete length */
			if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_LENGTH_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
				strTemp.Format(_T("[RESULT]: Decode packet packet length error!\n"));
				WriteLogFile(strTemp);
				return AI_ERROR_GENERAL;
			}
			deocde_reg_value_length = (UINT8_T)decode_data;
			strTemp.Format(_T("[REGISTER VALUE LENGTH]:[%04d:0x%02X]\n"), decode_start, deocde_reg_value_length);
			WriteLogFile(strTemp);
			if ((deocde_reg_value_length % 2) != 0) {
				strTemp.Format(_T("[RESULT]: Decode packet packet length error!\n"));
				WriteLogFile(strTemp);
				return AI_ERROR_GENERAL;
			}
			decode_reg_number = (deocde_reg_value_length / 2);
			decode_start = (decode_start + AI_BASE_LNET_PACKET_LENGTH_LENGTH);
		}
		for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
			if (field_index >= decode_reg_number)
				break;

			/* get reg value */
			if ((decode_function_code == AI_BASE_LNET_PACKET_FCODE_READ_HOLD_REGS) ||
				(decode_function_code == AI_BASE_LNET_PACKET_FCODE_READ_INPUT_REGS)) {
				if (PacketDataGet(packet_buffer, packet_length, decode_start, AI_BASE_LNET_PACKET_REG_VALUE_LENGTH, &decode_data) != AI_ERROR_SUCCESS) {
					strTemp.Format(_T("[RESULT]: Decode packet value error!\n"));
					WriteLogFile(strTemp);
					return AI_ERROR_PACKET;
				}
				decode_reg_value = (UINT16_T)decode_data;
				strTemp.Format(_T("[REGISTER ADDRESS]:[%04d:0x%02X]\n"), decode_start, (field_index));
				WriteLogFile(strTemp);
				strTemp.Format(_T("[REGISTER VALUE]:[%04d:0x%02X]\n"), decode_start, (decode_reg_value));
				WriteLogFile(strTemp);
				decode_start = (decode_start + AI_BASE_LNET_PACKET_REG_VALUE_LENGTH);
			} 
		}
	}
#endif
	return AI_ERROR_SUCCESS;
}

ERROR_T SiAiLogFile::LogRegister(AI_DAT_PTR ai_data, UINT32_T number) 
{
	return AI_ERROR_SUCCESS;
}

ERROR_T AiServerLogPacket(UINT8_T direction, UINT8_T *packet_buffer, UINT32_T packet_length)
{
	g_SiAiLogFile->LogPacket(direction, packet_buffer, packet_length);

	return AI_ERROR_SUCCESS;
}

ERROR_T AiServerLogFp16(UINT8_T *str)
{
	g_SiAiLogFile->LogFp16(str);

	return AI_ERROR_SUCCESS;
}

ERROR_T AiServerLogFp32(UINT8_T *str)
{
	g_SiAiLogFile->LogFp32(str);

	return AI_ERROR_SUCCESS;
}

ERROR_T AiServerLogFp64(UINT8_T *str)
{
	g_SiAiLogFile->LogFp32(str);

	return AI_ERROR_SUCCESS;
}

