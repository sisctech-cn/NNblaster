#pragma once

class SiAiDeviceSelectService
{
public:
	SiAiDeviceSelectService();   
	virtual ~SiAiDeviceSelectService();

public:
	CWinThread *m_Thread;
	int m_ThreadFlag;

public:
	static ERROR_T AiServerDataUnitNumberGet(void);
	static ERROR_T AiServerDataUnitInfoGet(void);
	static ERROR_T AiServerDataUiUpdate(void);

public:
	static UINT ServiceProcess(LPVOID pParam);
	void ServiceStart(void);
	int ServiceCheck(int pre_service);
	int BaseAddrGet(UINT8_T *addr, UINT32_T *index);
	int NetworkAddrGet(UINT8_T *addr, UINT32_T *index);
	int ControlAddrGet(UINT8_T *addr, UINT32_T *index);
	int CenterAddrGet(UINT8_T *addr, UINT32_T *index);
	int ManageAddrGet(UINT8_T *addr, UINT32_T *index);
	int ServerAddrGet(UINT8_T *addr, UINT32_T *index);
	int BaseAddrGetByComboIndex(INT32_T combo_index, UINT8_T *addr, UINT32_T *index);
	int NetworkAddrGetByComboIndex(INT32_T combo_index, UINT8_T *addr, UINT32_T *index);
	int ControlAddrGetByComboIndex(INT32_T combo_index, UINT8_T *addr, UINT32_T *index);
	int CenterAddrGetByComboIndex(INT32_T combo_index, UINT8_T *addr, UINT32_T *index);
	int ManageAddrGetByComboIndex(INT32_T combo_index, UINT8_T *addr, UINT32_T *index);
	int ServerAddrGetByComboIndex(INT32_T combo_index, UINT8_T *addr, UINT32_T *index);

};


