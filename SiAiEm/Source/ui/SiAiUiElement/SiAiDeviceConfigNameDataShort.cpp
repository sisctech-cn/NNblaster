// SiAiDeviceConfigNameDataShort.cpp : 实现文件
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

IMPLEMENT_DYNAMIC(SiAiDeviceConfigNameDataShort, CDialogEx)

SiAiDeviceConfigNameDataShort::SiAiDeviceConfigNameDataShort(CWnd* pParent /*=NULL*/)
    : CDialogEx(SiAiDeviceConfigNameDataShort::IDD, pParent)
{
	timer_enable = 0;
}

SiAiDeviceConfigNameDataShort::~SiAiDeviceConfigNameDataShort()
{
}

BOOL_T SiAiDeviceConfigNameDataShort::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
	SetTimer(AI_SERVER_CONFIG_NAME_DATA_TIMER, AI_SERVER_CONFIG_NAME_DATA_TIMER_PERIOD, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void SiAiDeviceConfigNameDataShort::OnPaint()
{
}

void SiAiDeviceConfigNameDataShort::DataSet(UINT16_T sdk_data_index, UINT64_T sdk_data_sub_index)
{
	m_sdk_data_index = sdk_data_index;
	m_sdk_data_sub_index = sdk_data_sub_index;

	return;
}

HBRUSH SiAiDeviceConfigNameDataShort::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void SiAiDeviceConfigNameDataShort::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// soft info
	DDX_Control(pDX, IDC_STATIC_CONFIG_NAME, m_cstaticNameData);
	DDX_CBString(pDX, IDC_STATIC_CONFIG_NAME, m_strNameData);
	DDX_Control(pDX, IDC_EDIT_CONFIG_NAME_DATA, m_ceditNumberData);
	DDX_Text(pDX, IDC_EDIT_CONFIG_NAME_DATA, m_strNumberData);
	DDX_Control(pDX, IDC_STATIC_CONFIG_NAME_DATA_UNIT, m_cstaticUnitData);
	DDX_CBString(pDX, IDC_STATIC_CONFIG_NAME_DATA_UNIT, m_strUnitData);
}


BEGIN_MESSAGE_MAP(SiAiDeviceConfigNameDataShort, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_MESSAGE(AI_SERVER_DISPLAY_CHANGE, OnDisplayChange)
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CONFIG_SET, &SiAiDeviceConfigNameDataShort::OnBnClickedButtonConfigSet)
    ON_EN_CHANGE(IDC_EDIT_CONFIG_NAME_DATA, &SiAiDeviceConfigNameDataShort::OnEnChangeEditNameData)
END_MESSAGE_MAP()

void SiAiDeviceConfigNameDataShort::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

void SiAiDeviceConfigNameDataShort::OnOK()
{
    UpdateData(true);
    UpdateData(false);
    return;
}
void SiAiDeviceConfigNameDataShort::OnCancel()
{
    return;
}

LRESULT SiAiDeviceConfigNameDataShort::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
    UpdateData(false);
    UpdateData(false);
    return 0;
}

void SiAiDeviceConfigNameDataShort::OnEnChangeEditNameData()
{
    return;
}

void SiAiDeviceConfigNameDataShort::OnBnClickedButtonConfigSet()
{
	timer_enable = 1;
	return;
}

void SiAiDeviceConfigNameDataShort::AiServerDataSet(LPVOID pParam)
{
	return;
}

UINT SiAiDeviceConfigNameDataShort::ServiceProcess(LPVOID pParam)
{
	SiAiDeviceConfigNameData *nameDataDlg = (SiAiDeviceConfigNameData *)pParam;
	AiServerDataSet(pParam);
	return 0;
}

void SiAiDeviceConfigNameDataShort::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
	if (timer_enable == 1) {
		m_Thread = AfxBeginThread(ServiceProcess, this);
		timer_enable = 0;
	}

    CDialogEx::OnTimer(nIDEvent);
}

