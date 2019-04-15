#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define AI_SERVER_DISPLAY_CHANGE					(WM_USER + 100)
// SiAiDeviceConfigNameData 对话框

class SiAiStateNet : public CDialogEx
{
	DECLARE_DYNAMIC(SiAiStateNet)

public:
	SiAiStateNet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SiAiStateNet();

// 对话框数据
	enum { IDD = IDD_AI_SERVER_STATE_NET_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	CBitmap m_background;
	CDC m_dc;

public:
	SiAiConfigIdent m_SiAiConfigIdent;
	SiAiStateFileContent m_SiAiStateCommand;
	SiAiStateFileContent m_SiAiStateClock;
	SiAiStateFileF16Content m_SiAiStateF16;
	SiAiStateFileF32Content m_SiAiStateF32;
	SiAiConfigWeightsConvert m_SiAiConfigWeightsConvert;
	CTabCtrl m_ai_server_net_tab;

public:
	int timer_enable;
	CWinThread *m_Thread;

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
	afx_msg void OnTcnSelchangeNetTab(NMHDR *pNMHDR, LRESULT *pResult);

public:
	static void AiServerDataSet(LPVOID pParam);
	static UINT ServiceProcess(LPVOID pParam);

public:

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
