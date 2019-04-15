// SiAiSdkData.cpp : 实现文件
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

////////////////////////////////////////////////////////////////////////////
// Extern Variable Declaration

////////////////////////////////////////////////////////////////////////////
// Function Declaration
SiAiSdkData::SiAiSdkData()
{
	return;
}

SiAiSdkData::~SiAiSdkData()
{
}

void SiAiSdkData::ComboDataInit(SiAiDeviceConfigComboDataPtr combo_data)
{
	int index;

	for(index = 0; index < AI_SERVER_CONFIG_DATA_COMBO_MAX; index++) {
		combo_data->valid[index] = 0;
		combo_data->value[index] = 0;
		combo_data->strValue[index].Format(_T(""));
	}
	return;
}

void SiAiSdkData::ComboDataSet(SiAiDeviceConfigComboDataPtr combo_data, int value, CString strValue)
{
	int index;

	for(index = 0; index < AI_SERVER_CONFIG_DATA_COMBO_MAX; index++) {
		if (combo_data->valid[index] != 1) {
			combo_data->valid[index] = 1;
			combo_data->value[index] = value;
			combo_data->strValue[index].Format(_T("%s"), strValue.GetBuffer());
			break;
		}
	}

	return;
}

CString SiAiSdkData::ComboDataStringGet(SiAiDeviceConfigComboDataPtr combo_data, UINT64_T value) 
{
	int index;
	CString temp;

	for(index = 0; index < AI_SERVER_CONFIG_DATA_COMBO_MAX; index++) {
		if (combo_data->valid[index] != 0) {
			if (combo_data->value[index] == value) {
				return combo_data->strValue[index];
			}
		}
    }

	temp.Format(_T(""));
	return temp; 
}


