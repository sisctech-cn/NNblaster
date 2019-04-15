#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define AI_SERVER_FILE_CONTENT_PAGE_LENGTH			(2*128*1024)
#define AI_SERVER_FILE_CONTENT_BUFFER_LENGTH		(4*128*1024)

#define AI_SERVER_DISPLAY_CHANGE					(WM_USER + 100)
// SiAiDeviceConfigNameData �Ի���

class SiAiStateFileF16Content : public CDialogEx
{
	DECLARE_DYNAMIC(SiAiStateFileF16Content)

public:
	SiAiStateFileF16Content(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SiAiStateFileF16Content();

// �Ի�������
	enum { IDD = IDD_AI_SERVER_STATE_FILE_CONTENT_F16_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

public:
	CBitmap m_background;
	CDC m_dc;

public:
	// data
	CString m_file_path;
	UINT32_T m_page_number;
	UINT8_T m_page_buffer[AI_SERVER_FILE_CONTENT_BUFFER_LENGTH+1];

public:
	CEdit m_ceditF16FilePath;

public:
	CString m_strF16FilePath;
	CString m_strF16TxtFilePath;

public:
	// data
	CString m_strFilePageData;

public:
	// data
	CEdit m_ceditFilePageData;
	
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

public:
	static void AiServerDataSet(LPVOID pParam);
	static UINT ServiceProcess(LPVOID pParam);
	void DisplayFileContent(UINT32_T page_number);
	void ConvertFile();

public:
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonPageFirst();
	afx_msg void OnBnClickedButtonPageNext();
	afx_msg void OnBnClickedButtonPageBefore();
	afx_msg void OnBnClickedButtonPageEnd();
	afx_msg void OnBnClickedButtonF16FileBrowser();

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonAccuracy();
};