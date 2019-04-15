#pragma once

typedef struct ai_server_device_config_combo_data_struct {
	int valid[AI_SERVER_CONFIG_DATA_COMBO_MAX];
	UINT64_T value[AI_SERVER_CONFIG_DATA_COMBO_MAX];
	CString strValue[AI_SERVER_CONFIG_DATA_COMBO_MAX];
} SiAiDeviceConfigComboData, *SiAiDeviceConfigComboDataPtr;

class SiAiSdkData
{
public:
	SiAiSdkData();   
	virtual ~SiAiSdkData();

public:

public:
	void ComboDataInit(SiAiDeviceConfigComboDataPtr combo_data);
	void ComboDataSet(SiAiDeviceConfigComboDataPtr combo_data, int value, CString strValue);
	CString ComboDataStringGet(SiAiDeviceConfigComboDataPtr combo_data, UINT64_T value);

};


