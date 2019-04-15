// SiAiAlgorithm.cpp : 实现文件
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
SiAiDeviceSelectService *g_SiAiDeviceSelectService = NULL;

////////////////////////////////////////////////////////////////////////////
// Extern Variable Declaration
extern SiAiDlg *g_SiAiDlg;

////////////////////////////////////////////////////////////////////////////
// Function Declaration
SiAiDeviceSelectService::SiAiDeviceSelectService()
{
	m_ThreadFlag = 0;
	g_SiAiDeviceSelectService = this;
}

SiAiDeviceSelectService::~SiAiDeviceSelectService()
{
}

ERROR_T SiAiDeviceSelectService::AiServerDataUnitNumberGet(void) 
{
	return AI_ERROR_SUCCESS;
}

ERROR_T SiAiDeviceSelectService::AiServerDataUnitInfoGet(void) 
{
	return AI_ERROR_SUCCESS;
}

ERROR_T SiAiDeviceSelectService::AiServerDataUiUpdate(void) 
{
	return AI_ERROR_SUCCESS;
}

UINT SiAiDeviceSelectService::ServiceProcess(LPVOID pParam)
{
	ERROR_T result;

	g_SiAiDlg->AiServerDataStateRun(0);
	g_SiAiDlg->AiServerDataConfigRun(1);
	/* get the unit number */
	result = AiServerDataUnitNumberGet();
	if (result != AI_ERROR_SUCCESS) {
		g_SiAiDeviceSelectService->m_ThreadFlag = 0;
		return 0;
	}
	result = AiServerDataUnitInfoGet();
	if (result != AI_ERROR_SUCCESS) {
		g_SiAiDeviceSelectService->m_ThreadFlag = 0;
		return 0;
	}
	result = AiServerDataUiUpdate();
	if (result != AI_ERROR_SUCCESS) {
		g_SiAiDeviceSelectService->m_ThreadFlag = 0;
		return 0;
	}
	
	g_SiAiDlg->AiServerDataStateRun(1);
	g_SiAiDlg->AiServerDataConfigRun(0);
	g_SiAiDeviceSelectService->m_ThreadFlag = 0;
	return 0;
}

void SiAiDeviceSelectService::ServiceStart(void) {
	if (m_ThreadFlag == 0) {
		m_ThreadFlag = 1;
	}
	return;
}

int SiAiDeviceSelectService::ServiceCheck(int pre_service) {
	if (pre_service == 2) {
		return pre_service;
	}
	if (m_ThreadFlag == 2) {
		return 2;
	}
	if (m_ThreadFlag == 1) {
		m_ThreadFlag = 2;
		m_Thread = AfxBeginThread(ServiceProcess, this);
		return 2;
	}
	return pre_service;
}

int SiAiDeviceSelectService::BaseAddrGet(UINT8_T *addr, UINT32_T *index) {
	return 0;
}

int SiAiDeviceSelectService::ManageAddrGet(UINT8_T *addr, UINT32_T *index) {
	return 0;
}

int SiAiDeviceSelectService::ServerAddrGet(UINT8_T *addr, UINT32_T *index) {
	return 0;
}

int SiAiDeviceSelectService::BaseAddrGetByComboIndex(INT32_T combo_index, UINT8_T *addr, UINT32_T *index)
{
	return 0;
}

int SiAiDeviceSelectService::ManageAddrGetByComboIndex(INT32_T combo_index, UINT8_T *addr, UINT32_T *index)
{
	return 0;
}

int SiAiDeviceSelectService::ServerAddrGetByComboIndex(INT32_T combo_index, UINT8_T *addr, UINT32_T *index)
{
	return 0;
}








