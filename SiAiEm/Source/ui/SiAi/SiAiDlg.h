// SiAiDlg.h : header file
//

#if !defined(AFX_COMTOOLDLG_H__5384AAC6_E9BC_453F_89C6_00BE9259A9DF__INCLUDED_)
#define AFX_COMTOOLDLG_H__5384AAC6_E9BC_453F_89C6_00BE9259A9DF__INCLUDED_

#include "afxwin.h"
#include "afxcmn.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __cplusplus
extern "C" {
#endif
ERROR_T AiBaseUpdateDlgStatus(UINT8_T *format, ...);
UINT64_T AiBaseUpdateDlgStateClockDebugStepGet(void);
#ifdef __cplusplus
}
#endif

/////////////////////////////////////////////////////////////////////////////
// SiAiDlg dialog
class SiAiDlg : public CDialog
{
// Construction
public:
    SiAiDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
    //{{AFX_DATA(SiAiDlg)
    enum { IDD = IDD_AI_SERVER_DIALOG };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(SiAiDlg)
public:
	SiAiStateNet m_SiAiStateNetYoloV3;
	CTabCtrl m_ai_server_device_tab;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;
	CBitmap m_background;
	CDC m_dc;

    // Generated message map functions
    //{{AFX_MSG(SiAiDlg)
    virtual BOOL_T OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    // general oparation
    CButton m_cbuttonSoftQuit;
    CButton m_cbuttonSoftAbout;
    // comm control
    CComboBox	m_ccomboboxPortNumber;
    CComboBox	m_ccomboboxBaudRate;
	CButton m_cbuttonComReset;
	// device list
	CComboBox   m_ccomboboxDeviceSelect;
    // software status
    CStatic m_cstaticSoftInfo;
	CStatic m_cstaticDateTime;

public:	
    // comm control
    CString	m_strPortNumber;
    CString	m_strBaudRate;
    CString	m_strDataBits;
    CString	m_strStopBits;
    CString	m_strParity;
	// comm status
    CString	m_strComStatus;
    CString	m_strSendBytes;
    CString	m_strReceiveBytes;
    unsigned int m_nReceiveBytes;
    unsigned int m_nSendBytes;
    unsigned int m_nReceivePackets;
    unsigned int m_nSendPackets;
	// device list
	CString m_strDeviceSelect;
    // comm status
	CString m_strSoftStatus;
	// datetime
	CString m_strDateTime;
	
public:
	SiAiManageCom m_SiAiManageCom;
	CMutex m_mutex;

public:
	SiAiDeviceData m_SiAiDeviceData;

public:
	CWinThread *m_SerialSendThread;
	int m_SerialSendThreadFlag;
	CWinThread *m_SerialRecvThread;
	int m_SerialRecvThreadFlag;

public:
	SiAiLogFile m_SiAiLogFile;

public:
	static UINT SerialSendService(LPVOID pParam);
	static UINT SerialReceiveService(LPVOID pParam);

public:
    void InitStatusControl(void);

public:
    virtual void OnOK();
    virtual void OnCancel();
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);
	
public:
    void InitCommConfigurationControl(void);
    void InitCommStatusCounterControl(void);
    void InitCommStatusControl(void);
    void OpenCom(void);
    void CloseCom(void);
	
	afx_msg void OnBnClickedButtonSoftAbout();
	afx_msg void OnBnClickedButtonSoftQuit();
	afx_msg void OnBnClickedButtonComReset();
	afx_msg void OnBnClickedButtonDeviceReset();
	afx_msg void OnCbnSelchangeComboDeviceSelect();

public:
	ERROR_T AiServerDataStateRun(int value);
	ERROR_T AiServerDataConfigRun(int value);
	ERROR_T AiServerDataUpdateBootRun(int value);
	ERROR_T AiServerDataSocRun(int value);

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	SiAiSdkData m_sdk_data;

public:
	int m_service_check[AI_SERVER_SERVICE_CHECK_NUMBER];

public:
	// select device
	SiAiDeviceSelectService m_SiAiDeviceSelectService;

public:
	void DisplayStatus(UINT8_T *buffer);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMTOOLDLG_H__5384AAC6_E9BC_453F_89C6_00BE9259A9DF__INCLUDED_)





















