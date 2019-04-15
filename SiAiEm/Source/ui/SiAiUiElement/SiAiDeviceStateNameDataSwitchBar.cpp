// SiAiDeviceStateNameDataSwitchBar.cpp : 实现文件
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


// SiAiDeviceState 对话框

IMPLEMENT_DYNAMIC(SiAiDeviceStateNameDataSwitchBar, CDialogEx)

SiAiDeviceStateNameDataSwitchBar::SiAiDeviceStateNameDataSwitchBar(CWnd* pParent /*=NULL*/)
    : CDialogEx(SiAiDeviceStateNameDataSwitchBar::IDD, pParent)
{
}

SiAiDeviceStateNameDataSwitchBar::~SiAiDeviceStateNameDataSwitchBar()
{
}

BOOL_T SiAiDeviceStateNameDataSwitchBar::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void SiAiDeviceStateNameDataSwitchBar::OnPaint()
{
}


HBRUSH SiAiDeviceStateNameDataSwitchBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void SiAiDeviceStateNameDataSwitchBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// soft info
	DDX_Control(pDX, IDC_STATIC_STATE_NAME_DATA, m_cstaticNameData);
	DDX_CBString(pDX, IDC_STATIC_STATE_NAME_DATA, m_strNameData);
	DDX_Control(pDX, IDC_STATIC_STATE_NUMBER_DATA, m_cstaticNumberData);
	DDX_CBString(pDX, IDC_STATIC_STATE_NUMBER_DATA, m_strNumberData);
	DDX_Control(pDX, IDC_SLIDER_NUMBER_DATA_SWITCH, m_csliderSwitch);
	DDX_Control(pDX, IDC_STATIC_SWITCH_OFF, m_cstaticSwitchOff);
	DDX_CBString(pDX, IDC_STATIC_SWITCH_OFF, m_strSwitchOff);
	DDX_Control(pDX, IDC_STATIC_SWITCH_ON, m_cstaticSwitchOn);
	DDX_CBString(pDX, IDC_STATIC_SWITCH_ON, m_strSwitchOn);
}


BEGIN_MESSAGE_MAP(SiAiDeviceStateNameDataSwitchBar, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_MESSAGE(AI_SERVER_DISPLAY_CHANGE, OnDisplayChange)
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
	ON_STN_CLICKED(IDC_STATIC_STATE_NUMBER_DATA, &SiAiDeviceStateNameDataSwitchBar::OnStnClickedStaticStateNumberData)
END_MESSAGE_MAP()

void SiAiDeviceStateNameDataSwitchBar::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

void SiAiDeviceStateNameDataSwitchBar::OnOK()
{
    UpdateData(true);
    UpdateData(false);
    return;
}
void SiAiDeviceStateNameDataSwitchBar::OnCancel()
{
    return;
}

LRESULT SiAiDeviceStateNameDataSwitchBar::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
    UpdateData(false);
    UpdateData(false);
    return 0;
}

void SiAiDeviceStateNameDataSwitchBar::OnStnClickedStaticStateNumberData()
{
	// TODO: 在此添加控件通知处理程序代码
}


