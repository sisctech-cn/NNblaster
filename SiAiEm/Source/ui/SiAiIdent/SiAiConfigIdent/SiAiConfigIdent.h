#pragma once

#define AI_SERVER_DISPLAY_CHANGE					(WM_USER + 100)
// SiAiFp16Test 对话框

class SiAiConfigIdent : public CDialogEx
{
	DECLARE_DYNAMIC(SiAiConfigIdent)

public:
	SiAiConfigIdent(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SiAiConfigIdent();

// 对话框数据
	enum { IDD = IDD_AI_SERVER_CONFIG_IDENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	CBitmap m_background;
	CDC m_dc;

public:
	CWinThread *m_IdentThread;
	int m_IdentThreadFlag;
	static UINT IdentService(LPVOID pParam);
	
public:	
	int timer_enable;
	
public:
	CEdit m_ceditIdentFilePath;

public:
	CString m_strIdentFilePath;

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
	void DisplayIdentImage(void);

public:

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonIdent();
	afx_msg void OnBnClickedButtonIdentFileBrowser();
	afx_msg void onBnClickedButtonTest();
};
