#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define AI_SERVER_DISPLAY_CHANGE					(WM_USER + 100)
// SiAiDeviceStateNumberDataSwitchBar �Ի���

class SiAiDeviceStateNumberDataSwitchBar : public CDialogEx
{
	DECLARE_DYNAMIC(SiAiDeviceStateNumberDataSwitchBar)

public:
	SiAiDeviceStateNumberDataSwitchBar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SiAiDeviceStateNumberDataSwitchBar();

// �Ի�������
	enum { IDD = IDD_AI_SERVER_DEVICE_STATE_NUMBER_DATA_SWITCH_BAR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
