#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define AI_SERVER_DISPLAY_CHANGE					(WM_USER + 100)
// SiAiDeviceConfigButton 对话框

class SiAiDeviceConfigButton : public CDialogEx
{
	DECLARE_DYNAMIC(SiAiDeviceConfigButton)

public:
	SiAiDeviceConfigButton(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SiAiDeviceConfigButton();

// 对话框数据
	enum { IDD = IDD_AI_SERVER_DEVICE_CONFIG_BUTTON_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	CBitmap m_background;
	CDC m_dc;

public:
	// data
	CString m_strNameData;
	
public:	
	// data
	CStatic m_cstaticNameData;
	
public:
	UINT16_T m_sdk_data_index;
	UINT64_T m_sdk_data_sub_index;
	int timer_enable;
	CWinThread *m_Thread;

public:
	void DataSet(UINT16_T sdk_data_index, UINT64_T sdk_data_sub_index);

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
	afx_msg void OnBnClickedButtonConfigSet();

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

};
