#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define AI_SERVER_DISPLAY_CHANGE					(WM_USER + 100)
// SiAiDeviceConfigNameDataCombo 对话框

class SiAiDeviceConfigNameDataCombo : public CDialogEx
{
	DECLARE_DYNAMIC(SiAiDeviceConfigNameDataCombo)

public:
	SiAiDeviceConfigNameDataCombo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SiAiDeviceConfigNameDataCombo();

// 对话框数据
	enum { IDD = IDD_AI_SERVER_DEVICE_CONFIG_NAME_DATA_COMBO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	CBitmap m_background;
	CDC m_dc;

public:
	// data
	CString m_strNameData;
	CString m_strNumberData;
	
public:	
	// data
	CStatic m_cstaticNameData;
    CComboBox m_ccomboNumberData;
	
public:
	UINT16_T m_sdk_data_index;
	UINT64_T m_sdk_data_sub_index;
	SiAiDeviceConfigComboData m_combo_data;
	int timer_enable;
	CWinThread *m_Thread;
	
public:
	void ComboDataSet(SiAiDeviceConfigComboDataPtr combo_data, UINT16_T sdk_data_index, UINT64_T sdk_data_sub_index);
	void ComboDataCurSet(UINT64_T value);
	UINT64_T ComboDataCurGet(void);
	CString ComboDataStringGet(UINT64_T value);

protected:	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT OnDisplayChange(WPARAM wParam, LPARAM lParam);
	
public:

public:
	virtual BOOL_T OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
    virtual void OnCancel();

public:
	static UINT ServiceProcess(LPVOID pParam);
	static void AiServerDataSet(LPVOID pParam);

public:
	afx_msg void OnCbnSelchangeComboNumberData();
	afx_msg void OnBnClickedButtonConfigSet();

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
