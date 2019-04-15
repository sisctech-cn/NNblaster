// DeviceDataImage.cpp : 实现文件
//

#include "stdafx.h"
#include "SiAiGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// Global Variable Declaration
SiAiDeviceData *g_SiAiDeviceData = NULL;

////////////////////////////////////////////////////////////////////////////
// Extern Variable Declaration
extern SiAiDlg *g_SiAiDlg;


SiAiDeviceData::SiAiDeviceData()
{
	g_SiAiDeviceData = this;
	 
	m_CloseComFlag = 0;
	m_recv_timeout = AI_SERVER_DATA_RECEIVE_TIMEOUT;
	m_retry_times = 0;
	m_retry_times_max = 0;
	m_retry_times_total = 0;
	m_ThreadFlagSemaphore = CreateSemaphore(NULL, 1, 1, "SiAiDeviceDataThreadFlag");
	m_SendState = DEVICE_INFO_SEND_STATE_END;
	WaitForSingleObject(m_ThreadFlagSemaphore, INFINITE);
    m_CommandThreadFlag = COMMAND_THREAD_STOP;
	ReleaseSemaphore(m_ThreadFlagSemaphore, 1, NULL);
    m_CommandThread = AfxBeginThread(CommandService, this);
}

SiAiDeviceData::~SiAiDeviceData()
{
}

int SiAiDeviceData::DeviceDataToDeviceStart(SiAiDeviceData *deviceData)
{
    return 0;
}

int SiAiDeviceData::DeviceDataToDeviceComOpen(SiAiDeviceData *deviceData)
{
	g_SiAiDlg->m_strSoftStatus.Format(_T(""));

    if (g_SiAiDlg->m_SiAiManageCom.phy.GetHandle() != INVALID_HANDLE_VALUE) {
		{
			CString	temp;
			temp.Format(_T("%s "), g_SiAiDlg->m_strSoftStatus);
			temp.AppendFormat(_T("RM=%d RT=%d %d:%s "), deviceData->m_retry_times_max, deviceData->m_retry_times_total, GetTickCount(), _T("通信口打开...OK."));
			g_SiAiDlg->m_strSoftStatus = temp.Right(AI_SERVER_SOFT_STATUS_STRING_LENGTH);
			g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(g_SiAiDlg->m_strSoftStatus);
		}
		return 0;
    }
    g_SiAiDlg->OpenCom();
    if (g_SiAiDlg->m_SiAiManageCom.phy.GetHandle() == INVALID_HANDLE_VALUE) {
		{
			CString	temp;
			temp.Format(_T("%s "), g_SiAiDlg->m_strSoftStatus);
			temp.AppendFormat(_T("%d:%s"), GetTickCount(), _T(AI_SERVER_UI_INFO_COM_OPER_ERROR));
			g_SiAiDlg->m_strSoftStatus = temp.Right(AI_SERVER_SOFT_STATUS_STRING_LENGTH);
			g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(g_SiAiDlg->m_strSoftStatus);
		}
        return -1;
    }

	{
		CString	temp;
		temp.Format(_T("%s "), g_SiAiDlg->m_strSoftStatus);
		temp.AppendFormat(_T("RM=%d RT=%d %d:%s "), deviceData->m_retry_times_max, deviceData->m_retry_times_total, GetTickCount(), _T("通信口打开...OK."));
		g_SiAiDlg->m_strSoftStatus = temp.Right(AI_SERVER_SOFT_STATUS_STRING_LENGTH);
        g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(g_SiAiDlg->m_strSoftStatus);
	}
    return 0;
}

int SiAiDeviceData::DeviceDataToDeviceComClose(SiAiDeviceData *deviceData)
{
    if (g_SiAiDlg->m_SiAiManageCom.phy.GetHandle() == INVALID_HANDLE_VALUE) {
		{
			CString	temp;
			temp.Format(_T("%s "), g_SiAiDlg->m_strSoftStatus);
			temp.AppendFormat(_T("%d:%s"), GetTickCount(), _T(AI_SERVER_UI_INFO_COM_CLOSE_OK));
			g_SiAiDlg->m_strSoftStatus = temp.Right(AI_SERVER_SOFT_STATUS_STRING_LENGTH);
			g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(g_SiAiDlg->m_strSoftStatus);
		}
        return 0;
    }
	if (deviceData->m_CloseComFlag == 1) {
		g_SiAiDlg->CloseCom();
		if (g_SiAiDlg->m_SiAiManageCom.phy.GetHandle() != INVALID_HANDLE_VALUE) {
			{
				CString	temp;
				temp.Format(_T("%s "), g_SiAiDlg->m_strSoftStatus);
				temp.AppendFormat(_T("%d:%s"), GetTickCount(), _T(AI_SERVER_UI_INFO_COM_OPER_ERROR));
				g_SiAiDlg->m_strSoftStatus = temp.Right(AI_SERVER_SOFT_STATUS_STRING_LENGTH);
				g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(g_SiAiDlg->m_strSoftStatus);
			}
			return -1;
		}
		{
			CString	temp;
			temp.Format(_T("%s "), g_SiAiDlg->m_strSoftStatus);
			temp.AppendFormat(_T("%d:%s"), GetTickCount(), _T(AI_SERVER_UI_INFO_COM_CLOSE_OK));
			g_SiAiDlg->m_strSoftStatus = temp.Right(AI_SERVER_SOFT_STATUS_STRING_LENGTH);
			g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(g_SiAiDlg->m_strSoftStatus);
		}
		deviceData->m_CloseComFlag = 0;
	}
	{
		CString	temp;
		temp.Format(_T("%s "), g_SiAiDlg->m_strSoftStatus);
		temp.AppendFormat(_T("%d:%s"), GetTickCount(), _T(AI_SERVER_UI_INFO_COM_CLOSE_OK));
		g_SiAiDlg->m_strSoftStatus = temp.Right(AI_SERVER_SOFT_STATUS_STRING_LENGTH);
		g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(g_SiAiDlg->m_strSoftStatus);
	}
	return 0;
}

int SiAiDeviceData::DeviceDataToDeviceSend(SiAiDeviceData *deviceData)
{
    AI_BASE_LNET_INTERFACE_PTR inteface;
	UINT8_T command;
    UINT8_T source_addr;
	UINT8_T dest_addr;
    UINT32_T field_num;
    AI_BASE_LNET_PACKET_FIELD_READ_PTR field_read;
	AI_BASE_LNET_PACKET_FIELD_WRITE_PTR field_write;
#if (AI_PRODUCT_OPTION_LOG_COMM == 1)
	DWORD oldCount, newCount;
#endif
	CString temp;

#if (AI_PRODUCT_OPTION_LOG_COMM == 1)
	oldCount=GetTickCount();
#endif
	command = deviceData->m_command;
	if (command == AI_BASE_LNET_PACKET_COMMAND_READ_REQUEST) {
		// prepare data
		inteface = &(g_SiAiDlg->m_SiAiManageCom.inteface);
		source_addr = deviceData->m_source_addr;
		dest_addr = deviceData->m_dest_addr;
		field_num = deviceData->m_field_num;
		field_read = deviceData->m_field_link_read;

		// send data
		if (AiBaseLnetInterfaceRequestRead(inteface, source_addr, dest_addr, field_num, field_read) != AI_ERROR_SUCCESS)  {
			{
				CString	temp;
				temp.Format(_T("%s "), g_SiAiDlg->m_strSoftStatus);
				temp.AppendFormat(_T("%d:%s%d"), GetTickCount(), _T(AI_SERVER_UI_INFO_COM_SEND_DATA), deviceData->m_field_num);
				g_SiAiDlg->m_strSoftStatus = temp.Right(AI_SERVER_SOFT_STATUS_STRING_LENGTH);
				g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(g_SiAiDlg->m_strSoftStatus);
			}
			return -1;
		}
	}
	if (command == AI_BASE_LNET_PACKET_COMMAND_WRITE_REQUEST) {
		// prepare data
		inteface = &(g_SiAiDlg->m_SiAiManageCom.inteface);
		source_addr = deviceData->m_source_addr;
		dest_addr = deviceData->m_dest_addr;
		field_num = deviceData->m_field_num;
		field_write = deviceData->m_field_link_write;

		// send data
		if (AiBaseLnetInterfaceRequestWrite(inteface, source_addr, dest_addr, field_num, field_write) != AI_ERROR_SUCCESS)  {
			{
				CString	temp;
				temp.Format(_T("%s "), g_SiAiDlg->m_strSoftStatus);
				temp.AppendFormat(_T("%d:%s%d"), GetTickCount(), _T(AI_SERVER_UI_INFO_COM_SEND_SATA_ERROR), deviceData->m_field_num);
				g_SiAiDlg->m_strSoftStatus = temp.Right(AI_SERVER_SOFT_STATUS_STRING_LENGTH);
				g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(g_SiAiDlg->m_strSoftStatus);
			}
			return -1;
		}
	}

#if (AI_PRODUCT_OPTION_LOG_COMM == 1)
	newCount=GetTickCount();
	temp.Format(_T("AiBaseLnetInterfaceRequestRead dateTime=[%s] "), g_SiAiDlg->m_SiAiLogFile.GetCurrentDateTime());
	temp.AppendFormat(_T("start=%u "), oldCount);
	temp.AppendFormat(_T("end=%u "), newCount);
	temp.AppendFormat(_T("period=%u\n"), (newCount-oldCount));
	g_SiAiDlg->m_SiAiLogFile.WriteLogFile(temp);
#endif  
	{
		CString	temp;
		temp.Format(_T("%s "), g_SiAiDlg->m_strSoftStatus);
		temp.AppendFormat(_T("%d:%s%d"), GetTickCount(), _T(AI_SERVER_UI_INFO_COM_SEND_DATA), deviceData->m_field_num);
		g_SiAiDlg->m_strSoftStatus = temp.Right(AI_SERVER_SOFT_STATUS_STRING_LENGTH);
		g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(g_SiAiDlg->m_strSoftStatus);
	}

    return 0;
}


int SiAiDeviceData::DeviceDataToDeviceRecv(SiAiDeviceData *deviceData)
{
    AI_BASE_LNET_INTERFACE_PTR inteface;
    DWORD timeout = deviceData->m_recv_timeout;
    DWORD       OldCount,Count;
	
    // prepare data
    inteface = &(g_SiAiDlg->m_SiAiManageCom.inteface);
    OldCount=GetTickCount();
    while(inteface->sequence_send_last != inteface->sequence_recv_last) {
        Count=GetTickCount();
        if(Count-OldCount>=timeout) {
			CString	temp;
			temp.Format(_T("%s "), g_SiAiDlg->m_strSoftStatus);
			temp.AppendFormat(_T("%d:%s"), GetTickCount(), _T(AI_SERVER_UI_INFO_COM_RECV_DATA_TIMEOUT));
			g_SiAiDlg->m_strSoftStatus = temp.Right(AI_SERVER_SOFT_STATUS_STRING_LENGTH);
			g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(g_SiAiDlg->m_strSoftStatus);
            return -2; // timeout
        } else {
            Sleep(1);
        }
    }
    if (inteface->retcode_recv_last != AI_ERROR_SUCCESS) {
		{
			CString	temp;
			temp.Format(_T("%s "), g_SiAiDlg->m_strSoftStatus);
			temp.AppendFormat(_T("%d:%s "), GetTickCount(), _T(AI_SERVER_UI_INFO_COM_RECV_DATA_ERROR));
			g_SiAiDlg->m_strSoftStatus = temp.Right(AI_SERVER_SOFT_STATUS_STRING_LENGTH);
			g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(g_SiAiDlg->m_strSoftStatus);
		}
        return -1; // error
    }

	{
		CString	temp;
		temp.Format(_T("%s "), g_SiAiDlg->m_strSoftStatus);
		temp.AppendFormat(_T("%d:%s "), GetTickCount(), _T(AI_SERVER_UI_INFO_COM_RECV_DATA_OK));
		g_SiAiDlg->m_strSoftStatus = temp.Right(AI_SERVER_SOFT_STATUS_STRING_LENGTH);
		g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(g_SiAiDlg->m_strSoftStatus);
	}
    return 0;	// finish;
}

int SiAiDeviceData::DeviceDataToDevice(SiAiDeviceData *deviceData)
{
    int retcode;

    if (deviceData->m_CommandThreadFlag == COMMAND_THREAD_STOP_USER) {
        switch(deviceData->m_SendState) {
        case DEVICE_DATA_ERROR:
            deviceData->m_SendState = DEVICE_DATA_COM_CLOSE;
            return -1;
        case DEVICE_DATA_START:
        case DEVICE_DATA_COM_OPEN:
        case DEVICE_DATA_SEND:
        case DEVICE_DATA_RECV:
            deviceData->m_SendState = DEVICE_DATA_COM_CLOSE;
            return 0;
        case DEVICE_DATA_COM_CLOSE:
            if (DeviceDataToDeviceComClose(deviceData) != 0) {
                deviceData->m_SendState = DEVICE_DATA_ERROR;
                return -1;
            }
            deviceData->m_SendState = DEVICE_DATA_END;
            return 0;
        case DEVICE_DATA_END:
            return 0;
        default:
            deviceData->m_SendState = DEVICE_DATA_ERROR;
            return -1;
        }
    }


    switch(deviceData->m_SendState) {
    case DEVICE_DATA_ERROR:
        deviceData->m_SendState = DEVICE_DATA_COM_CLOSE;
        return -1;
    case DEVICE_DATA_START:
        retcode = DeviceDataToDeviceStart(deviceData);
        if (retcode != 0) {
            deviceData->m_SendState = DEVICE_DATA_ERROR;
            return -1;
        }
        deviceData->m_SendState = DEVICE_DATA_COM_OPEN;
        return 0;
    case DEVICE_DATA_COM_OPEN:
        retcode = DeviceDataToDeviceComOpen(deviceData);
        if (retcode != 0) {
            deviceData->m_SendState = DEVICE_DATA_ERROR;
            return -1;
        }
        deviceData->m_SendState = DEVICE_DATA_SEND;
        return 0;
    case DEVICE_DATA_SEND:
        retcode = DeviceDataToDeviceSend(deviceData);
        if (retcode != 0) {
            deviceData->m_SendState = DEVICE_DATA_ERROR;
            return -1;
        }
        deviceData->m_SendState = DEVICE_INFO_SEND_STATE_RECV_PN;
        return 0;
    case DEVICE_DATA_RECV:
        retcode = DeviceDataToDeviceRecv(deviceData);
        if (retcode == -1) { // error
			deviceData->m_retry_times = 0;
            deviceData->m_SendState = DEVICE_DATA_ERROR;
            return -1;
        }
        if (retcode == -2) { // timeout, retry
			if (deviceData->m_retry_times < AI_SERVER_DATA_RETRY_TIMES) {
				deviceData->m_retry_times++;
				deviceData->m_retry_times_total++;
				if (deviceData->m_retry_times_total > 10000) {
					deviceData->m_retry_times_total = 0;
				}
				if (deviceData->m_retry_times > deviceData->m_retry_times_max) {
					deviceData->m_retry_times_max = deviceData->m_retry_times; 
				}
				deviceData->m_SendState = DEVICE_DATA_START;
			} else {
				deviceData->m_retry_times = 0;
				deviceData->m_SendState = DEVICE_DATA_ERROR;
			}
            return 0;
        }
        if (retcode == 0) { // finish
			deviceData->m_retry_times = 0;
            deviceData->m_SendState = DEVICE_DATA_COM_CLOSE;
            return 0;
        }
		deviceData->m_retry_times = 0;
		deviceData->m_SendState = DEVICE_DATA_COM_CLOSE;
        return 0;
    case DEVICE_DATA_COM_CLOSE:
        if (DeviceDataToDeviceComClose(deviceData) != 0) {
            deviceData->m_SendState = DEVICE_DATA_ERROR;
            return -1;
        }
        deviceData->m_SendState = DEVICE_DATA_END;
        return 0;
    case DEVICE_DATA_END:
        return 0;
    default:
        deviceData->m_SendState = DEVICE_DATA_ERROR;
        return -1;
    }
    return -1;
}

UINT SiAiDeviceData::CommandService(LPVOID pParam)
{
    SiAiDeviceData *deviceData = (SiAiDeviceData *)pParam;
    int retcode;

    while(deviceData->m_CommandThreadFlag != COMMAND_THREAD_EXIT_START) {
        if (deviceData->m_CommandThreadFlag == COMMAND_THREAD_STOP) {
            Sleep(1);
            continue;
        }
		if (deviceData->m_SendState == DEVICE_DATA_END) {
			Sleep(1);
			continue;
		}
        retcode = DeviceDataToDevice(deviceData);
    }

	WaitForSingleObject(deviceData->m_ThreadFlagSemaphore, INFINITE);
    deviceData->m_CommandThreadFlag = COMMAND_THREAD_EXIT_FINISH;
	ReleaseSemaphore(deviceData->m_ThreadFlagSemaphore, 1, NULL);
    return 0;
}

void SiAiDeviceData::DeviceDataStartLink() 
{
    // TODO: 在此添加控件通知处理程序代码
	WaitForSingleObject(m_ThreadFlagSemaphore, INFINITE);
	if ((m_CommandThreadFlag == COMMAND_THREAD_EXIT_START) || (m_CommandThreadFlag == COMMAND_THREAD_EXIT_FINISH)) {
		ReleaseSemaphore(m_ThreadFlagSemaphore, 1, NULL);
		return;
	} else {
		if (m_field_num > AI_BASE_LNET_PACKET_FIELD_MAX) {
			ReleaseSemaphore(m_ThreadFlagSemaphore, 1, NULL);
			return;
		}
	    m_SendState = DEVICE_DATA_START;
		m_CommandThreadFlag = COMMAND_THREAD_START;
	}
	ReleaseSemaphore(m_ThreadFlagSemaphore, 1, NULL);

	while (m_SendState != DEVICE_DATA_END) {
		Sleep(1);
	}
	return;
}

#if (AI_PRODUCT_OPTION_MODBUS == 0)
void SiAiDeviceData::DeviceDataStart()
{
	UINT32_T field_num;

	field_num = m_field_num;
	if (field_num == 0)
		return;
	if (field_num > AI_BASE_LNET_PACKET_FIELD_MAX)
		return;

	memcpy((char *)m_field_link_read, (char *)m_field_read, (sizeof(AI_BASE_LNET_PACKET_FIELD_READ)*AI_BASE_LNET_PACKET_FIELD_MAX));
	memcpy((char *)m_field_link_write, (char *)m_field_write, (sizeof(AI_BASE_LNET_PACKET_FIELD_WRITE)*AI_BASE_LNET_PACKET_FIELD_MAX));	
	DeviceDataStartLink();
	return;
}
#endif
#if (AI_PRODUCT_OPTION_MODBUS == 1)
void SiAiDeviceData::DeviceDataStart()
{
	UINT8_T command;
	UINT32_T field_num;
	UINT32_T index;

	command = m_command;
	field_num = m_field_num;
	if (field_num == 0)
		return;
	if (field_num > AI_BASE_LNET_PACKET_FIELD_MAX)
		return;

	if (command == AI_BASE_LNET_PACKET_COMMAND_READ_REQUEST) {
		for(index = 0; index < field_num; index++) {
			m_field_num = 1;
			memcpy((char *)&(m_field_link_read[0]), (char *)&(m_field_read[index]), (sizeof(AI_BASE_LNET_PACKET_FIELD_READ)*1));
			DeviceDataStartLink();
		}
	}
	if (command == AI_BASE_LNET_PACKET_COMMAND_WRITE_REQUEST) {
		for(index = 0; index < field_num; index++) {
			m_field_num = 1;
			memcpy((char *)&(m_field_link_write[0]), (char *)&(m_field_write[index]), (sizeof(AI_BASE_LNET_PACKET_FIELD_WRITE)*1));
			DeviceDataStartLink();
		}
	}

    return;
}
#endif


void SiAiDeviceData::DeviceDataStop()
{
    // TODO: 在此添加控件通知处理程序代码
	WaitForSingleObject(m_ThreadFlagSemaphore, INFINITE);
    m_CommandThreadFlag = COMMAND_THREAD_STOP_USER;
	ReleaseSemaphore(m_ThreadFlagSemaphore, 1, NULL);
}



