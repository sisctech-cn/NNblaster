#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define AI_SERVER_DISPLAY_CHANGE					(WM_USER + 100)
// SiAiDeviceConfigNumberDataSwitchBar 对话框

class SiAiDeviceConfigNumberDataSwitchBar : public CDialogEx
{
	DECLARE_DYNAMIC(SiAiDeviceConfigNumberDataSwitchBar)

public:
	SiAiDeviceConfigNumberDataSwitchBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SiAiDeviceConfigNumberDataSwitchBar();

// 对话框数据
	enum { IDD = IDD_AI_SERVER_DEVICE_CONFIG_NUMBER_DATA_SWITCH_BAR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	CBitmap m_background;
	CDC m_dc;

public:
	// data
	CString m_strNumberData;
	
public:	
	// data
    CStatic m_cstaticNumberData;
	CSliderCtrl m_csliderSwitch;
	
public:


public:

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

public:
	afx_msg void OnStnClickedStaticStateNumberData();
	
};
