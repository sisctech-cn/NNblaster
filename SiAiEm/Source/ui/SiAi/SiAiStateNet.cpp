// SiAiStateFileContent.cpp : 实现文件
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

IMPLEMENT_DYNAMIC(SiAiStateNet, CDialogEx)

SiAiStateNet::SiAiStateNet(CWnd* pParent /*=NULL*/)
    : CDialogEx(SiAiStateNet::IDD, pParent)
{
	timer_enable = 0;
}

SiAiStateNet::~SiAiStateNet()
{
}

BOOL_T SiAiStateNet::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
	// set tab */
	m_ai_server_net_tab.InsertItem(0, AI_SERVER_UI_INFO_IDENT, 0);
	m_SiAiConfigIdent.Create(IDD_AI_SERVER_CONFIG_IDENT_DIALOG, &m_ai_server_net_tab);
	m_ai_server_net_tab.InsertItem(1, AI_SERVER_UI_INFO_STATE_COMMAND, 0);
	m_SiAiStateCommand.Create(IDD_AI_SERVER_STATE_FILE_CONTENT_DIALOG, &m_ai_server_net_tab);
	m_ai_server_net_tab.InsertItem(2, AI_SERVER_UI_INFO_STATE_CLOCK, 0);
	m_SiAiStateClock.Create(IDD_AI_SERVER_STATE_FILE_CONTENT_DIALOG, &m_ai_server_net_tab);
	m_ai_server_net_tab.InsertItem(3, AI_SERVER_UI_INFO_STATE_F16, 0);
	m_SiAiStateF16.Create(IDD_AI_SERVER_STATE_FILE_CONTENT_F16_DIALOG, &m_ai_server_net_tab);
	m_ai_server_net_tab.InsertItem(4, AI_SERVER_UI_INFO_STATE_F32, 0);
	m_SiAiStateF32.Create(IDD_AI_SERVER_STATE_FILE_CONTENT_F32_DIALOG, &m_ai_server_net_tab);
	m_ai_server_net_tab.InsertItem(5, AI_SERVER_UI_INFO_WEIGHTS_F32, 0);
	m_SiAiConfigWeightsConvert.Create(IDD_AI_SERVER_CONFIG_WEIGHTS_CONVERT_DIALOG, &m_ai_server_net_tab);

	/* set file name */
	m_SiAiStateCommand.m_file_path.Format(_T("%s"), _T("CmodelCommand"));
	m_SiAiStateClock.m_file_path.Format(_T("%s"), _T("CmodelClock"));

	CRect clientRC;
	m_ai_server_net_tab.GetClientRect(clientRC);
	clientRC.DeflateRect(0, 20, 0, 0);

	m_SiAiConfigIdent.MoveWindow(clientRC);
	m_SiAiConfigIdent.ShowWindow(SW_SHOW);
	m_SiAiStateCommand.MoveWindow(clientRC);
	m_SiAiStateCommand.ShowWindow(SW_HIDE);
	m_SiAiStateClock.MoveWindow(clientRC);
	m_SiAiStateClock.ShowWindow(SW_HIDE);
	m_SiAiStateF16.MoveWindow(clientRC);
	m_SiAiStateF16.ShowWindow(SW_HIDE);
	m_SiAiStateF32.MoveWindow(clientRC);
	m_SiAiStateF32.ShowWindow(SW_HIDE);
	m_SiAiConfigWeightsConvert.MoveWindow(clientRC);
	m_SiAiConfigWeightsConvert.ShowWindow(SW_HIDE);
	m_ai_server_net_tab.SetCurSel(0);
	
	SetTimer(AI_SERVER_STATE_FILE_CONTENT_TIMER, AI_SERVER_STATE_FILE_CONTENT_TIMER_PERIOD, NULL);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void SiAiStateNet::OnPaint()
{
}

HBRUSH SiAiStateNet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void SiAiStateNet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// soft info
	// m_tab
	DDX_Control(pDX, IDC_AI_SERVER_NET_TAB, m_ai_server_net_tab);
}


BEGIN_MESSAGE_MAP(SiAiStateNet, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_MESSAGE(AI_SERVER_DISPLAY_CHANGE, OnDisplayChange)
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_AI_SERVER_NET_TAB, &SiAiStateNet::OnTcnSelchangeNetTab)
END_MESSAGE_MAP()

void SiAiStateNet::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

void SiAiStateNet::OnOK()
{
    UpdateData(true);
    UpdateData(false);
    return;
}
void SiAiStateNet::OnCancel()
{
    return;
}

LRESULT SiAiStateNet::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
    UpdateData(false);
    UpdateData(false);
    return 0;
}

void SiAiStateNet::AiServerDataSet(LPVOID pParam)
{
	return;
}

UINT SiAiStateNet::ServiceProcess(LPVOID pParam)
{
	SiAiStateNet *nameDataDlg = (SiAiStateNet *)pParam;

	AiServerDataSet(pParam);
	return 0;
}

void SiAiStateNet::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
	if (timer_enable == 1) {
		m_Thread = AfxBeginThread(ServiceProcess, this);
		timer_enable = 0;
	}

    CDialogEx::OnTimer(nIDEvent);
}


void SiAiStateNet::OnTcnSelchangeNetTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int nCurSel = m_ai_server_net_tab.GetCurSel();
	if (nCurSel == 0) {
		m_SiAiConfigIdent.ShowWindow(SW_SHOW);
		m_SiAiStateCommand.ShowWindow(SW_HIDE);
		m_SiAiStateClock.ShowWindow(SW_HIDE);
		m_SiAiStateF16.ShowWindow(SW_HIDE);
		m_SiAiStateF32.ShowWindow(SW_HIDE);
		m_SiAiConfigWeightsConvert.ShowWindow(SW_HIDE);
		m_SiAiConfigIdent.timer_enable = 1;
	}
	if (nCurSel == 1) {
		m_SiAiConfigIdent.ShowWindow(SW_HIDE);
		m_SiAiStateCommand.ShowWindow(SW_SHOW);
		m_SiAiStateClock.ShowWindow(SW_HIDE);
		m_SiAiStateF16.ShowWindow(SW_HIDE);
		m_SiAiStateF32.ShowWindow(SW_HIDE);
		m_SiAiConfigWeightsConvert.ShowWindow(SW_HIDE);
		m_SiAiConfigIdent.timer_enable = 0;
	}
	if (nCurSel == 2) {
		m_SiAiConfigIdent.ShowWindow(SW_HIDE);
		m_SiAiStateCommand.ShowWindow(SW_HIDE);
		m_SiAiStateClock.ShowWindow(SW_SHOW);
		m_SiAiStateF16.ShowWindow(SW_HIDE);
		m_SiAiStateF32.ShowWindow(SW_HIDE);
		m_SiAiConfigWeightsConvert.ShowWindow(SW_HIDE);
		m_SiAiConfigIdent.timer_enable = 0;
	}
	if (nCurSel == 3) {
		m_SiAiConfigIdent.ShowWindow(SW_HIDE);
		m_SiAiStateCommand.ShowWindow(SW_HIDE);
		m_SiAiStateClock.ShowWindow(SW_HIDE);
		m_SiAiStateF16.ShowWindow(SW_SHOW);
		m_SiAiStateF32.ShowWindow(SW_HIDE);
		m_SiAiConfigWeightsConvert.ShowWindow(SW_HIDE);
		m_SiAiConfigIdent.timer_enable = 0;
	}
	if (nCurSel == 4) {
		m_SiAiConfigIdent.ShowWindow(SW_HIDE);
		m_SiAiStateCommand.ShowWindow(SW_HIDE);
		m_SiAiStateClock.ShowWindow(SW_HIDE);
		m_SiAiStateF16.ShowWindow(SW_HIDE);
		m_SiAiStateF32.ShowWindow(SW_SHOW);
		m_SiAiConfigWeightsConvert.ShowWindow(SW_HIDE);
		m_SiAiConfigIdent.timer_enable = 0;
	}
	if (nCurSel == 5) {
		m_SiAiConfigIdent.ShowWindow(SW_HIDE);
		m_SiAiStateCommand.ShowWindow(SW_HIDE);
		m_SiAiStateClock.ShowWindow(SW_HIDE);
		m_SiAiStateF16.ShowWindow(SW_HIDE);
		m_SiAiStateF32.ShowWindow(SW_HIDE);
		m_SiAiConfigWeightsConvert.ShowWindow(SW_SHOW);
		m_SiAiConfigIdent.timer_enable = 0;
	}

	return;
}
