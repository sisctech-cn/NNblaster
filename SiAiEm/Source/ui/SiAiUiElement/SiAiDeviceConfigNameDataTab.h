#pragma once

class SiAiDeviceConfigNameDataTab 
{
public:
	SiAiDeviceConfigNameDataTab();   // ��׼���캯��
	virtual ~SiAiDeviceConfigNameDataTab();

public:
	SiAiDeviceConfigNameData m_dialog_name_data;
	CTabCtrl m_tab_ctrl;

public:
	void ConfigNameDataTabInit(CDialogEx *dialogEx, CString strTitle, int left, int top, int right, int bottom);

};
