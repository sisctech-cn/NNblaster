#pragma once

class SiAiDeviceConfigNameDataTab 
{
public:
	SiAiDeviceConfigNameDataTab();   // 标准构造函数
	virtual ~SiAiDeviceConfigNameDataTab();

public:
	SiAiDeviceConfigNameData m_dialog_name_data;
	CTabCtrl m_tab_ctrl;

public:
	void ConfigNameDataTabInit(CDialogEx *dialogEx, CString strTitle, int left, int top, int right, int bottom);

};
