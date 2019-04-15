#pragma once

#define AI_SERVER_DISPLAY_CHANGE					(WM_USER + 100)
// SiAiDeviceConfigNumberDataProcessBar 对话框

class SiAiDeviceConfigNumberDataProcessBar : public CDialogEx
{
	DECLARE_DYNAMIC(SiAiDeviceConfigNumberDataProcessBar)

public:
	SiAiDeviceConfigNumberDataProcessBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SiAiDeviceConfigNumberDataProcessBar();

// 对话框数据
	enum { IDD = IDD_AI_SERVER_DEVICE_CONFIG_NUMBER_DATA_PROCESS_BAR_DIALOG };

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
