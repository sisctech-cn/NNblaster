// SiAiDeviceConfigButton.cpp : 实现文件
//

#include "stdafx.h"
#include "SiAiGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// Global Variable Declaration

////////////////////////////////////////////////////////////////////////////
// Extern Variable Declaration
extern SiAiDlg *g_SiAiDlg;
extern SiAiDeviceSelectService *g_SiAiDeviceSelectService;


// SiAiDeviceConfig 对话框

IMPLEMENT_DYNAMIC(SiAiDeviceConfigButton, CDialogEx)

SiAiDeviceConfigButton::SiAiDeviceConfigButton(CWnd* pParent /*=NULL*/)
    : CDialogEx(SiAiDeviceConfigButton::IDD, pParent)
{
	timer_enable = 0;
}

SiAiDeviceConfigButton::~SiAiDeviceConfigButton()
{
}

BOOL_T SiAiDeviceConfigButton::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
	SetTimer(AI_SERVER_CONFIG_BUTTON_TIMER, AI_SERVER_CONFIG_BUTTON_TIMER_PERIOD, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void SiAiDeviceConfigButton::OnPaint()
{
}

void SiAiDeviceConfigButton::DataSet(UINT16_T sdk_data_index, UINT64_T sdk_data_sub_index)
{
	m_sdk_data_index = sdk_data_index;
	m_sdk_data_sub_index = sdk_data_sub_index;

	return;
}

HBRUSH SiAiDeviceConfigButton::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void SiAiDeviceConfigButton::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// soft info
	DDX_Control(pDX, IDC_STATIC_CONFIG_NAME, m_cstaticNameData);
	DDX_CBString(pDX, IDC_STATIC_CONFIG_NAME, m_strNameData);
}


BEGIN_MESSAGE_MAP(SiAiDeviceConfigButton, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_MESSAGE(AI_SERVER_DISPLAY_CHANGE, OnDisplayChange)
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CONFIG_CONFIRM, &SiAiDeviceConfigButton::OnBnClickedButtonConfigSet)
END_MESSAGE_MAP()

void SiAiDeviceConfigButton::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

void SiAiDeviceConfigButton::OnOK()
{
    UpdateData(true);
    UpdateData(false);
    return;
}
void SiAiDeviceConfigButton::OnCancel()
{
    return;
}

LRESULT SiAiDeviceConfigButton::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
    UpdateData(false);
    UpdateData(false);
    return 0;
}


void SiAiDeviceConfigButton::OnBnClickedButtonConfigSet()
{
	return;
}

void SiAiDeviceConfigButton::AiServerDataSet(LPVOID pParam)
{
	return;
}

UINT SiAiDeviceConfigButton::ServiceProcess(LPVOID pParam)
{
	SiAiDeviceConfigButton *buttonDataDlg = (SiAiDeviceConfigButton *)pParam;

	AiServerDataSet(pParam);
	return 0;
}

void SiAiDeviceConfigButton::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
	if (timer_enable == 1) {
		m_Thread = AfxBeginThread(ServiceProcess, this);
		timer_enable = 0;
	}

    CDialogEx::OnTimer(nIDEvent);
}


