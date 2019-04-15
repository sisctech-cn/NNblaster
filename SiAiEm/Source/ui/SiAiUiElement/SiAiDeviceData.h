#pragma once

class SiAiDeviceData
{
public:
	SiAiDeviceData();   // 标准构造函数
	virtual ~SiAiDeviceData();

public:	
	void DeviceDataStartLink();
	void DeviceDataStart();
	void DeviceDataStop();

public:
	CWinThread *m_CommandThread;
	int m_CommandThreadFlag;
	unsigned long m_SendState;
	HANDLE m_ThreadFlagSemaphore;

public:
	int m_CloseComFlag;

public:
	DWORD m_recv_timeout;
	int m_retry_times;
	int m_retry_times_max;
	int m_retry_times_total;

public:
	UINT8_T m_command;
	UINT8_T m_source_addr;
	UINT8_T m_dest_addr;
    UINT32_T m_field_num;
    AI_BASE_LNET_PACKET_FIELD_READ m_field_read[AI_BASE_LNET_PACKET_FIELD_MAX];
	AI_BASE_LNET_PACKET_FIELD_WRITE m_field_write[AI_BASE_LNET_PACKET_FIELD_MAX];
	AI_BASE_LNET_PACKET_FIELD_READ m_field_link_read[AI_BASE_LNET_PACKET_FIELD_MAX];
	AI_BASE_LNET_PACKET_FIELD_WRITE m_field_link_write[AI_BASE_LNET_PACKET_FIELD_MAX];

public:
	static int DeviceDataToDeviceStart(SiAiDeviceData *deviceDataDlg);
	static int DeviceDataToDeviceComOpen(SiAiDeviceData *deviceDataDlg);
	static int DeviceDataToDeviceSend(SiAiDeviceData *deviceDataDlg);
	static int DeviceDataToDeviceRecv(SiAiDeviceData *deviceDataDlg);
	static int DeviceDataToDeviceComClose(SiAiDeviceData *deviceDataDlg);
	static int DeviceDataToDevice(SiAiDeviceData *deviceDataDlg);

public:
	static UINT CommandService(LPVOID pParam);

};
