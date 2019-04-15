// SiAiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SiAiGlobal.h"
#include <io.h>  
#include <fcntl.h>

////////////////////////////////////////////////////////////////////////////
// Global Variable Declaration
SiAiDlg *g_SiAiDlg = NULL;

////////////////////////////////////////////////////////////////////////////
// extern
// state

// Global Variable Declaration
int ComBaudRate[] = {9600,19200,38400,115200};
int ComParitySelNum =1;
CString ComParity[] = {_T("None")};
int ComDataBits[] = {8};
int ComStopBits[] = {1};

/////////////////////////////////////////////////////////////////////////////
// SiAiDlg dialog

SiAiDlg::SiAiDlg(CWnd* pParent /*=NULL*/)
    : CDialog(SiAiDlg::IDD, pParent)
{
	UINT32_T index;
    g_SiAiDlg = this;

	for(index = 0; index < AI_SERVER_SERVICE_CHECK_NUMBER; index++) {
		m_service_check[index] = 0;
	}
    //{{AFX_DATA_INIT(SiAiDlg)
    // status control
    m_strSoftStatus = _T("");
    m_strComStatus = _T(AI_SERVER_UI_INFO_CLOSE);
    m_strSendBytes = _T("0");
    m_strReceiveBytes  = _T("0");
    m_nReceiveBytes = 0;
    m_nSendBytes = 0;
    m_nReceivePackets = 0;
    m_nSendPackets = 0;
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void SiAiDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_BUTTON_SOFT_QUIT, m_cbuttonSoftQuit);
    DDX_Control(pDX, IDC_BUTTON_SOFT_ABOUT, m_cbuttonSoftAbout);
    // comm parameter
    DDX_Control(pDX, IDC_COMBO_COM_SELECT, m_ccomboboxPortNumber);
    DDX_CBString(pDX, IDC_COMBO_COM_SELECT, m_strPortNumber);
    DDX_Control(pDX, IDC_COMBO_COM_BAUDRATE, m_ccomboboxBaudRate);
    DDX_CBString(pDX, IDC_COMBO_COM_BAUDRATE, m_strBaudRate);
    DDX_Control(pDX, IDC_BUTTON_COM_RESET, m_cbuttonComReset);
	// device select
	DDX_Control(pDX, IDC_COMBO_DEVICE_SELECT, m_ccomboboxDeviceSelect);
    // soft info
    DDX_Control(pDX, IDC_STATIC_SOFT_INFO, m_cstaticSoftInfo);
    DDX_CBString(pDX, IDC_STATIC_SOFT_INFO, m_strSoftStatus);
	// date time
    DDX_Control(pDX, IDC_STATIC_DATETIME, m_cstaticDateTime);
    DDX_CBString(pDX, IDC_STATIC_DATETIME, m_strDateTime);
    // m_tab
    DDX_Control(pDX, IDC_AI_SERVER_TAB, m_ai_server_device_tab);
}

BEGIN_MESSAGE_MAP(SiAiDlg, CDialog)
    //{{AFX_MSG_MAP(SiAiDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
    ON_NOTIFY(TCN_SELCHANGE, IDC_AI_SERVER_TAB, &SiAiDlg::OnTcnSelchangeTab)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_SOFT_ABOUT, &SiAiDlg::OnBnClickedButtonSoftAbout)
    ON_BN_CLICKED(IDC_BUTTON_SOFT_QUIT, &SiAiDlg::OnBnClickedButtonSoftQuit)
    ON_COMMAND(IDCLOSE, &SiAiDlg::OnClose)
	ON_BN_CLICKED(IDC_BUTTON_COM_RESET, &SiAiDlg::OnBnClickedButtonComReset)
	ON_WM_DEVICECHANGE()
	ON_BN_CLICKED(IDC_BUTTON_DEVICE_RESET, &SiAiDlg::OnBnClickedButtonDeviceReset)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE_SELECT, &SiAiDlg::OnCbnSelchangeComboDeviceSelect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SiAiDlg message handlers

void SiAiDlg::InitStatusControl(void)
{
    UpdateData(true);
    m_strSoftStatus = _T("");
    m_cstaticSoftInfo.SetWindowText(_T(""));
    UpdateData(false);
    return;
}

void InitConsoleWindow() 
{ 
	return;
}


BOOL_T SiAiDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL) {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // set tab */
    m_ai_server_device_tab.InsertItem(0, AI_SERVER_UI_INFO_NET_YOLOV3, 0);
	m_SiAiStateNetYoloV3.Create(IDD_AI_SERVER_STATE_NET_DIALOG, &m_ai_server_device_tab);

    CRect clientRC;
    m_ai_server_device_tab.GetClientRect(clientRC);
    clientRC.DeflateRect(0, 20, 0, 0);

	m_SiAiStateNetYoloV3.MoveWindow(clientRC);
	m_SiAiStateNetYoloV3.ShowWindow(SW_SHOW);
    m_ai_server_device_tab.SetCurSel(0);
	AiServerDataStateRun(1);

    // comm control
    m_strPortNumber = _T("");
    m_strBaudRate = _T("");
    m_strDataBits = _T("");
    m_strStopBits = _T("");
    m_strParity = _T("");
    // device
    InitCommConfigurationControl();
    InitCommStatusControl();
	// device list
	m_strDeviceSelect = _T("");

	{
		AI_DAT_PTR ai_data_ptr = BucSdkGetAiDat();
		m_SiAiLogFile.LogRegister((AI_DAT_PTR)ai_data_ptr, AI_DAT_INDEX_N);
	}

    m_SerialSendThreadFlag = SERIAL_THREAD_SEND_START;
    m_SerialSendThread = AfxBeginThread(SerialSendService, this);
    m_SerialRecvThreadFlag = SERIAL_THREAD_RECV_START;
    m_SerialRecvThread = AfxBeginThread(SerialReceiveService, this);

	SetTimer(AI_SERVER_LNET_TIMER, AI_SERVER_LNET_TIMER_PERIOD, NULL);

	InitConsoleWindow();
    // TODO: Add extra initialization here
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void SiAiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void SiAiDlg::OnPaint()
{
    if (IsIconic()) {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    } else {
        HBITMAP hBitmap;

        hBitmap = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_LOGO));
        ((CButton *)GetDlgItem(IDC_BUTTON_LOGO))->SetBitmap(hBitmap);

        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR SiAiDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}


void SiAiDlg::OnOK()
{
    UpdateData(true);
    UpdateData(false);
    return;
}


void SiAiDlg::OnCancel()
{
    return;
}


HBRUSH SiAiDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

// SiAiCom 消息处理程序
void SiAiDlg::InitCommConfigurationControl(void)
{
    CString temp;
	CString temp1;

    UpdateData(true);
    m_strPortNumber = _T("");
    m_strBaudRate = _T("");
    m_strDataBits = _T("");
    m_strStopBits = _T("");
    m_strParity = _T("");

    //显示波特率
	m_ccomboboxBaudRate.ResetContent();
    for(int i=0; i<sizeof(ComBaudRate)/sizeof(int); i++) {
        temp.Format("%d",ComBaudRate[i]);
        m_ccomboboxBaudRate.AddString((LPCTSTR)temp);
    }
    temp.Format("%d",115200);
    m_ccomboboxBaudRate.SetCurSel(m_ccomboboxBaudRate.FindString(0,temp));

    //显示串口设置
	m_ccomboboxPortNumber.ResetContent();
    for(int i=1; i<=(AI_SERVER_MAX_SERIAL_PORT-1); i++) {
        temp.Format("COM%d",i);
		temp1.Format("\\\\.\\COM%d",i);
        m_SiAiManageCom.phy.OpenSerialPort(temp1.GetBuffer());
        temp1.ReleaseBuffer();
        if(g_SiAiDlg->m_SiAiManageCom.phy.GetHandle() != INVALID_HANDLE_VALUE) {
            m_ccomboboxPortNumber.AddString((LPCTSTR)temp);
        }
        m_SiAiManageCom.phy.CloseCommPort();
    }
    if(m_ccomboboxPortNumber.GetCount()) {
        m_ccomboboxPortNumber.SetCurSel(0);
    }
    UpdateData(false);

    return;
}

void SiAiDlg::InitCommStatusCounterControl(void)
{
    UpdateData(true);
    UpdateData(false);
    return;
}

void SiAiDlg::InitCommStatusControl(void)
{
    InitCommStatusCounterControl();
    UpdateData(true);
    UpdateData(false);
    return;
}


void SiAiDlg::CloseCom(void)
{
    CString temp;
    // close com
    m_SiAiManageCom.phy.CloseCommPort();
    m_strComStatus = _T("关闭");
    temp =	"通信口"+m_strPortNumber+m_strComStatus+
            AI_SERVER_UI_INFO_BANDRATE	+m_strBaudRate+"bps"+
            AI_SERVER_UI_INFO_PARITY+m_strParity+
            AI_SERVER_UI_INFO_DATA_BIT+m_strDataBits+
            AI_SERVER_UI_INFO_STOP_BIT+m_strStopBits;
 //  g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(temp);

    return;
}

void SiAiDlg::OpenCom(void)
{
    int SelBaudRate,SelDataBits,SelStopBits;
    char SelParity;
    CString temp;
	CString temp1;

    m_ccomboboxPortNumber.GetLBText(m_ccomboboxPortNumber.GetCurSel(), m_strPortNumber);
    temp=m_strPortNumber;
    m_ccomboboxBaudRate.GetLBText(m_ccomboboxBaudRate.GetCurSel(), m_strBaudRate);
    SelBaudRate=atoi(m_strBaudRate);
    m_strParity = _T("8");
    SelDataBits = 8;
    m_strParity = _T("无");
    SelParity = 0;
    m_strStopBits = _T("1");
    SelStopBits = ONESTOPBIT;
	temp1 = _T("\\\\.\\");
	temp1.Append(temp);
    m_SiAiManageCom.phy.OpenSerialPort(temp1, SelBaudRate, SelDataBits, SelParity, SelStopBits);
    temp1.ReleaseBuffer();
    if (g_SiAiDlg->m_SiAiManageCom.phy.GetHandle() != INVALID_HANDLE_VALUE) {
        m_strComStatus = _T("打开");
        temp =	"通信口"+m_strPortNumber+m_strComStatus+
                AI_SERVER_UI_INFO_BANDRATE+m_strBaudRate+"bps"+
                AI_SERVER_UI_INFO_PARITY+m_strParity+
                AI_SERVER_UI_INFO_DATA_BIT+m_strDataBits+
                AI_SERVER_UI_INFO_STOP_BIT+m_strStopBits;
        g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(temp);
    }

    return;
}

UINT SiAiDlg::SerialSendService(LPVOID pParam)
{
    SiAiDlg *updaterDlg = (SiAiDlg *)pParam;

    while(updaterDlg->m_SerialSendThreadFlag != SERIAL_THREAD_SEND_EXIT_START) {
        if (updaterDlg->m_SerialSendThreadFlag == SERIAL_THREAD_SEND_STOP) {
            Sleep(1);
            continue;
        }
        if (updaterDlg->m_SiAiManageCom.phy.GetHandle() == INVALID_HANDLE_VALUE) {
            Sleep(1);
            continue;
        }

//      updaterDlg->m_SiAiManageCom.link_tx_func((UINT32_T)&(updaterDlg->m_SiAiManageCom.inteface.link));
        if (updaterDlg->m_SiAiManageCom.inteface.link.phy.tx_state == AI_BASE_LNET_LINK_TX_IDLE) {
            Sleep(1);
        }

    }
    updaterDlg->m_SerialSendThreadFlag = SERIAL_THREAD_SEND_EXIT_FINISH;
    return 0;
}

UINT SiAiDlg::SerialReceiveService(LPVOID pParam)
{
    SiAiDlg *updaterDlg = (SiAiDlg *)pParam;
	ERROR_T result;
#if (AI_PRODUCT_OPTION_LOG_COMM == 1)
	DWORD oldCount, newCount;
	CString temp;
#endif

    while(updaterDlg->m_SerialRecvThreadFlag != SERIAL_THREAD_RECV_EXIT_START) {
        if (updaterDlg->m_SerialRecvThreadFlag == SERIAL_THREAD_RECV_STOP) {
            Sleep(1);
            continue;
        }
        if (updaterDlg->m_SiAiManageCom.phy.GetHandle() == INVALID_HANDLE_VALUE) {
            Sleep(1);
            continue;
        }

        updaterDlg->m_SiAiManageCom.AiManageComRecv();
#if (AI_PRODUCT_OPTION_LOG_COMM == 1)
		oldCount=GetTickCount();
#endif
        result = AiBaseLnetInterfaceRun(&(updaterDlg->m_SiAiManageCom.inteface));
#if (AI_PRODUCT_OPTION_LOG_COMM == 1)
		newCount=GetTickCount();
		if (result == AI_ERROR_SUCCESS) {
			temp.Format(_T("AiBaseLnetInterfaceRun dateTime=[%s] "), updaterDlg->m_SiAiLogFile.GetCurrentDateTime());
			temp.AppendFormat(_T("start=%u "), oldCount);
			temp.AppendFormat(_T("end=%u "), newCount);
			temp.AppendFormat(_T("period=%u\n"), (newCount-oldCount));
			updaterDlg->m_SiAiLogFile.WriteLogFile(temp);
		}
#endif
		if (result != AI_ERROR_SUCCESS) {
			Sleep(1);
		}
    }
    updaterDlg->m_SerialRecvThreadFlag = SERIAL_THREAD_RECV_EXIT_FINISH;
    return 0;
}


void SiAiDlg::OnBnClickedButtonSoftAbout()
{
    // TODO: 在此添加控件通知处理程序代码
    SiAiHelp dlgAbout;
    dlgAbout.DoModal();

    return;
}


void SiAiDlg::OnBnClickedButtonSoftQuit()
{
    // TODO: 在此添加控件通知处理程序代码
	UINT32_T timeout;
	UINT32_T timeout_max = 1000;

    m_SerialSendThreadFlag = SERIAL_THREAD_SEND_EXIT_START;
    m_SerialRecvThreadFlag = SERIAL_THREAD_RECV_EXIT_START;
	WaitForSingleObject(m_SiAiDeviceData.m_ThreadFlagSemaphore, INFINITE);
	m_SiAiDeviceData.m_CommandThreadFlag = COMMAND_THREAD_EXIT_START;
	ReleaseSemaphore(m_SiAiDeviceData.m_ThreadFlagSemaphore, 1, NULL);
	timeout = 0;
    while(m_SerialSendThreadFlag != SERIAL_THREAD_SEND_EXIT_FINISH) {
		timeout++;
		if (timeout > timeout_max) {
			break;
		}
        Sleep(1);
    }
	timeout = 0;
    while(m_SerialRecvThreadFlag != SERIAL_THREAD_RECV_EXIT_FINISH) {
		timeout++;
		if (timeout > timeout_max) {
			break;
		}
        Sleep(1);
    }
	timeout = 0;
	while(m_SiAiDeviceData.m_CommandThreadFlag != COMMAND_THREAD_EXIT_FINISH) {
		timeout++;
		if (timeout > timeout_max) {
			break;
		}
        Sleep(1);
    }
    CloseCom();
    Sleep(1000);
    PostQuitMessage(0);

    return;
}

void SiAiDlg::OnClose()
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    OnCancel();
    //CDialogEx::OnClose();
}



void SiAiDlg::OnBnClickedButtonComReset()
{
	// TODO: 在此添加控件通知处理程序代码
	InitCommConfigurationControl();
}

BOOL SiAiDlg::OnDeviceChange(UINT nEventType,DWORD_PTR dwData)  
{  
        //DEV_BROADCAST_DEVICEINTERFACE* dbd = (DEV_BROADCAST_DEVICEINTERFACE*) dwData;  
    switch (nEventType)  
    {  
    case DBT_DEVICEREMOVECOMPLETE://移除设备  
    case DBT_DEVICEARRIVAL://添加设备
	case DBT_DEVNODES_CHANGED:
		CloseCom();
//		InitCommConfigurationControl();
        break;  
  
    default:  
        break;  
    }  
  
    return TRUE;  
} 


void SiAiDlg::OnBnClickedButtonDeviceReset()
{
	m_SiAiDeviceData.m_CloseComFlag = 1;
	m_SiAiDeviceData.m_retry_times = 0;
	m_SiAiDeviceData.m_retry_times_max = 0;
	m_SiAiDeviceData.m_retry_times_total = 0;
	m_SiAiDeviceSelectService.ServiceStart();
	return;
}

void SiAiDlg::OnCbnSelchangeComboDeviceSelect()
{
	// TODO: 在此添加控件通知处理程序代码
}

ERROR_T SiAiDlg::AiServerDataStateRun(int value) 
{
	return AI_ERROR_SUCCESS;
}

ERROR_T SiAiDlg::AiServerDataConfigRun(int value) 
{
	return AI_ERROR_SUCCESS;
}

ERROR_T SiAiDlg::AiServerDataUpdateBootRun(int value) 
{
	return AI_ERROR_SUCCESS;
}

ERROR_T SiAiDlg::AiServerDataSocRun(int value) 
{
	return AI_ERROR_SUCCESS;
}

void SiAiDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;

    int nCurSel = m_ai_server_device_tab.GetCurSel();
    if (nCurSel == 0) {
		m_SiAiStateNetYoloV3.ShowWindow(SW_SHOW);
		m_SiAiStateNetYoloV3.timer_enable = 1;
	} 

	return;
}

void SiAiDlg::OnTimer(UINT_PTR nIDEvent) {
	UINT32_T index;
	int pre_service = AI_SERVER_SERVICE_CHECK_NUMBER;
	COleDateTime current=COleDateTime::GetCurrentTime();
	CString strDate;

	strDate = current.Format("%Y-%m-%d %H:%M:%S");
	m_strDateTime = strDate;
	m_cstaticDateTime.SetWindowText(m_strDateTime);

	/* pre service check */
	for(index = 0; index < AI_SERVER_SERVICE_CHECK_NUMBER; index++) {
		if (m_service_check[index] != 0) {
			pre_service = index;
			break;
		}
	}

	for(index = 0; index < AI_SERVER_SERVICE_CHECK_NUMBER; index++) {
		switch (index) {
		case 0:
			if ((pre_service == AI_SERVER_SERVICE_CHECK_NUMBER) || (pre_service == index)) {
				m_service_check[index] = m_SiAiDeviceSelectService.ServiceCheck(0);
				if (m_service_check[index] != 0) {
						CDialog::OnTimer(nIDEvent);
						return;
				}
			}
			break;
		default:
			CDialog::OnTimer(nIDEvent);
			return;
		}
	}
	
	CDialog::OnTimer(nIDEvent);
	return;
}

void SiAiDlg::DisplayStatus(UINT8_T *buffer)
{
	CString temp;

	temp.Format(_T("%s"), buffer);
	g_SiAiDlg->m_cstaticSoftInfo.SetWindowText(temp);

	return;
}

#ifdef __cplusplus
extern "C" {
#endif
ERROR_T AiBaseUpdateDlgStatus(UINT8_T *format, ...)
{
	UINT8_T buffer[1024];

	va_list vlArgs;
	va_start(vlArgs, format);
	memset((char *)buffer, 0, 1024);
	vsnprintf((char *)buffer, 1024 - 1, (char *)format, vlArgs);
	va_end(vlArgs);

	g_SiAiDlg->DisplayStatus(buffer);
	return AI_ERROR_SUCCESS;
}

UINT64_T AiBaseUpdateDlgStateClockDebugStepGet(void)
{
	return g_SiAiDlg->m_SiAiStateNetYoloV3.m_SiAiStateClock.m_debug_step;
}
#ifdef __cplusplus
}
#endif